# JVM Class Injector
___
A utility for creating .dll with injection of installed classes into the JVM process.
___
### Requires
+ Java (Java Compiler) 1.8+
+ CMake
___
### Guide
+ Create java classes that will be injected in specified dir
    Injecting class must have an inject method (for example):
    ```java
    public class TestClass {
    
        public static void inject() {
            System.out.println("Hello from injected class!");
        }
    
    }
    ```
+ Run CMake
+ Take output injector.dll in out dir
___
### Configuration
In CMakeList.txt
```cmake
set(INJECTING_CLASS_PATH "classes") # Path to injecting classes (by default in java/classes)
set(JAVA_VERSION "1.8") # Java compiler target version
```
