/*
 *  OpenArms MK.3 System
 *  
 *  Use Encoder Rotary for switching motion and use EMG sensor for detecting muscle signals.
 *  
 *   The circuit:
 *   - Use Bluno Nano board : ATmeaga328P
 *   - Encoder Rotary : pin D2 - DT, pinD13 - CLK, 5V - VCC 
 *   - EMG Sensor : A7 - SIG, 5V - VCC
 *   - Servo Motors(finger) : ALL pwm pin except D3, 7.4V - VCC
 *   - Servo Motor(palm) : D3, 7.4V - VCC
 *   
 *   created 2017 / 11 / 07
 *   by Dongmin Kim <Ethan> ( ARTIT ROBOTICS )
 *   This code is in the Github domain.
 *   http://.../
 *   
 */
#include <Servo.h>

uint8_t encoder_DT = 12;
uint8_t encoder_CLK = 2;

uint8_t EMG_SIG = A7;

Servo fServo_1;
Servo fServo_2;
Servo fServo_3;
Servo fServo_4;
Servo fServo_5;
Servo pServo_1;

static unsigned long lastInterruptTime = 0;
static int signal_index = 0;

int actStates = 0;
volatile unsigned int motionStates = 0;
int signal_array[10];
int signal_average;

//EMG Sensor L/H Threshold
static int MAX_EMG_VAL = 650;
static int MIN_EMG_VAL = 250;

char prime_num = '0';
char buf[10];
 
void setup() {
  fServo_1.attach(11);
  fServo_2.attach(10);
  fServo_3.attach(9);
  fServo_4.attach(6);
  fServo_5.attach(5);
  pServo_1.attach(3);
  
  pinMode(encoder_DT, INPUT_PULLUP);
  pinMode(encoder_CLK, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder_CLK), change_motion, LOW);

  init_EMG();
  init_motion();
  Serial.begin(115200);
}

void loop() {
  if(signal_average > MAX_EMG_VAL && actStates == 0) {
    actStates = 1;
    select_motion();
  }
  if(signal_average < MIN_EMG_VAL && actStates == 1) {
    actStates = 2;
  }
  if(signal_average > MAX_EMG_VAL && actStates == 2) {
    actStates = 3;
    init_motion();
  }
  if(signal_average < MIN_EMG_VAL && actStates == 3) {
    actStates = 0;
  }
  if(motionStates >= 5 && motionStates <= 20) {
    Serial.print("EMG VALUE : ");
    Serial.println(signal_average);
    
    boolean prime = false;
    int index = 0;
    
    while(Serial.available()) {
      if(prime == false){
        prime = true;
        prime_num = Serial.read();
      }
      else{
        buf[index] = Serial.read();
        index++;
      }
    }
    if(prime_num == '1'){
      MAX_EMG_VAL = atoi(buf);
      Serial.print("MAX_EMG_VAL : ");
      Serial.println(MAX_EMG_VAL);
    }else if(prime_num == '2'){
      MIN_EMG_VAL = atoi(buf);
      Serial.print("MIN_EMG_VAL : ");
      Serial.println(MIN_EMG_VAL);
    }
  prime_num = '0';
  }
  get_EMG();
  delay(50);
}

void init_EMG() {
  for(int i = 0; i < 10; i++) {
    signal_array[i] = analogRead(EMG_SIG);
    delay(10);
  }
  for(int i = 0; i < 10; i++) {
    signal_average += signal_array[i];
  }
  signal_average /= 10;
}

void get_EMG() {
  signal_array[signal_index] = analogRead(EMG_SIG);
  delay(10);
  for(int i = 0; i < 10; i++) {
    signal_average += signal_array[i];
  }
  signal_average /= 10;
  signal_index++;
  if(signal_index == 10) signal_index = 0;
}

void init_motion() {
  fServo_5.write(150);
  delay(100);
  fServo_4.write(140);
  delay(100);
  fServo_3.write(180);
  delay(100);
  fServo_2.write(120);
  delay(100);
  pServo_1.write(30);
  delay(100);
  fServo_1.write(160);
}

void select_motion() {
  switch(motionStates){
    case 0://잡기(Catch)
      pServo_1.write(80);
      delay(100);
      fServo_1.write(10);
      delay(100);
      fServo_2.write(10);
      delay(100);
      fServo_3.write(10);
      delay(100);
      fServo_4.write(10);
      delay(100);
      fServo_5.write(10);
      break;
    case 1://집기(Pick)
      fServo_1.write(10);
      fServo_2.write(10);
      fServo_3.write(180);
      fServo_4.write(140);
      fServo_5.write(150);
      pServo_1.write(90);
      break;
    case 2://공 잡기(Catch ball)<Not Yet>
      fServo_5.write(10);
      fServo_4.write(10);
      delay(300);
      pServo_1.write(110);
      fServo_1.write(10);
      delay(100);
      fServo_3.write(50);
      fServo_2.write(40);
      break;
    case 3://악수(Shake hand)
      pServo_1.write(110);
      delay(1000);
      fServo_5.write(90);
      delay(100);
      fServo_4.write(90);
      delay(100);
      fServo_3.write(90);
      delay(100);
      fServo_2.write(90);
      delay(100);
      fServo_1.write(90);
  }
}


//ISR(Interrupt Service Routine)
void change_motion() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(encoder_DT) == LOW) {
      motionStates--;
    }
    else {
      motionStates++;
    }
    motionStates = min(20, max(0, motionStates));
    lastInterruptTime = interruptTime;
    Serial.println(motionStates);
  }
}
