/*
   Gamepad module provides three different mode namely Digital, JoyStick and Accerleometer.

   You can reduce the size of library compiled by enabling only those modules that you want to
   use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/game-pad-module/
*/
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
#include <AFMotor.h>
#include <Servo.h>

//initial motors pin
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);


bool drive_mode = true;
bool start_tmp = false;
bool start_btn = false;
bool select_tmp = false;
bool select_btn = false;

bool flashlight_on = false;
bool claw_open = true;

Servo claw_servo;
int claw_servo_angle = 100;
Servo base_servo;
int base_servo_angle = 0;
Servo arm_servo;
int arm_servo_angle = 30;
Servo forearm_servo;
int forearm_servo_angle = 170;
Servo barrel_servo;
int barrel_servo_angle = 0;

int speed = 100;
int servo_speed = 10;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Dabble.begin(9600);      //Enter baudrate of your bluetooth.Connect bluetooth on Bluetooth port present on evive.
  
  claw_servo.attach(A0);
  base_servo.attach(A1);
  arm_servo.attach(A2);
  forearm_servo.attach(A3);
  barrel_servo.attach(A4);

  claw_servo.write(claw_servo_angle);
  base_servo.write(base_servo_angle);
  arm_servo.write(arm_servo_angle);
  forearm_servo.write(forearm_servo_angle);
  barrel_servo.write(barrel_servo_angle);
}

void reset_pins() {
  // Set everything to low
  motor1.setSpeed(0); //Define minimum velocity
  motor1.run(RELEASE); //stop the motor when release the button
  motor2.setSpeed(0); //Define minimum velocity
  motor2.run(RELEASE); //rotate the motor clockwise
  motor3.setSpeed(0); //Define minimum velocity
  motor3.run(RELEASE); //stop the motor when release the button
  motor4.setSpeed(0); //Define minimum velocity
  motor4.run(RELEASE); //stop the motor when release the button
}

// Driving movements

void forward()
{
  motor1.setSpeed(speed); //Define maximum velocity
  motor1.run(FORWARD); //rotate the motor clockwise
  motor2.setSpeed(speed); //Define maximum velocity
  motor2.run(FORWARD); //rotate the motor clockwise
  motor3.setSpeed(speed);//Define maximum velocity
  motor3.run(FORWARD); //rotate the motor clockwise
  motor4.setSpeed(speed);//Define maximum velocity
  motor4.run(FORWARD); //rotate the motor clockwise
}

void back()
{
  motor1.setSpeed(speed); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(speed); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(speed); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(speed); //Define maximum velocity 
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void left()
{
  motor1.setSpeed(speed); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(speed); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(speed); //Define maximum velocity
  motor3.run(FORWARD);  //rotate the motor clockwise
  motor4.setSpeed(speed); //Define maximum velocity
  motor4.run(FORWARD);  //rotate the motor clockwise
}

void right()
{
  motor1.setSpeed(speed); //Define maximum velocity
  motor1.run(FORWARD); //rotate the motor clockwise
  motor2.setSpeed(speed); //Define maximum velocity
  motor2.run(FORWARD); //rotate the motor clockwise
  motor3.setSpeed(speed); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(speed); //Define maximum velocity
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}


// Arm movements
void move_arm_base(int dir) {
  base_servo.write(base_servo_angle + (dir*servo_speed));
}

void move_arm(int dir) {
  arm_servo.write(arm_servo_angle + (dir*servo_speed));
  
}

void move_forearm(int dir) {
  forearm_servo.write(forearm_servo_angle + (dir*servo_speed));
}

void move_arm_claw() {
  if (select_btn) {
    claw_open = !claw_open;
    if (claw_open){
      claw_servo.write(100);
    }else{
      claw_servo.write(160);
    }
  }
}

// Barel and Flashlight

void move_barrel(int dir) {
  barrel_servo.write(barrel_servo_angle + (dir*servo_speed));
}

void flashlight() {
  flashlight_on = !flashlight_on;

}

void handle_single_press_buttons() {
  if (GamePad.isStartPressed() != start_tmp) {
    start_btn = true;
    drive_mode = !drive_mode;
  }
  if (GamePad.isSelectPressed() != select_tmp) {
    select_btn = true;
  }
  select_tmp = GamePad.isSelectPressed();
  start_tmp = GamePad.isStartPressed();
}

void loop() {
  reset_pins();
  Dabble.processInput();
  handle_single_press_buttons();
  if (drive_mode) {
    if (GamePad.isUpPressed())
    {
      forward();
    }
    else if (GamePad.isDownPressed())
    {
      back(); 
    }

    if (GamePad.isLeftPressed())
    {
      left();
    }
    else if (GamePad.isRightPressed())
    {
      right();
    }
  } else {
    move_arm_claw();
    if (GamePad.isSquarePressed()){
      move_arm_base(-1);
    }
    else if (GamePad.isCirclePressed()){
      move_arm_base(1);
    }

    if (GamePad.isUpPressed()){
      move_arm(-1);
    }
    else if (GamePad.isDownPressed()){
      move_arm(1);
    }


    if (GamePad.isTrianglePressed()){
      move_forearm(-1);
    }
    else if (GamePad.isCrossPressed()){
      move_forearm(1);
    }

    if (GamePad.isLeftPressed()){
      move_barrel(-1);
    }
    else if (GamePad.isRightPressed()){
      move_barrel(1);
    }

  }
  

  delay(100);
}
