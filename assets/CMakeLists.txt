file(GLOB FOLDER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.png)

foreach(RESOURCE ${FOLDER_FILES})
    add_files(${RESOURCE} "RESOURCES")
endforeach()




