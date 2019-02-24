LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# Only compile source java files
LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_MODULE := jarvvnx

LOCAL_SDK_VERSION := current

LOCAL_DEX_PREOPT := false

include $(BUILD_JAVA_LIBRARY)

# Use the following include to make our test apk.
include $(call all-makefiles-under,$(LOCAL_PATH))
