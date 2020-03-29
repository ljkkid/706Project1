void InitializeGyro()
{
  float sum = 0;
    for (int i = 0; i < 100; i++) //  read 100 values of voltage when gyro is at still, to calculate the zero-drift
    {
         
        sum += analogRead(gyroPin);
        delay(5);
    }
    gyroZeroVoltage = sum / 100.0; // average the sum as the zero drifting
    gyroZeroVoltage = (gyroZeroVoltage* gyroVoltage) / 1023.0;

    Serial.println(gyroZeroVoltage);
}

//Find the output of the sensor
float ShortDistance_Sensor(int Sensor){
   float volts = 0;
   float cm = 0;
   
   volts = analogRead(Sensor)* (5.0/1024.0);
   cm = 13*pow(volts, -1);
   
   // Print out results
   if (cm > 30) {
      //Serial.print("Sensor Out of range");
    return 0;
   } else {
      //Serial.print("Short distance sensor: ");
      //Serial.print(cm);
      //Serial.println("cm");
    return cm;
   }
}

// Check alignment
boolean CheckAlignment(){
  float distanceTolerance = 1;
  bool headingCorrect = false;
  bool distanceCorrect = false;

  float heading = ReturnAngle();

  
  if (abs(heading) < 1){
    headingCorrect = true;
  }
  float distanceFromWall = (ShortDistanceRear_Sensor() + ShortDistanceFront_Sensor()) / 2.0;

  Serial.print(distanceFromWall);
  Serial.print(",");
  Serial.println(heading);
  
  if ((distanceFromWall > 15 - distanceTolerance) & (distanceFromWall < 15 + distanceTolerance)){
    distanceCorrect = true;
  }

  return (headingCorrect & distanceCorrect);
}

// Find the angle of the car
float ReturnAngle(){
    float rear = ShortDistanceRear_Sensor();
    float front = ShortDistanceFront_Sensor();

    float difference = rear - front;
    float tempangle = (180.0/(PI)) * atan2(14.2, difference);
    float angle =  90 - tempangle;
    //Serial.print("Car angle: ");
    //Serial.println(angle);
    return angle + offset;
}

// Find the output of the long distance sensor
float LongDistance_Sensor(int Sensor){
   float volts = 0;
   float cm = 0;
   float sensorOffset = -5.0;
   volts = analogRead(Sensor);
   cm = 4809.4*pow(volts, -0.941);
   
   // Print out results
   if (cm > 80) {
      //Serial.print("Long Distance Sensor Out of range");
    return 999999;
   } else {
      //Serial.print("Long distance sensor: ");
      //Serial.print(cm);
      //Serial.println("cm");
    return cm + sensorOffset;
   }
}


//Find the output of the sensor
float ShortDistanceRear_Sensor(){
   float volts = 0;
   float cm = 0;
   
   volts = analogRead(Rear_Side);
   cm = 2280.3*pow(volts, -1.006);
   
   // Print out results
   if (cm > 30) {
      //Serial.print("Sensor Out of range");
    return 99999;
   } else {
      //Serial.print("Short distance rear sensor: ");
      //Serial.print(cm);
      //Serial.println("cm");
    return cm + sideOffset;
   }
}

//Find the output of the sensor
float ShortDistanceFront_Sensor(){
   float volts = 0;
   float cm = 0;
   
   volts = analogRead(Front_Side);
   cm = 2083.8*pow(volts, -0.949);
   
   // Print out results
   if (cm > 30) {
     //Serial.println("Sensor Out of range");
    return 99999;
   } else {
      //Serial.print("Short distance front sensor: ");
      //Serial.print(cm);
      //Serial.println("cm");
    return cm + sideOffset;
   }
}

void GYRO_update()
{
//This line converts the 0-1023 signal to 0-5V
  float gyroRate = (analogRead(gyroPin) * gyroVoltage) / 1023.0;

  //This line finds the voltage offset from sitting still
  gyroRate -= gyroZeroVoltage;

  //This line divides the voltage we found by the gyro's sensitivity
  gyroRate /= gyroSensitivity;

  //Ignore the gyro if our angular velocity does not meet our threshold
  if (gyroRate >= rotationThreshold || gyroRate <= -rotationThreshold) {
    //This line divides the value by 100 since we are running in a 10ms loop (1000ms/10ms)
    gyroRate /= 100;
    
    gyroTurnAngle -= 2*gyroRate;
  }

  //Keep our angle between 0-359 degrees
  if (gyroTurnAngle < -180)
    gyroTurnAngle += 360;
  else if (gyroTurnAngle > 179)
    gyroTurnAngle -= 360;

  //DEBUG 
  /*
  Serial.print(360.0);
  Serial.print(",");
  Serial.print(0.0);
  Serial.print(",");
  Serial.println(gyroTurnAngle);
  */
}


 boolean is_battery_voltage_OK()
{
  static byte Low_voltage_counter;
  static unsigned long previous_millis;

  int Lipo_level_cal;
  int raw_lipo;
  //the voltage of a LiPo cell depends on its chemistry and varies from about 3.5V (discharged) = 717(3.5V Min) https://oscarliang.com/lipo-battery-guide/
  //to about 4.20-4.25V (fully charged) = 860(4.2V Max)
  //Lipo Cell voltage should never go below 3V, So 3.5V is a safety factor.
  raw_lipo = analogRead(A0);
  Lipo_level_cal = (raw_lipo - 717);
  Lipo_level_cal = Lipo_level_cal * 100;
  Lipo_level_cal = Lipo_level_cal / 143;

  if (Lipo_level_cal > 0 && Lipo_level_cal < 160) {
    previous_millis = millis();
    Serial.print("Lipo level:");
    Serial.print(Lipo_level_cal);
    Serial.print("%");
    // SerialCom->print(" : Raw Lipo:");
    // SerialCom->println(raw_lipo);
    Serial.println("");
    Low_voltage_counter = 0;
    return true;
  } else {
    if (Lipo_level_cal < 0)
      Serial.println("Lipo is Disconnected or Power Switch is turned OFF!!!");
    else if (Lipo_level_cal > 160)
      Serial.println("!Lipo is Overchanged!!!");
    else {
      Serial.println("Lipo voltage too LOW, any lower and the lipo with be damaged");
      Serial.print("Please Re-charge Lipo:");
      Serial.print(Lipo_level_cal);
      Serial.println("%");
    }

    Low_voltage_counter++;
    if (Low_voltage_counter > 5)
      return false;
    else
      return true;
  }
}
