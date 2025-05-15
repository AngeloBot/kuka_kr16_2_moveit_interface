from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder
from ament_index_python.packages import get_package_share_directory

from launch_ros.substitutions import FindPackageShare
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument, RegisterEventHandler
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.conditions import IfCondition, UnlessCondition

def generate_launch_description():
    
    this_pkg="kuka_kr16_2_moveit_interface"
    description_pkg="kuka_kr16_2_angelo"
    moveit_config_pkg="kuka_moveit_test"

    #moveit_config_yaml="config/kr16_2_simulated_config.yaml"
    robot_name="kr16_2"
    
    # Launch configuration variables
    use_c_node= LaunchConfiguration('use_c_node')
    use_sim_time = LaunchConfiguration('use_sim_time')
 
    # Declare the launch arguments
    declare_use_sim_time_cmd = DeclareLaunchArgument(
        name='use_sim_time',
        default_value='true',
        description='Use simulation (Gazebo) clock if true')
    
    # Declare the launch arguments
    declare_use_c_node_cmd = DeclareLaunchArgument(
        name='use_c_node',
        default_value='true',
        description='true for composition, false for inheritance')
    

    moveit_config = (
        MoveItConfigsBuilder(robot_name=robot_name,package_name=moveit_config_pkg)
        .robot_description(file_path=get_package_share_directory(moveit_config_pkg) + "/config/kr16_2.urdf.xacro")
        .robot_description_kinematics(file_path= get_package_share_directory(moveit_config_pkg) + "/config/kinematics.yaml")
        .robot_description_semantic(file_path= get_package_share_directory(moveit_config_pkg) + "/config/kr16_2.srdf")
        .pilz_cartesian_limits(file_path= get_package_share_directory(moveit_config_pkg) + "/config/pilz_cartesian_limits.yaml")
        .trajectory_execution(file_path= get_package_share_directory(moveit_config_pkg) + "/config/moveit_controllers.yaml")
        .joint_limits(file_path= get_package_share_directory(moveit_config_pkg) + "/config/joint_limits.yaml")
        .planning_scene_monitor(publish_robot_description=True, publish_robot_description_semantic=True, publish_planning_scene=True) #comment if launching moveit_py_node together
        #.moveit_cpp(file_path=get_package_share_directory(rl_pkg)+ "/config/planning_configs.yaml") #comment if not launching moveit_py_node together
        .planning_pipelines(pipelines=["ompl"])
        .to_moveit_configs()
    )
    # MoveGroupInterface demo executable
    moveit_interface_node_i = Node(
        #name="moveit_cpp_interface_node",
        package=this_pkg,
        executable="moveit_interface_node_i",
        output="screen",
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            {'use_sim_time': use_sim_time}
        ],
        condition=UnlessCondition(use_c_node)
    )

    moveit_interface_node_c = Node(
        #name="moveit_cpp_interface_node",
        package=this_pkg,
        executable="moveit_interface_node_c",
        output="screen",
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            {'use_sim_time': use_sim_time}
            
        ],
        condition=IfCondition(use_c_node)
    )

    #return LaunchDescription([moveit_interface_node_i])

    ld=LaunchDescription()

    ld.add_action(declare_use_sim_time_cmd)
    ld.add_action(declare_use_c_node_cmd)
    ld.add_action(moveit_interface_node_c)
    ld.add_action(moveit_interface_node_i)

    return ld