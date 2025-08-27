#include <WiFi.h>
#include <WebServer.h>

// ---- WiFi Setup ----
const char* ssid = "Xtracodenary";
const char* password = "Xtra#1234";

// Create WebServer on port 80
WebServer server(80);

// ---- Motor Pins ----
const int motorForwardPin1 = 3;
const int motorForwardPin2 = 4;
const int motorBackwardPin1 = 5;
const int motorBackwardPin2 = 6;
const int motorUpwardPin1 = 7;
const int motorUpwardPin2 = 8;
const int motorDownwardPin1 = 9;
const int motorDownwardPin2 = 10;

// ---- Sensors ----
const int irSensorPin = 2;
const int trigPin = 11;
const int echoPin = 12;
const int ultrasonicThreshold = 50;

// ---- Functions ----
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

void moveForward() {
  digitalWrite(motorForwardPin1, HIGH);
  digitalWrite(motorForwardPin2, LOW);
  digitalWrite(motorBackwardPin1, HIGH);
  digitalWrite(motorBackwardPin2, LOW);
}

void moveBackward() {
  digitalWrite(motorForwardPin1, LOW);
  digitalWrite(motorForwardPin2, HIGH);
  digitalWrite(motorBackwardPin1, LOW);
  digitalWrite(motorBackwardPin2, HIGH);
}

void moveUpward() {
  digitalWrite(motorUpwardPin1, HIGH);
  digitalWrite(motorUpwardPin2, LOW);
  digitalWrite(motorDownwardPin1, HIGH);
  digitalWrite(motorDownwardPin2, LOW);
}

void moveDownward() {
  digitalWrite(motorUpwardPin1, LOW);
  digitalWrite(motorUpwardPin2, HIGH);
  digitalWrite(motorDownwardPin1, LOW);
  digitalWrite(motorDownwardPin2, HIGH);
}

long readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

// ---- Webpage HTML ----
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>IoT Robot Control</title>
  <style>
    button { width: 120px; height: 60px; font-size: 20px; margin: 10px; }
  </style>
</head>
<body>
  <h1>ESP32 Robot Control</h1>
  <button onclick="location.href='/forward'">Forward</button><br>
  <button onclick="location.href='/left'">Left</button>
  <button onclick="location.href='/stop'">Stop</button>
  <button onclick="location.href='/right'">Right</button><br>
  <button onclick="location.href='/backward'">Backward</button><br>
  <button onclick="location.href='/up'">Up</button>
  <button onclick="location.href='/down'">Down</button>
</body>
</html>
)rawliteral";

// ---- Web Routes ----
void handleRoot() { server.send(200, "text/html", webpage); }
void handleForward() { moveForward(); server.send(200, "text/plain", "Moving Forward"); }
void handleBackward() { moveBackward(); server.send(200, "text/plain", "Moving Backward"); }
void handleUp() { moveUpward(); server.send(200, "text/plain", "Moving Upward"); }
void handleDown() { moveDownward(); server.send(200, "text/plain", "Moving Downward"); }
void handleStop() { stopAllMotors(); server.send(200, "text/plain", "Stopped"); }

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(motorForwardPin1, OUTPUT);
  pinMode(motorForwardPin2, OUTPUT);
  pinMode(motorBackwardPin1, OUTPUT);
  pinMode(motorBackwardPin2, OUTPUT);
  pinMode(motorUpwardPin1, OUTPUT);
  pinMode(motorUpwardPin2, OUTPUT);
  pinMode(motorDownwardPin1, OUTPUT);
  pinMode(motorDownwardPin2, OUTPUT);

  // Sensors
  pinMode(irSensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.println(WiFi.localIP());

  // Web routes
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/up", handleUp);
  server.on("/down", handleDown);
  server.on("/stop", handleStop);

  server.begin();
}

void loop() {
  server.handleClient();

  // Obstacle avoidance (auto safety)
  int irDistance = digitalRead(irSensorPin);
  long ultrasonicDistance = readUltrasonicDistance();

  if (irDistance == LOW || ultrasonicDistance <= ultrasonicThreshold) {
    stopAllMotors(); // Stop if obstacle detected
    Serial.println("Obstacle detected! Motors stopped.");
  }
}