cmake_minimum_required(VERSION 3.28)
project(jvm_class_injector)

set(CMAKE_CXX_STANDARD 11)

add_library(jvm_class_injector SHARED dll/main.cpp dll/injector/injector.cpp dll/class/class_injector.cpp
        dll/class/classes.h)

set_target_properties(jvm_class_injector PROPERTIES
        OUTPUT_NAME "injector"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
        PREFIX "")

set(INJECTING_CLASS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/injecting-classes")
set(JAVA_VERSION "1.8")

set(JAVA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/java")

add_custom_target(compile_injecting_classes
        COMMAND ${CMAKE_COMMAND} -E chdir ${INJECTING_CLASS_PATH} javac -source ${JAVA_VERSION} -target ${JAVA_VERSION} *.java
        COMMENT "Compile injecting classes"
)
add_custom_target(compile_header_preparer_class
        COMMAND ${CMAKE_COMMAND} -E chdir ${JAVA_DIR} javac -source ${JAVA_VERSION} -target ${JAVA_VERSION} JavaClassPreparer.java
        COMMENT "Compile header preparer class"
)
add_custom_target(prepare_injecting_classes_header
        COMMAND ${CMAKE_COMMAND} -E chdir ${JAVA_DIR} java JavaClassPreparer ${INJECTING_CLASS_PATH}
        DEPENDS compile_injecting_classes compile_header_preparer_class
        COMMENT "Prepare injecting classes header"
)

add_dependencies(jvm_class_injector prepare_injecting_classes_header)