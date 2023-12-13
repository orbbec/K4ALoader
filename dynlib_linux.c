// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#define _GNU_SOURCE // ldaddr() extention in dlfcn.h

// This library
#include "dynlib.h"
#include "handle.h"

// System dependencies
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <dlfcn.h>

#define K4A_RESULT_FROM_BOOL(call) (call) ? K4A_RESULT_SUCCEEDED : K4A_RESULT_FAILED

#define TOSTRING(x) STRINGIFY(x)

typedef struct _dynlib_context_t
{
    void *handle;
} dynlib_context_t;

K4A_DECLARE_CONTEXT(dynlib_t, dynlib_context_t);
// static char *generate_file_name(const char *name, uint32_t version) {
static char *generate_file_orbbec_name(const char *name)
{
    const char *lib_prefix = "lib";
    const char *lib_suffix = "so";

    size_t max_buffer_size =
        strlen(name) + strlen("/") + strlen(".") + strlen(lib_suffix) + strlen("libOrbbecSDK") + 1;

    char *versioned_file_name = malloc(max_buffer_size);
    if (versioned_file_name == NULL)
    {
        // LOG_ERROR("malloc failed with size %llu", max_buffer_size);
        return NULL;
    }
    versioned_file_name[0] = '\0';
    // NOTE: 0 is appended to the name for legacy reasons, a time when the depth engine plugin version was tracked with
    // major and minor versions
    // snprintf(versioned_file_name, max_buffer_size, "%s%s.%s.%u.0", lib_prefix, name, lib_suffix, version);
    snprintf(versioned_file_name, max_buffer_size, "%s/libOrbbecSDK.%s", name, lib_suffix);
    printf("%s", versioned_file_name);
    return versioned_file_name;
}

static char *generate_file_k4a_name(const char *name)
{
    const char *lib_prefix = "lib";
    const char *lib_suffix = "so";

    size_t max_buffer_size =
        strlen(name) + strlen("/") + strlen(".") + strlen(lib_suffix) + strlen("libk4a") + 1;

    char *versioned_file_name = malloc(max_buffer_size);
    if (versioned_file_name == NULL)
    {
        // LOG_ERROR("malloc failed with size %llu", max_buffer_size);
        return NULL;
    }
    versioned_file_name[0] = '\0';
    // NOTE: 0 is appended to the name for legacy reasons, a time when the depth engine plugin version was tracked with
    // major and minor versions
    // snprintf(versioned_file_name, max_buffer_size, "%s%s.%s.%u.0", lib_prefix, name, lib_suffix, version);
    snprintf(versioned_file_name, max_buffer_size, "%s/libk4a.%s", name, lib_suffix);

    return versioned_file_name;
}
// k4a_result_t dynlib_create(const char *name, uint32_t version, dynlib_t *dynlib_handle) {
k4a_result_t dynlib_create(const char *name, dynlib_t *dynlib_handle, dynlib_t* dynlib_orbbec_handle)
{
    if (name == NULL)
    {
        printf("so folder path is empty.\n");
        return K4A_RESULT_FAILED;
    }

    char *versioned_orbbec_name = generate_file_orbbec_name(name);
    char *versioned_k4a_name = generate_file_k4a_name(name);
    if (versioned_orbbec_name == NULL && versioned_k4a_name == NULL)
    {
        return K4A_RESULT_FAILED;
    }

    dynlib_context_t *dynlib = dynlib_t_create(dynlib_handle);
    k4a_result_t      result = K4A_RESULT_FROM_BOOL(dynlib != NULL);

    dynlib_context_t* dynlib_orbbec = dynlib_t_create(dynlib_orbbec_handle);
    result = K4A_RESULT_FROM_BOOL(dynlib != NULL);

    if (K4A_SUCCEEDED(result))
    {
        dynlib_orbbec->handle = dlopen(versioned_orbbec_name, RTLD_NOW);

        dynlib->handle = dlopen(versioned_k4a_name, RTLD_NOW);
        result = (dynlib->handle != NULL) ? K4A_RESULT_SUCCEEDED : K4A_RESULT_FAILED;
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
        printf("Failed to load dll.\n");
    }

    return result;
}

k4a_result_t dynlib_find_symbol(dynlib_t dynlib_handle, const char *symbol, void **address)
{
    k4a_result_t result = K4A_RESULT_SUCCEEDED;
    dynlib_context_t *dynlib = dynlib_t_get_context(dynlib_handle);

    void *ptr = dlsym(dynlib->handle, symbol);
    result = K4A_RESULT_FROM_BOOL(ptr != NULL);

    if (K4A_SUCCEEDED(result))
    {
        *address = ptr;
    }
    else
    {
        printf("Failed to find symbol %s in dynamic library. Error: %s\n", symbol, dlerror());
    }

    if (K4A_SUCCEEDED(result))
    {
        Dl_info info;
        if (dladdr(*address, &info) != 0)
        {
            printf("Depth Engine loaded %s\n", info.dli_fname);
        }
        else
        {
            printf("Failed calling dladdr %x\n", dlerror());
        }
    }

    return result;
}

void dynlib_destroy(dynlib_t dynlib_handle, dynlib_t dynlib_orbbec_handle)
{
    //orbbecsdk
    if(dynlib_orbbec_handle)
    {
        dynlib_context_t* dynlib_orbbec = dynlib_t_get_context(dynlib_orbbec_handle);

        dlclose(dynlib_orbbec->handle);

        dynlib_t_destroy(dynlib_orbbec_handle);
        dynlib_orbbec = NULL;
    }
    //k4a
    if (dynlib_handle)
    { 
        dynlib_context_t *dynlib = dynlib_t_get_context(dynlib_handle);

        dlclose(dynlib->handle);

        dynlib_t_destroy(dynlib_handle);
        dynlib = NULL;
    }
}
