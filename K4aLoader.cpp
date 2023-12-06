#include "k4aLoader.h"
#include "k4aplugin.h"
#include "dynlib.h"
#include <stdio.h>

typedef struct
{
    dynlib_t orbbec_handle;
    dynlib_t handle; // k4a
} deloader_context_t;

OB_EXTENSION_API instance_t k4a_load(const char* dll)
{
    deloader_context_t *ctx = new(deloader_context_t);
    instance_t instance;
    printf("Start loading dynamic libraries.\n");

    k4a_result_t result = dynlib_create(dll, &ctx->handle, &ctx->orbbec_handle);
    if (K4A_SUCCEEDED(result))
    {
        printf("Dynamic library loaded.\n");
    }
    else
    {
        printf("Dynamic library loading failure.\n");
    }
    instance.context = (void*)ctx;

    printf("Start initializing function pointers.\n");
    if (K4A_SUCCEEDED(result))
    {
        dynlib_t hdl = ((deloader_context_t*)instance.context)->handle;
        result = dynlib_find_symbol(hdl, "k4a_device_get_installed_count", (void **)&instance.k4a_device_get_installed_count);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_installed_count function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_calibration", (void **)&instance.k4a_device_get_calibration);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_calibration function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_open", (void **)&instance.k4a_device_open);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_open function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_close", (void **)&instance.k4a_device_close);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_close function failed to load.\n");
        }
    }
    printf("Function pointer initialization complete.\n");

    if (K4A_SUCCEEDED(result))
    {
        instance.loaded = true;
    }

    return instance;
}

OB_EXTENSION_API void free_instance(instance_t instance)
{
    if (instance.context)
    {
        dynlib_destroy(((deloader_context_t*)instance.context)->handle, ((deloader_context_t*)instance.context)->orbbec_handle);
        delete instance.context;
        instance.context = nullptr;
    }
}
