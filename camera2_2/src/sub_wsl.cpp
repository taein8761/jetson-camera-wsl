#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include <memory>
#include <functional>
#include <iostream>

using std::placeholders::_1;

class ImageSubscriber : public rclcpp::Node
{
public:
    ImageSubscriber()
        : Node("sub_wsl")
    {
        auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();
        sub_ = this->create_subscription<sensor_msgs::msg::CompressedImage>(
            "image/compressed", qos,
            std::bind(&ImageSubscriber::image_callback, this, _1)
        );

      
        std::string filename = "output.mp4";
        int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');  
        int fps = 30;
        cv::Size frame_size(640, 480); 
        writer_.open(filename, codec, fps, frame_size, true);

        if (!writer_.isOpened()) {
            RCLCPP_ERROR(this->get_logger(), "VideoWriter �ʱ�ȭ ����!");
        }
        else {
            RCLCPP_INFO(this->get_logger(), "output.mp4 ���� ���� ����");
        }

        rclcpp::on_shutdown([this]() {
            RCLCPP_INFO(this->get_logger(), "Ctrl+C �Է� - ���� ����");
            writer_.release();
            cv::destroyAllWindows();
            });
    }

private:
    void image_callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg)
    {
        cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
        if (frame.empty()) {
            RCLCPP_WARN(this->get_logger(), "�̹��� ���ڵ� ����");
            return;
        }

        // ù �����ӿ��� ũ�� �ʱ�ȭ
        if (!writer_.isOpened() || writer_frame_size_ != frame.size()) {
            writer_frame_size_ = frame.size();
            writer_.release();
            writer_.open("output.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 30, writer_frame_size_, true);
        }

        // ȭ�� ���
        cv::imshow("Image View", frame);
        cv::waitKey(1);

        // ����
        writer_.write(frame);

        RCLCPP_INFO(this->get_logger(), "Frame �����: %dx%d", frame.cols, frame.rows);
    }

    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr sub_;
    cv::VideoWriter writer_;
    cv::Size writer_frame_size_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImageSubscriber>());
    rclcpp::shutdown();
    return 0;
}
