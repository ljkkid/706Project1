void OL_Setup(){
  enable_motors();
}

// Randomly set? 

float Kx = 0.5;
float Ky = 0.5;
float Kz = 0.7;

void Drive_Ctrl(double X_effort, double Y_effort, double Z_rot_effort){
  //Drive_Ctrl
  //Takes input of controller efforts for three dirrections of motions (primary, perpendicular, rotation)
  //Controller Efforts can be ballanced with proportional constants
  //Coordinate System 
  //    X Forward Positive
  //    Y Right Positive
  //    Z CCW Positive
  
  double X_speed = Kx * X_effort;
  double Y_speed = Ky * Y_effort;
  double Z_rot_effect = Kz * Z_rot_effort;
  
  double LF_speed = X_speed + Y_speed - Z_rot_effect;
  double LR_speed = X_speed - Y_speed - Z_rot_effect;
  double RF_speed = - X_speed - Y_speed - Z_rot_effect;
  double RR_speed = - X_speed + Y_speed - Z_rot_effect;
                    
    left_front_motor.writeMicroseconds(1500 + LF_speed);
    left_rear_motor.writeMicroseconds(1500 + LR_speed);
    right_rear_motor.writeMicroseconds(1500 + RF_speed);
    right_front_motor.writeMicroseconds(1500 + RR_speed);

}

void Stop(){                                                                  // stop motors 
  left_front_motor.writeMicroseconds(1500);
  left_rear_motor.writeMicroseconds(1500);
  right_rear_motor.writeMicroseconds(1500);
  right_front_motor.writeMicroseconds(1500);
}

void enable_motors() {                                //enable all motors, was called in INITIALZING SATE 
  left_front_motor.attach(left_front);
  left_rear_motor.attach(left_rear);
  right_rear_motor.attach(right_rear);
  right_front_motor.attach(right_front);
}

void disable_motors(){                             // function disable all motors, called in  STOPPED STATE
  left_front_motor.detach();
  left_rear_motor.detach();
  right_rear_motor.detach();
  right_front_motor.detach();

  pinMode(left_front,INPUT);                   // set pinMode for next step 
  pinMode(left_rear,INPUT);
  pinMode(right_rear,INPUT);
  pinMode(right_front,INPUT);
}
