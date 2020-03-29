#include <PID_v1.h>

#include <Servo.h>  //Need for Servo pulse output

void InitializeGyro();

// to be replaced
//void CL_ALIGN_DRIVE(float speedVal);

// Real functions
void Stop();
void enable_motors();
void InitializeGyro();
void InitializeGyro();
//float LongDistance_Sensor(int Sensor);

bool batteryVoltOK = false;
float lastGyroMillis  = 0;
float lastStateMillis = 0;
float gyroTurnAngle = 0;



void setup()
{
    SetupCommands();
    OL_Setup();
    CL_Setup();
}

void loop()
{
  if(batteryVoltOK){
    if (millis() > (lastStateMillis + 50)){
      //Serial.println(LongDistance_Sensor(A12));
      //CL_RIGHT_TURN();
      //Drive_Ctrl(0,0,150);
      state_machine_run();
      //Serial.println(gyroTurnAngle);
      //Serial.println(LongDistance_Sensor(A12));
    }
    if (millis() > (lastGyroMillis + 10)){
      GYRO_update();
    }
  }else {
    Serial.println("Battery voltage to low");
    delay(5000);
  }
}
