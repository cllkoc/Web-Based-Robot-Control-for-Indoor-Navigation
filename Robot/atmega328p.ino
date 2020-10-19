#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
volatile float roll, pitch, yaw,yaw2;
volatile int angle,a2,md,bl;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
volatile boolean c = 0;

void setup() {
  pinMode(2,INPUT_PULLUP);
  Serial.begin(9600);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  /*
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  */
  // Call this function if you need to get the IMU error values for your module
 attachInterrupt(digitalPinToInterrupt(2),kesme,FALLING);
  delay(20);

}

void loop() {
// === Read gyroscope data === //
  c = 0;
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(true);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = GyroZ + 0.50; // GyroErrorZ ~ (-0.8)

  if(GyroZ < 1.1 && GyroZ > -1.1)
  {
      GyroZ = 0;

  }
  
  yaw =  yaw + GyroZ * elapsedTime;
  angle = yaw * -1;
   
   if(angle < 0 && angle > -360)
   {  
     angle = angle + 360;
   }
   if(angle < -359)
     {
      a2 = angle*-1;
      bl = a2 / 360;
      angle = a2 - bl*360;
     }
   if(angle > 0)
   angle = angle % 360;  
   c=1;
  delay(2);
}
void kesme()
{  if(c)
   {Serial.println((int)angle);}
}
