// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "K4aLoader.h"

static void deloader_init_once(deloader_global_context_t* global);
static void deloader_deinit(void);

// Creates a function called deloader_global_context_t_get() which returns the initialized
// singleton global
K4A_DECLARE_GLOBAL(deloader_global_context_t, deloader_init_once);

// Load Depth Engine
static void deloader_init_once(deloader_global_context_t* global)
{
    // All members are initialized to zero

    ////k4a_result_t result = dynlib_create(K4A_PLUGIN_DYNAMIC_LIBRARY_NAME, K4A_PLUGIN_VERSION, &global->handle);
    //if (K4A_FAILED(result))
    //{
    //    //LOG_ERROR("Failed to Load Depth Engine Plugin (%s). Depth functionality will not work",
    //    //    K4A_PLUGIN_DYNAMIC_LIBRARY_NAME);
    //    //LOG_ERROR("Make sure the depth engine plugin is in your loaders path", 0);
    //}

    //if (K4A_SUCCEEDED(result))
    //{
    //    result = dynlib_find_symbol(global->handle, K4A_PLUGIN_EXPORTED_FUNCTION, (void**)&global->registerFn);
    //}

    //if (K4A_SUCCEEDED(result))
    //{
    //    //result = K4A_RESULT_FROM_BOOL(global->registerFn(&global->plugin));
    //    global->registerFn(&global->plugin);
    //}

    //if (K4A_SUCCEEDED(result))
    //{
    //    //result = K4A_RESULT_FROM_BOOL(verify_plugin(&global->plugin));
    //}

    //if (K4A_SUCCEEDED(result))
    //{
    //    global->loaded = true;
    //}
}

static bool is_plugin_loaded(deloader_global_context_t* global)
{
    return global->loaded;
}


void deloader_deinit(void)
{
    deloader_global_context_t* global = deloader_global_context_t_get();

    if (global->handle)
    {
        dynlib_destroy(global->handle);
    }
}

K4aLoader::K4aLoader(std::string dll, deloader_global_context_t* global)
{
    printf("Start loading dynamic libraries.\n");
    k4a_result_t result = dynlib_create(dll.c_str(), &global->handle);
    if (K4A_SUCCEEDED(result))
    {
        printf("Dynamic library loaded.\n");
    }
    else
    {
        printf("Dynamic library loading failure.\n");
    }

    printf("Start initializing function pointers.\n");
    if (K4A_SUCCEEDED(result))
    {
        result = dynlib_find_symbol(global->handle, "k4a_device_get_installed_count", (void**)&functionList.k4a_device_get_installed_count);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_installed_count function failed to load.\n");
        }
    }
    printf("Function pointer initialization complete.\n");

    if (K4A_SUCCEEDED(result))
    {
        global->loaded = true;
    }
    
}

K4aLoader::~K4aLoader() 
{
    deloader_deinit();
}
