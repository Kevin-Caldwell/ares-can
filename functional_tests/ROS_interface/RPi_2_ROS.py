# Contents of this script sends SCP Messages from RPi back to Ground Station through ROS
# Communicates contents from CAN data through ROS

import rclpy
from rclpy.node import Node
from ares_can_lib.rsx_python.science_can import *
from ares_can_lib.rsx_python.CAN_utilities import initialize_bus
import numpy as np
import time

from science.msg import SCP

# Link from RPi to ROS, where Pi sends SCP messages back with ROS
class RPi_2_ROS_Link(Node):

    def __init__(self):
        super().__init__('ros_sender')
        
        self.publisher = self.create_publisher(
            SCP,
            "SCP", 
            10
        )
        
    def ros_sender_push(self, msg : SCP):
        
        # Fill up ROS topic with info from SCP 
        return 1 #placeholder

        # Send msg back to ground station over ROS

if __name__ == "__main__": 
    # Instantiate CAN bus
    BUS = initialize_bus()

    # Create instance of ROS listener 
    sender = RPi_2_ROS_Link()
    
    while (True):
        process_rx(BUS)

        # We need to figure out whether an SCP is supposed to get sent back through ROS or not
        time.sleep(1)
        
