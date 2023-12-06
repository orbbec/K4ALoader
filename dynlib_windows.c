// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This library
#include "dynlib.h"
#include "handle.h"

// Dependent libraries

// System dependencies
#include <windows.h>
#include <stdio.h>
#include <pathcch.h>

#define K4A_RESULT_FROM_BOOL(call) (call) ? K4A_RESULT_SUCCEEDED : K4A_RESULT_FAILED

#define TOSTRING(x) STRINGIFY(x)
#define VOID_VALUE

typedef struct _dynlib_context_t {
    HMODULE handle;
} dynlib_context_t;

K4A_DECLARE_CONTEXT(dynlib_t, dynlib_context_t);

//static char *generate_file_name(const char *name, uint32_t version) {
static char *generate_file_orbbec_name(const char *name) {
    // Format of the depth engine name is: depthengine_<K4A_PLUGIN_VERSION>_0
    //                                     depthengine_2_0
    //size_t max_buffer_size = strlen(name) + strlen(TOSTRING(DYNLIB_MAX_VERSION)) + strlen("_0") + strlen("_") + 1;
    size_t max_buffer_size = strlen(name) + strlen("\\") + strlen("OrbbecSDK.dll") + strlen("_") + 1;

    char *versioned_file_name = malloc(max_buffer_size);
    if(versioned_file_name == NULL) {
        // LOG_ERROR("malloc failed with size %llu", max_buffer_size);
        return NULL;
    }
    versioned_file_name[0] = '\0';

    // NOTE: _0 is appended to the name for legacy reasons, a time when the depth engine plugin version was tracked with
    // major and minor versions
    //snprintf(versioned_file_name, max_buffer_size, "%s_%u_0", name, version);
    snprintf(versioned_file_name, max_buffer_size, "%s\\OrbbecSDK.dll", name);

    return versioned_file_name;
}

static char* generate_file_k4a_name(const char* name) {
    // Format of the depth engine name is: depthengine_<K4A_PLUGIN_VERSION>_0
    //                                     depthengine_2_0
    //size_t max_buffer_size = strlen(name) + strlen(TOSTRING(DYNLIB_MAX_VERSION)) + strlen("_0") + strlen("_") + 1;
    size_t max_buffer_size = strlen(name) + strlen("\\") + strlen("k4a.dll") + strlen("_") + 1;

    char* versioned_file_name = malloc(max_buffer_size);
    if (versioned_file_name == NULL) {
        // LOG_ERROR("malloc failed with size %llu", max_buffer_size);
        return NULL;
    }
    versioned_file_name[0] = '\0';

    // NOTE: _0 is appended to the name for legacy reasons, a time when the depth engine plugin version was tracked with
    // major and minor versions
    //snprintf(versioned_file_name, max_buffer_size, "%s_%u_0", name, version);
    snprintf(versioned_file_name, max_buffer_size, "%s\\k4a.dll", name);

    return versioned_file_name;
}

//k4a_result_t dynlib_create(const char *name, uint32_t version, dynlib_t *dynlib_handle) {
k4a_result_t dynlib_create(const char *name, dynlib_t *dynlib_handle, dynlib_t* dynlib_orbbec_handle) {
    if (name == NULL)
    {
        printf("dll folder path is empty.\n");
        return K4A_RESULT_FAILED;
    }

    char* versioned_orbbec_name = generate_file_orbbec_name(name);
    char *versioned_k4a_name = generate_file_k4a_name(name);
    if(versioned_orbbec_name == NULL && versioned_k4a_name == NULL) {
        return K4A_RESULT_FAILED;
    }

    dynlib_context_t *dynlib = dynlib_t_create(dynlib_handle);
    k4a_result_t      result = K4A_RESULT_FROM_BOOL(dynlib != NULL);

    dynlib_context_t* dynlib_orbbec = dynlib_t_create(dynlib_orbbec_handle);
    result = K4A_RESULT_FROM_BOOL(dynlib != NULL);

    if(K4A_SUCCEEDED(result)) {
        dynlib_orbbec->handle = LoadLibraryA(versioned_orbbec_name, NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);
        result         = (dynlib_orbbec->handle != NULL) ? K4A_RESULT_SUCCEEDED : K4A_RESULT_FAILED;
        if (K4A_SUCCEEDED(result)){
            dynlib->handle = LoadLibraryA(versioned_k4a_name, NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);
            result = (dynlib->handle != NULL) ? K4A_RESULT_SUCCEEDED : K4A_RESULT_FAILED;
        }
        else
        {
            printf("Failed to load dll for OrbbecSDK.\n");
        }
    }

    if(versioned_orbbec_name != NULL) {
        free(versioned_orbbec_name);
    }

    if (versioned_k4a_name != NULL) {
        free(versioned_k4a_name);
    }

    if(K4A_FAILED(result)) {
        dynlib_t_destroy(*dynlib_orbbec_handle);
        *dynlib_orbbec_handle = NULL;
        dynlib_t_destroy(*dynlib_handle);
        *dynlib_handle = NULL;
    }

    return result;
}

k4a_result_t dynlib_find_symbol(dynlib_t dynlib_handle, const char *symbol, void **address) {

    k4a_result_t      result = K4A_RESULT_SUCCEEDED;
    dynlib_context_t *dynlib = dynlib_t_get_context(dynlib_handle);

    void *ptr = (void *)GetProcAddress(dynlib->handle, symbol);
    result    = K4A_RESULT_FROM_BOOL(ptr != NULL);

    if(K4A_SUCCEEDED(result)) {
        *address = ptr;
    }
    else {
        printf("Failed to find symbol %s in dynamic library, GLE is 0x%08x", symbol, GetLastError());
    }

    return result;
}

void dynlib_destroy(dynlib_t dynlib_handle, dynlib_t dynlib_orbbec_handle) {
    //orbbecsdk
    dynlib_context_t* dynlib_orbbec = dynlib_t_get_context(dynlib_orbbec_handle);

    BOOL freed = FreeLibrary(dynlib_orbbec->handle);
    if (!freed) {
        printf("Failed to unload dynamic orbbec library.\n");
    }

    dynlib_t_destroy(dynlib_orbbec_handle);
    dynlib_orbbec = NULL;
    //k4a
    dynlib_context_t *dynlib = dynlib_t_get_context(dynlib_handle);

    freed = FreeLibrary(dynlib->handle);
    if(!freed) {
        printf("Failed to unload dynamic k4a library.\n");
    }

    dynlib_t_destroy(dynlib_handle);
    dynlib = NULL;
}
