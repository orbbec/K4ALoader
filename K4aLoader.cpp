#include "k4aLoader.h"
#include "dynlib.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    dynlib_t orbbec_handle;
    dynlib_t handle; // k4a
} deloader_context_t;

k4aloader_handle* k4aloader_dll_file_load(const char* dll)
{
    deloader_context_t *ctx = new(deloader_context_t);
    k4aloader_handle* instance = (k4aloader_handle*)malloc(sizeof(k4aloader_handle));
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
    instance->context = (void*)ctx;

    printf("Start initializing function pointers.\n");
    if (K4A_SUCCEEDED(result))
    {
        dynlib_t hdl = ((deloader_context_t*)instance->context)->handle;
        result = dynlib_find_symbol(hdl, "k4a_device_get_installed_count", (void **)&instance->k4a_device_get_installed_count);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_installed_count function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_set_debug_message_handler", (void**)&instance->k4a_set_debug_message_handler);
        if (K4A_FAILED(result))
        {
            printf("k4a_set_debug_message_handler function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_set_allocator", (void**)&instance->k4a_set_allocator);
        if (K4A_FAILED(result))
        {
            printf("k4a_set_allocator function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_calibration", (void **)&instance->k4a_device_get_calibration);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_calibration function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_open", (void **)&instance->k4a_device_open);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_open function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_close", (void **)&instance->k4a_device_close);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_close function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_capture", (void**)&instance->k4a_device_get_capture);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_capture function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_imu_sample", (void**)&instance->k4a_device_get_imu_sample);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_imu_sample function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_create", (void**)&instance->k4a_capture_create);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_create function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_release", (void**)&instance->k4a_capture_release);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_release function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_reference", (void**)&instance->k4a_capture_reference);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_reference function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_get_color_image", (void**)&instance->k4a_capture_get_color_image);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_get_color_image function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_get_depth_image", (void**)&instance->k4a_capture_get_depth_image);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_get_depth_image function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_get_ir_image", (void**)&instance->k4a_capture_get_ir_image);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_get_ir_image function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_set_color_image", (void**)&instance->k4a_capture_set_color_image);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_set_color_image function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_set_depth_image", (void**)&instance->k4a_capture_set_depth_image);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_set_depth_image function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_set_ir_image", (void**)&instance->k4a_capture_set_ir_image);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_set_ir_image function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_set_temperature_c", (void**)&instance->k4a_capture_set_temperature_c);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_set_temperature_c function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_capture_get_temperature_c", (void**)&instance->k4a_capture_get_temperature_c);
        if (K4A_FAILED(result))
        {
            printf("k4a_capture_get_temperature_c function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_create", (void**)&instance->k4a_image_create);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_create function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_create_from_buffer", (void**)&instance->k4a_image_create_from_buffer);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_create_from_buffer function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_buffer", (void**)&instance->k4a_image_get_buffer);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_buffer function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_size", (void**)&instance->k4a_image_get_size);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_size function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_format", (void**)&instance->k4a_image_get_format);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_format function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_width_pixels", (void**)&instance->k4a_image_get_width_pixels);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_width_pixels function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_height_pixels", (void**)&instance->k4a_image_get_height_pixels);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_height_pixels function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_stride_bytes", (void**)&instance->k4a_image_get_stride_bytes);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_stride_bytes function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_timestamp_usec", (void**)&instance->k4a_image_get_timestamp_usec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_timestamp_usec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_device_timestamp_usec", (void**)&instance->k4a_image_get_device_timestamp_usec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_device_timestamp_usec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_system_timestamp_nsec", (void**)&instance->k4a_image_get_system_timestamp_nsec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_system_timestamp_nsec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_exposure_usec", (void**)&instance->k4a_image_get_exposure_usec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_exposure_usec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_white_balance", (void**)&instance->k4a_image_get_white_balance);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_white_balance function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_get_iso_speed", (void**)&instance->k4a_image_get_iso_speed);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_get_iso_speed function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_set_device_timestamp_usec", (void**)&instance->k4a_image_set_device_timestamp_usec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_set_device_timestamp_usec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_set_timestamp_usec", (void**)&instance->k4a_image_set_timestamp_usec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_set_timestamp_usec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_set_system_timestamp_nsec", (void**)&instance->k4a_image_set_system_timestamp_nsec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_set_system_timestamp_nsec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_set_exposure_usec", (void**)&instance->k4a_image_set_exposure_usec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_set_exposure_usec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_set_exposure_time_usec", (void**)&instance->k4a_image_set_exposure_time_usec);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_set_exposure_time_usec function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_set_white_balance", (void**)&instance->k4a_image_set_white_balance);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_set_white_balance function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_set_iso_speed", (void**)&instance->k4a_image_set_iso_speed);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_set_iso_speed function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_reference", (void**)&instance->k4a_image_reference);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_reference function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_image_release", (void**)&instance->k4a_image_release);
        if (K4A_FAILED(result))
        {
            printf("k4a_image_release function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_start_cameras", (void**)&instance->k4a_device_start_cameras);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_start_cameras function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_stop_cameras", (void**)&instance->k4a_device_stop_cameras);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_stop_cameras function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_start_imu", (void**)&instance->k4a_device_start_imu);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_start_imu function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_stop_imu", (void**)&instance->k4a_device_stop_imu);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_stop_imu function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_serialnum", (void**)&instance->k4a_device_get_serialnum);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_serialnum function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_version", (void**)&instance->k4a_device_get_version);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_version function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_color_control_capabilities", (void**)&instance->k4a_device_get_color_control_capabilities);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_color_control_capabilities function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_color_control", (void**)&instance->k4a_device_get_color_control);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_color_control function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_set_color_control", (void**)&instance->k4a_device_set_color_control);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_set_color_control function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_raw_calibration", (void**)&instance->k4a_device_get_raw_calibration);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_raw_calibration function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_calibration", (void**)&instance->k4a_device_get_calibration);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_calibration function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_device_get_sync_jack", (void**)&instance->k4a_device_get_sync_jack);
        if (K4A_FAILED(result))
        {
            printf("k4a_device_get_sync_jack function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_calibration_get_from_raw", (void**)&instance->k4a_calibration_get_from_raw);
        if (K4A_FAILED(result))
        {
            printf("k4a_calibration_get_from_raw function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_calibration_3d_to_3d", (void**)&instance->k4a_calibration_3d_to_3d);
        if (K4A_FAILED(result))
        {
            printf("k4a_calibration_3d_to_3d function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_calibration_2d_to_3d", (void**)&instance->k4a_calibration_2d_to_3d);
        if (K4A_FAILED(result))
        {
            printf("k4a_calibration_2d_to_3d function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_calibration_3d_to_2d", (void**)&instance->k4a_calibration_3d_to_2d);
        if (K4A_FAILED(result))
        {
            printf("k4a_calibration_3d_to_2d function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_calibration_2d_to_2d", (void**)&instance->k4a_calibration_2d_to_2d);
        if (K4A_FAILED(result))
        {
            printf("k4a_calibration_2d_to_2d function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_calibration_color_2d_to_depth_2d", (void**)&instance->k4a_calibration_color_2d_to_depth_2d);
        if (K4A_FAILED(result))
        {
            printf("k4a_calibration_color_2d_to_depth_2d function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_transformation_create", (void**)&instance->k4a_transformation_create);
        if (K4A_FAILED(result))
        {
            printf("k4a_transformation_create function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_transformation_destroy", (void**)&instance->k4a_transformation_destroy);
        if (K4A_FAILED(result))
        {
            printf("k4a_transformation_destroy function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_transformation_depth_image_to_color_camera", (void**)&instance->k4a_transformation_depth_image_to_color_camera);
        if (K4A_FAILED(result))
        {
            printf("k4a_transformation_depth_image_to_color_camera function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_transformation_depth_image_to_color_camera_custom", (void**)&instance->k4a_transformation_depth_image_to_color_camera_custom);
        if (K4A_FAILED(result))
        {
            printf("k4a_transformation_depth_image_to_color_camera_custom function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_transformation_color_image_to_depth_camera", (void**)&instance->k4a_transformation_color_image_to_depth_camera);
        if (K4A_FAILED(result))
        {
            printf("k4a_transformation_color_image_to_depth_camera function failed to load.\n");
        }
        result = dynlib_find_symbol(hdl, "k4a_transformation_depth_image_to_point_cloud", (void**)&instance->k4a_transformation_depth_image_to_point_cloud);
        if (K4A_FAILED(result))
        {
            printf("k4a_transformation_depth_image_to_point_cloud function failed to load.\n");
        }
    }
    printf("Function pointer initialization complete.\n");

    if (K4A_FAILED(result))
    {
        free(instance);
        instance = NULL;
    }

    return instance;
}

void k4aloader_free_loaded_dll_file(k4aloader_handle* handle)
{
    if (handle->context)
    {
        dynlib_destroy(((deloader_context_t*)handle->context)->handle, ((deloader_context_t*)handle->context)->orbbec_handle);
        delete handle->context;
        handle->context = nullptr;
    }
    free(handle);
    handle = NULL;
}
