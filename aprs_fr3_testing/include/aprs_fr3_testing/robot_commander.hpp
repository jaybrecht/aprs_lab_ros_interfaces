#include <rclcpp/rclcpp.hpp>

#include <franka/exception.h>
#include <franka/robot.h>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <geometry_msgs/msg/pose.hpp>

#include <aprs_fr3_testing/examples_common.h>

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
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr move_to_home_srv_;

  void get_robot_state();
  void timer_callback();
  void move_to_home_cb_(const std::shared_ptr<std_srvs::srv::Trigger::Request>request, 
    std::shared_ptr<std_srvs::srv::Trigger::Response> response);
};
