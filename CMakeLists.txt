cmake_minimum_required(VERSION 3.16)


macro(add_files file file_collection)

    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

    if (_relPath)
        list (APPEND ${file_collection} "${_relPath}/${file}")
    else()
        list (APPEND ${file_collection} "${file}")
    endif()

    if (_relPath)
        set (${file_collection} ${${file_collection}} PARENT_SCOPE)
    endif()

endmacro()


project(RecipeManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Sql Concurrent)

qt_standard_project_setup()
qt_policy(SET QTP0001 NEW) #use new policy (old is deprecated)

qt_add_executable(appRecipeManager)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(src_gui)
add_subdirectory(assets)

qt_add_qml_module(appRecipeManager
        URI RecipeManager
        VERSION 1.0
        RESOURCE_PREFIX "/"

        QML_FILES ${QML_FILES}
        RESOURCES ${RESOURCES}
)

#CMakeConfig.hpp - constants from cmake for the entire project to use
add_definitions(-DROOT_PATH_CMAKE=${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DMAX_CREDENTIAL_LENGTH_CMAKE=20)
add_definitions(-DMAX_TITLE_LENGTH_CMAKE=150)
add_definitions(-DINSERT_RECIPES_CMAKE=false)
add_definitions(-DPATH_TO_RECIPES_CMAKE=\"${CMAKE_CURRENT_SOURCE_DIR}/assets/recipes.csv\")
add_definitions(-DPATH_TO_RECIPE_IMAGES_CMAKE=\"${CMAKE_CURRENT_SOURCE_DIR}/assets/recipeImages\")

target_include_directories(appRecipeManager PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(appRecipeManager
    PRIVATE Qt6::Quick
    PRIVATE Qt${QT_VERSION_MAJOR}::Sql
    PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent
)

set_target_properties(appRecipeManager PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS appRecipeManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
