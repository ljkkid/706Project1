enum State_enum
{
    STOP,
    ALIGN,
    FORWARDS,
    ROTATE_RIGHT,
    DEBUG
};

// Hello mate

//Default motor control pins
const byte left_front = 46;
const byte left_rear = 47;
const byte right_rear = 50;
const byte right_front = 51;

//Defined sensor pins
const int Rear_Side = A10;
const int Front_Side = A11;
const int Forward_Facing = A12;
const int gyroPin = A3;

// Variables
uint8_t state = ALIGN;
uint8_t turnCount = 0;
float speedVal = 150;
//hard angle offset
float offset = 9;
float gyroVoltage = 5.0;
float gyroSensitivity = 0.007; 
float gyroZeroVoltage = 0;
float rotationThreshold = 2; 
float sideOffset= 5;
float distanceAhead = 0;

Servo left_front_motor; 
Servo left_rear_motor;  
Servo right_rear_motor;  
Servo right_front_motor;  

const unsigned int MAX_DIST = 23200;



void SetupCommands(){

  Serial.begin(9600);
  Serial.println("Hey matey I'm doing some setup");
  //Serial.println(gyroZeroVoltage);
  pinMode(gyroPin, INPUT);

  InitializeGyro();

  batteryVoltOK = is_battery_voltage_OK();

  state = ALIGN;
  Serial.println("ALIGN");
}




void state_machine_run()
{
      // ----------------------------------------State output section ------------------------------------------------
    switch (state)
    {
    case STOP:
    
        Stop();
        break;

    case ALIGN:
        CL_ALIGN_DRIVE(0);
        break;

    case FORWARDS:
        CL_ALIGN_DRIVE(speedVal);
        break;
    case ROTATE_RIGHT:
        Serial.print("Angle = ");
        Serial.print(gyroTurnAngle);
        Serial.print(" , Output");
        CL_RIGHT_TURN();
        break;
    }

    
    // -----------------------------------Exit state section ------------------------------------------------
    switch (state)
    {
    case STOP:
        // Do we need a way to restart the robot?
        break;
    case ALIGN:
        // Only leave if we are aligned correctly
        if (CheckAlignment())
        {
            state = FORWARDS;
            Serial.println("FORWARDS");
            Stop();
        }
        break;

    case FORWARDS:
        distanceAhead = 0;
        for (int i =0; i<10;i++){
          float temp = LongDistance_Sensor(Forward_Facing);
          Serial.println(temp);
          distanceAhead += temp;
        }
        distanceAhead = distanceAhead/10;
        Serial.print(distanceAhead);
        if (distanceAhead <= 15)
        {
            //Record how many corners we've reached
            turnCount++;

            // only needs to complete three turns
            if (turnCount < 4) {
                state = ROTATE_RIGHT;
                Serial.println("ROTATE_RIGHT");
                Serial.println("Just about to enter the state");
                // CL_RIGHT_TURN is going to -90 degrees as such reset to gyro angle
                gyroTurnAngle = 0;
            }
            else {
                state = STOP;
                Serial.println("STOP");
            }
        }
        break;

    case ROTATE_RIGHT:
        float toleranceDegrees = 10;
        if (gyroTurnAngle < (-90 + toleranceDegrees) & (gyroTurnAngle > (-90 - toleranceDegrees)))
        {
          Serial.println("FROWARDS");
            Stop();
            state = ALIGN;
        }
        break;
    }


}


