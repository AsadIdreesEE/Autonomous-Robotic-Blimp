const int irSensor = 2; //define pin where sensor's out pin is connected
 
void setup() {
  Serial.begin(9600); // set the baud rate for serial communication
pinMode(irSensor,INPUT); //set sensor pin as input to receive data from the pin
}
 
void loop() {
  
int irData=digitalRead(irSensor); //create a variable to store sensor data
Serial.print("IR SENSOR DATA:"); //print the data on the serial monitor
Serial.println(irData);
}