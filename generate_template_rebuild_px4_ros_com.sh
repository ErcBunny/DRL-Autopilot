python3 ./PX4-Autopilot/msg/tools/uorb_to_ros_urtps_topics.py -i ./PX4-Autopilot/msg/tools/urtps_bridge_topics.yaml -o ./ros2_workspace/src/px4_ros_com/templates/urtps_bridge_topics.yaml
python3 ./PX4-Autopilot/msg/tools/uorb_to_ros_msgs.py ./PX4-Autopilot/msg/ ./ros2_workspace/src/px4_msgs/msg/
./ros2_workspace/src/px4_ros_com/scripts/build_ros2_workspace.bash
