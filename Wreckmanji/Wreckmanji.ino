#include <Servo.h>
#include<math.h>
const int servo1 = 9;
const int servo2 = 10;
Servo bottomServo;
Servo topServo;
const int readSwitch = 11;
const int bottom_arm = 100;
const int top_arm = 100;
typedef enum State {
  WAITING,
  MOVEMENT,
  LEFT,
  RIGHT,
  UP,
  DOWN
} states;
//these are the coordinates of the start/final destination when moving in a straight line
double start_x;
double start_y;
double final_x;
double final_y;
//these are the coordinates you want to get to
double x;
double y;
//the current x and y coordinates of the servos
double curr_x;
double curr_y;
//used to keep track of time when moving in a line
double timer;
//the new angles we want the servos to move to
int new_top_angle;
int new_bottom_angle;
//true if we want the magnet to move in a straight line
bool linear_movement;
State state;
void setup() {
  // put your setup code here, to run once:
  bottomServo.attach(servo1);
  topServo.attach(servo2);
  curr_x = 0;
  curr_y = 0;
  x = 100;
  y = 100;
  new_top_angle = 90;
  new_bottom_angle = 90;
  linear_movement = false;
  timer = 0;
  state = WAITING;
  //start_x = 0;
  //start_y = 0;
  //final_x = 100;
  //final_y = 100;
}

void loop() {
  // put your main code here, to run repeatedly:
  //eventually we'll implement the other stuff, for now only WAITING and MOVEMENt
  switch (state) {
    //calculate the angle you need to move to and set state to movement
    case WAITING:
      if (linear_movement) {
        if (final_x != curr_x || final_y != curr_y) {
          timer += 0.1;
          x = LERP(start_x, final_x, timer);
          y = LERP(start_y, final_y, timer);
        }
      }
      if (x != curr_x || y != curr_y) {
        if (x > 0) {
          new_top_angle = (int) ((1.5 * PI - acos((pow(x, 2) + pow(y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm))) * 180 / PI);
          new_bottom_angle = (int) ((atan((float) y/x) - atan(top_arm*cos(new_top_angle)/(bottom_arm-top_arm*sin(new_top_angle)))) * 180 / PI);
          
        } else {
          new_top_angle = (int) ((acos((pow(x, 2) + pow(y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm) - PI/2)) * 180 / PI);
          new_bottom_angle = (int) ((PI + atan((float) y/x) - atan(top_arm*cos(new_top_angle)/(bottom_arm-top_arm*sin(new_top_angle)))) * 180 / PI);
        }
      }
      state = MOVEMENT;
      break;
    case MOVEMENT:
      bottomServo.write(new_bottom_angle);
      delay(500);
      topServo.write(new_top_angle);
      delay(500);
      curr_x = x;
      curr_y = y;
      state = WAITING;
  }
}

/**
 * Linear interpolation
 * Returns the value along the line between start_x and final_x based on the value of t
 * t ranges from 0 to 1 (at 0 the function returns start_x and at 1 it returns final_x)
 */
double LERP(double start_x, double final_x, double t) {
  return start_x + t * (final_x - start_x);
}
