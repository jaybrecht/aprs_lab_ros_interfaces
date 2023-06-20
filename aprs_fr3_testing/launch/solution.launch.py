from launch import LaunchDescription
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution, LaunchConfiguration
from launch.actions import DeclareLaunchArgument, OpaqueFunction, IncludeLaunchDescription, Shutdown
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.conditions import IfCondition
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node

from moveit_configs_utils import MoveItConfigsBuilder
from ament_index_python.packages import get_package_share_directory
import yaml
import os

def launch_setup(context, *args, **kwargs):
    robot_description_content = Command(
        [
            PathJoinSubstitution([FindExecutable(name="xacro")]),
            " ",
            PathJoinSubstitution([FindPackageShare("aprs_fr3_description"), "urdf", "aprs_fr3.urdf.xacro"]), 
            " "
        ]
    )

    robot_description = {"robot_description": robot_description_content} 

    # Franka Bringup
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[robot_description],
    )

    controllers = PathJoinSubstitution([FindPackageShare('aprs_fr3_testing'),'config','controllers.yaml'])

    controller_manager = Node(
        package='controller_manager',
        executable='ros2_control_node',
        parameters=[robot_description, controllers],
        on_exit=Shutdown(),
    )

    joint_state_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster'],
        output='screen',
    )

    nodes_to_start = [
        robot_state_publisher,
        controller_manager,
        joint_state_broadcaster_spawner,
    ]

    return nodes_to_start

def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])