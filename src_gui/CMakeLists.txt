file(GLOB FOLDER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.qml LoginRegisterComponents/*.qml SearchComponents/*.qml UserComponents/*.qml)

list(APPEND FOLDER_FILES ${LOGIN_REGISTER_FILES})

foreach(QML_FILE ${FOLDER_FILES})
    add_files(${QML_FILE} "QML_FILES")
endforeach()


