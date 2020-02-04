#include <mavros/mavros_plugin.h>
 #include <pluginlib/class_list_macros.h>
 #include <iostream>
 #include <mavros_msgs/MPCCmd.h>

 namespace mavros {
 namespace extra_plugins{

 class MPCCommandPlugin : public plugin::PluginBase {
 public:
     MPCCommandPlugin() : PluginBase(),
         nh_("~mpc_command")
    { };

     void initialize(UAS &uas_)
     {
         PluginBase::initialize(uas_);
         cmd_sub_ = nh_.subscribe("command", 10, &MPCCommandPlugin::command_cb, this);
     };

     Subscriptions get_subscriptions()
     {
         return {/* RX disabled */ };
     }

 private:
     ros::NodeHandle nh_;
     ros::Subscriber cmd_sub_;

    void command_cb(const mavros_msgs::MPCCmd::ConstPtr &mpc_command)
     {
         mavlink::common::msg::MPC_COMMAND msg;

         msg.thrust     = mpc_command -> thrust;
         msg.tilt_angle = mpc_command -> tilt_angle;
         msg.torque_x   = mpc_command -> torque_x;
         msg.torque_y   = mpc_command -> torque_y;
         msg.torque_z   = mpc_command -> torque_z;

         UAS_FCU(m_uas)->send_message_ignore_drop(msg);
     }
 };
 }   // namespace extra_plugins
 }   // namespace mavros

PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::MPCCommandPlugin, mavros::plugin::PluginBase)

