#include <circular_buffer.h>
#include <science_can.h>

void setup() {
  Serial.begin(115200);
  SPI.begin();
  Science::mcp2515.reset();
  Science::mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  Science::mcp2515.setNormalMode();
  Serial.println("MCP2515 init OK Yayyyyy :)");
}

void loop() {
  // Serial.println("DOING NOTHING");
  if (const int recv_cnt = Science::process_rx()) {
    Serial.print("Received ");
    Serial.print(recv_cnt);
    Serial.println(" Messages.");
  }

  while (!Science::rx_buffer.empty()) {
    Science::ScienceCANMessage incoming_message = Science::rx_buffer.pop();
    if (incoming_message.peripheral_ = kPeripheralServo) {
      servo_step(incoming_message.data_[0]);
    }
  }

  if (const int send_cnt = Science::process_tx()) {
    Serial.print("Sent ");
    Serial.print(send_cnt);
    Serial.println(" Messages.");
  }
}