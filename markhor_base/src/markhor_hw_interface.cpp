#include <markhor_hw_interface.hpp>

MarkhorHWInterface::MarkhorHWInterface()
  : running_(true)
  , private_nh("~")
  , start_srv_(nh.advertiseService("start", &MarkhorHWInterface::start_callback, this))
  , stop_srv_(nh.advertiseService("stop", &MarkhorHWInterface::start_callback, this))
{
  private_nh.param<double>("max_speed", max_speed, 1.0);

  std::fill_n(this->pos, NUM_JOINTS, 0.0);
  std::fill_n(this->vel, NUM_JOINTS, 0.0);
  std::fill_n(this->eff, NUM_JOINTS, 0.0);
  std::fill_n(this->cmd, NUM_JOINTS, 0.0);

  std::string tracks[NUM_JOINTS] = { "flipper_fr_motor_j", "flipper_rr_motor_j", "flipper_fl_motor_j",
                                     "flipper_rl_motor_j" };
  // connect and register the joint state and velocity interfaces
  for (unsigned int i = 0; i < NUM_JOINTS; ++i)
  {
    hardware_interface::JointStateHandle state_handle(tracks[i], &pos[i], &vel[i], &eff[i]);
    jnt_state_interface.registerHandle(state_handle);

    hardware_interface::JointHandle vel_handle(jnt_state_interface.getHandle(tracks[i]), &cmd[i]);
    jnt_vel_interface.registerHandle(vel_handle);
  }
  registerInterface(&jnt_state_interface);
  registerInterface(&jnt_vel_interface);

  // Initialize publishers and subscribers
  front_left_track_vel_pub_ = nh.advertise<std_msgs::Float32>("front_left_track_vel", 1);
  rear_left_track_vel_pub_ = nh.advertise<std_msgs::Float32>("rear_right_track_vel", 1);
  front_right_track_vel_pub_ = nh.advertise<std_msgs::Float32>("front_right_track_vel", 1);
  rear_right_track_vel_pub_ = nh.advertise<std_msgs::Float32>("rear_track_vel", 1);
}

void MarkhorHWInterface::write()
{
  double diff_ang_speed_front_left = cmd[0];
  double diff_ang_speed_rear_left = cmd[1];
  double diff_ang_speed_front_right = cmd[2];
  double diff_ang_speed_rear_right = cmd[3];
  limitDifferentialSpeed(diff_ang_speed_front_left, diff_ang_speed_rear_left, diff_ang_speed_front_right,
                         diff_ang_speed_rear_right);
  // Set data
  front_left_track_vel_msg.data = diff_ang_speed_front_left;
  rear_left_track_vel_msg.data = diff_ang_speed_rear_left;
  front_right_track_vel_msg.data = diff_ang_speed_front_right;
  rear_right_track_vel_msg.data = diff_ang_speed_rear_right;

  // TODO
  // write to the motor API

  // Publish data
  front_left_track_vel_pub_.publish(front_left_track_vel_msg);
  rear_left_track_vel_pub_.publish(rear_left_track_vel_msg);
  front_right_track_vel_pub_.publish(front_right_track_vel_msg);
  rear_right_track_vel_pub_.publish(rear_right_track_vel_msg);
}

void MarkhorHWInterface::read(const ros::Duration& period)
{
}

ros::Time MarkhorHWInterface::get_time()
{
  prev_update_time = curr_update_time;
  curr_update_time = ros::Time::now();
  return curr_update_time;
}

ros::Duration MarkhorHWInterface::get_period()
{
  return curr_update_time - prev_update_time;
}

bool MarkhorHWInterface::start_callback(std_srvs::Empty::Request& /*req*/, std_srvs::Empty::Response& /*res*/)
{
  running_ = true;
  return true;
}

bool MarkhorHWInterface::stop_callback(std_srvs::Empty::Request& /*req*/, std_srvs::Empty::Response& /*res*/)
{
  running_ = false;
  return true;
}

void MarkhorHWInterface::limitDifferentialSpeed(double& diff_speed_front_left, double& diff_speed_rear_left,
                                                double& diff_speed_front_right, double& diff_speed_rear_right)
{
  // std::max can take a list to find the max value inside.
  double speed = std::max({ std::abs(diff_speed_front_left), std::abs(diff_speed_rear_left),
                            std::abs(diff_speed_front_right), std::abs(diff_speed_rear_right) });
  if (speed > max_speed)
  {
    diff_speed_front_left *= max_speed / speed;
    diff_speed_rear_left *= max_speed / speed;
    diff_speed_front_right *= max_speed / speed;
    diff_speed_rear_right *= max_speed / speed;
  }
}
