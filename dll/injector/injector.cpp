#include "injector.h"

#include <windows.h>

#include "../class/class_injector.h"
#include "../jvm/jni.h"

typedef jint (JNICALL*GetCreatedJavaVMs)(JavaVM **, jsize, jsize *);

static HMODULE GetJvmDLL() {
    return GetModuleHandleA("jvm.dll");
}

static GetCreatedJavaVMs Get_GetCreatedJavaVMs(HMODULE jvm_dll) {
    return reinterpret_cast<GetCreatedJavaVMs>(GetProcAddress(jvm_dll, "JNI_GetCreatedJavaVMs"));
}

static jsize GetVMsCount(GetCreatedJavaVMs jvm_getter) {
    jsize nVMs;
    jvm_getter(nullptr, 0, &nVMs);
    return nVMs;
}

static JavaVM **GetVMs(GetCreatedJavaVMs jvm_getter, jsize vms_count) {
    auto **buffer = static_cast<JavaVM **>(malloc(vms_count * sizeof(JavaVM *)));
    jvm_getter(buffer, vms_count, &vms_count);
    return buffer;
}

void Debug(const wchar_t *error) {
    MessageBoxW(NULL, error, L"Jvm Class Injector", MB_OK | MB_ICONEXCLAMATION);
}

void FindJVMsAndInject() {
    Beep(1000, 200);

    auto jvm_dll = GetJvmDLL();
    if (jvm_dll == nullptr) {
        Debug(L"Not found jvm.dll");
        Beep(1000, 1500);
        return;
    }

    auto jvm_getter = Get_GetCreatedJavaVMs(jvm_dll);
    if (jvm_getter == nullptr) {
        Debug(L"JNI GetCreatedJavaVMs is null");
        Beep(1000, 1500);
        return;
    }

    auto vms_count = GetVMsCount(jvm_getter);
    auto vms = GetVMs(jvm_getter, vms_count);

    if (vms_count > 0) {
        for (jsize i = 0; i < vms_count; i++) {
            JavaVM *jvm = vms[i];
            JNIEnv *env;

            jvm->AttachCurrentThread(reinterpret_cast<void **>(&env), nullptr);
            jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_8);

            if (!env) {
                Debug(L"Couldn`t get env");
                Beep(1000, 1500);
                jvm->DetachCurrentThread();

                break;
            }

            InjectClasses(env);

            jvm->DetachCurrentThread();
        }
    }
}
