#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "managerdb.hpp"
#include "debug_interceptor.hpp"
#include "user.hpp"

#include "CMakeConfig.hpp"

int main(int argc, char *argv[]){

    QGuiApplication app(argc, argv);

    auto debugInterceptor{ Debug_Interceptor::getInstance() };
    auto managerDB{ QSharedPointer<ManagerDB>(new ManagerDB()) };
    auto user { QSharedPointer<User>(new User()) };

    QQmlApplicationEngine engine;

    //adding constants to every .qml
    engine.rootContext()->setContextProperty("MAX_CREDENTIAL_LENGTH", MAX_CREDENTIAL_LENGTH);
    engine.rootContext()->setContextProperty("ROOT_PATH", ROOT_PATH);

    //adding objects to every .qml
    engine.rootContext()->setContextProperty("managerDB", managerDB.data());
    engine.rootContext()->setContextProperty("user", user.data());

    const QUrl url(u"qrc:/RecipeManager/src_gui/Main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
