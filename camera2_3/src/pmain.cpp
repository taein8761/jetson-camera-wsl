// pmain.cpp
#include "pub.hpp"

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CameraPublisher>();
    node->run();
    rclcpp::shutdown();
    return 0;
}
