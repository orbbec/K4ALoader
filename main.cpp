#include <fstream>
#include <iostream>
#include <k4aloader/k4a.hpp>
#include <memory>
#include <sstream>
#include <vector>

#include "k4aLoader.h"
#include "k4atypes.h"

// 注意： 编译前请先修改动态库加载路径！！！
// Note: Please change the dynamic library load path before compiling!!!!
struct colorPointT {
  int16_t xyz[3];
  uint8_t rgb[3];
};

int captureFrameCount;
int mode;

void deviceFunctionTest() {
  k4a_runtime_handle* instance = k4aloader_create_runtime_handle("D:\\Test\\TestLoad");
  if (instance == nullptr) {
    printf("Failed to load file of dll!!!\n");
    return;
  }

  auto count = k4a::device::get_installed_count(instance);
  printf("deviceCount: %d\n", count);

  if (count > 0) {
    // open device
    auto dev = k4a::device::open(0);
    // dev = k4a::device::open(0);
    if (!dev.is_valid()) {
      printf("device is not valid!!!\n");
      return;
    }
    auto hd = dev.handle();
    if (hd == nullptr) {
      printf("device handle is null!!!\n");
      return;
    }

    // get serialnum
    auto serial = dev.get_serialnum();
    printf("serialnum: %s\n", serial);

    // get version
    auto version = dev.get_version();
    printf("version: \n");
    printf("	rgb: %d.%d.%d\n", version.rgb.major, version.rgb.minor, version.rgb.iteration);
    printf("	depth: %d.%d.%d\n", version.depth.major, version.depth.minor, version.depth.iteration);
    printf("	audio: %d.%d.%d\n", version.audio.major, version.audio.minor, version.audio.iteration);
    printf("	depth_sensor: %d.%d.%d\n", version.depth_sensor.major, version.depth_sensor.minor, version.depth_sensor.iteration);
    printf("	firmware_build: %d\n", version.firmware_build);
    printf("	firmware_signature: %d\n", version.firmware_signature);

    // get raw calibration
    auto rawCalibration = dev.get_raw_calibration();
    printf("rawCalibrationSize: %d\n", rawCalibration.size());

    // get calibration
    auto cal = k4a::calibration::get_from_raw(rawCalibration, K4A_DEPTH_MODE_NFOV_UNBINNED, K4A_COLOR_RESOLUTION_1080P, instance);
    printf("Calibration1: \n");
    printf("	depth_mode: %d\n", cal.calibration_.depth_mode);
    printf("	color_resolution: %d\n", cal.calibration_.color_resolution);
    printf("	-extrinsics- rotation: %f, translation: %f\n", cal.calibration_.extrinsics[0][0].rotation[0], cal.calibration_.extrinsics[0][0].translation[0]);
    printf("	-depth_camera_calibration-  resolution_width: %d, resolution_height: %d, metric_radius: %f\n", cal.calibration_.depth_camera_calibration.resolution_width,
           cal.calibration_.depth_camera_calibration.resolution_height, cal.calibration_.depth_camera_calibration.metric_radius);
    printf("	-color_camera_calibration-  resolution_width: %d, resolution_height: %d, metric_radius: %f\n", cal.calibration_.color_camera_calibration.resolution_width,
           cal.calibration_.color_camera_calibration.resolution_height, cal.calibration_.color_camera_calibration.metric_radius);
    // auto calibration = dev.get_calibration(K4A_DEPTH_MODE_NFOV_UNBINNED, K4A_COLOR_RESOLUTION_1080P);
    cal.reset();

    auto cal2 = dev.get_calibration(K4A_DEPTH_MODE_NFOV_UNBINNED, K4A_COLOR_RESOLUTION_1080P);
    printf("Calibration2: \n");
    printf("	depth_mode: %d\n", cal2.calibration_.depth_mode);
    printf("	color_resolution: %d\n", cal2.calibration_.color_resolution);
    printf("	-extrinsics- rotation: %f, translation: %f\n", cal2.calibration_.extrinsics[0][0].rotation[0], cal2.calibration_.extrinsics[0][0].translation[0]);
    printf("	-depth_camera_calibration-  resolution_width: %d, resolution_height: %d, metric_radius: %f\n", cal2.calibration_.depth_camera_calibration.resolution_width,
           cal2.calibration_.depth_camera_calibration.resolution_height, cal2.calibration_.depth_camera_calibration.metric_radius);
    printf("	-color_camera_calibration-  resolution_width: %d, resolution_height: %d, metric_radius: %f\n", cal2.calibration_.color_camera_calibration.resolution_width,
           cal2.calibration_.color_camera_calibration.resolution_height, cal2.calibration_.color_camera_calibration.metric_radius);
    cal2.reset();

    // sync in/out connected
    bool sic = dev.is_sync_in_connected();
    printf("syncInConnectedStatus: %d\n", sic);
    bool soc = dev.is_sync_out_connected();
    printf("syncOutConnectedStatus: %d\n", soc);

    // color control
    k4a_color_control_mode_t mode;
    int32_t val;
    dev.set_color_control(K4A_COLOR_CONTROL_WHITEBALANCE, K4A_COLOR_CONTROL_MODE_AUTO, 0);
    dev.get_color_control(K4A_COLOR_CONTROL_WHITEBALANCE, &mode, &val);
    printf("K4A_COLOR_CONTROL_GAIN.mode: %d\n", mode);
    printf("K4A_COLOR_CONTROL_GAIN.value: %d\n", val);

    dev.set_color_control(K4A_COLOR_CONTROL_WHITEBALANCE, K4A_COLOR_CONTROL_MODE_MANUAL, 5000);
    dev.get_color_control(K4A_COLOR_CONTROL_WHITEBALANCE, &mode, &val);
    printf("K4A_COLOR_CONTROL_GAIN.mode: %d\n", mode);
    printf("K4A_COLOR_CONTROL_GAIN.value: %d\n", val);

    // start camera
    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
    config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
    config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    config.camera_fps = K4A_FRAMES_PER_SECOND_30;
    dev.start_cameras(&config);
    dev.start_imu();

    k4a::capture cap;
    int captureFrameCount = 3;
    k4a_imu_sample_t imu;
    while (captureFrameCount-- > 0) {
      auto res = dev.get_capture(&cap);
      if (res) {
        printf("Capture");

        // color image
        auto imageColor = cap.get_color_image();
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
        auto imageIr = cap.get_color_image();
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
        auto imageDepth = cap.get_color_image();
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
        cap.reset();

        // imu
        auto re = dev.get_imu_sample(&imu);
        if (re) {
          printf("-ImuData- \n");
          printf("	temperature: %f\n", imu.temperature);
          printf("	acc_timestamp_usec: %d\n", imu.acc_timestamp_usec);
          printf("	acc-x: %f\n", imu.acc_sample.xyz.x);
          printf("	acc-y: %f\n", imu.acc_sample.xyz.y);
          printf("	acc-z: %f\n", imu.acc_sample.xyz.z);
          printf("	acc-v0: %f\n", imu.acc_sample.v[0]);
          printf("	acc-v1: %f\n", imu.acc_sample.v[1]);
          printf("	acc-v2: %f\n", imu.acc_sample.v[2]);
          printf("	gyro_timestamp_usec: %d\n", imu.gyro_timestamp_usec);
          printf("	gyro-x: %f\n", imu.gyro_sample.xyz.x);
          printf("	gyro-y: %f\n", imu.gyro_sample.xyz.y);
          printf("	gyro-z: %f\n", imu.gyro_sample.xyz.z);
          printf("	gyro-v0: %f\n", imu.gyro_sample.v[0]);
          printf("	gyro-v1: %f\n", imu.gyro_sample.v[1]);
          printf("	gyro-v2: %f\n", imu.gyro_sample.v[2]);
        } else {
          printf("-ImuData- None\n");
        }
        fflush(stdout);
      }
    }
    dev.stop_cameras();
    dev.stop_imu();
    dev.close();
  }
  k4aloader_free_runtime_handle(instance);
}

