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

    move_to_home_srv_ = this->create_service<std_srvs::srv::Trigger>(
        "go_to_home", 
        std::bind(
            &RobotCommander::move_to_home_cb_, this,
            std::placeholders::_1, std::placeholders::_2));

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

void RobotCommander::move_to_home_cb_(const std::shared_ptr<std_srvs::srv::Trigger::Request> request, 
    std::shared_ptr<std_srvs::srv::Trigger::Response> response){
    (void)request;
    std::array<double, 7> q_goal = {{0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4}};
    geometry_msgs::msg::Pose target_pose;
    try{
        MotionGenerator motion_generator(0.2, q_goal);
        robot_->control(motion_generator);
    } catch (const franka::Exception& e){
        response->success = false;
        return;
    }
    response->success = true;
}
