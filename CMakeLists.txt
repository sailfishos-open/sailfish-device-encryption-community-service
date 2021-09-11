cmake_minimum_required(VERSION 3.5)

project(sailfish-device-encryption-community-service
    LANGUAGES CXX
    VERSION 0.1
    DESCRIPTION "Service for Device Encryption"
    )

find_package (Qt5 COMPONENTS Core DBus LinguistTools REQUIRED)

include(FindPkgConfig)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SERVICE_NAME "org.sailfishos.open.device.encryption" CACHE STRING "Service name on DBus")

# sources

## C++ sources
file(GLOB CXX_SRC src/*.cpp)
file(GLOB CXX_HEADERS src/*.h)

add_custom_target(Sources SOURCES
    ${QML_SRC}
    ${CXX_SRC} ${CXX_HEADERS}
    rpm/service.spec
    )

## Translations
set(TRANSLATION_TS_FILES
        translations/en.ts
        )

set(TRANSLATION_SOURCE_FILES
        ${CXX_SRC}
        ${QML_SRC}
        )

qt5_create_translation(TRANSLATION_QM_FILES
        ${TRANSLATION_SOURCE_FILES}
        ${TRANSLATION_TS_FILES})

add_custom_target(translations ALL DEPENDS ${TRANSLATION_QM_FILES})

# compilation
add_compile_options(
    -DSERVICE_NAME="${SERVICE_NAME}"
    )

add_executable(sailfish-device-encryption-community-service ${CXX_SRC} ${CXX_HEADERS})

add_dependencies(sailfish-device-encryption-community-service translations)

target_compile_definitions(sailfish-device-encryption-community-service PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    )
target_include_directories(sailfish-device-encryption-community-service PRIVATE
    $<BUILD_INTERFACE:
    >)
target_link_libraries(sailfish-device-encryption-community-service
    Qt5::Core
    Qt5::DBus
    sfosdevenc
    )

# installs
install(TARGETS sailfish-device-encryption-community-service
    RUNTIME DESTINATION bin
    )
install(FILES ${TRANSLATION_QM_FILES}
    DESTINATION share/sailfish-device-encryption-community-service/translations)
# dbus
install(FILES dbus/org.sailfishos.open.device.encryption.conf
    DESTINATION share/dbus-1/system.d)
install(FILES dbus/org.sailfishos.open.device.encryption.service
    DESTINATION share/dbus-1/system-services)
