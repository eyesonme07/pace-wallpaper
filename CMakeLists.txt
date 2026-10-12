cmake_minimum_required(VERSION 3.16)

project(F1_Live_Companion VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

set(BOOST_ROOT "C:/Boost/boost_1_90_0")
set(OpenCV_DIR "C:/OpenCV/opencv/build/x64/vc16/lib")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)

qt_standard_project_setup(REQUIRES 6.8)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

qt_add_executable(appF1_Live_Companion
    main.cpp
)

set_source_files_properties(qml/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(appF1_Live_Companion
    URI F1_Live_Companion
    QML_FILES
        Main.qml
        qml/CustomButton.qml
        qml/Wallpaper_page.qml
        qml/Widgets_page.qml
        qml/About_page.qml
        qml/Location_page.qml
        qml/Update_data_page.qml
        qml/Theme.qml
        SOURCES cpp/API.cpp
        SOURCES header/API.h
        SOURCES header/F1_Request.h
        SOURCES cpp/F1_Request.cpp
        SOURCES
        SOURCES header/NetworkAccess.h
        SOURCES header/headers.h
        SOURCES
        SOURCES cpp/settings.cpp
        SOURCES header/settings.h
        RESOURCES
        SOURCES cpp/wallpaper.cpp
        SOURCES header/wallpaper.h
        SOURCES header/qmlAdapter.h
        SOURCES
        SOURCES header/PrepareImage.h
        SOURCES cpp/PrepareImage.cpp
        SOURCES header/Backend.h
        SOURCES cpp/Backend.cpp
        SOURCES header/Logger.h

)

qt_add_resources(appF1_Live_Companion "source"
    PREFIX "/source"
    BASE "resource/ssl"          # без слэша в начале, путь относительно папки с CMakeLists.txt
    FILES
        resource/ssl/cacert.pem
)

set_target_properties(appF1_Live_Companion PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appF1_Live_Companion PRIVATE
    BOOST_JSON_NO_LIB
    BOOST_ALL_NO_LIB
)

target_link_libraries(appF1_Live_Companion PRIVATE ${OpenCV_LIBS})
target_link_libraries(appF1_Live_Companion
    PRIVATE Qt6::Quick
)
target_link_libraries(appF1_Live_Companion PRIVATE Qt6::Concurrent)
target_link_libraries(appF1_Live_Companion
    PRIVATE Boost::headers OpenSSL::SSL OpenSSL::Crypto
)

install(TARGETS appF1_Live_Companion
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)