void captureFunctionTest() {
  k4a_runtime_handle* instance = k4aloader_create_runtime_handle("D:\\Test\\TestLoad");
  if (instance == nullptr) {
    printf("Failed to load file of dll!!!\n");
    return;
  }
  auto count = k4a::device::get_installed_count(instance);
  k4a::capture cap;

  if (count > 0) {
    auto device = k4a::device::open(0);
    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
    config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
    config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    config.camera_fps = K4A_FRAMES_PER_SECOND_30;
    device.start_cameras(&config);

    int loop = 50;
    while (loop-- > 0) {
      auto res = device.get_capture(&cap);
      printf("Capture0");

      if (!cap.is_valid()) {
        printf("capture is not valid!!!\n");
        return;
      }
      auto hd = cap.handle();
      if (!hd) {
        printf("The handle of capture is null!!!\n");
        return;
      }
      // color image
      auto imageColor = cap.get_color_image();
      if (imageColor) {
        printf(" | Color res:%4dx%4d stride:%5d,dataSize:%d ",
               imageColor.get_height_pixels(),
               imageColor.get_width_pixels(),
               imageColor.get_stride_bytes(),
               (int)imageColor.get_size());
      } else {
        printf(" | Color None                       ");
      }

      // IR16 image
      auto imageIr = cap.get_color_image();
      if (imageIr) {
        printf(" | Ir16 res:%4dx%4d stride:%5d ",
               imageIr.get_height_pixels(),
               imageIr.get_width_pixels(),
               imageIr.get_stride_bytes(),
               (int)imageIr.get_size());
      } else {
        printf(" | Ir16 None                      ");
      }

      // depth16 image
      auto imageDepth = cap.get_color_image();
      if (imageDepth) {
        printf(" | Depth16 res:%4dx%4d stride:%5d\n",
               imageDepth.get_height_pixels(),
               imageDepth.get_width_pixels(),
               imageDepth.get_stride_bytes(),
               (int)imageDepth.get_size());
      } else {
        printf(" | Depth16 None\n");
      }

      auto temp = cap.get_temperature_c();
      if (temp) {
        printf("temperature: %f\n", temp);
      } else {
        printf("temperature None\n");
      }
      cap.reset();

      k4a::capture cap2 = k4a::capture::create(instance);
      cap2.set_color_image(imageColor);
      cap2.set_depth_image(imageDepth);
      cap2.set_ir_image(imageIr);
      cap2.set_temperature_c(temp);
      imageColor.reset();
      imageDepth.reset();
      imageIr.reset();

      printf("Capture1");

      // color image
      imageColor = cap2.get_color_image();
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
      imageIr = cap2.get_color_image();
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
      imageDepth = cap2.get_color_image();
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
      cap2.reset();
    }

    device.stop_cameras();
    device.close();
  }
  k4aloader_free_runtime_handle(instance);
}

