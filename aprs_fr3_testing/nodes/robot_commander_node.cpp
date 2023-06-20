#include <aprs_fr3_testing/robot_commander.hpp>

#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto robot_commander = std::make_shared<RobotCommander>();

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(robot_commander);
  
  executor.spin();

  rclcpp::shutdown();
}