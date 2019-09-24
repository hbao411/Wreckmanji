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
  BEGINNING,
  WAITING,
  MOVEMENT,
  END,
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
  Serial.begin(9600);
  // put your setup code here, to run once:
  bottomServo.attach(servo1);
  topServo.attach(servo2);
  curr_x = 0;
  curr_y = 0;
  new_top_angle = 90;
  new_bottom_angle = 90;
  linear_movement = true;
  timer = 0;
  state = BEGINNING;
  start_x = 0;
  start_y = 0;
  final_x = -107;
  final_y = 95;
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //eventually we'll implement the other stuff, for now only WAITING and MOVEMENt
  switch (state) {
    case BEGINNING:
      bottomServo.write(new_top_angle);
      delay(500); 
      topServo.write(new_bottom_angle);
      delay(500);
      state = WAITING;
      Serial.print("We have printed!\n");
      break;
    //calculate the angle you need to move to and set state to movement
    case WAITING:
      /*if (linear_movement) {
        if (final_x != curr_x || final_y != curr_y) {
          if (timer != 1) {
            timer += 0.05;
          } else {
            exit(0);
          }
          Serial.println(timer);
          x = LERP(start_x, final_x, timer);
          y = LERP(start_y, final_y, timer);
        }
      }*/
      if (final_x != curr_x || final_y != curr_y) {
        //moved calculating coordinates elsewhere
        /*if (x > 0) {
          new_top_angle = (int) ((1.5 * PI - acos((pow(final_x, 2) + pow(final_y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm))) * 180 / PI);
          new_bottom_angle = (int) ((atan((float) y/x) - atan(top_arm*cos(new_top_angle)/(bottom_arm-top_arm*sin(new_top_angle)))) * 180 / PI);
          
        } else {
          new_top_angle = (int) ((acos((pow(x, 2) + pow(y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm) - PI/2)) * 180 / PI);
          new_bottom_angle = (int) ((PI + atan((float) y/x) - atan(top_arm*cos(new_top_angle)/(bottom_arm-top_arm*sin(new_top_angle)))) * 180 / PI);
        }*/
        state = MOVEMENT;
      }
      
      /*if (timer == 1) {
        state = END;
      } else {
        state = MOVEMENT;
      }*/
      break;
    case MOVEMENT:
      start_x = curr_x;
      start_y = curr_y;
      double x;
      double y;
      if (final_x < 0) {
        double new_top_angle_radians = acos((pow(final_x, 2) + pow(final_y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm)) - PI/2;
        new_top_angle = (int) (new_top_angle_radians * 180 / PI);
        new_bottom_angle = (int) ((PI + atan((float) final_y/final_x) - atan(top_arm*cos(new_top_angle_radians)/(bottom_arm-top_arm*sin(new_top_angle_radians)))) * 180 / PI);
        Serial.print(new_top_angle);
        Serial.print("\n");
        Serial.print(new_bottom_angle);
        Serial.print("\n");
      } else {
        double new_top_angle_radians = (1.5 * PI - acos((pow(final_x, 2) + pow(final_y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm)));
        new_top_angle = (int) (new_top_angle_radians * 180 / PI);
        new_bottom_angle = (int) ((atan((float) final_y/final_x) - atan(top_arm*cos(new_top_angle_radians)/(bottom_arm-top_arm*sin(new_top_angle_radians)))) * 180 / PI);
        Serial.print(new_top_angle);
        Serial.print("\n");
        Serial.print(new_bottom_angle);
        Serial.print("\n");
      }
      bottomServo.write(new_bottom_angle);
      topServo.write(new_top_angle);
      state = END;
      
      break;
    case END:
      exit(0);
  }
  delay(200);
}

/**
 * Linear interpolation
 * Returns the value along the line between start_x and final_x based on the value of t
 * t ranges from 0 to 1 (at 0 the function returns start_x and at 1 it returns final_x)
 */
double LERP(double start_x, double final_x, double t) {
  return start_x + t * (final_x - start_x);
}