void imageFunctionTest() {
  k4a_runtime_handle* instance = k4aloader_create_runtime_handle("D:\\Test\\TestLoad");
  if (instance == nullptr) {
    printf("Failed to load file of dll!!!\n");
    return;
  }
  auto count = k4a::device::get_installed_count(instance);
  k4a::capture cap;

  if (count > 0) {
    auto device = k4a::device::open(0);
    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
    config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
    config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    config.camera_fps = K4A_FRAMES_PER_SECOND_30;
    device.start_cameras(&config);

    int loop = 50;
    while (loop-- > 0) {
      auto res = device.get_capture(&cap);
      printf("Capture0");

      // color image
      auto imageColor = cap.get_color_image();
      if (imageColor) {
        if (imageColor.is_valid()) {
          printf("imageColor is not valid!!!\n");
        }
        auto imhd = imageColor.handle();
        if (!imhd) {
          printf("The handle of image is null!!!\n");
        }
        printf(" | Color res:%4dx%4d stride:%5d,dataSize:%d ",
               imageColor.get_height_pixels(),
               imageColor.get_width_pixels(),
               imageColor.get_stride_bytes(),
               (int)imageColor.get_size());

        auto width = imageColor.get_width_pixels();
        auto height = imageColor.get_height_pixels();
        auto stride = imageColor.get_stride_bytes();
        auto dataSize = (int)imageColor.get_size();
        auto buffer = imageColor.get_buffer();
        printf("bufferSize: %d\n", strlen((char*)buffer));
        auto sec = imageColor.get_device_timestamp();
        printf("timeStamp: %lld\n", sec.count());
        auto exp = imageColor.get_exposure();
        printf("exposure: %d\n", exp);
        auto format = imageColor.get_format();
        printf("format: %d\n", format);
        auto iso = imageColor.get_iso_speed();
        printf("speed: %d\n", iso);
        auto sysTimeStamp = imageColor.get_system_timestamp();
        printf("sysTimeStamp: %lld\n", sysTimeStamp.count());
        auto whiteBlance = imageColor.get_white_balance();
        printf("whiteBlance: %d\n", whiteBlance);

        imageColor.reset();
        printf("Capture1 >>> >>> >>>\n");
        k4a::image imageColor1 = k4a::image::create(format, width, height, stride, instance);
        // k4a::image imageColor2 = k4a::image::create_from_buffer(format, width, height, stride, buffer, dataSize, nullptr, nullptr, instance);
        // auto si = imageColor2.get_size();

        imageColor1.set_timestamp(sec);

        auto exp1 = imageColor1.get_exposure();
        auto sec1 = imageColor1.get_device_timestamp();
        printf("timeStamp: %lld\n", sec1.count());
        imageColor1.reset();
        // imageColor2.reset();
      } else {
        printf(" | Color None                       ");
      }

      cap.reset();
    }

    device.stop_cameras();
    device.close();
  }
  k4aloader_free_runtime_handle(instance);
}

