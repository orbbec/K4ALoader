#include "k4aLoader.h"
#include "k4atypes.h"

#include <iostream>
#include <memory>
#include <k4loader/k4a.hpp>

void openStream() {

    k4a_runtime_handle* kl_handle = k4aloader_create_runtime_handle("D:\\Test\\TestLoad");

    if (kl_handle != NULL)
    {
        const int32_t TIMEOUT_IN_MS = 1000;
        k4a::capture capture = nullptr;

        int count = k4a::device::get_installed_count(kl_handle);
        printf("device_count: %d\n", count);
        if (count > 0) {
            auto dev = k4a::device::open(0);

            auto raw = dev.get_raw_calibration();
            auto size = raw.size();
            printf("raw_size: %d\n", size);

            k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
            config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
            config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
            config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
            config.camera_fps = K4A_FRAMES_PER_SECOND_30;
            dev.start_cameras(&config);

            while (true)
            {
                auto res = dev.get_capture(&capture);

                if (res)
                {
                    printf("Capture");

                    // Probe for a color image
                    auto imageColor = capture.get_color_image();
                    if (imageColor)
                    {
                        printf(" | Color res:%4dx%4d stride:%5d,dataSize:%d ",
                            imageColor.get_height_pixels(),
                            imageColor.get_width_pixels(),
                            imageColor.get_stride_bytes(),
                            (int)imageColor.get_size());
                        imageColor.reset();
                        printf("\n");
                    }
                    else
                    {
                        printf(" | Color None                       ");
                        printf("\n");
                    }

                    // release capture
                    capture.reset();
                    fflush(stdout);

                }
            }

        }
        k4aloader_free_runtime_handle(kl_handle);
    }
    else
    {
        printf("Failed to load this.\n");
    }
}

int main()
{
    
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