#include <Servo.h> // [2972] 서보 헤더파일 포함

/////////////////////////////
// Configurable parameters //
/////////////////////////////

// Arduino pin assignment
#define PIN_SERVO 10 // [2951] servo moter를 아두이노 GPIO 10번 핀에 연결
#define PIN_IR A0    // [2961] 적외선센서를 아두이노 A0핀에 연결

// Framework setting
#define _DIST_TARGET 255 // [2952] 탁구공의 목표 위치를 25.5cm로 설정
#define _DIST_MIN 100 // [2972] 거리 센서가 인식 가능하게 설정한 최소 거리
#define _DIST_MAX 400 // [2972] 거리 센서가 인식 가능하게 설정한 최대 거리

// Distance sensor
#define _DIST_ALPHA 0.2   // [2959] ema 필터에 적용할 알파값
#include "medianfilter.h"

// Servo range
#define _DUTY_MIN 1650 // [2952] 서보의 최소 각도값 왼쪽으로 올라감 1650 =>이름표 반대 기준
#define _DUTY_NEU 1900 // [2952] 서보의 중간 각도값 (1750- 1820) 1900
#define _DUTY_MAX 2100 // [1691] 서보의 최대 각도 2100

// Servo speed control
#define _SERVO_ANGLE 30  //[2967] 서보 각도 설정
#define _SERVO_SPEED 350 //[2959] 서보의 속도 설정 140 220

// Event periods
#define _INTERVAL_DIST 4  //[2959] 센서의 거리측정 인터벌값
#define _INTERVAL_SERVO 20 //[2967] 서보 INTERVAL값 설정
#define _INTERVAL_SERIAL 100  //[2959] 시리얼 모니터/플로터의 인터벌값 설정

// PID parameters
#define _KP 1.3// [2957] 비례 제어 값 KP => 1.2 KD => 50
                // [2961] 비례이득
#define _KD 55 // 미분 제어 값

#define _KI 0.005

#define _ITERM_MAX 10


#define sizei 5


//////////////////////
// global variables //
//////////////////////

// Servo instance
Servo myservo;  // [2972] 서보 정의

// Distance sensor
float dist_target; // location to send the ball
float dist_raw, dist_ema;    // [2961] dist_raw : 적외선센서로 얻은 거리를 저장하는 변수
                             // [2961] dist_ema : 거리를 ema필터링을 한 값을 저장하는 변수
float alpha;    // [2959] ema의 알파값을 저장할 변수

// Event periods
unsigned long last_sampling_time_dist, last_sampling_time_servo, last_sampling_time_serial;// [2957] last_sampling_time_dist : 거리센서 측정 주기
                          // [2957] last_sampling_time_servo : 서보 위치 갱신 주기
                          // [2957] last_sampling_time_serial : 제어 상태 시리얼 출력 주기
bool event_dist, event_servo, event_serial, prev; // [2957] 각각의 주기에 도달했는지를 불리언 값으로 저장하는 변수

// Servo speed control
int duty_chg_per_interval = (_DUTY_MAX - _DUTY_MIN) * _SERVO_SPEED / 180 * _INTERVAL_DIST / 1000; // [2952] 한 주기당 변화할 서보 활동량을 정의
int duty_target, duty_curr; // [2961] 목표위치, 서보에 입력할 위치

// PID variables
float error_curr, control, pterm, dterm, iterm;
float error_prev = 0.0;

// 가중치 필터링
float arr_dist[sizei];
int index;
float dist_sum = 0.0;

// LPF 필터링
static long apt = 0; 
unsigned long oldmil;

int fc = 11; 
float dt = _INTERVAL_DIST/1000.0; 
float lambda = 2*PI*fc*dt;
float calidist = 0.0, filter_dist = 0.0;
float dist_prev = 0.0;


void quick_sort(float *data, int start, int end){
  if(start >= end){
    //원소가 1개일 때
    return;
  }

  int pivot = start;
  int i = pivot + 1;
  int j = end;
  float temp;

  while(i <= j){
    while(i <= end && data[i] <= data[pivot]){
      ++i;
    }
    while(j > start && data[j] >= data[pivot]){
      --j;
    }

    if(i > j){
      temp = data[j];
      data[j] = data[pivot];
      data[pivot] = temp;
    }
    else{
      temp = data[i];
      data[i] = data[j];
      data[j] = temp;
    }
  }

  quick_sort(data, start, j-1);
  quick_sort(data, j+1, end);
}

