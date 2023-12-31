#include "managerdb.hpp"
#include "rapidcsv.hpp"
#include "CMakeConfig.hpp"

ManagerDB::ManagerDB(User* user, QObject *parent) : QObject{parent}
{
    QObject::connect(this, &ManagerDB::sendDBUser, user, &User::sendDBUser);
    this->user = user;
}

ManagerDB::~ManagerDB()
{
    if(db.isOpen()) db.close();
}

void ManagerDB::setupDB(){

    qInfo() << "Establishing default connection";

    db = QSqlDatabase::addDatabase("QMYSQL", "default_connection");

    db.setHostName("cooldb.mysql.database.azure.com");
    db.setDatabaseName("recipe_manager");
    db.setUserName("default_user");
    db.setPassword("default_user_pass");

    if(!db.open()){
        qCritical() << QString("Failed to establish default connection!\n %1").arg(db.lastError().text());
        return;
    }

    qInfo() << "Established default connection successfully";

    emit sendDBUser(db);

    if(INSERT_RECIPES) insertRecipes(); //Requires connecting as an admin user
}

bool ManagerDB::loadDriver(){

    qInfo() << "Loading driver/plugin";

    QPluginLoader loader("/home/roditu/Qt/6.5.1/gcc_64/plugins/sqldrivers/libqsqlmysql.so"); //for now hardcoded

    qInfo() << loader.metaData();

    if(loader.load()){
        qInfo() << "Loaded the plugin";
        return true;
    }

    qCritical() << loader.errorString();

    return false;
}

void ManagerDB::makeThreadConnection(QSqlDatabase& dbThread)
{
    qInfo() << "Establishing new thread connection (or replacing)";

    QSqlDatabase dbTemp = QSqlDatabase::addDatabase("QMYSQL", "thread_connection");

    dbTemp.setHostName("cooldb.mysql.database.azure.com");
    dbTemp.setDatabaseName("recipe_manager");
    dbTemp.setUserName("default_user");
    dbTemp.setPassword("default_user_pass");

    if(!dbTemp.open()){
        qCritical() << QString("Failed to establish new thread connection!\n %1").arg(dbTemp.lastError().text());
        return;
    }

    qInfo() << "Established new thread connection successfully";

    dbThread = dbTemp;
}

void ManagerDB::convertFractions(QString& input) {

    static QRegularExpression fractionRegex(QStringLiteral("[\u00BC-\u00BE\u2150-\u215E]"));

    QRegularExpressionMatchIterator i = fractionRegex.globalMatch(input);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString fraction = match.captured(0);

        fraction = fraction.normalized(QString::NormalizationForm_KD);

        input.replace(match.capturedStart(), match.capturedLength(), fraction);
    }
}

//Requires connecting as an admin user
void ManagerDB::insertRecipes(){

    using namespace std;

    if(!QFile(PATH_TO_RECIPES).exists()){
        qWarning() << QString("Path to recipes: %1 doesnt exist, exiting function").arg(PATH_TO_RECIPES);
        return;
    }

    rapidcsv::Document doc(PATH_TO_RECIPES, rapidcsv::LabelParams(), rapidcsv::SeparatorParams(',', false, rapidcsv::sPlatformHasCR, true));

    vector<vector<string>> columns{};

    QList<QString> columnNames{"id", "title", "ingredients", "instructions", "cleaned_ingredients", "image_bin"};

    foreach(QString columnName, columnNames){
        columns.push_back(doc.GetColumn<string>(columnName.toStdString()));
    }

    int recipesSize = columns[0].size(), columnsSize = columns.size();

    for(int i = 0; i < recipesSize; i++){

        QSqlQuery query{db};

        query.prepare("INSERT INTO recipes (title, ingredients, instructions, image_bin, cleaned_ingredients) "
                      "VALUES (:title, :ingredients, :instructions, :image_bin, :cleaned_ingredients)");

        for(int j = 1; j < columnsSize; j++){

            QString curr = QString::fromStdString(columns[j][i]), columnName = columnNames[j];

            if(columnName == "image_bin"){

                QString temp = PATH_TO_RECIPE_IMAGES + curr + ".jpg";

                QByteArray byteImage = user->imageToBinary(temp);

                if(byteImage.isNull()){
                    qWarning() << QString("Failed to convert image: %1, exiting function").arg(curr);
                    return;
                }

                query.bindValue(QString(":%1").arg(columnName), byteImage, QSql::Binary);
            }

            else{
                convertFractions(curr);
                query.bindValue(QString(":%1").arg(columnName), curr);
            }
        }

        if(!query.exec()){
            qWarning() << QString("Error executing query: %1, exiting function").arg(query.lastError().text());
            return;
        }
    }
}

void ManagerDB::listDrivers(){

    qInfo() << "Listing drivers";

    foreach(QString driver, QSqlDatabase::drivers()){
        qInfo() << driver;

        QSqlDatabase db = QSqlDatabase::addDatabase(driver);
        QSqlDriver* obj = db.driver();

        qInfo() << "Transactions: " << obj->hasFeature(QSqlDriver::DriverFeature::Transactions);
        qInfo() << "QuerySize: " << obj->hasFeature(QSqlDriver::DriverFeature::QuerySize);
        qInfo() << "BLOB: " << obj->hasFeature(QSqlDriver::DriverFeature::BLOB);
        qInfo() << "Unicode: " << obj->hasFeature(QSqlDriver::DriverFeature::Unicode);
        qInfo() << "PreparedQueries: " << obj->hasFeature(QSqlDriver::DriverFeature::PreparedQueries);
        qInfo() << "NamedPlaceholders: " << obj->hasFeature(QSqlDriver::DriverFeature::NamedPlaceholders);
        qInfo() << "PositionalPlaceholders: " << obj->hasFeature(QSqlDriver::DriverFeature::PositionalPlaceholders);
        qInfo() << "LastInsertId: " << obj->hasFeature(QSqlDriver::DriverFeature::LastInsertId);
        qInfo() << "BatchOperations: " << obj->hasFeature(QSqlDriver::DriverFeature::BatchOperations);
        qInfo() << "SimpleLocking: " << obj->hasFeature(QSqlDriver::DriverFeature::SimpleLocking);
        qInfo() << "LowPrecisionNumbers: " << obj->hasFeature(QSqlDriver::DriverFeature::LowPrecisionNumbers);
        qInfo() << "EventNotifications: " << obj->hasFeature(QSqlDriver::DriverFeature::EventNotifications);
        qInfo() << "FinishQuery: " << obj->hasFeature(QSqlDriver::DriverFeature::FinishQuery);
        qInfo() << "MultipleResultSets: " << obj->hasFeature(QSqlDriver::DriverFeature::MultipleResultSets);
        qInfo() << "CancelQuery: " << obj->hasFeature(QSqlDriver::DriverFeature::CancelQuery);
    }
}






