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
  Serial.println("DOING NOTHING");
}