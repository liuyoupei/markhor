#include <markhor_hw_interface_flipper.hpp>
#include <string>
#include <unistd.h>

MarkhorHWInterfaceFlipper::MarkhorHWInterfaceFlipper()
{
  joint_names_.push_back("flipper_fl_j");
  joint_names_.push_back("flipper_fr_j");
  joint_names_.push_back("flipper_rl_j");
  joint_names_.push_back("flipper_rr_j");

  num_joints = joint_names_.size();

  // Status
  joint_position_.resize(num_joints, 0.0);
  joint_velocity_.resize(num_joints, 0.0);
  joint_effort_.resize(num_joints, 0.0);

  // Command
  joint_position_command_.resize(num_joints, 0.0);
  joint_velocity_command_.resize(num_joints, 0.0);
  joint_effort_command_.resize(num_joints, 0.0);

  setupRosControl();
  setupCtreDrive();

  nh.getParam("/markhor/markhor_base_flipper_node/config_folder_location", config_folder_str);
  nh.getParam("/markhor/markhor_base_flipper_node/config_file_1", config_file_1);
  nh.getParam("/markhor/markhor_base_flipper_node/config_file_2", config_file_2);

  loadDrivePosition();
}

void MarkhorHWInterfaceFlipper::setupRosControl()
{
  // connect and register the joint state and effort interfaces
  for (unsigned int joint_id = 0; joint_id < num_joints; ++joint_id)
  {
    hardware_interface::JointStateHandle state_handle(joint_names_[joint_id], &joint_position_[joint_id],
                                                      &joint_velocity_[joint_id], &joint_effort_[joint_id]);

    joint_state_interface_.registerHandle(state_handle);

    hardware_interface::JointHandle joint_handle_position = hardware_interface::JointHandle(
        joint_state_interface_.getHandle(joint_names_[joint_id]), &joint_position_command_[joint_id]);

    position_joint_interface_.registerHandle(joint_handle_position);
  }

  registerInterface(&joint_state_interface_);
  registerInterface(&position_joint_interface_);
}

void MarkhorHWInterfaceFlipper::setupCtreDrive()
{
  std::string interface = "can0";
  ctre::phoenix::platform::can::SetCANInterface(interface.c_str());

  const int kTimeoutMs = 30;

  // if (nh.getParam("/markhor/markhor_base_flipper_node/front_left", drive_fl_id) == true)
  // {
  //   ROS_INFO("CREATE FRONT LEFT");
  //   front_left_drive = std::make_unique<TalonSRX>(drive_fl_id);
  //   front_left_drive->ConfigFactoryDefault();
  //   front_left_drive->GetSensorCollection().SetPulseWidthPosition(0, 30);
  // int error;
  // do
  // {
  //   error = front_left_drive->GetSensorCollection().SetPulseWidthPosition(0, 30); // TODO à changer
  //   ROS_INFO("SetPulseWidthPosition error code : %d", error);
  // } while (error == 0);
  //   front_left_drive->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 50);
  //   front_left_drive->SetSensorPhase(true);
  //   front_left_drive->ConfigNominalOutputForward(0, kTimeoutMs);
  //   front_left_drive->ConfigNominalOutputReverse(0, kTimeoutMs);
  //   front_left_drive->ConfigPeakOutputForward(0.3, kTimeoutMs);
  //   front_left_drive->ConfigPeakOutputReverse(-0.3, kTimeoutMs);

  //   float kP, kI, kD = 0.0;
  //   nh.getParam("/markhor/markhor_base_flipper_node/kP", kP);
  //   nh.getParam("/markhor/markhor_base_flipper_node/kI", kI);
  //   nh.getParam("/markhor/markhor_base_flipper_node/kD", kD);

  //   front_left_drive->SelectProfileSlot(0, 0);
  //   front_left_drive->Config_kF(0, 0, 30);
  //   front_left_drive->Config_kP(0, kP, 30);
  //   front_left_drive->Config_kI(0, kI, 30);
  //   front_left_drive->Config_kD(0, kD, 30);
  // }
  if (nh.getParam("/markhor/markhor_base_flipper_node/front_right", drive_fr_id) == true)
  {
    ROS_INFO("CREATE FRONT RIGHT");
    // front_right_drive = std::make_unique<TalonSRX>(drive_fr_id);
    // front_right_drive->ConfigFactoryDefault();
    // front_right_drive->GetSensorCollection().SetPulseWidthPosition(0, 30);
    // int error;
    // do
    // {
    //   error = front_left_drive->GetSensorCollection().SetPulseWidthPosition(0, 30); // TODO à changer
    //   ROS_INFO("SetPulseWidthPosition error code : %d", error);
    // } while (error == 0);
    // front_right_drive->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 50);
    // front_right_drive->SetSensorPhase(true);
    // front_right_drive->ConfigNominalOutputForward(0, kTimeoutMs);
    // front_right_drive->ConfigNominalOutputReverse(0, kTimeoutMs);
    // front_right_drive->ConfigPeakOutputForward(0.3, kTimeoutMs);
    // front_right_drive->ConfigPeakOutputReverse(-0.3, kTimeoutMs);

    // float kP, kI, kD = 0.0;
    // nh.getParam("/markhor/markhor_base_flipper_node/kP", kP);
    // nh.getParam("/markhor/markhor_base_flipper_node/kI", kI);
    // nh.getParam("/markhor/markhor_base_flipper_node/kD", kD);

    // front_right_drive->SelectProfileSlot(0, 0);
    // front_right_drive->Config_kF(0, 0, 30);
    // front_right_drive->Config_kP(0, kP, 30);
    // front_right_drive->Config_kI(0, kI, 30);
    // front_right_drive->Config_kD(0, kD, 30);
  }
  if (nh.getParam("/markhor/markhor_base_flipper_node/rear_left", drive_rl_id) == true)
  {
    ROS_INFO("CREATE REAR LEFT");
    rear_left_drive = std::make_unique<TalonSRX>(drive_rl_id);
    rear_left_drive->ConfigFactoryDefault();
    rear_left_drive->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 50);
    rear_left_drive->SetSensorPhase(true);
    rear_left_drive->ConfigNominalOutputForward(0, kTimeoutMs);
    rear_left_drive->ConfigNominalOutputReverse(0, kTimeoutMs);
    rear_left_drive->ConfigPeakOutputForward(0.3, kTimeoutMs);
    rear_left_drive->ConfigPeakOutputReverse(-0.3, kTimeoutMs);

    float kP, kI, kD = 0.0;
    nh.getParam("/markhor/markhor_base_flipper_node/kP", kP);
    nh.getParam("/markhor/markhor_base_flipper_node/kI", kI);
    nh.getParam("/markhor/markhor_base_flipper_node/kD", kD);

    rear_left_drive->SelectProfileSlot(0, 0);
    rear_left_drive->Config_kF(0, 0, 30);
    rear_left_drive->Config_kP(0, kP, 30);
    rear_left_drive->Config_kI(0, kI, 30);
    rear_left_drive->Config_kD(0, kD, 30);

    nh.getParam("/markhor/markhor_base_flipper_node/rear_left_drive_upper_limit", rear_left_drive_upper_limit);
    nh.getParam("/markhor/markhor_base_flipper_node/rear_left_drive_lower_limit", rear_left_drive_lower_limit);
  }
  // if (nh.getParam("/markhor/markhor_base_flipper_node/rear_right", drive_rr_id) == true)
  // {
  //   rear_right_drive = std::make_unique<TalonSRX>(drive_rr_id);
  // }

  nh.getParam("/markhor/markhor_base_flipper_node/ratio", ratio);
}

