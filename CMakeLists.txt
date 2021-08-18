cmake_minimum_required(VERSION 3.15)

project("AudioPluginScanner" VERSION 0.0.1)

message(${CMAKE_CURRENT_BINARY_DIR})

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../JUCE/ ${CMAKE_CURRENT_BINARY_DIR}/JUCE/)

juce_add_console_app(${PROJECT_NAME}
    PRODUCT_NAME    "AudioPluginScanner"
    PLUGINHOST_AU   TRUE)

target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        JUCE_PLUGINHOST_VST3=1
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${project_sources}
        juce::juce_audio_processors)
