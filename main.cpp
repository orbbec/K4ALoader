#include <iostream>
#include <k4loader/k4a.hpp>
#include <memory>

#include "k4aLoader.h"
#include "k4atypes.h"

// ע�⣺����ǰ�����޸Ķ�̬�����·��������
int captureFrameCount;
int mode;
// C++接口开流
void openAllStream() {
  // 输入动态库所在文件夹路径, 加载动态库
  k4aRuntimeHandle* klHandle = k4aloader_create_runtime_handle("D:\\k4aLoader\\k4aLoader\\Test\\wrapper");

  if (klHandle != nullptr) {
    k4a::capture capture;
    // 静态方法调用需要输入k4a_runtime_handle作为参数
    int count = k4a::device::get_installed_count(klHandle);
    printf("deviceCount: %d\n", count);

    if (count > 0) {
      // 下面为隐士调用k4aLoader中的get_current_k4a_runtime_handle去获取当前默认的k4a_runtime_handle打开k4a设备
      // 默认的k4a_runtime_handle可调用k4aLoader中的switch_k4a_runtime_handle函数进行切换
      auto dev = k4a::device::open(0);
      // 也可输入k4a_runtime_handle去打开设备
      // You can also type k4a_runtime_handle to open the device.
      // auto dev = k4a::device::open(kl_handle, 0);
      if (dev) {
        k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
        config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
        config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
        config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
        config.camera_fps = K4A_FRAMES_PER_SECOND_30;

        dev.start_cameras(&config);

        while (captureFrameCount-- > 0) {
          auto res = dev.get_capture(&capture);

          if (res) {
            printf("Capture");

            // color image
            auto imageColor = capture.get_color_image();
            if (imageColor) {
              printf(" | Color res:%4dx%4d stride:%5d,dataSize:%d ",
                     imageColor.get_height_pixels(),
                     imageColor.get_width_pixels(),
                     imageColor.get_stride_bytes(),
                     (int)imageColor.get_size());
              imageColor.reset();
            } else {
              printf(" | Color None                       ");
            }

            // IR16 image
            auto imageIr = capture.get_color_image();
            if (imageIr) {
              printf(" | Ir16 res:%4dx%4d stride:%5d ",
                     imageIr.get_height_pixels(),
                     imageIr.get_width_pixels(),
                     imageIr.get_stride_bytes(),
                     (int)imageIr.get_size());
              imageIr.reset();
            } else {
              printf(" | Ir16 None                      ");
            }

            // depth16 image
            auto imageDepth = capture.get_color_image();
            if (imageDepth) {
              printf(" | Depth16 res:%4dx%4d stride:%5d\n",
                     imageDepth.get_height_pixels(),
                     imageDepth.get_width_pixels(),
                     imageDepth.get_stride_bytes(),
                     (int)imageDepth.get_size());
              imageDepth.reset();
            } else {
              printf(" | Depth16 None\n");
            }

            // release capture
            capture.reset();
            fflush(stdout);
          }
        }
      }
      // 关闭设备连接, 同时会对使用的k4a_runtime_handle的引用计数减1
      // Closes the device connection and decrements the reference count of k4a_runtime_handle by 1.
      if (dev) {
        dev.stop_cameras();
        dev.close();
      }
    }
    // 释放动态库连接, 需注意只有当k4a_runtime_handle引用计数为0时才会被彻底释放掉
    // Release the dynamic library link, note that it will only be completely released when the k4a_runtime_handle reference count is zero.
    k4aloader_free_runtime_handle(klHandle);
  } else {
    printf("Failed to load this.\n");
  }
}

