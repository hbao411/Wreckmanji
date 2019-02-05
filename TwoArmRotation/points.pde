//**** POINT-CREATING FUNCTIONS
PNT P() {return P(0,0); };                                        // makes point at origin                                                                       
PNT P(float x, float y) {return new PNT(x,y); };                   // make point (x,y)
PNT P(PNT Q) {return new PNT(Q.x,Q.y); };                   // make copy of Q
PNT Mouse() {return P(mouseX,mouseY);}              // current mouse position
PNT PMouse() {return P(pmouseX,pmouseY);}           // previous mouse position
PNT LERP(PNT A, float t, PNT B) {return P(A.x+t*(B.x-A.x),A.y+t*(B.y-A.y));} // Lerp (1-t)A+tB, Weighted average of points

//**** FUNCTIONS THAT MEASURE PROPERTIES OF POINTS
float dist(PNT P, PNT Q) {return sqrt(sq(Q.x-P.x)+sq(Q.y-P.y));}     // Distance |PQ|      

//**** COMMANDS FOR DISPLAYING GRAPHIC PRIMITIVES AND TEXT USING POINTS
void drawCircle(PNT C, float r) {ellipse(C.x, C.y, 2*r, 2*r);};    // draws circ(C,r) 
void drawSquare(PNT C, float r) {rect(C.x-r/2, C.y-r/2, r, r);};
void drawFirstArc(PNT C, float r, float d) {arc(C.x, C.y, 2*r, 2*r, 0, d);};
void drawSecondArc(PNT C, float angle, float offset, float r, float d) {arc(C.x, C.y, 2*r, 2*r, angle+offset, angle+d+offset);};
void showLabelInCircle(PNT P, String S) {fill(white); drawCircle(P,13); fill(black); writeLabel(P,S);} // Writes S in circle of current color                      
void drawEdge(PNT A, PNT B)  {line(A.x,A.y,B.x,B.y);}    // render edge [A,B]
void drawTriangle(PNT A, PNT B, PNT C)  {beginShape();  vert(A); vert(B); vert(C); endShape(CLOSE);}    // render triangle A, B, C
void drawQuad(PNT A, PNT B, PNT C, PNT D)  {beginShape();  vert(A); vert(B); vert(C); vert(D); endShape(CLOSE);}  // render quad A, B, C, D
void drawOpenQuad(PNT A, PNT B, PNT C, PNT D)  {beginShape();  vert(A); vert(B); vert(C); vert(D); endShape();}  // render quad A, B, C, D
void vert(PNT P) {vertex(P.x,P.y);};  // use between beginShape and endShape to display polylines   
void writeLabel(PNT P, String S) {text(S, P.x-1.,P.y-2.5); }       // writes string S near P
// Teextyre mapping
void vert(PNT P, float u, float v) {vertex(P.x,P.y,u,v);};  // use between beginShape and endShape to display polygon with texture   
void drawQuadTextured(PNT A, PNT B, PNT C, PNT D, PImage pix)  // render quad A, B, C, D with texture inside
  {
  beginShape();  
   texture(pix);
   vert(A,0,0); vert(B,1,0); vert(C,1,1); vert(D,0,1); 
  endShape(CLOSE);
  }  


//**** POINT CLASS
class PNT // POINT
  { 
  float x=0, y=0; 
  PNT (float px, float py) {x = px; y = py;};
  void moveBy(float dx, float dy) {x += dx; y += dy;}
  void teleport() {moveBy(mouseX-pmouseX,mouseY-pmouseY);}
  void teleportOnCircle(PNT origin, float radius) {
    VCT direction = Normalized(V(origin, P(mouseX,mouseY)));
    PNT newLocation = P(origin, radius, direction);
    moveBy(newLocation.x - x, newLocation.y - y);
  }
  void teleportOnArc(PNT origin, float radius, PNT bound_up, PNT bound_down, float dof) {
    PNT newLocation;
    PNT mouse = P(mouseX,mouseY);
    VCT direction = Normalized(V(origin, mouse));
    VCT boundVector = Normalized(V(origin,bound_up));
    VCT startVector = V(origin,bound_down);
    PNT start = P(origin, startVector);
    if (dof > 180) {
      if (det(boundVector,direction)>0 && det(startVector,direction)<0) {
        newLocation = dist(mouse,bound_up)<dist(mouse,start) ? P(bound_up) : P(start);
      } else {
        newLocation = P(origin, radius, direction);
      }
    } else {
      if (!(det(startVector,direction)>=0 && det(boundVector,direction)<=0)) {
        newLocation = dist(mouse,bound_up)<dist(mouse,start) ? P(bound_up) : P(start);
      } else {
        newLocation = P(origin, radius, direction);
      }
    }
    moveBy(newLocation.x - x, newLocation.y - y);
  }
  void followOnArc(PNT origin, PNT a, float angle, float armLength) {
    VCT direction = Normalized(Rotated(V(origin,a),angle));
    PNT newLocation = P(a, armLength, direction);
    moveBy(newLocation.x - x, newLocation.y - y);
  }
  void setTo(PNT P) {x = P.x; y = P.y;} // copies coordinates from another points
  void setTo(float px, float py) {x = px; y = py;}
  float norm() {return(sqrt(sq(x)+sq(y)));};
  } // end of PNT class
  
void declarePoints(PNT[] Points)  // creates objects for all Point[i], MUST BE DONE AT INITALIZATION
{
  for (int i=0; i<1; i++) Points[i]=P(Origin.x + armLength_1, Origin.y); // initializes ALL Point[i] to (0,0)
}  
