#include <science_can.h>
#include <devices/can_config.h>


#include <Servo.h>

Servo optics_servo;
constexpr int OPTICS_SERVO_PIN = 3;
constexpr int OPTICS_UVLED_PIN = 4;
constexpr int OPTICS_BLUELED_PIN = 5;

// PINOUT to Arduino Uno
// SCK to Pin 13
// SI to Pin 11
// SO to Pin 12
// CS to 10

module_t CAN_MODULE = kModuleOptics;

void setup() {

  Serial.begin(115200);

  SPI.begin();

  // Reset MCP2515
  Science::mcp2515.reset();

  // Set CAN speed (must match your bus!)
  Science::mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);

  // Switch to normal mode
  Science::mcp2515.setNormalMode();

  Serial.println("MCP2515 init OK Yayyyyy :)");

  // attaches the servo on pin 9 to the servo object
  optics_servo.attach(OPTICS_SERVO_PIN);

  // Serial.println("Servo attached!");

}

void servo_step(int steps, int angle)
{
  static int vel = 0;
  vel += angle * steps;
  vel %= 180;

  optics_servo.write(vel);
}

void loop() {
  if (const int recv_cnt = Science::process_rx()) {
    Serial.print("Received ");
    Serial.print(recv_cnt);
    Serial.println(" Messages.");
  }

  while (!Science::rx_buffer.empty()) {
    Science::ScienceCANMessage incoming_message = Science::rx_buffer.pop();
    if (incoming_message.peripheral_ = kPeripheralServo) {
      servo_step(incoming_message.data_[0], 90);
    }
    if (incoming_message.peripheral_ = kPeripheralUVLED) {
      digitalWrite(OPTICS_UVLED_PIN, incoming_message.data_[0]);
    }
    if (incoming_message.peripheral_ = kPeripheralBlueLED) {
      digitalWrite(OPTICS_BLUELED_PIN, incoming_message.data_[0]);
    }
  }

  if (const int send_cnt = Science::process_tx()) {
    Serial.print("Sent ");
    Serial.print(send_cnt);
    Serial.println(" Messages.");
  }
}
