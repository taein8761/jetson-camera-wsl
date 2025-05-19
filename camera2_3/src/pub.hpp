// pub.hpp

#pragma once

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"
#include <string>

class CameraPublisher : public rclcpp::Node
{
public:
    CameraPublisher();
    void run();

private:
    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher_;
    cv::VideoCapture cap_;
    rclcpp::WallRate loop_rate_;  
};
