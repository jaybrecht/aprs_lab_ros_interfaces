#include <aprs_fr3_testing/robot_commander.hpp>

#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto robot_commander = std::make_shared<RobotCommander>("172.16.0.2");

  rclcpp::spin(robot_commander);

  // rclcpp::executors::MultiThreadedExecutor executor;
  // executor.add_node(robot_commander);
  
  // executor.spin();

  rclcpp::shutdown();
}