void MarkhorHWInterfaceFlipper::write()
{
  if (hasResetOccurred() == true)
  {
    loadDrivePosition();
  }

  // ROS_INFO("HWI FLIPPER WRITE");
  // ROS_INFO("position FL command : %f", joint_position_command_[0]);
  // ROS_INFO("position FR command : %f", joint_position_command_[1]);
  ROS_INFO("position RL command : %f", joint_position_command_[2]);
  ROS_INFO("RL lower limit : %f", rear_left_drive_lower_limit);
  ROS_INFO("RL upper limit : %f", rear_left_drive_upper_limit);
  // ROS_INFO("position RR command : %f", joint_position_command_[3]);

  ctre::phoenix::unmanaged::FeedEnable(100);

  // // Write to drive
  // front_left_drive->Set(ControlMode::Position, joint_position_command_[0]);
  // front_right_drive->Set(ControlMode::Position, joint_position_command_[1]);
  // printDriveInfo(front_right_drive);
  printDriveInfo(rear_left_drive);
  if (joint_position_command_[2] >= rear_left_drive_lower_limit &&
      joint_position_command_[2] < rear_left_drive_upper_limit)
  {
    rear_left_drive->Set(ControlMode::Position, joint_position_command_[2]);
  }
  saveDrivePosition();
  // front_right_drive->Set(ControlMode::Position, joint_position_command_[1]);
  // rear_left_drive->Set(ControlMode::Position, front_left_track_vel_msg.data);
  // rear_right_drive->Set(ControlMode::Position, front_left_track_vel_msg.data);
}

void MarkhorHWInterfaceFlipper::read()
{
  // Read from the motor API, going to read from the TalonSRX objects
  // ROS_INFO("HWI FLIPPER READs");
}

void MarkhorHWInterfaceFlipper::printDriveInfo(std::unique_ptr<TalonSRX>& drive)
{
  ROS_INFO("HasResetOccurred : %s", drive->HasResetOccurred() ? "true" : "false");
  ROS_INFO("GetPulseWidthPosition %d", drive->GetSensorCollection().GetPulseWidthPosition());
  ROS_INFO("GetClosedLoopError %d", drive->GetClosedLoopError(0));
  ROS_INFO("GetClosedLoopTarget %f", drive->GetClosedLoopTarget(0));
}

