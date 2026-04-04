# The C CAN Library

The CAN library provides a basic wrapper over
the bare CAN message, so Science microcontrollers
can use it as needed.
The RX code takes in a raw incoming CAN message, 
converts it into a Science packet, and places it
into the RX buffer. This is repeated for all 
incoming CAN messages. The RX code oly filters 
through messages addressed with the value as 
the receiver. 

The TX code takes in a Science packet from the
TX buffer, packs it into a CAN message and sends
it through the CAN bus.

During the processing stage, RX buffer elements are
expected to be read and processed by the 
componenets, which eventually generate TX commands
to send to the CAN bus. 

The can_setup function does not successfully execute
the following two lines of code:
```
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHz);
  mcp2515.setNormalMode();
```
These need to be run in the .ino files. 
If the science_can header files are turned into 
an Arduino library, I believe the setup function
in the class for that library would be able to
successfully call these functions instead.
