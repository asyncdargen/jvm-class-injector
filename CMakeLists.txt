cmake_minimum_required(VERSION 3.28)
project(jvm_class_injector)

set(CMAKE_CXX_STANDARD 14)

add_library(jvm_class_injector SHARED dll/main.cpp dll/injector/injector.cpp dll/class/class_injector.cpp
        dll/class/classes.h)

set_target_properties(jvm_class_injector PROPERTIES OUTPUT_NAME "injector")

set(JAVA_DIR "../java")
set(INJECTING_CLASS_PATH "classes")
set(JAVA_VERSION "1.8")

add_custom_target(compile_injecting_classes
        COMMAND ${CMAKE_COMMAND} -E chdir ${JAVA_DIR} javac -source ${JAVA_VERSION} -target ${JAVA_VERSION} ${INJECTING_CLASS_PATH}/*.java
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