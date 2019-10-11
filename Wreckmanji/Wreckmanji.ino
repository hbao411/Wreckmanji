#include <Servo.h>
#include<math.h>

/**
 * Code determining two arm movement
 * TODO: Separate code into arm movement and game state
 */

// Servo Pins
const int servo1 = 9;
const int servo2 = 10;

// Servo Objects
Servo top_servo;
Servo bottom_servo;

// Arm Frame of Reference
const int bottom_arm = 100;
const int top_arm = 100;

// Arm State
typedef enum State {
  BEGINNING,
  WAITING,
  MOVEMENT,
  END
} states;
State state;

//these are the coordinates of the start/final destination when moving in a straight line
double start_x;
double start_y;
double final_x;
double final_y;

// Current Linear Coordinates of Arms
double curr_x;
double curr_y;

// Time while Moving
double timer;

// Straight Line Movement
bool linear_movement;

// Default Arm Movement Delay Time
int arm_delay_time;

// State Delay Time
int state_delay_time


// Initialize Default Values
void setup() {
  Serial.begin(9600);
  bottomServo.attach(servo1);
  topServo.attach(servo2);
  curr_x = 0;
  curr_y = 0;
  linear_movement = true;
  timer = 0;
  state = BEGINNING;
  start_x = 0;
  start_y = 0;
  final_x = 270;
  final_y = 342;
  arm_delay_time = 50;
  state_delay_time = 200;
}

void loop() {
  
  switch (state) {
    
    // Move Arms to Start Position
    case BEGINNING:
      int new_top_angle = 90;
      int new_bottom_angle = 90;
      servo_write(new_top_angle, new_bottom_angle, arm_delay_time);
      state = WAITING;
      break;

    // TODO: Wait for new Input
    case WAITING:
      if (final_x != curr_x || final_y != curr_y) {
        state = MOVEMENT;
      }
      break;

    // Actual Movement of Arm to New Position
    case MOVEMENT:
      start_x = curr_x;
      start_y = curr_y;
      double x;
      double y;
      int old_top_angle = new_top_angle;
      int old_bottom_angle = new_bottom_angle;
      if (final_x < 0) {
        double new_top_angle_radians = acos((pow(final_x, 2) + pow(final_y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm)) - PI/2;
        new_top_angle = (int) (new_top_angle_radians * 180 / PI);
        new_bottom_angle = (int) ((PI + atan((float) final_y/final_x) - atan(top_arm*cos(new_top_angle_radians)/(bottom_arm-top_arm*sin(new_top_angle_radians)))) * 180 / PI);
      } else {
        double new_top_angle_radians = (1.5 * PI - acos((pow(final_x, 2) + pow(final_y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm*top_arm)));
        new_top_angle = (int) (new_top_angle_radians * 180 / PI);
        new_bottom_angle = (int) ((atan((float) final_y/final_x) - atan(top_arm*cos(new_top_angle_radians)/(bottom_arm-top_arm*sin(new_top_angle_radians)))) * 180 / PI);
      }
      state = END;
      break;

    // Temporary End of Program
    case END:
      exit(0);
      
  }
  delay(state_delay_time);
}

//TODO: Change to Radians!

/**
 * Finds New Top Angle Given New Coordinates
 */
int find_top_angle() {
    double rads = acos((pow(final_x, 2) + pow(final_y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2))/(2 * bottom_arm * top_arm));
    if (final_x < 0) {
      return (int) ((rads - PI / 2) * 180 / PI);
    } else {
      return (int) ((1.5 * PI - rads) * 180 / PI);
    }
}


/**
 * Finds New Bottom Angle Given New Coordinates
 */
int find_bottom_angle(double final_x, double final_y, double top_angle_radians) {
  double rads = atan((float) final_y / final_x) - atan(top_arm * cos(new_top_angle_radians)/(bottom_arm - top_arm * sin(new_top_angle_radians)));
  if (final_x < 0) {
    return (int) ((PI + rads) * 180 / PI);
  } else {
    return (int) (rads * 180 / PI);
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


/**
 * Writes new angles to top and bottom servos given a delay_time
 * Recommend a delay of around 50
 */
void servo_write(int new_top_angle, int new_bottom_angle, int delay_time) {
  int old_top_angle = top_servo.read();
  int old_bottom_angle = bottom_servo.read();
  while (old_top_angle != new_top_angle && old_bottom_angle != new_bottom_angle) {
        if (old_top_angle != new_top_angle) {
          top_servo.write((old_top_angle < new_top_angle) ? --old_top_angle : ++old_top_angle);
        }
        if (old_bottom_angle != new_bottom_angle) {
          bottom_servo.write((old_bottom_angle < new_bottom_angle) ? ++old_bottom_angle : --old_bottom_angle);
        }
        delay(delay_time);
  }
}
