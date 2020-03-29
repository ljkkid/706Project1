

// PID setup
double Setpoint_turn, Setpoint_strafe, Setpoint_rot; // will be the desired value
double Input_turn, Input_strafe, Input_rot; // IR sensors
double Output_turn, Output_strafe, Output_rot; // motor powers

//PID parameters
double Kp_turn =5, Ki_turn =1, Kd_turn =0;
double Kp_strafe =100, Ki_strafe =50, Kd_strafe =0;
double Kp_rot =5, Ki_rot =1.5, Kd_rot =0;

//create PID instance
PID turnPID(&Input_turn, &Output_turn, &Setpoint_turn, Kp_turn , Ki_turn, Kd_turn, DIRECT);
PID strafePID(&Input_strafe, &Output_strafe, &Setpoint_strafe, Kp_strafe , Ki_strafe, Kd_strafe, DIRECT);
PID rotPID(&Input_rot, &Output_rot, &Setpoint_rot, Kp_rot , Ki_rot, Kd_rot, DIRECT);

void CL_Setup(){
  // Hardcode the distance away from the wall
  Setpoint_strafe = 15;
  Setpoint_turn = 0;
  Setpoint_rot = -90;
  // Turn the PID on
  strafePID.SetMode(AUTOMATIC);
  turnPID.SetMode(AUTOMATIC);
  rotPID.SetMode(AUTOMATIC);
  // Adjust PID values;
  strafePID.SetTunings(Kp_strafe,Ki_strafe,Kd_strafe);
  turnPID.SetTunings(Kp_turn,Ki_turn,Kd_turn);
  rotPID.SetTunings(Kp_rot,Ki_rot,Kd_rot);
  strafePID.SetOutputLimits(-500,500);
  turnPID.SetOutputLimits(-500,500);
  rotPID.SetOutputLimits(-500,500);

}



void CL_ALIGN_DRIVE(float speedVal){
  // take input as sensor reading values in CMs
  Input_turn= ReturnAngle();
  // PID calculation
  turnPID.Compute();
  // write the output as calculated by the PID function


  // take input as sensor reading values in CMs
  Input_strafe= ShortDistanceRear_Sensor();
  // PID calculation
  
  
  strafePID.Compute();
  // write the output as calculated by the PID function


  Drive_Ctrl(speedVal, Output_strafe, Output_turn);
  /*
  Serial.print(" PID outputs Turn: ");
  Serial.print(Output_turn);
  Serial.print(" Strafe: ");
  Serial.println(Output_strafe);
  */
  
  
}


void CL_RIGHT_TURN(){
  //Serial.println("Turning...");
  // take input as sensor reading values in CMs
  Input_rot = gyroTurnAngle;
  // PID calculation
  rotPID.Compute();
  // write the output as calculated by the PID function
  Drive_Ctrl(0,0,Output_rot);
  Serial.print(Input_rot);
  Serial.print(" , ");
  Serial.println(Output_rot);
  
}
