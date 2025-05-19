// pub.cpp
#include "pub.hpp"

CameraPublisher::CameraPublisher()
: Node("campub"), loop_rate_(40.0)
{
    publisher_ = this->create_publisher<sensor_msgs::msg::CompressedImage>(
        "image/compressed", rclcpp::QoS(10).best_effort());

    std::string src = "nvarguscamerasrc sensor-id=0 ! "
                      "video/x-raw(memory:NVMM), width=(int)640, height=(int)360, "
                      "format=(string)NV12 ! nvvidconv flip-method=0 ! video/x-raw, "
                      "width=(int)640, height=(int)360, format=(string)BGRx ! "
                      "videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    cap_.open(src, cv::CAP_GSTREAMER);
    if (!cap_.isOpened()) {
        RCLCPP_ERROR(this->get_logger(), "Could not open video!");
        rclcpp::shutdown();
    }
}

void CameraPublisher::run()
{
    cv::Mat frame;
    std_msgs::msg::Header hdr;
    sensor_msgs::msg::CompressedImage::SharedPtr msg;

    while (rclcpp::ok()) {
        cap_ >> frame;
        if (frame.empty()) {
            RCLCPP_ERROR(this->get_logger(), "Frame is empty");
            break;
        }
        msg = cv_bridge::CvImage(hdr, "bgr8", frame).toCompressedImageMsg();
        publisher_->publish(*msg);
        loop_rate_.sleep();
    }
}