// [2964] 실제 거리가 100mm, 400mm일 때 센서가 읽는 값(각 a, b)
float a = 667.0; // 100
float b = 458.0; // 150
float c = 366.0; // 200
float d = 318.0; // 250
float e = 286.0; // 300
float f = 258.0; // 350
float g = 236.0; // 400


float ir_distance_filtered(short val){ // return value unit: mm
  if(val >= b){
    return 100 + 50.0 / (b-a) * (val - a);
  }

  else if(val < b && val >= c){
    return 150 + 50.0 / (c-b) * (val - b);
  }

  else if(val < c && val >= d){
    return 200 + 50.0 / (d-c) * (val - c);
  }

  else if(val < d && val >= e){
    return 250 + 50.0 / (e-d) * (val - d);
  }

  else if(val < e && val >= f){
    return 300 + 50.0 / (f-e) * (val - e);
  }

  else if(val < f){
    return 350 + 50.0 / (g-f) * (val - f);
  }
}

MedianFilter<ir_distance_filtered> filter;

void setup() {
  myservo.attach(PIN_SERVO); // [2952] 서보 모터를 GPIO 10번 포트에 연결
  
  // initialize global variables
  alpha = _DIST_ALPHA;   // [2959] ema의 알파값 초기화
  dist_ema = 0;          // [2959] dist_ema 초기화
  duty_curr = _DUTY_NEU; // [2959] duty_durr 중간값으로 초기화
  prev = false;
  index = 0;
  // move servo to neutral position
  myservo.writeMicroseconds(_DUTY_NEU); // [2952] 서보 모터를 중간 위치에 지정
  
  // initialize serial port
  Serial.begin(57600); // [2952] 시리얼 포트를 115200의 속도로 연결
    
  // convert angle speed into duty change per interval.
  duty_chg_per_interval = (_DUTY_MAX - _DUTY_MIN) * (_SERVO_SPEED / 180.0) * (_INTERVAL_SERVO / 1000.0);                // [2959] 한 주기마다 이동할 양(180.0, 1000.0은 실수타입이기 때문에 나눗셈의 결과가 실수타입으로 리턴)
  // [2974] INTERVAL -> _INTERVAL_SERVO 로 수정
  
  // [2974] 이벤트 변수 초기화
  last_sampling_time_dist = 0; // [2974] 마지막 거리 측정 시간 초기화
  last_sampling_time_servo = 0; // [2974] 마지막 서보 업데이트 시간 초기화
  last_sampling_time_serial = 0; // [2974] 마지막 출력 시간 초기화
  event_dist = event_servo = event_serial = false; // [2974] 각 이벤트 변수 false로 초기화

  filter.init();
  
   // inertial_filtering

}
  

