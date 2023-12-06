#include "K4aLoader.h"
#include "k4atypes.h"

#include <iostream>
#include <memory>

int main()
{
    deloader_global_context_t global;

    //auto loader = std::make_shared<K4aLoader>("/home/orbbec/Desktop/k4aLoader/K4ALoader-master/lib", &global);
    auto loader = std::make_shared<K4aLoader>("D:\\k4aLoader\\k4aLoader\\lib", &global);
    // int count = loader.functionList.k4a_device_get_installed_count();

    // std::cout << count;

    k4a_device_t device;
    if (loader->functionList.k4a_device_open(0, &device) != K4A_RESULT_SUCCEEDED)
    {
        printf("Failed to open device.\n");
        return -1;
    }

    k4a_calibration_t calibration;
    if (loader->functionList.k4a_device_get_calibration(device, K4A_DEPTH_MODE_NFOV_UNBINNED, K4A_COLOR_RESOLUTION_OFF, &calibration) != K4A_RESULT_SUCCEEDED)
    {
        printf("Failed to get device calibration.\n");
        loader->functionList.k4a_device_close(device);
        return -1;
    }

    k4a_calibration_camera_t camera = calibration.depth_camera_calibration;
    k4a_calibration_intrinsic_parameters_t intrinsics = camera.intrinsics.parameters;

    printf("Focal Length: %.2f, %.2f\n", intrinsics.param.fx, intrinsics.param.fy);

    loader->functionList.k4a_device_close(device);
    return 0;
}
