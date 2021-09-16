#define PIN_LED 7
int toggle, num = 0;
void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, 0); //LED 키기
  delay(1000);
}

void loop() {
  num++;
  toggle = ChangeToggle(toggle);
  digitalWrite(PIN_LED, toggle);
  delay(100);
  if(num == 10){
    digitalWrite(PIN_LED, 1);
    while(1){
      
    }
  }
}

int ChangeToggle(int toggle){
  return !toggle;
}