void loop() {
  dist_raw = filter.read();
/////////////////////
// Event generator //
/////////////////////

  unsigned long time_curr = millis();  // [2964] event 발생 조건 설정
  if(time_curr >= last_sampling_time_dist + _INTERVAL_DIST) {
    last_sampling_time_dist += _INTERVAL_DIST;
    event_dist = true; // [2957] 거리 측정 주기에 도달했다는 이벤트 발생
  }
  if(time_curr >= last_sampling_time_servo + _INTERVAL_SERVO) {
    last_sampling_time_servo += _INTERVAL_SERVO;
    event_servo = true; // [2957] 서보모터 제어 주기에 도달했다는 이벤트 발생
  }
  if(time_curr >= last_sampling_time_serial + _INTERVAL_SERIAL) {
    last_sampling_time_serial += _INTERVAL_SERIAL;
    event_serial = true; // [2957] 출력주기에 도달했다는 이벤트 발생
  }

// LPF filtering
  unsigned long dmil = 0;
  if (time_curr != oldmil) { 
    dmil = time_curr-oldmil; 
    oldmil = time_curr;   
  } 
  
  apt -= dmil; 

////////////////////
// Event handlers //
////////////////////

  // get a distance reading from the distance sensor
  if(event_dist) { 
     event_dist = false;
 
      dist_raw = filter.read();

      if (apt <= 0){
        apt += _INTERVAL_DIST; 
        dist_raw = lambda/(1+lambda)*dist_raw+1/(1+lambda)*dist_prev;
      }
      
      // ema filtering
      if (dist_ema == 0){                  // [2959] 맨 처음
        dist_ema = dist_raw;               // [2959] 맨 처음 ema값 = 필터링된 측정값
      }                                    // [2963] dist_ema를 dist_raw로 초기화
      else{
        dist_ema = alpha * dist_raw + (1-alpha) * dist_ema;   // [2959] ema 구현
      }

      if (dist_ema > _DIST_MAX){
        dist_ema = _DIST_MAX;
      }

      else if(dist_ema < _DIST_MIN){
        dist_ema = _DIST_MIN;
      }
      dist_prev = dist_ema;
      
      //filtering 
      arr_dist[index] = dist_ema;
      index++;

      if(index >= sizei){ // 가중치가 높은 값 사용
        
        quick_sort(arr_dist, 0, sizei-1); // 퀵 정렬
        
        dist_ema = arr_dist[sizei / 2];
        index = 0;
        
        error_curr = _DIST_TARGET - dist_ema; // [2959] 목표 위치와 현재 위치의 차이
        pterm = _KP*error_curr;
        dterm = _KD * (error_curr - error_prev);
        iterm += _KI * error_curr;

        if(iterm > _ITERM_MAX) iterm = _ITERM_MAX;
        if(iterm < -_ITERM_MAX) iterm = -_ITERM_MAX;
        
        control = pterm + dterm + iterm;
        
        duty_target = _DUTY_NEU + control;

        if(duty_target < _DUTY_MIN) { duty_target = _DUTY_MIN; } 
        else if(duty_target > _DUTY_MAX) { duty_target = _DUTY_MAX; }
        error_prev = error_curr;
        if(abs(iterm) > _ITERM_MAX) iterm = 0;
        dist_prev = dist_ema;
      }

  }
  
  if(event_servo) {
    event_servo = false; // [2974] 서보 이벤트 실행 후, 다음 주기를 기다리기 위해 이벤트 종료
    // adjust duty_curr toward duty_target by duty_chg_per_interval
    if(duty_target > duty_curr) {  // [2964] 현재 서보 각도 읽기
      duty_curr += duty_chg_per_interval; // [2961] duty_curr은 주기마다 duty_chg_per_interval만큼 증가
      if(duty_curr > duty_target) {duty_curr = duty_target;} // [2956] duty_target 지나쳤을 경우, duty_target에 도달한 것으로 duty_curr값 재설정
    }
    else {
      duty_curr -= duty_chg_per_interval;  // [2961] duty_curr은 주기마다 duty_chg_per_interval만큼 감소
      if (duty_curr < duty_target) {duty_curr = duty_target;} // [2956] duty_target 지나쳤을 경우, duty_target에 도달한 것으로 duty_curr값 재설정
    }
    // update servo position
    myservo.writeMicroseconds(duty_curr);  // [2964] 서보 움직임 조절
    
  }
  
  if(event_serial) {
    event_serial = false; // [2974] 출력 이벤트 실행 후, 다음 주기까지 이벤트 종료
    Serial.print("IR:");
    Serial.print(dist_ema); 
    Serial.print(",T:");
    Serial.print(_DIST_TARGET);
    Serial.print(",P:");
    Serial.print(map(pterm, -1000, 1000, 510, 610));
    Serial.print(",D:");
    Serial.print(map(dterm, -1000, 1000, 510, 610));
    Serial.print(",I:");
    Serial.print(map(iterm, -1000, 1000, 510, 610));
    Serial.print(",DTT:"); 
    Serial.print(map(duty_target, 1000, 2000, 410, 510));
    Serial.print(",DTC:"); 
    Serial.print(map(duty_curr, 1000, 2000, 410, 510)); // [2957] 서보모터에 입력한 값 출력
    Serial.println(",-G:245,+G:265,m:0,M:800");
  }
  
  
}
