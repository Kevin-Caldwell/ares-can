# Enables RPi to send a series spin commands to a servo over CAN
# Able to be recieved by arduino

from ares_can_lib.rsx_python.science_can import *
from ares_can_lib.rsx_python.CAN_utilities import initialize_bus

# Instantiate CAN bus
BUS = initialize_bus()

# Example ScienceCanPacket to dissect
multipacket_request = ScienceCanPacket()

multipacket_request.priority = 0
multipacket_request.multipacket_id = 1
multipacket_request.sender = SCI_MODULE_RPI
multipacket_request.receiver = SCI_MODULE_MULTISPECTRAL
multipacket_request.peripheral = SCI_PERIPHERAL_NONE
multipacket_request.extra = SCI_ERROR_SUCCESS
multipacket_request.dlc = 8
multipacket_request.data = bytes([0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])

pulse = assemble_frame_from_SCP(rsx_sci_pkt=multipacket_request)

import time
while (True):
    multipacket_request.data = bytes([(multipacket_request.data[0] + 1) % 18, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])
    TX_BUFFER.append(multipacket_request)
    process_tx(BUS)
    print("Multipacket Request Sent")
    time.sleep(10)