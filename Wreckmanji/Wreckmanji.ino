#include <Servo.h>
#include<math.h>
const int servo1 = 8;
const int servo2 = 9;
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
          new_top_angle = (int) 1.5 * PI - acos((pow(x, 2) + pow(y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm));
          new_bottom_angle = (int) (atan((float) y/x) - atan(top_arm*cos(new_top_angle)/(bottom_arm-top_arm*sin(new_top_angle))));
          
        } else {
          new_top_angle = (int) acos((pow(x, 2) + pow(y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm) - PI/2);
          new_bottom_angle = (int) (PI + atan((float) y/x) - atan(top_arm*cos(new_top_angle)/(bottom_arm-top_arm*sin(new_top_angle))));
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

double LERP(double start_x, double x, double t) {
  return start_x + t * (x - start_x);
}
