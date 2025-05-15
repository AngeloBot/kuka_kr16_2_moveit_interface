#include "kuka_tabletennis_interfaces/srv/create_ball.hpp"
#include "ros_gz_interfaces/msg/entity_factory.hpp"
#include "ignition/msgs8/ignition/msgs/boolean.pb.h"
#include "rclcpp/rclcpp.hpp"
#include "gz/transport/Node.hh"

namespace ball_creation_service {
    std::string SERVICE_NODE_NAME = "create_ball_service_node";
    std::string SERVICE_NAME = "create_ball";
    

    class BallCreationService : public rclcpp::Node {
        private:
            void create_ball(const std::shared_ptr<kuka_tabletennis_interfaces::srv::CreateBall::Request> request,
                                std::shared_ptr<kuka_tabletennis_interfaces::srv::CreateBall::Response> response) {
                ros_gz_interfaces::msg::EntityFactory ign_req;
                // Convert request to gz_req
                ignition::msgs::Boolean reply;
                bool call_succeeded;
                this -> gz_node_ -> Request("/world/empty/create", ign_req, 5000, reply, call_succeeded);
                response-> success = call_succeeded && reply.data();
            }
            
            
            rclcpp::Service<kuka_tabletennis_interfaces::srv::CreateBall>::SharedPtr service_;
            gz::transport::Node* gz_node;
        public:
            BallCreationService(std::string name): rclcpp::Node(name) { //, gz::transport::Node* gz_node): rclcpp::Node(name) {
                service_ = this->create_service<kuka_tabletennis_interfaces::srv::CreateBall>(
                        SERVICE_NAME,
                    std::bind(&BallCreationService::create_ball, this, std::placeholders::_1, std::placeholders::_2));
                }
            gz::transport::Node* gz_node_;
        };
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    //auto gz_node = std::make_shared<gz::transport::Node>("gz_node_service_linkage");
    auto node = std::make_shared<ball_creation_service::BallCreationService>(
            ball_creation_service::SERVICE_NODE_NAME); //, gz_node);
    rclcpp::spin(node);
    rclcpp::shutdown();

}
