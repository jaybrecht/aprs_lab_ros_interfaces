#include <rclcpp/rclcpp.hpp>

#include <franka/exception.h>
#include <franka/robot.h>
#include <std_msgs/msg/string.hpp>

#include <chrono>

using namespace std::chrono_literals;

class RobotCommander : public rclcpp::Node
{
public:
  RobotCommander(const std::string&);
private:
  std::unique_ptr<franka::Robot> robot_;
  rclcpp::TimerBase::SharedPtr publish_timer_;
  rclcpp::TimerBase::SharedPtr state_timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  franka::RobotState current_state_;

  void get_robot_state();
  void timer_callback();
};
