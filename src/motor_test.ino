/*
    This is OpenArms MK.3 project test motion code.
    Produce

    
  created 5 Nov 2017
  by Dongmin Kim(Ethan)
*/


#include <Servo.h>
Servo fServo_1;
Servo fServo_2;
Servo fServo_3;
Servo fServo_4;
Servo fServo_5;
Servo pServo_1;
 char num = '0';
void setup() {
  fServo_1.attach(11);
  fServo_2.attach(10);
  fServo_3.attach(9);
  fServo_4.attach(6);
  fServo_5.attach(5);
  pServo_1.attach(3);
  //baud rate 115200 because bluno nano's bluetooth default module setting communicate main MCU using 115200 in UART communication.
  Serial.begin(115200);
}

void loop() {
  test_motion();
  delay(50);
}

void test_motion() {
  if(Serial.available()){
    num = Serial.read();
  }
  //YOU CAN TUNE YOUR OWN SERVOS' VALUE
  //'1' IS DEFUALT STATE YOU CAN CHANGE !'1' PART, SO YOU CAN DESIGN YOUR OWN MOTION.
  //pServo is palm servo and fServo is finger servo.
  if(num != '1'){
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
  }else{
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
