cmake_minimum_required(VERSION 3.16)

project(UltraTTS.x86_64 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Use qt_wrap_cpp to explicitly run the moc compiler on the header file.
# The output is stored in the TTSApp_MOC_SOURCES variable.
qt_wrap_cpp(TTSApp_MOC_SOURCES
    tts_app.h
)

# Now, add the executable with all the necessary sources,
# including the moc-generated ones.
add_executable(UltraTTS.x86_64
    main.cpp
    tts_app.cpp
    ${TTSApp_MOC_SOURCES}
)

target_link_libraries(UltraTTS.x86_64
    PRIVATE
        Qt6::Widgets
)