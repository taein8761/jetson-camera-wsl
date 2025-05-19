#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include <memory>
#include <functional>
#include <iostream>

using std::placeholders::_1;

void mysub_callback(rclcpp::Node::SharedPtr node, const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    // 압축 이미지 디코딩
    cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
    if (frame.empty()) {
        RCLCPP_WARN(node->get_logger(), "Failed to decode image.");
        return;
    }

    // Grayscale 변환
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Binary 변환
    cv::Mat binary;
    cv::threshold(gray, binary, 100, 255, cv::THRESH_BINARY);

    // 출력
    cv::imshow("Original (WSL)", frame);
    cv::imshow("Grayscale (WSL)", gray);
    cv::imshow("Binary (WSL)", binary);
    cv::waitKey(1);

    RCLCPP_INFO(node->get_logger(), "Image Received: %s | Size: %dx%d",
        msg->format.c_str(), frame.cols, frame.rows);
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("camsub_wsl");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();
    std::function<void(const sensor_msgs::msg::CompressedImage::SharedPtr msg)> fn;
    fn = std::bind(mysub_callback, node, _1);
    auto mysub = node->create_subscription<sensor_msgs::msg::CompressedImage>(
        "image/compressed", qos_profile, fn);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
