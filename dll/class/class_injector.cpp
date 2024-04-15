#include "class_injector.h"

#include "classes.h"
#include "../injector/injector.h"
#include "../jvm/jni.h"

void InjectClasses(JNIEnv* env) {
    Debug(L"Pre 52");
    jclass systemClass = env->FindClass("java/lang/System");
    jfieldID outField = env->GetStaticFieldID(systemClass, "out", "Ljava/io/PrintStream;");

    jclass printStreamClass = env->FindClass("java/io/PrintStream");
    jmethodID print = env->GetMethodID(printStreamClass, "println", "(Ljava/lang/String;)V");

    jobject printStream = env->GetStaticObjectField(systemClass, outField);
    env->CallVoidMethod(printStream, print, env->NewStringUTF("penis"));


    for (int i = 0; i < sizeof(classes_sizes) / 4; i++) {
        auto class_bytes = classes_bytes[i];
        auto class_size = classes_sizes[i];

        auto defined_class = env->DefineClass(nullptr, nullptr, class_bytes, class_size);
        if (defined_class == nullptr) {
            Debug(L"Couldn`t define class");
            continue;
        }

        auto inject_method = env->GetStaticMethodID(defined_class, "inject", "()V");
        if (inject_method == nullptr) {
            Debug(L"Not found inject method");
            continue;
        }
        env->CallStaticVoidMethod(defined_class, inject_method);
    }

}
