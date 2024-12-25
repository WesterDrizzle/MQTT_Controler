# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.16)

# 定义项目名称和版本号，指定使用的语言
project(MQTT_Controler VERSION 0.1 LANGUAGES CXX)

# 开启自动UIC、MOC和RCC生成
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.7.3/mingw_64")
# 设置C++标准为C++17，并要求必须支持该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt包，优先查找Qt6，如果找不到则查找Qt5，要求必须包含Widgets组件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Mqtt)
# 根据Qt版本，查找相应的Widgets组件
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Mqtt)

# 定义项目源文件列表
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)
# set(QRC_SOURCE_FILES Resources.qrc)
# qt6_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
# SOURCE_GROUP("Resource Files" FILES ${QRC_SOURCE_FILES})



# 根据Qt版本选择不同的执行文件添加方式
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # 对于Qt6，使用qt_add_executable，并手动完成最终化
    qt_add_executable(MQTT_Controler
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Source.qrc
        imagebtn.h imagebtn.cpp
    )
    # 可以为Android平台设置包源目录
    # set_property(TARGET MQTT_Controler APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #              ${CMAKE_CURRENT_SOURCE_DIR}/android)
else()
    # 对于Qt5，根据目标平台选择库类型
    if(ANDROID)
        # 对于Android平台，添加共享库
        add_library(MQTT_Controler SHARED
            ${PROJECT_SOURCES}
        )
        # 设置Android包源目录
        # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        # 对于其他平台，添加执行文件
        add_executable(MQTT_Controler
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接项目到Qt的Widgets组件
target_include_directories(MQTT_Controler PRIVATE "build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug/MQTT_Controler_autogen/include" "./")
target_link_libraries(MQTT_Controler PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Mqtt)

# 为iOS和macOS设置捆绑标识符
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MQTT_Controler)
endif()
# 设置项目属性，包括版本号和平台特定属性
set_target_properties(MQTT_Controler PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 包含GNU安装目录标准
include(GNUInstallDirs)
# 定义项目的安装规则
install(TARGETS MQTT_Controler
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 对于Qt6，完成执行文件的最终化
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MQTT_Controler)
endif()# 设置CMake的最低版本要求
