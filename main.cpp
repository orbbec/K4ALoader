#include "k4aLoader.h"
#include "k4atypes.h"

#include <iostream>
#include <memory>

int main()
{

    k4aloader_handle* instance = k4aloader_dll_file_load("D:\\Test\\TestLoad");

    if (instance != NULL)
    {
        int count = instance->k4a_device_get_installed_count();

        printf("device_count: %d\n", count);
        
        k4aloader_free_loaded_dll_file(instance);
    }
    else
    {
        printf("Failed to load this.\n");
    }
    
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