#pragma once

#include "k4aplugin.h"
#include "k4a.h"
#include "k4atypes.h"
#include "global.h"
//#include <k4ainternal/logging.h>
#include "dynlib.h"
#include <string>

typedef struct
{
    k4a_plugin_t plugin;
    dynlib_t handle;
    k4a_register_plugin_fn registerFn;
    volatile bool loaded;
} deloader_global_context_t;

typedef struct _fun_instance_
{
    uint32_t     (*k4a_device_get_installed_count)(void);
    k4a_result_t (*k4a_device_open)(uint32_t index, k4a_device_t* device_handle);
    void (*k4a_device_close)(k4a_device_t device_handle);
    k4a_result_t (*k4a_device_get_calibration)(k4a_device_t device_handle,
        const k4a_depth_mode_t depth_mode,
        const k4a_color_resolution_t color_resolution,
        k4a_calibration_t* calibration);
    //k4a_result_t (*k4a_device_close)(uint32_t index, k4a_device_t* device_handle);
    //k4a_result_t (*k4a_device_get_version)(k4a_device_t device_handle, k4a_hardware_version_t* version);

} functionInstance;

class K4aLoader {
public:
	K4aLoader(std::string dll, deloader_global_context_t* global);
    ~K4aLoader();

private:

public:
    functionInstance functionList;
};