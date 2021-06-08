cmake_minimum_required(VERSION 3.11)
set(TARGET_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE RESOURCES_FILES "*.qrc")

# Configure Qt
find_package( Qt5 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Charts )

add_executable(${TARGET_NAME} ${SOURCES})

target_sources(${TARGET_NAME} PRIVATE ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCES_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2
    Qt5::Charts
)
