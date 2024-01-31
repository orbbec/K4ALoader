#include "k4aLoader.h"
#include "k4atypes.h"

#include <iostream>
#include <memory>

int main()
{

    //auto loader = std::make_shared<K4aLoader>("/home/orbbec/Desktop/k4aLoader/K4ALoader-master/lib", &global);
    //auto loader = std::make_shared<K4aLoader>("D:\\k4aLoader\\k4aLoader\\lib", &global);

    instance_t instance = k4a_load("D:\\k4aLoader\\k4aLoader\\lib");
    if (instance.loaded)
    {

        int count = instance.k4a_device_get_installed_count();

        printf("device_count: %d", count);
        if (count > 0)
        {
            k4a_device_t device;
            if (instance.k4a_device_open(0, &device) != K4A_RESULT_SUCCEEDED)
            {
                printf("Failed to open device.\n");
                return -1;
            }

            k4a_calibration_t calibration;
            if (instance.k4a_device_get_calibration(device, K4A_DEPTH_MODE_NFOV_UNBINNED, K4A_COLOR_RESOLUTION_OFF, &calibration) != K4A_RESULT_SUCCEEDED)
            {
                printf("Failed to get device calibration.\n");
                instance.k4a_device_close(device);
                return -1;
            }

            k4a_calibration_camera_t camera = calibration.depth_camera_calibration;
            k4a_calibration_intrinsic_parameters_t intrinsics = camera.intrinsics.parameters;

            printf("Focal Length: %.2f, %.2f\n", intrinsics.param.fx, intrinsics.param.fy);

            instance.k4a_device_close(device);
        }

        free_instance(instance);
    }
    else
    {
        printf("Failed to load this.");
    }

    return 0;
}

//#include <Windows.h>
//#include <iostream>
//
//int main() {
//    // ��̬��·��
//    const char* libraryPath =
//        "D:\\k4aLoader\\k4aLoader\\lib\\OrbbecSDK.dll";
//
//    const char* libraryPathEx =
//        "D:\\k4aLoader\\k4aLoader\\lib\\k4a.dll";
//
//    // ���ض�̬��
//    HMODULE libraryHandle = LoadLibraryA(libraryPath);
//    if (!libraryHandle) {
//        std::cerr << "Failed to load library: " << GetLastError() << std::endl;
//        return 1;
//    }
//
//    HMODULE libraryHandleEx = LoadLibraryA(libraryPathEx);
//    if (!libraryHandleEx) {
//        std::cerr << "Failed to load library: " << GetLastError() << std::endl;
//        return 1;
//    }
//
//    // ��ȡ������ַ
//    typedef uint32_t(*FunctionType)(void);
//    FunctionType function = reinterpret_cast<FunctionType>(
//        GetProcAddress(libraryHandleEx, "k4a_device_get_installed_count"));
//    if (!function) {
//        std::cerr << "Failed to get function address: " << GetLastError()
//            << std::endl;
//        FreeLibrary(libraryHandle);
//        FreeLibrary(libraryHandleEx);
//        return 1;
//    }
//
//    //// ���ú���
//    int count = function();
//    std::cout << "==============count:" << count << std::endl;
//
//    //function = nullptr;
//    // �رն�̬��
//    FreeLibrary(libraryHandle);
//    FreeLibrary(libraryHandleEx);
//
//    return 0;
//}