// Pin definitions for motors
const int motorForwardPin1 = 3;  // Forward motor 1 (IN1)
const int motorForwardPin2 = 4;  // Forward motor 1 (IN2)
const int motorBackwardPin1 = 5; // Forward motor 2 (IN3)
const int motorBackwardPin2 = 6; // Forward motor 2 (IN4)
const int motorUpwardPin1 = 7;   // Upward motor 1 (IN1)
const int motorUpwardPin2 = 8;   // Upward motor 1 (IN2)
const int motorDownwardPin1 = 9; // Downward motor 2 (IN3)
const int motorDownwardPin2 = 10;// Downward motor 2 (IN4)

// Pin definitions for sensors
const int irSensorPin = 2;       // IR Proximity Sensor pin
const int trigPin = 11;          // Ultrasonic sensor trig pin
const int echoPin = 12;          // Ultrasonic sensor echo pin

// Bluetooth setup
char bluetoothCommand;           // Store Bluetooth input

// Thresholds
const int irThreshold = 20;      // 20 cm for IR proximity sensor
const int ultrasonicThreshold = 50;  // 50 cm for ultrasonic sensor

void setup() {
  // Motor pin setups
  pinMode(motorForwardPin1, OUTPUT);
  pinMode(motorForwardPin2, OUTPUT);
  pinMode(motorBackwardPin1, OUTPUT);
  pinMode(motorBackwardPin2, OUTPUT);
  
  pinMode(motorUpwardPin1, OUTPUT);
  pinMode(motorUpwardPin2, OUTPUT);
  pinMode(motorDownwardPin1, OUTPUT);
  pinMode(motorDownwardPin2, OUTPUT);
  
  // Sensor pin setups
  pinMode(irSensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize Bluetooth communication
  Serial.begin(9600);  // Bluetooth communication via HC-05
}

// Function to stop all motors
void stopAllMotors() {
  digitalWrite(motorForwardPin1, LOW);
  digitalWrite(motorForwardPin2, LOW);
  digitalWrite(motorBackwardPin1, LOW);
  digitalWrite(motorBackwardPin2, LOW);
  
  digitalWrite(motorUpwardPin1, LOW);
  digitalWrite(motorUpwardPin2, LOW);
  digitalWrite(motorDownwardPin1, LOW);
  digitalWrite(motorDownwardPin2, LOW);
}

// Function to read the ultrasonic sensor distance
long readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;  // Convert to distance in cm
}

// Function to move forward
void moveForward() {
  digitalWrite(motorForwardPin1, HIGH);
  digitalWrite(motorForwardPin2, LOW);
  digitalWrite(motorBackwardPin1, HIGH);
  digitalWrite(motorBackwardPin2, LOW);
}

// Function to move backward
void moveBackward() {
  digitalWrite(motorForwardPin1, LOW);
  digitalWrite(motorForwardPin2, HIGH);
  digitalWrite(motorBackwardPin1, LOW);
  digitalWrite(motorBackwardPin2, HIGH);
}

// Function to move upward
void moveUpward() {
  digitalWrite(motorUpwardPin1, HIGH);
  digitalWrite(motorUpwardPin2, LOW);
  digitalWrite(motorDownwardPin1, HIGH);
  digitalWrite(motorDownwardPin2, LOW);
}

// Function to move downward
void moveDownward() {
  digitalWrite(motorUpwardPin1, LOW);
  digitalWrite(motorUpwardPin2, HIGH);
  digitalWrite(motorDownwardPin1, LOW);
  digitalWrite(motorDownwardPin2, HIGH);
}

void loop() {
  // Check for Bluetooth commands (arrow key controls)
  if (Serial.available() > 0) {
    bluetoothCommand = Serial.read();
    
    // Manual control via Bluetooth (arrow key commands)
    switch (bluetoothCommand) {
      case 1 :  // Up arrow
        moveUpward();
        break;

      case 2:  // Down arrow
        moveDownward();
        break;

      case 3:  // Left arrow (move backward)
        moveBackward();
        break;

      case 4:  // Right arrow (move forward)
        moveForward();
        break;

      case 0:  // Stop
        stopAllMotors();
        break;
    }
  }

  // Read IR proximity sensor
  int irDistance = digitalRead(irSensorPin);
  
  // Read ultrasonic sensor distance
  long ultrasonicDistance = readUltrasonicDistance();
  
  // If both sensors detect obstacles
  if (irDistance == LOW && ultrasonicDistance <= ultrasonicThreshold) {
    moveBackward();  // Move backward for IR obstacle
    moveUpward();    // Move upward for ultrasonic obstacle
  }
  
  // If only IR sensor detects an obstacle within 20 cm, move backward
  else if (irDistance == LOW) {
    moveBackward();
  }
  
  // If only ultrasonic sensor detects an obstacle within 50 cm, move upward
  else if (ultrasonicDistance <= ultrasonicThreshold) {
    moveUpward();
  }
  
  // If no obstacle detected and no Bluetooth input, stop motors
  else {
    stopAllMotors();
  }
}
