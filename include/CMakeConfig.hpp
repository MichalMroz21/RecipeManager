#ifndef CMAKECONFIG_HPP
#define CMAKECONFIG_HPP

    #ifdef ROOT_PATH_CMAKE
    #define ROOT_PATH QT_STRINGIFY(ROOT_PATH_CMAKE)
    #else
    #define ROOT_PATH ""
    #endif

    #ifdef MAX_CREDENTIAL_LENGTH_CMAKE
    #define MAX_CREDENTIAL_LENGTH MAX_CREDENTIAL_LENGTH_CMAKE
    #else
    #define MAX_CREDENTIAL_LENGTH -1
    #endif

    #ifdef INSERT_RECIPES_CMAKE
    #define INSERT_RECIPES INSERT_RECIPES_CMAKE
    #else
    #define INSERT_RECIPES false
    #endif

    #ifdef PATH_TO_RECIPES_CMAKE
    #define PATH_TO_RECIPES PATH_TO_RECIPES_CMAKE
    #else
    #define PATH_TO_RECIPES ""
    #endif

    #ifdef PATH_TO_RECIPE_IMAGES_CMAKE
    #define PATH_TO_RECIPE_IMAGES PATH_TO_RECIPE_IMAGES_CMAKE
    #else
    #define PATH_TO_RECIPE_IMAGES ""
    #endif

    #ifdef MAX_TITLE_LENGTH_CMAKE
    #define MAX_TITLE_LENGTH MAX_TITLE_LENGTH_CMAKE
    #else
    #define MAX_TITLE_LENGTH -1
    #endif

#endif // CMAKECONFIG_HPP
