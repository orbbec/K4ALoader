#include "k4aLoader.h"
#include "dynlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <map>

typedef struct
{
    dynlib_t orbbec_handle;
    dynlib_t handle; // k4a
} deloader_context_t;

std::map<const char*, k4a_runtime_handle*> created_runtime_handle_map;

char* map_default_key;

void assignIfNotEqual(char** mutableString, const char* constString) {
    if (*mutableString == NULL || strcmp(*mutableString, constString) != 0) {
        size_t length = strlen(constString) + 1;
        char* newString = (char*)malloc(length * sizeof(char));
        if (newString == NULL) {
            // 处理内存分配失败的情况
            fprintf(stderr, "Memory allocation failed.\n");
            return;
        }
        strcpy(newString, constString);

        free(*mutableString);  // 释放旧的内存空间
        *mutableString = newString;
    }
}

k4a_runtime_handle* k4aloader_create_runtime_handle(const char* dll)
{
    for (auto it = created_runtime_handle_map.begin(); it != created_runtime_handle_map.end(); ++it) {
        if (it->first == dll)
        {
            assignIfNotEqual(&map_default_key, it->first);
            it->second->ref++;
            printf("Dynamic library loaded.\n");
            return it->second;
        }
    }

    deloader_context_t *ctx = new(deloader_context_t);
    k4a_runtime_handle* kl_handle = (k4a_runtime_handle*)malloc(sizeof(k4a_runtime_handle));
    kl_handle->ref = 0;
    printf("Start loading dynamic libraries.\n");
    k4a_result_t result = dynlib_create(dll, &ctx->handle, &ctx->orbbec_handle);
    if (K4A_FAILED(result))
    {
        printf("Dynamic library loading failure.\n");
    }
    kl_handle->context = (void*)ctx;

    printf("Start initializing function pointers.\n");
    if (K4A_SUCCEEDED(result))
    {
        dynlib_t hdl = ((deloader_context_t*)kl_handle->context)->handle;
        k4a_result_t result_fu = dynlib_find_symbol(hdl, "k4a_device_get_installed_count", (void **)&kl_handle->k4a_device_get_installed_count);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_installed_count function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_set_debug_message_handler", (void**)&kl_handle->k4a_set_debug_message_handler);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_set_debug_message_handler function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_set_allocator", (void**)&kl_handle->k4a_set_allocator);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_set_allocator function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_calibration", (void **)&kl_handle->k4a_device_get_calibration);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_calibration function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_open", (void **)&kl_handle->k4a_device_open);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_open function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_close", (void **)&kl_handle->k4a_device_close);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_close function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_capture", (void**)&kl_handle->k4a_device_get_capture);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_capture function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_imu_sample", (void**)&kl_handle->k4a_device_get_imu_sample);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_imu_sample function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_create", (void**)&kl_handle->k4a_capture_create);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_create function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_release", (void**)&kl_handle->k4a_capture_release);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_release function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_reference", (void**)&kl_handle->k4a_capture_reference);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_reference function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_get_color_image", (void**)&kl_handle->k4a_capture_get_color_image);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_get_color_image function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_get_depth_image", (void**)&kl_handle->k4a_capture_get_depth_image);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_get_depth_image function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_get_ir_image", (void**)&kl_handle->k4a_capture_get_ir_image);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_get_ir_image function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_set_color_image", (void**)&kl_handle->k4a_capture_set_color_image);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_set_color_image function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_set_depth_image", (void**)&kl_handle->k4a_capture_set_depth_image);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_set_depth_image function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_set_ir_image", (void**)&kl_handle->k4a_capture_set_ir_image);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_set_ir_image function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_set_temperature_c", (void**)&kl_handle->k4a_capture_set_temperature_c);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_set_temperature_c function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_capture_get_temperature_c", (void**)&kl_handle->k4a_capture_get_temperature_c);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_capture_get_temperature_c function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_create", (void**)&kl_handle->k4a_image_create);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_create function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_create_from_buffer", (void**)&kl_handle->k4a_image_create_from_buffer);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_create_from_buffer function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_buffer", (void**)&kl_handle->k4a_image_get_buffer);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_buffer function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_size", (void**)&kl_handle->k4a_image_get_size);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_size function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_format", (void**)&kl_handle->k4a_image_get_format);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_format function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_width_pixels", (void**)&kl_handle->k4a_image_get_width_pixels);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_width_pixels function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_height_pixels", (void**)&kl_handle->k4a_image_get_height_pixels);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_height_pixels function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_stride_bytes", (void**)&kl_handle->k4a_image_get_stride_bytes);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_stride_bytes function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_timestamp_usec", (void**)&kl_handle->k4a_image_get_timestamp_usec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_timestamp_usec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_device_timestamp_usec", (void**)&kl_handle->k4a_image_get_device_timestamp_usec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_device_timestamp_usec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_system_timestamp_nsec", (void**)&kl_handle->k4a_image_get_system_timestamp_nsec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_system_timestamp_nsec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_exposure_usec", (void**)&kl_handle->k4a_image_get_exposure_usec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_exposure_usec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_white_balance", (void**)&kl_handle->k4a_image_get_white_balance);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_white_balance function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_get_iso_speed", (void**)&kl_handle->k4a_image_get_iso_speed);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_get_iso_speed function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_set_device_timestamp_usec", (void**)&kl_handle->k4a_image_set_device_timestamp_usec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_set_device_timestamp_usec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_set_timestamp_usec", (void**)&kl_handle->k4a_image_set_timestamp_usec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_set_timestamp_usec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_set_system_timestamp_nsec", (void**)&kl_handle->k4a_image_set_system_timestamp_nsec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_set_system_timestamp_nsec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_set_exposure_usec", (void**)&kl_handle->k4a_image_set_exposure_usec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_set_exposure_usec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_set_exposure_time_usec", (void**)&kl_handle->k4a_image_set_exposure_time_usec);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_set_exposure_time_usec function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_set_white_balance", (void**)&kl_handle->k4a_image_set_white_balance);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_set_white_balance function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_set_iso_speed", (void**)&kl_handle->k4a_image_set_iso_speed);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_set_iso_speed function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_reference", (void**)&kl_handle->k4a_image_reference);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_reference function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_image_release", (void**)&kl_handle->k4a_image_release);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_image_release function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_start_cameras", (void**)&kl_handle->k4a_device_start_cameras);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_start_cameras function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_stop_cameras", (void**)&kl_handle->k4a_device_stop_cameras);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_stop_cameras function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_start_imu", (void**)&kl_handle->k4a_device_start_imu);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_start_imu function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_stop_imu", (void**)&kl_handle->k4a_device_stop_imu);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_stop_imu function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_serialnum", (void**)&kl_handle->k4a_device_get_serialnum);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_serialnum function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_version", (void**)&kl_handle->k4a_device_get_version);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_version function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_color_control_capabilities", (void**)&kl_handle->k4a_device_get_color_control_capabilities);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_color_control_capabilities function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_color_control", (void**)&kl_handle->k4a_device_get_color_control);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_color_control function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_set_color_control", (void**)&kl_handle->k4a_device_set_color_control);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_set_color_control function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_raw_calibration", (void**)&kl_handle->k4a_device_get_raw_calibration);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_raw_calibration function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_calibration", (void**)&kl_handle->k4a_device_get_calibration);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_calibration function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_device_get_sync_jack", (void**)&kl_handle->k4a_device_get_sync_jack);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_device_get_sync_jack function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_calibration_get_from_raw", (void**)&kl_handle->k4a_calibration_get_from_raw);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_calibration_get_from_raw function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_calibration_3d_to_3d", (void**)&kl_handle->k4a_calibration_3d_to_3d);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_calibration_3d_to_3d function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_calibration_2d_to_3d", (void**)&kl_handle->k4a_calibration_2d_to_3d);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_calibration_2d_to_3d function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_calibration_3d_to_2d", (void**)&kl_handle->k4a_calibration_3d_to_2d);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_calibration_3d_to_2d function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_calibration_2d_to_2d", (void**)&kl_handle->k4a_calibration_2d_to_2d);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_calibration_2d_to_2d function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_calibration_color_2d_to_depth_2d", (void**)&kl_handle->k4a_calibration_color_2d_to_depth_2d);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_calibration_color_2d_to_depth_2d function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_transformation_create", (void**)&kl_handle->k4a_transformation_create);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_transformation_create function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_transformation_destroy", (void**)&kl_handle->k4a_transformation_destroy);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_transformation_destroy function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_transformation_depth_image_to_color_camera", (void**)&kl_handle->k4a_transformation_depth_image_to_color_camera);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_transformation_depth_image_to_color_camera function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_transformation_depth_image_to_color_camera_custom", (void**)&kl_handle->k4a_transformation_depth_image_to_color_camera_custom);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_transformation_depth_image_to_color_camera_custom function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_transformation_color_image_to_depth_camera", (void**)&kl_handle->k4a_transformation_color_image_to_depth_camera);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_transformation_color_image_to_depth_camera function failed to load.\n");
        }
        result_fu = dynlib_find_symbol(hdl, "k4a_transformation_depth_image_to_point_cloud", (void**)&kl_handle->k4a_transformation_depth_image_to_point_cloud);
        if (K4A_FAILED(result_fu))
        {
            printf("k4a_transformation_depth_image_to_point_cloud function failed to load.\n");
        }
    }
    printf("Function pointer initialization complete.\n");

    if (K4A_FAILED(result))
    {
        free(kl_handle);
        kl_handle = NULL;
    }

    if (kl_handle != NULL)
    {
        assignIfNotEqual(&map_default_key, dll);
        kl_handle->ref++;
        created_runtime_handle_map.insert(std::make_pair(dll, kl_handle));
    }

    return kl_handle;
}

