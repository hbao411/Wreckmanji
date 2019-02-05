void keyPressed()  // executed each time a key is pressed: sets the Boolean "keyPressed" till it is released   
{
  if(key=='a') {
    armLength_1 = max(0,armLength_1 - 1);
    Bound_1 = P(Origin, V(armLength_1 * cos(dof_1*PI / 180), armLength_1 * sin(dof_1*PI / 180)));
  }
  if(key=='d') {
    armLength_1++;
    Bound_1 = P(Origin, V(armLength_1 * cos(dof_1*PI / 180), armLength_1 * sin(dof_1*PI / 180)));
  }
  if(key=='+') {
    dof_1 = min(360,dof_1 + 1);
    Bound_1 = P(Origin, V(armLength_1 * cos(dof_1*PI / 180), armLength_1 * sin(dof_1*PI / 180)));
  }
  if(key=='-') {
    dof_1 = max(1, dof_1 - 1);
    Bound_1 = P(Origin, V(armLength_1 * cos(dof_1*PI / 180), armLength_1 * sin(dof_1*PI / 180)));
  }
  if(key=='2') {
    dof_2 = min(360,dof_2 + 1);
    Bound_2_Up.followOnArc(Origin,A,(dof_2+armOffset)*PI/180,armLength_2);
    Bound_2_Down.followOnArc(Origin,A,armOffset * PI / 180,armLength_2);
  }
  if(key=='1') {
    dof_2 = max(1, dof_2 - 1);
    Bound_2_Up.followOnArc(Origin,A,(dof_2+armOffset)*PI/180,armLength_2);
    Bound_2_Down.followOnArc(Origin,A,armOffset * PI / 180,armLength_2);
  }
  if(key=='0') {
    armOffset = min(360,armOffset + 1);
    Bound_2_Up.followOnArc(Origin,A,(dof_2+armOffset)*PI/180,armLength_2);
    Bound_2_Down.followOnArc(Origin,A,armOffset * PI / 180,armLength_2);
  }
  if(key=='9') {
    armOffset = max(1, armOffset - 1);
    Bound_2_Up.followOnArc(Origin,A,(dof_2+armOffset)*PI/180,armLength_2);
    Bound_2_Down.followOnArc(Origin,A,armOffset * PI / 180,armLength_2);
  }
  if(key=='}') displayBoundary = !displayBoundary;
  if(key=='c') mode = 1;
  if(key=='m') mode = 2;
  if(key=='p') mode = 0;
  if(key=='r') showArmRange = !showArmRange;
  if(key=='s') showArms = !showArms;
  if(key=='n') showMagnet = !showMagnet;
}

void mousePressed()   // executed when the mouse is pressed
{
  if (mode == 1) {
    P = Mouse();
    moveToPoint(P);
  } else if (mode == 0) {
    PNT M=Mouse();
    P=A;
    if(dist(M,B)<dist(M,P)) P=B; 
  } else if (mode == 2) {
    target = Mouse();
    onRight = B.x > 400;
    movingToCenter = (onRight && target.x < 400) || (!onRight && target.x > 400);
    animating = true;
    time = 0;
    start = B;
  }
}

void mouseDragged() // executed when the mouse is dragged (while mouse buttom pressed)
  {
  if (mode == 0) {
    if (P == A) {
      if (dof_1 == 360) {
        P.teleportOnCircle(Origin,armLength_1);
      } else {
        P.teleportOnArc(Origin,armLength_1,Bound_1, P(Origin, V(armLength_1, 0)),dof_1);
      }
      Bound_2_Up.followOnArc(Origin,A,(dof_2+armOffset)*PI/180,armLength_2);
      Bound_2_Down.followOnArc(Origin,A,armOffset * PI / 180,armLength_2);
      B.followOnArc(Origin,A,magnetAngle,armLength_2);
    } else if (P == B && dof_2 == 360) {
      P.teleportOnCircle(A, armLength_2);
      magnetAngle = angle(V(Origin,A),V(A,B));
    } else if (P == B) {
      P.teleportOnArc(A,armLength_2,Bound_2_Up,Bound_2_Down,dof_2);
      magnetAngle = angle(V(Origin,A),V(A,B));
    } else {
      P.teleport(); // teleporting mouse motion
    }
  }
} 

void moveToPoint(PNT P) {
  float x = P.x-400;
  float y = P.y-400;
  float angle2;
  float angle1;
  if (x > 0) {
    angle2 = -acos((sq(x)+sq(y)-sq(armLength_1)-sq(armLength_2)) / (2*armLength_1*armLength_2));
    angle1 = atan(y/x)-atan((armLength_2*sin(angle2))/(armLength_1+armLength_2*cos(angle2)));
  } else {
    angle2 = acos((sq(x)+sq(y)-sq(armLength_1)-sq(armLength_2)) / (2*armLength_1*armLength_2));
    angle1 = PI + atan(y/x)-atan((armLength_2*sin(angle2))/(armLength_1+armLength_2*cos(angle2)));
  }
  VCT firstVector = Rotated(V(armLength_1, 0),angle1);
  A = P(Origin, firstVector);
  B = P(A, armLength_2, Rotated(Normalized(firstVector),angle2));
}
