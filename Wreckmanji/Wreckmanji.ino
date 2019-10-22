#include <Servo.h>
#include<math.h>

/**
   Code determining two arm movement
   TODO: Separate code into arm movement and game state
*/

// Servo Pins
const int servo1 = 9;
const int servo2 = 10;

// Servo Objects
Servo top_servo;
Servo bottom_servo;

// Arm Frame of Reference
const int top_arm = 100;
const int bottom_arm = 100;

// Arm State
typedef enum State {
  BEGINNING,
  MOVEMENT,
  END
} states;
State state;

// Current Linear Coordinates of Arms
double curr_x;
double curr_y;

// Destination Linear Coordinates of Arms
double dest_x;
double dest_y;

// Current Angles of Bottom/Top Arms
int top_angle;
int bottom_angle;

// Time while Moving
double timer;

// Straight Line Movement
bool linear_movement;

// Default Arm Movement Delay Time
int arm_delay_time;

// State Delay Time
int state_delay_time;


// Initialize Default Values
void setup() {
  Serial.begin(9600);

  top_servo.attach(servo2);
  bottom_servo.attach(servo1);


  // Start Coordinates/Angles
  curr_x = 0;
  curr_y = 0;
  top_angle = 90;
  bottom_angle = 90;
  
  linear_movement = true;
  timer = 0;
  state = BEGINNING;
  arm_delay_time = 50;
  state_delay_time = 200;

  // Input Destination Coordinates
  dest_x = 270;
  dest_y = 342;
}

void loop() {

  switch (state) {

    // Move Arms to Start Position
    case BEGINNING: {
      servo_write(top_angle, bottom_angle, arm_delay_time);
      state = MOVEMENT;
      break;
    }

    // Actual Movement of Arm to New Position
    case MOVEMENT: {
      double top_angle_radians = find_top_angle();
      top_angle = convert_to_degrees(top_angle_radians);
      bottom_angle = convert_to_degrees(find_bottom_angle(top_angle_radians));
      servo_write(top_angle, bottom_angle, arm_delay_time);
      state = END;
      break;
    }

    // Temporary End of Program
    case END: {
      exit(0);
    }
  }
  delay(state_delay_time);
}


/**
   Converts angle from radians to degrees
*/
int convert_to_degrees(double rads) {
  if (dest_x < 0) {
    return (int) ((rads - PI / 2) * 180 / PI);
  } else {
    return (int) ((1.5 * PI - rads) * 180 / PI);
  }
}


/**
   Finds New Top Angle Given New Coordinates (In Radians)
*/
double find_top_angle() {
  return acos((pow(dest_x, 2) + pow(dest_y, 2) - pow(top_arm, 2) - pow(bottom_arm, 2)) / (2 * bottom_arm * top_arm));
}


/**
   Finds New Bottom Angle Given New Coordinates (In Radians)
*/
double find_bottom_angle(double top_angle_radians) {
  return atan((float) dest_y / dest_x) - atan(top_arm * cos(top_angle_radians) / (bottom_arm - top_arm * sin(top_angle_radians)));
}


/**
   Writes new angles to top and bottom servos given a delay_time
   Recommend a delay of around 50
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
