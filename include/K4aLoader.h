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
    //k4a_result_t (*k4a_set_debug_message_handler)(k4a_logging_message_cb_t* message_cb, void* message_cb_context, k4a_log_level_t min_level);
    //k4a_result_t (*k4a_set_allocator)(k4a_memory_allocate_cb_t allocate, k4a_memory_destroy_cb_t free);
    //k4a_result_t (*k4a_device_open)(uint32_t index, k4a_device_t* device_handle);
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