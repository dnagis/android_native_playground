LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    vvnx.cpp

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libutils \
    libsqlite



LOCAL_MODULE:= sqlvvnx

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Wall -Werror -Wunused -Wunreachable-code

include $(BUILD_EXECUTABLE)
