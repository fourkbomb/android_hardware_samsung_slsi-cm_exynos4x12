/*
 * Copyright (C) 2017 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <hardware/hardware.h>
#include <hardware/gralloc.h>
#include <utils/Log.h>
#include <dlfcn.h>
#include "gralloc_priv.h"

#ifndef VENDOR_GRALLOC
#define VENDOR_GRALLOC "/system/lib/hw/gralloc.exynos3.so"
#endif

int (*vendor_alloc)(alloc_device_t *dev,
		int w, int h, int format, int usage,
		buffer_handle_t *pHandle, int *pStride);

static int gralloc_alloc(alloc_device_t *dev,
		int w, int h, int format, int usage,
		buffer_handle_t *pHandle, int *pStride) {
	ALOGI("gralloc_alloc(%p, %d, %d, %d, %d, %p, %p)", dev, w, h, format, usage, pHandle, pStride);
	int res = vendor_alloc(dev, w, h, format, usage, pHandle, pStride);
	ALOGI("vendor_alloc() complete. pHandle => %p, pStride => %p", pHandle, pStride);

	// TODO inspect pHandle
	struct private_handle_t *hnd = (struct private_handle_t *)pHandle;
	ALOGI("private_handle_t: %d,%d,%d // 0x%x,0x%x,0x%x,0x%x // 0x%x,0x%x,0x%x,0x%x,0x%x",
			hnd->fd, hnd->fd1, hnd->fd2, hnd->magic, hnd->flags, hnd->size, hnd->offset,
			hnd->format, hnd->width, hnd->height, hnd->stride, hnd->vstride);

	return res;
}

struct gralloc_module_t HAL_MODULE_INFO_SYM;
static struct gralloc_module_t *realHalSym = NULL;

int gralloc_device_open(const hw_module_t* module, const char* name,
		hw_device_t** device) {

	void *lib;

	struct alloc_device_t **gralloc = (struct alloc_device_t **)device;

	lib = dlopen(VENDOR_GRALLOC, RTLD_LOCAL);
	if (!lib) {
		ALOGE("Failed to open vendor gralloc '" VENDOR_GRALLOC "': %s", dlerror());
		return -EINVAL;
	}

	realHalSym = (struct gralloc_module_t *)dlsym(lib, HAL_MODULE_INFO_SYM_AS_STR);
	if (!realHalSym) {
		ALOGE("Failed to locate gralloc module info sym '" HAL_MODULE_INFO_SYM_AS_STR "': %s", dlerror());
		goto dl_err;
	}

	int result = realHalSym->common.methods->open(realHalSym, name, device);
	if (result < 0) {
		ALOGE("Vendor gralloc open failed: %d", result);
		goto dl_err;
	}

	// shim alloc
	vendor_alloc = (*gralloc)->alloc;
	(*gralloc)->alloc = &gralloc_alloc;

	return 0;
	dl_err:
		dlclose(lib);
		return -EINVAL;
}

int gralloc_lock(gralloc_module_t const* module,
                        buffer_handle_t handle, int usage,
                        int l, int t, int w, int h,
                        void** vaddr) {
	if (realHalSym == NULL) return -EINVAL;
	return realHalSym->lock(realHalSym, handle, usage,
			l, t, w, h, vaddr);
}

int gralloc_unlock(gralloc_module_t const* module,
                          buffer_handle_t handle) {
	if (realHalSym == NULL) return -EINVAL;
	return realHalSym->unlock(realHalSym, handle);
}

int gralloc_register_buffer(gralloc_module_t const* module,
                                   buffer_handle_t handle) {
	if (realHalSym == NULL) return -EINVAL;
	return realHalSym->registerBuffer(realHalSym, handle);
}

int gralloc_unregister_buffer(gralloc_module_t const* module,
                                     buffer_handle_t handle) {
	if (realHalSym == NULL) return -EINVAL;
	return realHalSym->unregisterBuffer(realHalSym, handle);
}

static struct hw_module_methods_t gralloc_module_methods = {
	.open = gralloc_device_open
};

struct gralloc_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = GRALLOC_HARDWARE_MODULE_ID,
		.name = "Gralloc Shim Module",
		.author = "The LineageOS Project",
		.methods = &gralloc_module_methods
	},
	.registerBuffer = gralloc_register_buffer,
	.unregisterBuffer = gralloc_unregister_buffer,
	.lock = gralloc_lock,
	.unlock = gralloc_unlock
};
