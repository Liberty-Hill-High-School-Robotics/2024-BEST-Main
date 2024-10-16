//0463 BEST Code
#include <Gizmo.h> //"Import" Gizmo (Control Board)
#include <Servo.h> //"Import" Servo (Motors are also defined as servos)

//create instances for gizmo, motors, and servos
Gizmo gizmo;

//use the format motor/servo_subsystem_descriptor
//all lowercase
//like: motor_drive_left
//descriptor is optional (see line 14)
Servo motor_drive_left;
Servo motor_drive_right;
Servo motor_arm_base;
Servo servo_arm_joint;
Servo servo_dumptruck;
Servo servo_claw

//create variables (#define should only be used in read-only variables, otherwise use int, bool, etc.)
#define TANK_MODE 0
#define ARCADE_MODE 1
int mode = TANK_MODE;
bool prev_start_button = false;


//Outside of loop code:
void setup() {
  //start the gizmo
  gizmo.begin();

  //define which motor/servo is assigned to which pin (Motors/Servos should have a seperate pin)
  motor_drive_left.attach(GIZMO_MOTOR_1);
  motor_drive_right.attach(GIZMO_MOTOR_2);

  motor_arm_base.attach(GIZMO_MOTOR_3);
  servo_arm_joint.attach(GIZMO_SERVO_1);
  
  servo_dumptruck.attach(GIZMO_SERVO_2);
  servo_claw.attach(GIZMO_SERVO_3);

  //define the pinMode for the built-in LED on the Raspberry Pis (RPIs)
  pinMode(LED_BUILTIN, OUTPUT);
}

//this code will be looped forever
void loop() {
  //make the built-in LED flash while loop() is running
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  //refresh data on gizmo
  gizmo.refresh();

//no comments past this point ---------------------------------------------------------------------------------------------------------
  //COMMENT HERE
  bool start_button_pressed = gizmo.getButton(GIZMO_BUTTON_START);
  if (start_button_pressed && !prev_start_button) {
    if (mode == TANK_MODE) {
      mode = ARCADE_MODE;
    }
    else if (mode == ARCADE_MODE) {
      mode = TANK_MODE;
    }
  }
  prev_start_button = start_button_pressed;

  if (mode == TANK_MODE) {
  //COMMENT HERE
    motor_left.write(map(gizmo.getAxis(GIZMO_AXIS_LY), 0, 255, 0, 180));
    motor_right.write(map(gizmo.getAxis(GIZMO_AXIS_RY), 0, 255, 0, 180));
  }
  else if (mode == ARCADE_MODE) {
  //COMMENT HERE
    int speed = map(gizmo.getAxis(GIZMO_AXIS_LY), 0, 255, -90, 90);
    int steering = map(gizmo.getAxis(GIZMO_AXIS_LX), 0, 255, -90, 90);
    motor_left.write(constrain(speed - steering, -90, 90) + 90);
    motor_right.write(constrain(speed + steering, -90, 90) + 90);
  }

  //COMMENT HERE
  if (gizmo.getButton(GIZMO_BUTTON_RT)) {
    motor_task.write(0);
  }
  else if (gizmo.getButton(GIZMO_BUTTON_RSHOULDER)) {
    motor_task.write(180);
  }
  else {
    motor_task.write(90);
  }

  //COMMENT HERE
  if (gizmo.getButton(GIZMO_BUTTON_LT)) {
    servo_task.write(0);
  }
  else if (gizmo.getButton(GIZMO_BUTTON_LSHOULDER)) {
    servo_task.write(180);
  }
  else {
    servo_task.write(90);
  }
}
