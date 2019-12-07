const int measureVolt = A0;
const double res_LEFT = 121000;
const double res_RIGHT = 192.74;
const double res_const = 98490;
const double res_center_Y = 82576;
const double res_Up = 159488.01;
const double res_Down = 96.28;
const double res_center = 48741.0;
const double inputVoltage = 5.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  double voltage_value = analogRead(measureVolt);
  double voltage = voltage_value /1024 * inputVoltage; 
  
  double Pot_Res = res_const * (inputVoltage - voltage)/(voltage);
  //if Pot_Res < 
  Serial.print(Pot_Res);
  Serial.print('\n');
  delay(100);
}
