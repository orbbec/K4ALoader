#pragma once
#ifndef K4A_LOADER_DLL_H
#define K4A_LOADER_DLL_H

#if(defined WIN32 || defined _WIN32 || defined WINCE)
#define OB_EXTENSION_API __declspec(dllexport)
#else
#define OB_EXTENSION_API
#endif

#include "k4atypes.h"

typedef struct instance
{
    void* context;
    bool loaded = false;
    uint32_t(*k4a_device_get_installed_count)(void);
    k4a_result_t(*k4a_device_open)(uint32_t index, k4a_device_t* device_handle);
    void (*k4a_device_close)(k4a_device_t device_handle);
    k4a_result_t(*k4a_device_get_calibration)(k4a_device_t device_handle,
        const k4a_depth_mode_t depth_mode,
        const k4a_color_resolution_t color_resolution,
        k4a_calibration_t* calibration);
    //k4a_result_t (*k4a_device_close)(uint32_t index, k4a_device_t* device_handle);
    //k4a_result_t (*k4a_device_get_version)(k4a_device_t device_handle, k4a_hardware_version_t* version);

}instance_t;

OB_EXTENSION_API instance_t k4a_load(const char* dll);

OB_EXTENSION_API void free_instance(instance_t instance);

#endif