# Copyright (C) 2013 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


LOCAL_PATH := $(call my-dir)

# HAL module implemenation stored in
# hw/<OVERLAY_HARDWARE_MODULE_ID>.<ro.product.board>.so
include $(CLEAR_VARS)

LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)/hw
LOCAL_SHARED_LIBRARIES := liblog libcutils libion libutils libGLESv1_CM

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../include \
	$(TOP)/hardware/samsung_slsi-cm/exynos/include \
	$(TOP)/hardware/samsung_slsi-cm/exynos4/include

LOCAL_SRC_FILES :=	\
	gralloc_shim.c

LOCAL_CFLAGS := -DLOG_TAG=\"gralloc.shim\"
$(warning $(WITH_OSS_GRALLOC))
ifeq ($(WITH_OSS_GRALLOC),true)
LOCAL_CFLAGS += -DVENDOR_GRALLOC=\"/system/vendor/lib/hw/gralloc.exynos4.so\"
else
LOCAL_CFLAGS += -DVENDOR_GRALLOC=\"/system/lib/hw/gralloc.exynos3.so\"
endif

LOCAL_MODULE := gralloc.smdk4x12
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
