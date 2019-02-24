/*
***
* frameworks/base/cmds/
* 
* adb push out/target/product/mido/system/bin/jnivvnx /system/bin
* 
* basé sur cmds/app_process: avoir un JNIEnv
* 
* sans libandroid_runtime dans LOCAL_SHARED_LIBRARIES de l'Android.mk -> 'android_runtime/AndroidRuntime.h' file not found
* 
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <binder/IPCThreadState.h>
#include <hwbinder/IPCThreadState.h>
#include <android_runtime/AndroidRuntime.h>

namespace android {

class AppRuntime : public AndroidRuntime
{
public:
    AppRuntime(char* argBlockStart, const size_t argBlockLength) //Piste: AndroidRuntime.cpp --> li 
        : AndroidRuntime(argBlockStart, argBlockLength)
        , mClass(NULL)
    {
    }

    void setClassNameAndArgs(const String8& className, int argc, char * const *argv) {
        mClassName = className;
        for (int i = 0; i < argc; ++i) {
             mArgs.add(String8(argv[i]));
        }
    }

    virtual void onVmCreated(JNIEnv* env)
    {
        if (mClassName.isEmpty()) {
            return; // Zygote. Nothing to do here.
        }

        /*
         * This is a little awkward because the JNI FindClass call uses the
         * class loader associated with the native method we're executing in.
         * If called in onStarted (from RuntimeInit.finishInit because we're
         * launching "am", for example), FindClass would see that we're calling
         * from a boot class' native method, and so wouldn't look for the class
         * we're trying to look up in CLASSPATH. Unfortunately it needs to,
         * because the "am" classes are not boot classes.
         *
         * The easiest fix is to call FindClass here, early on before we start
         * executing boot class Java code and thereby deny ourselves access to
         * non-boot classes.
         */
        char* slashClassName = toSlashClassName(mClassName.string());
        mClass = env->FindClass(slashClassName);
        if (mClass == NULL) {
            ALOGE("ERROR: could not find class '%s'\n", mClassName.string());
        }
        free(slashClassName);

        mClass = reinterpret_cast<jclass>(env->NewGlobalRef(mClass));
    }

    virtual void onStarted()
    {
        sp<ProcessState> proc = ProcessState::self();
        ALOGV("App process: starting thread pool.\n");
        proc->startThreadPool();

        AndroidRuntime* ar = AndroidRuntime::getRuntime();
        ar->callMain(mClassName, mClass, mArgs);

        IPCThreadState::self()->stopProcess();
        hardware::IPCThreadState::self()->stopProcess();
    }

    virtual void onZygoteInit()
    {
        sp<ProcessState> proc = ProcessState::self();
        ALOGV("App process: starting thread pool.\n");
        proc->startThreadPool();
    }

    virtual void onExit(int code)
    {
        if (mClassName.isEmpty()) {
            // if zygote
            IPCThreadState::self()->stopProcess();
            hardware::IPCThreadState::self()->stopProcess();
        }

        AndroidRuntime::onExit(code);
    }


    String8 mClassName;
    Vector<String8> mArgs;
    jclass mClass;
};

}

using namespace android;



int main()
{



    fprintf(stdout, "Début de main de jnivvnx...\n");
    

    

    _exit(0);
}
