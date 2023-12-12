#ifndef K4ALOADER
#define K4ALOADER

#if(defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef K4ALOADER_EXPORTS
#define K4ALOADER_EXTENSION_API __declspec(dllexport)
#else
#ifndef OB_STATIC
#define K4ALOADER_EXTENSION_API __declspec(dllimport)
#else
#define K4ALOADER_EXTENSION_API
#endif
#endif
#else
#ifndef OB_STATIC
#define K4ALOADER_EXTENSION_API __attribute__((visibility("default")))
#else
#define K4ALOADER_EXTENSION_API
#endif
#endif

#include "k4atypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct K4aLoaderHandle
    {
        void* context;
        // Gets the number of connected devices
        uint32_t(*k4a_device_get_installed_count)(void);
        // Sets and clears the callback function to receive debug messages from the Azure Kinect device.
        k4a_result_t (*k4a_set_debug_message_handler)(k4a_logging_message_cb_t* message_cb,
            void* message_cb_context,
            k4a_log_level_t min_level);
        // Sets the callback functions for the SDK allocator
        k4a_result_t (*k4a_set_allocator)(k4a_memory_allocate_cb_t allocate, k4a_memory_destroy_cb_t free);
        // Open an Azure Kinect device.
        k4a_result_t(*k4a_device_open)(uint32_t index, k4a_device_t* device_handle);
        // Closes an Azure Kinect device.
        void (*k4a_device_close)(k4a_device_t device_handle);
        // Reads a sensor capture.
        k4a_wait_result_t (*k4a_device_get_capture)(k4a_device_t device_handle,
            k4a_capture_t* capture_handle,
            int32_t timeout_in_ms);
        // Reads an IMU sample.
        k4a_wait_result_t (*k4a_device_get_imu_sample)(k4a_device_t device_handle,
            k4a_imu_sample_t* imu_sample,
            int32_t timeout_in_ms);
        // Create an empty capture object.
        k4a_result_t (*k4a_capture_create)(k4a_capture_t* capture_handle);
        // Release a capture.
        void (*k4a_capture_release)(k4a_capture_t capture_handle);
        // Add a reference to a capture.
        void (*k4a_capture_reference)(k4a_capture_t capture_handle);
        // Get the color image associated with the given capture.
        k4a_image_t (*k4a_capture_get_color_image)(k4a_capture_t capture_handle);
        // Get the depth image associated with the given capture.
        k4a_image_t (*k4a_capture_get_depth_image)(k4a_capture_t capture_handle);
        // Get the IR image associated with the given capture.
        k4a_image_t (*k4a_capture_get_ir_image)(k4a_capture_t capture_handle);
        // Set or add a color image to the associated capture.
        void (*k4a_capture_set_color_image)(k4a_capture_t capture_handle, k4a_image_t image_handle);
        // Set or add a depth image to the associated capture.
        void (*k4a_capture_set_depth_image)(k4a_capture_t capture_handle, k4a_image_t image_handle);
        // Set or add an IR image to the associated capture.
        void (*k4a_capture_set_ir_image)(k4a_capture_t capture_handle, k4a_image_t image_handle);
        // Set the temperature associated with the capture.
        void (*k4a_capture_set_temperature_c)(k4a_capture_t capture_handle, float temperature_c);
        // Get the temperature associated with the capture.
        float (*k4a_capture_get_temperature_c)(k4a_capture_t capture_handle);
        // Create an image.
        k4a_result_t (*k4a_image_create)(k4a_image_format_t format,
            int width_pixels,
            int height_pixels,
            int stride_bytes,
            k4a_image_t* image_handle);
        // Create an image from a pre-allocated buffer.
        k4a_result_t (*k4a_image_create_from_buffer)(k4a_image_format_t format,
            int width_pixels,
            int height_pixels,
            int stride_bytes,
            uint8_t* buffer,
            size_t buffer_size,
            k4a_memory_destroy_cb_t* buffer_release_cb,
            void* buffer_release_cb_context,
            k4a_image_t* image_handle);
        // Get the image buffer.
        uint8_t* (*k4a_image_get_buffer)(k4a_image_t image_handle);
        // Get the image buffer size.
        size_t (*k4a_image_get_size)(k4a_image_t image_handle);
        // Get the format of the image.
        k4a_image_format_t (*k4a_image_get_format)(k4a_image_t image_handle);
        // Get the image width in pixels.
        int (*k4a_image_get_width_pixels)(k4a_image_t image_handle);
        // Get the image height in pixels.
        int (*k4a_image_get_height_pixels)(k4a_image_t image_handle);
        // Get the image stride in bytes.
        int (*k4a_image_get_stride_bytes)(k4a_image_t image_handle);
        // Get the image's device timestamp in microseconds.
        uint64_t (*k4a_image_get_timestamp_usec)(k4a_image_t image_handle);
        // Get the image's device timestamp in microseconds.
        uint64_t (*k4a_image_get_device_timestamp_usec)(k4a_image_t image_handle);
        // Get the image's system timestamp in nanoseconds.
        uint64_t (*k4a_image_get_system_timestamp_nsec)(k4a_image_t image_handle);
        // Get the image exposure in microseconds.
        uint64_t (*k4a_image_get_exposure_usec)(k4a_image_t image_handle);
        // Get the image white balance.
        uint32_t (*k4a_image_get_white_balance)(k4a_image_t image_handle);
        // Get the image ISO speed.
        uint32_t (*k4a_image_get_iso_speed)(k4a_image_t image_handle);
        // Set the device time stamp, in microseconds, of the image.
        void (*k4a_image_set_device_timestamp_usec)(k4a_image_t image_handle, uint64_t timestamp_usec);
        // K4A_DEPRECATED_EXPORT
        // Set the device time stamp, in microseconds, of the image.
        void (*k4a_image_set_timestamp_usec)(k4a_image_t image_handle, uint64_t timestamp_usec);
        // Set the system time stamp, in nanoseconds, of the image.
        void (*k4a_image_set_system_timestamp_nsec)(k4a_image_t image_handle, uint64_t timestamp_nsec);
        // Set the exposure time, in microseconds, of the image.
        void (*k4a_image_set_exposure_usec)(k4a_image_t image_handle, uint64_t exposure_usec);
        // K4A_DEPRECATED_EXPORT
        // Set the exposure time, in microseconds, of the image.
        void (*k4a_image_set_exposure_time_usec)(k4a_image_t image_handle, uint64_t exposure_usec);
        // Set the white balance of the image.
        void (*k4a_image_set_white_balance)(k4a_image_t image_handle, uint32_t white_balance);
        // Set the ISO speed of the image.
        void (*k4a_image_set_iso_speed)(k4a_image_t image_handle, uint32_t iso_speed);
        // Add a reference to the k4a_image_t.
        void (*k4a_image_reference)(k4a_image_t image_handle);
        // Remove a reference from the k4a_image_t.
        void (*k4a_image_release)(k4a_image_t image_handle);
        // Starts color and depth camera capture.
        k4a_result_t (*k4a_device_start_cameras)(k4a_device_t device_handle, const k4a_device_configuration_t* config);
        // Stops the color and depth camera capture.
        void (*k4a_device_stop_cameras)(k4a_device_t device_handle);
        // Starts the IMU sample stream.
        k4a_result_t (*k4a_device_start_imu)(k4a_device_t device_handle);
        // Stops the IMU capture.
        void (*k4a_device_stop_imu)(k4a_device_t device_handle);
        // Get the Azure Kinect device serial number.
        k4a_buffer_result_t (*k4a_device_get_serialnum)(k4a_device_t device_handle,
            char* serial_number,
            size_t* serial_number_size);
        // Get the version numbers of the device's subsystems.
        k4a_result_t (*k4a_device_get_version)(k4a_device_t device_handle, k4a_hardware_version_t* version);
        // Get the Azure Kinect color sensor control capabilities.
        k4a_result_t (*k4a_device_get_color_control_capabilities)(k4a_device_t device_handle,
            k4a_color_control_command_t command,
            bool* supports_auto,
            int32_t* min_value,
            int32_t* max_value,
            int32_t* step_value,
            int32_t* default_value,
            k4a_color_control_mode_t* default_mode);
        // Get the Azure Kinect color sensor control
        k4a_result_t (*k4a_device_get_color_control)(k4a_device_t device_handle,
            k4a_color_control_command_t command,
            k4a_color_control_mode_t* mode,
            int32_t* value);
        // Set the Azure Kinect color sensor control value.
        k4a_result_t (*k4a_device_set_color_control)(k4a_device_t device_handle,
            k4a_color_control_command_t command,
            k4a_color_control_mode_t mode,
            int32_t value);
        // Get the raw calibration blob for the entire Azure Kinect device.
        k4a_buffer_result_t (*k4a_device_get_raw_calibration)(k4a_device_t device_handle,
            uint8_t* data,
            size_t* data_size);
        // Get the camera calibration for the entire Azure Kinect device.
        k4a_result_t (*k4a_device_get_calibration)(k4a_device_t device_handle,
            const k4a_depth_mode_t depth_mode,
            const k4a_color_resolution_t color_resolution,
            k4a_calibration_t* calibration);
        // Get the device jack status for the synchronization in and synchronization out connectors.
        k4a_result_t (*k4a_device_get_sync_jack)(k4a_device_t device_handle,
            bool* sync_in_jack_connected,
            bool* sync_out_jack_connected);
        // Get the camera calibration for a device from a raw calibration blob.
        k4a_result_t (*k4a_calibration_get_from_raw)(char* raw_calibration,
            size_t raw_calibration_size,
            const k4a_depth_mode_t depth_mode,
            const k4a_color_resolution_t color_resolution,
            k4a_calibration_t* calibration);
        // Transform a 3D point of a source coordinate system into a 3D point of the target coordinate system
        k4a_result_t (*k4a_calibration_3d_to_3d)(const k4a_calibration_t* calibration,
            const k4a_float3_t* source_point3d_mm,
            const k4a_calibration_type_t source_camera,
            const k4a_calibration_type_t target_camera,
            k4a_float3_t* target_point3d_mm);
        // Transform a 2D pixel coordinate with an associated depth value of the source camera into a 3D point of the target
        k4a_result_t (*k4a_calibration_2d_to_3d)(const k4a_calibration_t* calibration,
            const k4a_float2_t* source_point2d,
            const float source_depth_mm,
            const k4a_calibration_type_t source_camera,
            const k4a_calibration_type_t target_camera,
            k4a_float3_t* target_point3d_mm,
            int* valid);
        // Transform a 3D point of a source coordinate system into a 2D pixel coordinate of the target camera.
        k4a_result_t (*k4a_calibration_3d_to_2d)(const k4a_calibration_t* calibration,
            const k4a_float3_t* source_point3d_mm,
            const k4a_calibration_type_t source_camera,
            const k4a_calibration_type_t target_camera,
            k4a_float2_t* target_point2d,
            int* valid);
        // Transform a 2D pixel coordinate with an associated depth value of the source camera into a 2D pixel coordinate of the target camera.
        k4a_result_t (*k4a_calibration_2d_to_2d)(const k4a_calibration_t* calibration,
            const k4a_float2_t* source_point2d,
            const float source_depth_mm,
            const k4a_calibration_type_t source_camera,
            const k4a_calibration_type_t target_camera,
            k4a_float2_t* target_point2d,
            int* valid);
        // Transform a 2D pixel coordinate from color camera into a 2D pixel coordinate of the depth camera.
        k4a_result_t (*k4a_calibration_color_2d_to_depth_2d)(const k4a_calibration_t* calibration,
            const k4a_float2_t* source_point2d,
            const k4a_image_t depth_image,
            k4a_float2_t* target_point2d,
            int* valid);
        // Get handle to transformation handle.
        k4a_transformation_t (*k4a_transformation_create)(const k4a_calibration_t* calibration);
        //  Destroy transformation handle.
        void (*k4a_transformation_destroy)(k4a_transformation_t transformation_handle);
        // Transforms the depth map into the geometry of the color camera.
        k4a_result_t (*k4a_transformation_depth_image_to_color_camera)(k4a_transformation_t transformation_handle,
            const k4a_image_t depth_image,
            k4a_image_t transformed_depth_image);
        // Transforms depth map and a custom image into the geometry of the color camera.
        k4a_result_t (*k4a_transformation_depth_image_to_color_camera_custom)(k4a_transformation_t transformation_handle,
                const k4a_image_t depth_image,
                const k4a_image_t custom_image,
                k4a_image_t transformed_depth_image,
                k4a_image_t transformed_custom_image,
                k4a_transformation_interpolation_type_t interpolation_type,
                uint32_t invalid_custom_value);
        // Transforms a color image into the geometry of the depth camera.
        k4a_result_t (*k4a_transformation_color_image_to_depth_camera)(k4a_transformation_t transformation_handle,
            const k4a_image_t depth_image,
            const k4a_image_t color_image,
            k4a_image_t transformed_color_image);
        // Transforms the depth image into 3 planar images representing X, Y and Z-coordinates of corresponding 3D points.
        k4a_result_t (*k4a_transformation_depth_image_to_point_cloud)(k4a_transformation_t transformation_handle,
            const k4a_image_t depth_image,
            const k4a_calibration_type_t camera,
            k4a_image_t xyz_image);

    }k4aloader_handle;

    K4ALOADER_EXTENSION_API k4aloader_handle* k4aloader_dll_file_load(const char* dll_file_path);

    K4ALOADER_EXTENSION_API void k4aloader_free_loaded_dll_file(k4aloader_handle* instance);

#ifdef __cplusplus
}
#endif

#endif