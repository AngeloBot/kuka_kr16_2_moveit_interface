cmake_minimum_required(VERSION 3.8)
project(kuka_kr16_2_moveit_interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(geometry_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(moveit_interface_node_i src/moveit_interface_node_i.cpp)
ament_target_dependencies(moveit_interface_node_i
  "rclcpp"
  "moveit_ros_planning_interface"
  "geometry_msgs"
)

add_executable(moveit_interface_node_c src/moveit_interface_node_c.cpp)
ament_target_dependencies(moveit_interface_node_c
  "rclcpp"
  "moveit_ros_planning_interface"
  "geometry_msgs"
)

add_executable(interface_test_c src/interface_test_c.cpp)
ament_target_dependencies(interface_test_c
  "rclcpp"
  "moveit_ros_planning_interface"
  "geometry_msgs"
)

add_executable(interface_test_i src/interface_test_i.cpp)
ament_target_dependencies(interface_test_i
  "rclcpp"
  "moveit_ros_planning_interface"
  "geometry_msgs"
)

install(TARGETS
  moveit_interface_node_i
  moveit_interface_node_c
  interface_test_c
  interface_test_i
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