void k4aloader_free_runtime_handle(k4a_runtime_handle* kl_handle)
{
    for (auto it = created_runtime_handle_map.begin(); it != created_runtime_handle_map.end(); ++it) {
        if (it->second == kl_handle)
        {         
            if ((it->second->ref - 1) == 0)
            {
                if (kl_handle->context)
                {
                    dynlib_destroy(((deloader_context_t*)kl_handle->context)->handle, ((deloader_context_t*)kl_handle->context)->orbbec_handle);
                    delete kl_handle->context;
                    kl_handle->context = nullptr;
                    printf("The k4a_runtime_handle has been released.\n");
                }
                free(kl_handle);
                kl_handle = NULL;
                map_default_key = NULL;
                if (created_runtime_handle_map.size() > 0)
                {
                    assignIfNotEqual(&map_default_key, created_runtime_handle_map.begin()->first);
                }
            }
            else
            {
                printf("This k4a_runtime_handle reference count is not 0 and cannot be released.\n");
            }
        }
    }
}

k4a_runtime_handle* get_current_k4a_runtime_handle()
{
    for (auto it = created_runtime_handle_map.begin(); it != created_runtime_handle_map.end(); ++it) 
    {
        if (strcmp(map_default_key, it->first) == 0)
        {
            it->second->ref++;
            return it->second;
        }
    }
    return nullptr;
}

bool switch_k4a_runtime_handle(const char* dll)
{
    for (auto it = created_runtime_handle_map.begin(); it != created_runtime_handle_map.end(); ++it) {
        if (it->first == dll)
        {
            assignIfNotEqual(&map_default_key, it->first);
            printf("Default runtime_handle switch succeeds.\n");
            return true;
        }
    }
    printf("The runtime_handle corresponding to the input dynamic library path does not exist and the switch fails.\n");
    return false;
}