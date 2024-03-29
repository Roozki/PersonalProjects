/*
 * Created By: Gareth Ellis
 * Created On: July 16th, 2016
 * Description: An example node that subscribes to a topic publishing strings,
 *              and re-publishes everything it receives to another topic with
 *              a "!" at the end
 */

#include <TeensyInit.h>

Wee::Wee(int argc, char **argv, std::string node_name) {
    // Setup NodeHandles
    ros::init(argc, argv, node_name);
    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");

    // Obtains character from the parameter server (or launch file)
    std::string parameter_name    = "port";
    std::string default_character = "/dev/tty0";
    //SB_getParam(private_nh, parameter_name, suffix, default_character);

    // Setup Subscriber(s)
    std::string topic_to_subscribe_to = "subscribe_topic";
    int queue_size                    = 10;
    microcontroller_subscriber                  = nh.subscribe(
    topic_to_subscribe_to, queue_size, &Wee::subscriberCallBack, this);

    // Setup Publisher(s)
    std::string topic = private_nh.resolveName("publish_topic");
    queue_size        = 1;
    microcontroller_publisher = private_nh.advertise<std_msgs::String>(topic, queue_size);
    ROS_INFO("Ready to chat with a teensy!");
}

void Wee::subscriberCallBack(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("Received message");
    std::string input_string = msg->data.c_str();
    std::string new_msg = addCharacterToString(input_string, suffix);
    republishMsg(new_msg);
}

std::string Wee::addCharacterToString(std::string input_string, std::string suffix) {
    return input_string.append(suffix);
}

void Wee::republishMsg(std::string msg_to_publish) {
    std_msgs::String string_to_publish;
    string_to_publish.data = msg_to_publish;
    microcontroller_publisher.publish(string_to_publish);
    ROS_INFO("Published message");
}
