#include "K4aLoader.h"

#include <iostream>

int main()
{
    deloader_global_context_t global;

    K4aLoader loader("D:\\k4aLoader\\k4aLoader\\lib", &global);

    int count = loader.functionList.k4a_device_get_installed_count();

    std::cout << count;
    return 0;
}

