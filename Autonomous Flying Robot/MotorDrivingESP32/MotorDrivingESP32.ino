// L9110S Motor A control using ESP32

#define MOTOR_A1 25  // IN1
#define MOTOR_A2 26  // IN2

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  stopMotor();  // Initially stop the motor
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'f') {
      forward();
    } else if (cmd == 'r') {
      reverse();
    } else if (cmd == 's') {
      stopMotor();
    }
  }
}

void forward() {
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  Serial.println("Motor Forward");
}

void reverse() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  Serial.println("Motor Reverse");
}

void stopMotor() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  Serial.println("Motor Stopped");
}
