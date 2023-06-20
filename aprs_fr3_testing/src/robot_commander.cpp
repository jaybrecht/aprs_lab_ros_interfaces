#include <aprs_fr3_testing/robot_commander.hpp>

RobotCommander::RobotCommander(const std::string& robot_ip)
 : Node("robot_commander")
{
    try {
        robot_ = std::make_unique<franka::Robot>(robot_ip);
    } catch (franka::Exception const& e) {
        std::cout << e.what() << std::endl;
        throw e;
    }

    publisher_=this->create_publisher <std_msgs::msg::String>("joint_0_state", 10);
    publish_timer_ = this->create_wall_timer(500ms, std::bind(&RobotCommander::timer_callback, this));
    state_timer_ = this->create_wall_timer(500ms, std::bind(&RobotCommander::get_robot_state, this));
}

void RobotCommander::timer_callback(){
    auto message = std_msgs::msg::String();
    message.data = std::to_string(current_state_.q[0]);
    publisher_->publish(message);
}

void RobotCommander::get_robot_state(){
    try {
        current_state_ = robot_->readOnce();
    } catch (franka::Exception const& e) {
        std::cout << e.what() << std::endl;
        throw e;
    }
}