void transformationFunctionTest() {
  k4a_runtime_handle* instance = k4aloader_create_runtime_handle("D:\\Test\\TestLoad");
  if (instance == nullptr) {
    printf("Failed to load file of dll!!!\n");
    return;
  }

  std::string outDir = "D:\\k4aLoader\\k4aLoader\\build";
  std::string fileName = "";
  uint32_t device_count = 0;
  k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
  k4a::image depthImage;
  k4a::image colorImage;

  device_count = k4a::device::get_installed_count(instance);

  if (device_count == 0) {
    printf("No K4A devices found\n");
    return;
  }

  auto device = k4a::device::open(0);

  config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
  config.color_resolution = K4A_COLOR_RESOLUTION_2160P;
  config.depth_mode = K4A_DEPTH_MODE_WFOV_UNBINNED;
  config.camera_fps = K4A_FRAMES_PER_SECOND_15;
  config.synchronized_images_only = true;  // ensures that depth and color images are both available in the capture

  auto cal = device.get_calibration(config.depth_mode, config.color_resolution);

  k4a::transformation tranf(cal.calibration_);

  device.start_cameras(&config);

  k4a::capture cap;

  while (true) {
    // Get a capture
    auto res = device.get_capture(&cap);

    if (!res) {
      continue;
    }

    // Get a depth image
    depthImage = cap.get_depth_image();
    if (depthImage == nullptr) {
      printf("Failed to get depth image from capture\n");
    }

    // Get a color image
    colorImage = cap.get_color_image();
    if (colorImage == nullptr) {
      printf("Failed to get color image from capture\n");
    }

    if (depthImage != nullptr && colorImage != nullptr) {
      break;
    } else if (depthImage != nullptr) {
      depthImage.reset();
      continue;
    } else if (colorImage != NULL) {
      colorImage.reset();
      continue;
    } else {
      continue;
    }
    cap.reset();
  }

  {
    // Compute color point cloud by warping color image into depth camera geometry
    // #ifdef _WIN32
    //		fileName = outDir + "\\color_to_depth.ply";
    // #else
    //		fileName = outDir + "/color_to_depth.ply";
    // #endif

#ifdef _WIN32
    fileName = outDir + "\\depth_to_color.ply";
#else
    fileName = outDir + "/depth_to_color.ply";
#endif

    // int depthImageWidthPixels = depthImage.get_width_pixels();
    // int depthImageHeightPixels = depthImage.get_height_pixels();
    int colorImageWidthPixels = colorImage.get_width_pixels();
    int colorImageHeightPixels = colorImage.get_height_pixels();
    // k4a::image transformedColorImage = k4a::image::create(K4A_IMAGE_FORMAT_COLOR_BGRA32, depthImageWidthPixels, depthImageHeightPixels, depthImageWidthPixels * 4 * (int)sizeof(uint8_t), instance);
    k4a::image transformedColorImage = colorImage;
    k4a::image transformedDepthImage = k4a::image::create(K4A_IMAGE_FORMAT_DEPTH16, colorImageWidthPixels, colorImageHeightPixels, colorImageWidthPixels * 4 * (int)sizeof(uint8_t), instance);

    // k4a::image pointCloudImage = k4a::image::create(K4A_IMAGE_FORMAT_COLOR_BGRA32, depthImageWidthPixels, depthImageHeightPixels, depthImageWidthPixels * 3 * (int)sizeof(int16_t), instance);
    k4a::image pointCloudImage = k4a::image::create(K4A_IMAGE_FORMAT_COLOR_BGRA32, colorImageWidthPixels, colorImageHeightPixels, colorImageWidthPixels * 3 * (int)sizeof(int16_t), instance);

    // tranf.color_image_to_depth_camera(depthImage, colorImage, &transformedColorImage);
    tranf.depth_image_to_color_camera(depthImage, &transformedDepthImage);

    // tranf.depth_image_to_point_cloud(depthImage, K4A_CALIBRATION_TYPE_DEPTH, &pointCloudImage);
    tranf.depth_image_to_point_cloud(transformedDepthImage, K4A_CALIBRATION_TYPE_COLOR, &pointCloudImage);

    std::vector<colorPointT> points;

    // int pointCloudWidth = pointCloudImage.get_width_pixels();
    // int pointCloudHeight = transformedColorImage.get_height_pixels();
    int pointCloudWidth = pointCloudImage.get_width_pixels();
    int pointCloudHeight = transformedColorImage.get_height_pixels();

    int16_t* pointCloudImageData = (int16_t*)(void*)pointCloudImage.get_buffer();
    uint8_t* colorImageData = transformedColorImage.get_buffer();

    for (int i = 0; i < pointCloudWidth * pointCloudHeight; i++) {
      colorPointT point;
      point.xyz[0] = pointCloudImageData[3 * i + 0];
      point.xyz[1] = pointCloudImageData[3 * i + 1];
      point.xyz[2] = pointCloudImageData[3 * i + 2];
      if (point.xyz[2] == 0) {
        continue;
      }

      point.rgb[0] = colorImageData[4 * i + 0];
      point.rgb[1] = colorImageData[4 * i + 1];
      point.rgb[2] = colorImageData[4 * i + 2];
      uint8_t alpha = colorImageData[4 * i + 3];

      if (point.rgb[0] == 0 && point.rgb[1] == 0 && point.rgb[2] == 0 && alpha == 0) {
        continue;
      }

      points.push_back(point);
    }

#define PLY_START_HEADER "ply"
#define PLY_END_HEADER "end_header"
#define PLY_ASCII "format ascii 1.0"
#define PLY_ELEMENT_VERTEX "element vertex"

    // save to the ply file
    std::ofstream ofs(fileName);  // text mode first
    ofs << PLY_START_HEADER << std::endl;
    ofs << PLY_ASCII << std::endl;
    ofs << PLY_ELEMENT_VERTEX << " " << points.size() << std::endl;
    ofs << "property float x" << std::endl;
    ofs << "property float y" << std::endl;
    ofs << "property float z" << std::endl;
    ofs << "property uchar red" << std::endl;
    ofs << "property uchar green" << std::endl;
    ofs << "property uchar blue" << std::endl;
    ofs << PLY_END_HEADER << std::endl;
    ofs.close();

    std::stringstream ss;
    for (size_t i = 0; i < points.size(); ++i) {
      // image data is BGR
      ss << (float)points[i].xyz[0] << " " << (float)points[i].xyz[1] << " " << (float)points[i].xyz[2];
      ss << " " << (float)points[i].rgb[2] << " " << (float)points[i].rgb[1] << " " << (float)points[i].rgb[0];
      ss << std::endl;
    }
    std::ofstream ofs_text(fileName, std::ios::out | std::ios::app);
    ofs_text.write(ss.str().c_str(), (std::streamsize)ss.str().length());

    pointCloudImage.reset();
    transformedColorImage.reset();
  }
  if (depthImage != NULL) {
    depthImage.reset();
  }
  if (colorImage != NULL) {
    colorImage.reset();
  }
  if (cap != NULL) {
    cap.reset();
  }
  device.stop_cameras();
  tranf.destroy();
  cal.reset();
  device.close();
  k4aloader_free_runtime_handle(instance);
}

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