// C接口开流
void openAllStreamC() {
  // 输入动态库所在文件夹路径, 加载动态库
  k4a_runtime_handle* instance = k4aloader_create_runtime_handle("D:\\k4aLoader\\k4aLoader\\Test\\wrapper");

  if (instance != NULL) {
    k4a_capture_t capture = NULL;
    const int32_t TIMEOUT_IN_MS = 1000;
    // 使用k4a_runtime_handle进行函数调用, 获取当前连接设备个数
    int count = instance->k4a_device_get_installed_count();
    printf("device_count: %d\n", count);

    if (count > 0) {
      k4a_device_t device;
      // 打开设备0
      if (K4A_RESULT_SUCCEEDED != instance->k4a_device_open(K4A_DEVICE_DEFAULT, &device)) {
        printf("Failed to open device\n");
        return;
      }
      // 配置开流config
      k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
      config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
      config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
      config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
      config.camera_fps = K4A_FRAMES_PER_SECOND_30;
      // 开流
      if (K4A_RESULT_SUCCEEDED != instance->k4a_device_start_cameras(device, &config)) {
        printf("Failed to start device\n");
        return;
      }
      // 取帧
      while (captureFrameCount-- > 0) {
        k4a_image_t image;

        // Get a depth frame
        switch (instance->k4a_device_get_capture(device, &capture, TIMEOUT_IN_MS)) {
          case K4A_WAIT_RESULT_SUCCEEDED:
            break;
          case K4A_WAIT_RESULT_TIMEOUT:
            printf("Timed out waiting for a capture\n");
            continue;
            break;
          case K4A_WAIT_RESULT_FAILED:
            printf("Failed to read a capture\n");
            return;
        }

        printf("Capture");

        // color image
        image = instance->k4a_capture_get_color_image(capture);
        if (image != NULL) {
          printf(" | Color res:%4dx%4d stride:%5d,dataSize:%d ",
                 instance->k4a_image_get_height_pixels(image),
                 instance->k4a_image_get_width_pixels(image),
                 instance->k4a_image_get_stride_bytes(image),
                 (int)instance->k4a_image_get_size(image));
          instance->k4a_image_release(image);
        } else {
          printf(" | Color None                       ");
        }

        // IR16 image
        image = instance->k4a_capture_get_ir_image(capture);
        if (image != NULL) {
          printf(" | Ir16 res:%4dx%4d stride:%5d ",
                 instance->k4a_image_get_height_pixels(image),
                 instance->k4a_image_get_width_pixels(image),
                 instance->k4a_image_get_stride_bytes(image));
          instance->k4a_image_release(image);
        } else {
          printf(" | Ir16 None                       ");
        }

        // depth16 image
        image = instance->k4a_capture_get_depth_image(capture);
        if (image != NULL) {
          printf(" | Depth16 res:%4dx%4d stride:%5d\n",
                 instance->k4a_image_get_height_pixels(image),
                 instance->k4a_image_get_width_pixels(image),
                 instance->k4a_image_get_stride_bytes(image));
          instance->k4a_image_release(image);
        } else {
          printf(" | Depth16 None\n");
        }

        // release capture
        instance->k4a_capture_release(capture);
        fflush(stdout);
      }
      // 关流、关闭设备
      if (device != NULL) {
        instance->k4a_device_stop_cameras(device);
        instance->k4a_device_close(device);
      }
    }
    // 释放动态库连接, 需注意只有当k4a_runtime_handle引用计数为0时才会被彻底释放掉
    k4aloader_free_runtime_handle(instance);
  } else {
    printf("Failed to load this.\n");
  }
}

