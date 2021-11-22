#include <Servo.h>

#define PIN_IR A0
#define PIN_LED 9

#define PIN_SERVO 10
#define DEG_LOW 2400
#define DEG_HIGH 2000
#define DEG_AVG 2200

#define _DIST_MIN 100
#define _DIST_MAX 450

#define _DIST_ALPHA 0.5 // EMA weight of new sample (range: 0 to 1). Setting this value to 1 effectively disables EMA filter.
Servo myservo;

int a, b; // unit: mm
float befVal = 0.0;
float dist_ema = 0.0; // unit: mm
float alpha;
float temp = 0.0;

void setup() {
 // initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, 1);
  
// initialize serial port
  Serial.begin(57600);

  a = 63;
  b = 310;
  myservo.attach(PIN_SERVO); 
  alpha = _DIST_ALPHA;
  befVal = 100+300.0 / (b-a) * (ir_distance() - a);

}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop() {

  myservo.writeMicroseconds(DEG_AVG);
  float raw_dist = ir_distance();
  float dist_cali = 100 + 300.0 / (b - a) * (raw_dist - a);
  dist_ema = alpha * dist_cali + ((1-alpha) * dist_cali); // dist_ema 사용

  if(dist_ema < _DIST_MIN || dist_ema > _DIST_MAX ){
    dist_ema = befVal;
  }

  else{
    befVal = dist_ema;
  }

  Serial.print("min:0,max:500,dist:");
  Serial.print(raw_dist);
  Serial.print(",dist_cali:");
  Serial.print(dist_cali);
  Serial.print(",dist_ema:");
  Serial.println(dist_ema);
  
  if(dist_ema < 275){
    myservo.writeMicroseconds(DEG_LOW); 
  }
  else {
    myservo.writeMicroseconds(DEG_HIGH);
  }
  delay(300);
  /*
  myservo.writeMicroseconds(DEG_AVG);
  delay(1000);
  myservo.writeMicroseconds(DEG_HIGH);
  delay(1000);
  myservo.writeMicroseconds(DEG_AVG);
  delay(1000);
  myservo.writeMicroseconds(DEG_LOW);
  delay(1000);
*/

}