void MarkhorHWInterfaceFlipper::saveDrivePosition()
{
  // TODO : Only write if the value is different

  if (alternating_value == true)
  {
    alternating_value = false;
    writeDrivePositionToFile(config_file_1);
  }
  else
  {
    alternating_value = true;
    writeDrivePositionToFile(config_file_2);
  }

  drive_config_file.close();
}

void MarkhorHWInterfaceFlipper::writeDrivePositionToFile(std::string config_file_name)
{
  drive_config_file.open((config_folder_str + config_file_name).c_str(), std::fstream::out | std::fstream::trunc);
  if (!drive_config_file)
  {
    ROS_INFO("error openning file %s", config_file_name.c_str());
  }
  drive_config_file << rear_left_drive->GetDeviceID() << ":"
                    << rear_left_drive->GetSensorCollection().GetPulseWidthPosition() << std::endl;
  drive_config_file.rdbuf()->pubsync();
}

void MarkhorHWInterfaceFlipper::loadDrivePosition()
{
  bool is_drives_config_file_1_empty, is_drives_config_file_2_empty;

  // TODO : there's doesn't need to have two fstream object, but right now it doesn't work with only one
  std::fstream drive_config_file_1, drive_config_file_2;

  // Check if the files exists
  drive_config_file_1.open((config_folder_str + config_file_1).c_str(), std::fstream::in);
  drive_config_file_2.open((config_folder_str + config_file_2).c_str(), std::fstream::in);
  if (drive_config_file_1.good() == false || drive_config_file_2.good() == false)
  {
    ROS_FATAL("Missing drives calibration file(s). You need to either create the files or calibrate it before using "
              "markhor_base_flipper node");
    ros::shutdown();
  }

  // Check if the files are empty
  if (drive_config_file_1.peek() != std::fstream::traits_type::eof())
  {
    is_drives_config_file_1_empty = false;
  }
  else
  {
    is_drives_config_file_1_empty = true;
    ROS_WARN("drives config file 1 is empty");
  }

  if (is_drives_config_file_1_empty == true && drive_config_file_2.peek() != std::fstream::traits_type::eof())
  {
    is_drives_config_file_2_empty = false;
  }
  else
  {
    is_drives_config_file_2_empty = true;
    if (drive_config_file_2.peek() == std::fstream::traits_type::eof())
    {
      ROS_WARN("drives config file 2 is empty");
    }
  }

  if (is_drives_config_file_1_empty == true && is_drives_config_file_2_empty == true)
  {
    // TODO : Add link to documentation on how to calibrate the flipper and create the files
    ROS_FATAL("Drives configuration files are empty. Please calibrate before using markhor_base_flipper node");
    ros::shutdown();
  }

  // Read the files
  if (is_drives_config_file_1_empty == false)
  {
    readDrivePositionFromFile(config_file_1, drive_config_file_1);
    drive_config_file_1.close();
  }
  if (is_drives_config_file_2_empty == false)
  {
    readDrivePositionFromFile(config_file_2, drive_config_file_2);
    drive_config_file_2.close();
  }
}

void MarkhorHWInterfaceFlipper::readDrivePositionFromFile(std::string config_file_name, std::fstream& config_file)
{
  std::string line;
  while (std::getline(config_file, line))
  {
    parseDrivePosition(line);
  }
}

void MarkhorHWInterfaceFlipper::parseDrivePosition(std::string line)
{
  int drive_id;
  float drive_position;
  std::string::size_type pos = line.find(':');
  if (pos != std::string::npos)
  {
    drive_id = std::stoi(line.substr(0, pos));
    drive_position = std::stof(line.substr(pos + 1, line.length()));
    std::unique_ptr<TalonSRX> tmp_drive;
    if (drive_id == drive_fl_id)
    {
      applyDrivePosition(front_left_drive, drive_position);
    }
    if (drive_id == drive_fr_id)
    {
      applyDrivePosition(front_right_drive, drive_position);
    }
    if (drive_id == drive_rl_id)
    {
      applyDrivePosition(rear_left_drive, drive_position);
    }
    if (drive_id == drive_rr_id)
    {
      applyDrivePosition(rear_right_drive, drive_position);
    }
  }
}

void MarkhorHWInterfaceFlipper::applyDrivePosition(std::unique_ptr<TalonSRX>& drive, float drive_position)
{
  int error;
  do
  {
    error = drive->GetSensorCollection().SetPulseWidthPosition(drive_position, 30);
    ROS_INFO("SetPulseWidthPosition error code : %d", error);
  } while (error != ErrorCode::OKAY);
}

bool MarkhorHWInterfaceFlipper::hasResetOccurred()
{
  if (front_left_drive)
  {
    if (front_left_drive->HasResetOccurred())
    {
      return true;
    }
  }
  if (front_right_drive)
  {
    if (front_right_drive->HasResetOccurred())
    {
      return true;
    }
  }
  if (rear_left_drive)
  {
    if (rear_left_drive->HasResetOccurred())
    {
      return true;
    }
  }
  if (rear_right_drive)
  {
    if (rear_right_drive->HasResetOccurred())
    {
      return true;
    }
  }
  return false;
}