// C++同时打开Meaga和Azure Kinect
void bothDeivceOpen() {
  // 输入动态库所在文件夹路径, 加载动态库
  k4aRuntimeHandle* klHandleWR = k4aloader_create_runtime_handle("D:\\k4aLoader\\k4aLoader\\Test\\wrapper");
  k4aRuntimeHandle* klHandleAK = k4aloader_create_runtime_handle("D:\\k4aLoader\\k4aLoader\\Test\\akdk");

  if (klHandleWR != nullptr && klHandleAK != nullptr) {
    k4a::capture captureWR = nullptr;
    k4a::capture captureAK = nullptr;
    // 静态方法调用需要输入k4a_runtime_handle作为参数
    int countWR = k4a::device::get_installed_count(klHandleWR);
    int countAK = k4a::device::get_installed_count(klHandleAK);

    printf("WrapperDeviceCount: %d\n", countWR);
    printf("AKDKDeviceCount: %d\n", countAK);

    if (countWR > 0 && countAK > 0) {
      // 显式传入SDK的handle打开不同device
      auto warDevice = k4a::device::open(0, klHandleWR);  // ob
      auto akDevice = k4a::device::open(0, klHandleAK);   // akdk

      k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
      config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
      config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
      config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
      config.camera_fps = K4A_FRAMES_PER_SECOND_15;

      warDevice.start_cameras(&config);
      akDevice.start_cameras(&config);

      while (captureFrameCount-- > 0) {
        auto resWR = warDevice.get_capture(&captureWR);

        if (resWR) {
          printf("Capture >>>>>> Wrapper");

          // color image
          auto imageColor = captureWR.get_color_image();
          if (imageColor) {
            printf(" | Color res:%4dx%4d stride:%5d,dataSize:%d ",
                   imageColor.get_height_pixels(),
                   imageColor.get_width_pixels(),
                   imageColor.get_stride_bytes(),
                   (int)imageColor.get_size());
            imageColor.reset();
          } else {
            printf(" | Color None                       ");
          }

          // IR16 image
          auto imageIr = captureWR.get_color_image();
          if (imageIr) {
            printf(" | Ir16 res:%4dx%4d stride:%5d ",
                   imageIr.get_height_pixels(),
                   imageIr.get_width_pixels(),
                   imageIr.get_stride_bytes(),
                   (int)imageIr.get_size());
            imageIr.reset();
          } else {
            printf(" | Ir16 None                      ");
          }

          // depth16 image
          auto imageDepth = captureWR.get_color_image();
          if (imageDepth) {
            printf(" | Depth16 res:%4dx%4d stride:%5d\n",
                   imageDepth.get_height_pixels(),
                   imageDepth.get_width_pixels(),
                   imageDepth.get_stride_bytes(),
                   (int)imageDepth.get_size());
            imageDepth.reset();
          } else {
            printf(" | Depth16 None\n");
          }

          // release capture
          captureWR.reset();
          fflush(stdout);
        }

        auto resAK = warDevice.get_capture(&captureAK);

        if (resAK) {
          printf("Capture >>>>>> AKDK");

          // color image
          auto imageColor = captureAK.get_color_image();
          if (imageColor) {
            printf(" | Color res:%4dx%4d stride:%5d,dataSize:%d ",
                   imageColor.get_height_pixels(),
                   imageColor.get_width_pixels(),
                   imageColor.get_stride_bytes(),
                   (int)imageColor.get_size());
            imageColor.reset();
          } else {
            printf(" | Color None                       ");
          }

          // IR16 image
          auto imageIr = captureAK.get_color_image();
          if (imageIr) {
            printf(" | Ir16 res:%4dx%4d stride:%5d ",
                   imageIr.get_height_pixels(),
                   imageIr.get_width_pixels(),
                   imageIr.get_stride_bytes(),
                   (int)imageIr.get_size());
            imageIr.reset();
          } else {
            printf(" | Ir16 None                      ");
          }

          // depth16 image
          auto imageDepth = captureAK.get_color_image();
          if (imageDepth) {
            printf(" | Depth16 res:%4dx%4d stride:%5d\n",
                   imageDepth.get_height_pixels(),
                   imageDepth.get_width_pixels(),
                   imageDepth.get_stride_bytes(),
                   (int)imageDepth.get_size());
            imageDepth.reset();
          } else {
            printf(" | Depth16 None\n");
          }

          // release capture
          captureAK.reset();
          fflush(stdout);
        }
      }
      // 关闭设备连接, 同时会对使用的k4a_runtime_handle的引用计数减1
      warDevice.close();
      akDevice.close();
    }
    // 释放动态库连接, 需注意只有当k4a_runtime_handle引用计数为0时才会被彻底释放掉
    k4aloader_free_runtime_handle(klHandleWR);
    k4aloader_free_runtime_handle(klHandleAK);
  } else {
    printf("Failed to load this.\n");
  }
}

int main() {
  captureFrameCount = 500;
  bothDeivceOpen();

  printf("Please enter the number of fetches: ");
  std::cin >> captureFrameCount;
  if (captureFrameCount <= 0) {
    printf("Enter a value no greater than 0, default is 500");
    captureFrameCount = 500;
  }
  printf("3 test modes: ");
  printf("0.C++ open stream test(Mega); 1.C open stream test(Mega); 2.Streaming on mega and kinect at the same time");
  printf("Please enter the mode: ");
  std::cin >> mode;
  switch (mode) {
    case 0:
      openAllStream();
      break;
    case 1:
      openAllStreamC();
      break;
    case 2:
      bothDeivceOpen();
      break;
    default:
      printf("The input pattern does not exist");
      break;
  }

  return 0;
}
