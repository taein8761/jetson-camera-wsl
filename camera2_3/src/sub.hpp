// sub.hpp
#pragma once

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"

class CameraSubscriber : public rclcpp::Node
{
public:
    CameraSubscriber();

private:
    void callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg);
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr subscription_;
};
