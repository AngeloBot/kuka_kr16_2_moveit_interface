/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2013, SRI International
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of SRI International nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#include "rclcpp/rclcpp.hpp"

#include "moveit/move_group_interface/move_group_interface.hpp"
//#include <moveit/planning_scene_interface/planning_scene_interface.hpp>

//#include <moveit_msgs/msg/display_robot_state.hpp>
//#include <moveit_msgs/msg/display_trajectory.hpp>

//#include <moveit_msgs/msg/attached_collision_object.hpp>
//#include <moveit_msgs/msg/collision_object.hpp>

//#include <moveit_visual_tools/moveit_visual_tools.h>

#include "geometry_msgs/msg/pose_stamped.hpp"
//#include <moveit/move_group_interface/move_group_interface.h>
//#include <moveit/robot_model_loader/robot_model_loader.h>
//#include <moveit/robot_state/robot_state.h>

using std::placeholders::_1;


// All source files that use ROS logging should define a file-specific
// static const rclcpp::Logger named LOGGER, located at the top of the file
// and inside the namespace with the narrowest scope (if there is one)
static const rclcpp::Logger LOGGER = rclcpp::get_logger("kuka_cpp_interface");
static const std::string PLANNING_GROUP = "kuka_arm";

class KukaMoveItCppInterface : public rclcpp::Node //custom class
{
  //using Inheritance
public:
  // default constructor
  KukaMoveItCppInterface(): Node("moveit_cpp_interface_node_i"){   
      move_group_ptr_= new moveit::planning_interface::MoveGroupInterface(std::make_shared<rclcpp::Node>(this->get_name()), PLANNING_GROUP);
      //moveit::planning_interface::PlanningSceneInterface planning_scene_interface_;
      //const moveit::core::JointModelGroup* joint_model_group_ = move_group_.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
      
      // Getting Basic Information
      // We can print the name of the reference frame for this robot.
      RCLCPP_INFO(LOGGER, "Planning frame: %s", move_group_ptr_->getPlanningFrame().c_str());
      // We can also print the name of the end-effector link for this group.
      RCLCPP_INFO(LOGGER, "End effector link: %s", move_group_ptr_-> getEndEffectorLink().c_str());
      // We can get a list of all the groups in the robot:
      RCLCPP_INFO(LOGGER, "Available Planning Groups:");
      std::copy( move_group_ptr_-> getJointModelGroupNames().begin(), move_group_ptr_-> getJointModelGroupNames().end(),
                std::ostream_iterator<std::string>(std::cout, ", "));

      subscription_= this->create_subscription<geometry_msgs::msg::PoseStamped>(
        "new_pose_goal",1, std::bind(&KukaMoveItCppInterface::moveCallback, this, _1));
  }

    ~KukaMoveItCppInterface()
    {
      delete move_group_ptr_;
    }
private:

    void moveCallback(const geometry_msgs::msg::PoseStamped & target_pose)
    {
      RCLCPP_INFO(this->get_logger(), "Moving to: w='%f' x='%f' y='%f' z='%f'",
        target_pose.pose.orientation.w,
        target_pose.pose.position.x,
        target_pose.pose.position.y,
        target_pose.pose.position.z);

        move_group_ptr_-> setPoseTarget(target_pose);
      moveit::planning_interface::MoveGroupInterface::Plan plan;
      bool success = (move_group_ptr_-> plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);
      RCLCPP_INFO(LOGGER, "Intended Pose Goal %s", success ? "" : "FAILED");
      if(success){
        move_group_ptr_-> move(); //execute move if plan was successful
      }
    }
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr subscription_;
    moveit::planning_interface::MoveGroupInterface* move_group_ptr_;
    //moveit::planning_interface::PlanningSceneInterface planning_scene_interface_;
    //const moveit::core::JointModelGroup* joint_model_group_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  RCLCPP_INFO(LOGGER, "Initializing kuka_moveit_cpp_interface_node_i");
  //auto kuka_cpp_interface_node = std::make_shared<KukaMoveItCppInterface>();
  //rclcpp::spin(kuka_cpp_interface_node);
  rclcpp::spin(std::make_shared<KukaMoveItCppInterface>());
  rclcpp::shutdown();
  return 0;
}