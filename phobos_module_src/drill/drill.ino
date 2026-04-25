#include <science_can.h>
#include <devices/can_config.h>


#include <Servo.h>

Servo drill_servo;
constexpr int DRILL_SERVO_PIN = 3;
constexpr int DRILL_ELECTROMAGNET_PIN = 4;
constexpr int ULTRASONIC_SENSOR_TRIG_PIN = 5;
constexpr int UNTRASONIC_SENSOR_ECHO_PIN = 6;
constexpr int ELECTROMAGNET_PIN = 7;

// PINOUT to Arduino Uno
// SCK to Pin 13
// SI to Pin 11
// SO to Pin 12
// CS to 10

module_t CAN_MODULE = kModuleDrill;

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
  drill_servo.attach(DRILL_SERVO_PIN);

  // Serial.println("Servo attached!");
  pinMode(ULTRASONIC_SENSOR_TRIG_PIN, OUTPUT);
  pinMode(UNTRASONIC_SENSOR_ECHO_PIN, INPUT);
  pinMode(ELECTROMAGNET_PIN, OUTPUT);

}

void servo_step(int steps, int angle)
{
  static int vel = 0;
  vel += angle * steps;
  vel %= 180;

  drill_servo.write(vel);
}

float sample_ultrasonic() {
  digialWrite(ULTRASONIC_SENSOR_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_SENSOR_TRIG_PIN, HIGH);
  delayMicrosections(10);
  digitalWrite(ULTRASONIC_SENSOR_TRIG_PIN, LOW);

  float duration = pulseIn(UNTRASONIC_SENSOR_ECHO_PIN, HIGH);
  return (duration * 0.343) / 2;
}

void electromagnet_toggle(bool state) {
  digitalWrite(ELECTROMAGNET_PIN, state);
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
    if (incoming_message.peripheral_ = kPeripheralUltrasonic) {
      float distance = sample_ultrasonic();
      Science::ScienceCANMessage msg = {
        .priority_ = 0,
        .multipacket_id_ = 0,
        .sender_ = kModuleDrill,
        .receiver_ = kModuleRPi,
        .peripheral_ = kPeripheralUltrasonic,
        .extra_ = 0,
        .dlc_ = 4
      };

      msg.data_[0] = *reinterpret_cast<uint8_t*>(&distance);
      msg.data_[1] = *(reinterpret_cast<uint8_t*>(&distance) + 1);
      msg.data_[2] = *(reinterpret_cast<uint8_t*>(&distance) + 2);
      msg.data_[3] = *(reinterpret_cast<uint8_t*>(&distance) + 3);

      Science::tx_buffer.push(msg);
    }

    if (incoming_message.peripheral_ = kPeripheralElectromagnet) {
      electromagnet_toggle(incoming_message.data_[0]);
    }
  }

  if (const int send_cnt = Science::process_tx()) {
    Serial.print("Sent ");
    Serial.print(send_cnt);
    Serial.println(" Messages.");
  }
}


