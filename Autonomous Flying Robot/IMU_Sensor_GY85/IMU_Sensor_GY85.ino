#include "GY_85.h"

// Create module object
GY_85 GY85;

void setup() {
    // Initialize the serial communication:
    Serial.begin(9600);

    // Initialize module
    GY85.init();
}

void loop() {   
    // Read data from sensors
    float* gyroReadings = GY85.readGyro();
    float gx = GY85.gyro_x(gyroReadings);
    float gy = GY85.gyro_y(gyroReadings);
    float gz = GY85.gyro_z(gyroReadings);
    
    // Log it to serial port
    //Serial.print("\t  gyro");
    Serial.print("\t");
    Serial.print(gx);
    Serial.print(" \t ");
    Serial.print(gy);
    Serial.print(" \t ");
    Serial.println(gz);
    //Serial.print("\t gyro temp:");
    //Serial.println(gt);
    
    // Make delay between readings
    delay(500);
}