  // base code 01 for graphics class 2018, Jarek Rossignac

// **** LIBRARIES
import processing.pdf.*;    // to save screen shots as PDFs, does not always work: accuracy problems, stops drawing or messes up some curves !!!
import java.awt.Toolkit;
import java.awt.datatransfer.*;


// **** GLOBAL VARIABLES

// COLORS
color // set more colors using Menu >  Tools > Color Selector
   black=#000000, grey=#5F5F5F, white=#FFFFFF, 
   red=#FF0000, green=#00FF01, blue=#0300FF,  
   yellow=#FEFF00, cyan=#00FDFF, magenta=#FF00FB, 
   orange=#FCA41F, dgreen=#026F0A, brown=#AF6E0B;
   
PNT A;
PNT B;
PNT Bound_1, Bound_2_Up, Bound_2_Down;
PNT Origin;
PNT P; //current point
PNT target;
PNT start;

boolean displayBoundary;
boolean animating;
boolean onRight;
boolean movingToCenter;
boolean showArmRange;
boolean showArms;
boolean showMagnet;
int time;
int mode = 0;

float magnetAngle;
float armOffset;

float armLength_1, armLength_2;
float dof_1, dof_2;

// **** SETUP *******
void setup()               // executed once at the begining LatticeImage
  {
  size(800, 800, P2D);            // window size
  frameRate(30);             // render 30 frames per second
  smooth();                  // turn on antialiasing
  textureMode(NORMAL); // addressed using [0,1]^2
  Origin = P(400,400);
  armLength_1 = 150;
  armLength_2 = 150;
  armOffset = -270;
  displayBoundary = false;
  showArmRange = true;
  showArms = true;
  showMagnet = true;
  A = P(Origin.x + armLength_1, Origin.y);
  B = P(A.x + armLength_2, A.y);
  P = A;
  dof_1 = 180;
  dof_2 = 180;
  magnetAngle = 0;
  Bound_1 = P(Origin, V(armLength_1 * cos(dof_1*PI / 180), armLength_1 * sin(dof_1*PI / 180)));
  Bound_2_Up = P(A, V(armLength_2 * cos((dof_2+armOffset)*PI / 180), armLength_2 * sin((dof_2+armOffset)*PI / 180)));
  Bound_2_Down = P(A, V(armLength_2 * cos(armOffset*PI / 180), armLength_2 * sin(armOffset*PI / 180)));
  } // end of setup


// **** DRAW
void draw()      // executed at each frame (30 times per second)
  {
    background(white);
    writeLine("First Arm Length:" + armLength_1,0);
    writeLine("Second Arm Length:" + armLength_2,1);
    writeLine("First Arm Degrees:" + dof_1,3);
    writeLine("Second Arm Degrees:" + dof_2,4);
    if (mode == 1) {
      writeLine("Point: (" + (P.x-400) + "," + (P.y-400) + ")",5);
      VCT v = Normalized(V(Origin,A));
      writeLine("A Angle: " + angle(V(1,0),v),6);
    }
    if (mode == 2 && animating) {
      if (time == 50) {
        animating = false;
      }
      PNT cur;
      if (movingToCenter) {
        PNT destination = P(start,1.1,V(start,Origin));
        if (time < 25) {
          cur = LERP(start,(time/25.0),destination);
        } else {
          cur = LERP(destination,((time-25)/25.0),target);
        }
        moveToPoint(cur);
      } else {
        cur = LERP(start,(time/50.0),target);
        moveToPoint(cur);
      }
      time++;
    }
    if (displayBoundary) drawBoundaries();
    if (showArmRange) {
      stroke(grey); noFill(); drawFirstArc(Origin, armLength_1,dof_1*PI / 180);
      drawSecondArc(A, angle(V(Origin,A)), armOffset * PI / 180, armLength_2,dof_2*PI / 180);
    }
    if (showArms) {
      fill(brown); stroke(brown); 
      drawCircle(Origin,12);
      drawCircle(A,12);
      stroke(grey,100); strokeWeight(5); drawVectorLineFrom(Origin,V(Origin,A));
      drawVectorLineFrom(A, V(A,B));
    }
    if (showMagnet) {
      fill(blue); stroke(black);
      drawSquare(B,15);
    }
  } // end of draw()
  
 void writeLine(String S, int i) { text(S,30,20+i*40);} // writes S at line i
 
 void drawBoundaries() {
   noFill(); stroke(yellow);
   for (float i = 0; i <= dof_1*PI/180; i+=PI/80) {
     PNT newA = P(Origin, Rotated(V(armLength_1,0),i));
     drawSecondArc(newA, angle(V(Origin,newA)), armOffset * PI / 180, armLength_2,dof_2*PI / 180);
   }
 }
