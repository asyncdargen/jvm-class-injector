#include "class_injector.h"

#include "classes.h"
#include "../injector/injector.h"
#include "../jvm/jni.h"

jobject GetClassLoader(JNIEnv* env) {
    return nullptr;
}

void InjectClasses(JNIEnv* env) {
    for (int i = 0; i < sizeof(classes_sizes) / 4; i++) {
        auto class_bytes = classes_bytes[i];
        auto class_size = classes_sizes[i];

        auto defined_class = env->DefineClass(nullptr, GetClassLoader(env), class_bytes, class_size);
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
