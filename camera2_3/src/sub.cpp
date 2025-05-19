// sub.cpp
#include "sub.hpp"

CameraSubscriber::CameraSubscriber()
: Node("camsub_wsl")
{
    subscription_ = this->create_subscription<sensor_msgs::msg::CompressedImage>(
        "image/compressed",
        rclcpp::QoS(10).best_effort(),
        std::bind(&CameraSubscriber::callback, this, std::placeholders::_1)
    );
}

void CameraSubscriber::callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
    if (frame.empty()) {
        RCLCPP_WARN(this->get_logger(), "Failed to decode image.");
        return;
    }
    cv::imshow("wsl", frame);
    cv::waitKey(1);
    RCLCPP_INFO(this->get_logger(), "Received Image : %s, %d x %d",
                msg->format.c_str(), frame.cols, frame.rows);
}
