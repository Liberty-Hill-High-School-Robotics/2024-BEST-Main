//0463 BEST Code
#include "Gizmo.h" //"Import" Gizmo (Control Board)
#include <Servo.h> //"Import" Servo (Motors are also defined as servos)
//#include <PID_v1.h> //import pid

Servo motor_drive_left;
Servo motor_drive_right;
Servo motor_arm_base;
Servo servo_arm_joint;
Servo servo_dumptruck;
Servo servo_claw;

//create instances for gizmo, motors, and servos
Gizmo gizmo;

//use the format motor/servo_subsystem_descriptor
//all lowercase
//like: motor_drive_left
//descriptor is optional (see line 18)


//tankdrive
//dumptruck open/close (toggle)

//arm go up
//arm go down
//arm constant voltage

//create variables (#define should only be used in read-only variables, otherwise use int, bool, etc.)
#define TANK_MODE 0
#define ARCADE_MODE 1
int mode = TANK_MODE;
bool prev_start_button = false;
//create PID loop variables
double kP = 1;
double kI = 1;
double KD = 1;
int testing = 0;
//PID armPID(&input, &output, &setpoint, kP, kI, KD);


//Outside of loop code:
void setup() {
  //start the gizmo
  gizmo.begin();
  //input = digitalRead(POT_INPUT);
  //define setpoint with button inputs
  //armPID.SetMode(AUTOMATIC);
  //define pinmodes
  pinMode(GIZMO_MOTOR_1, OUTPUT);
  pinMode(GIZMO_MOTOR_2, OUTPUT);
  pinMode(GIZMO_MOTOR_3, OUTPUT);
  //pinMode(GIZMO_SERVO_1, OUTPUT);
  pinMode(GIZMO_SERVO_2, OUTPUT);
  //pinMode(GIZMO_SERVO_3, OUTPUT);

  //define the pinMode for the built-in LED on the Raspberry Pis (RPIs)
  pinMode(LED_BUILTIN, OUTPUT);

  //define which motor/servo is assigned to which pin (Motors/Servos should have a seperate pin)
  motor_drive_left.attach(GIZMO_MOTOR_1);
  motor_drive_right.attach(GIZMO_MOTOR_2);

  motor_arm_base.attach(GIZMO_MOTOR_3);
  //servo_arm_joint.attach(GIZMO_SERVO_1);
  
  servo_dumptruck.attach(GIZMO_SERVO_2);
  //servo_claw.attach(GIZMO_SERVO_3);
}

//this code will be looped forever
void loop() {
  //make the built-in LED flash while loop() is running
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  //refresh data on gizmo
  gizmo.refresh();
  //input = digitalRead(POT_INPUT);
  //armPID.Compute();
  //motor_arm_base.write(output);

  //Create a boolean that is true when the gizmo detects the start button is pressed
  bool start_button_pressed = gizmo.getButton(GIZMO_BUTTON_START);
  
  //If the aforementioned boolean & (not) _ then switch from tank mode to arcade or vise-versa
  if (start_button_pressed && !prev_start_button) {
    if (mode == TANK_MODE) {
      mode = ARCADE_MODE;
    }
    else if (mode == ARCADE_MODE) {
      mode = TANK_MODE;
    }
  }
  //create a secondary boolean that will represent the last value of the start button (from last loop)
  prev_start_button = start_button_pressed;

//tank drive code (each joystick controls a side of the robot)
  if (mode == TANK_MODE) {
    motor_drive_left.write(map(gizmo.getAxis(GIZMO_AXIS_LY), 0, 255, 0, 180));
    motor_drive_right.write(map(gizmo.getAxis(GIZMO_AXIS_RY), 0, 255, 0, 180));
    //.write gives the motor a value to run at
    //.map will truncate the outputed values to a range
    //for example, a range of 0->1 can be mapped to 0->255; a value of .5 in the first example will now be 127.5
  }

  //arcade mode code
  else if (mode == ARCADE_MODE) {
    //code that makes drive motors run during arcade mode
    int speed = map(gizmo.getAxis(GIZMO_AXIS_LY), 0, 255, -90, 90 );
    int steering = map(gizmo.getAxis(GIZMO_AXIS_RX), 0, 255, -90, 90 );
    //simple math to combine speed + steering (allows for 0 degree turning and turning while moving)

    int outputL = constrain(speed - steering, -90, 90) + 90;
    int outputR = constrain(speed + steering, -90, 90) + 90;
    motor_drive_left.write(outputL);
    motor_drive_right.write(outputR);
  }


  if(gizmo.getButton(GIZMO_BUTTON_A) && gizmo.getButton(GIZMO_BUTTON_B)){
    motor_drive_left.write(15);
    motor_drive_right.write(15);
    //127- backwards slow
    //255 - forwards
    //-255 - forwards
    //0 - off
    //90 - nothing
    //15 - forwards
    //5 - forwards
    
    /*
    delay(1000);
    motor_drive_left.write(0);
    motor_drive_right.write(0);
    delay(1000);
    motor_drive_left.write(1);
    motor_drive_right.write(1);
    delay(1000);
    motor_drive_left.write(0);
    motor_drive_right.write(0);
    */
        
  }

  //Arm commands
  //up, down, and constant voltage
  //need to add motor commands
  //while r shoulder button pressed, set servo to 45 degrees

  
  if (gizmo.getButton(GIZMO_BUTTON_RSHOULDER)){
    servo_arm_joint.write(45);
  }
  if (!gizmo.getButton(GIZMO_BUTTON_RSHOULDER)){
    servo_arm_joint.write(0);
  }
  
  //create a command for the base joint of the arm using the PID loop

  //Claw Commands
  //code for claw opening and closing
  
  if (gizmo.getButton(GIZMO_BUTTON_LSHOULDER)){
    servo_claw.write(45);
  }
  if (!gizmo.getButton(GIZMO_BUTTON_LSHOULDER)){
    servo_claw.write(0);
  }
  

  //create dumptruck command
  
  if (gizmo.getButton(GIZMO_BUTTON_Y)){
    servo_dumptruck.write(45);
  }
  if (!gizmo.getButton(GIZMO_BUTTON_Y)){
    servo_dumptruck.write(0);
  }
  
  
}
