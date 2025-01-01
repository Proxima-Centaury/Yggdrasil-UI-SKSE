cmake_minimum_required(VERSION 3.21)

project(_Yggdrasil_UI VERSION 1.0.0 LANGUAGES CXX)

set(PROJECT_DISPLAY_NAME "Yggdrasil UI")

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")

    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_DISPLAY_NAME}")

endif()

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(OpenAL REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES
        plugin.cpp

        script/configuration.cpp
        script/log.cpp
        script/sfx.cpp
        script/translations.cpp
        script/ui.cpp
        script/sfx.cpp
        script/translations.cpp
        script/ui.cpp
        script/yggdrasil.cpp

        script/menu/shared.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)

if(DEFINED OUTPUT_FOLDER)

    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")

        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )

    endif()

endif()