import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.substitutions import Command, FindExecutable
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    urdf = os.path.join(get_package_share_directory('gmcs_panda_description'), 'urdf',
                                     'gmcs_panda.urdf.xacro')

    robot_description = Command(
        [FindExecutable(name='xacro'), ' ', urdf, ''])
    rviz_file = os.path.join(get_package_share_directory('gmcs_panda_description'), 'rviz', 'visualize.rviz')

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': ParameterValue(robot_description, value_type=str)}],
    )
    
    joint_state_publisher_gui = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui'
    )
    
    rviz = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['--display-config', rviz_file])
    
    nodes_to_start = [
        robot_state_publisher,
        joint_state_publisher_gui, 
        rviz
    ]

    return LaunchDescription(nodes_to_start)
