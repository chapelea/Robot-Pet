/*
    Attempting to make a robot "dog"
    @author Ethan Chapel
*/
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.hpp>

#define IR_Pin 10

const int RSPD1 = 75;
const int LSPD1 = 75;

const int RSPD2 = 30;
const int LSPD2 = 30;

const int LWhFwdPin = 6;
const int LWhBwdPin = 4;
const int LWhPWMPin = 2;

const int servoPin = 3;
const int buzzerPin = 9;
const int redLEDPin = 11; //Angry
const int RWhFwdPin = 5;
const int RWhBwdPin = 7;
const int RWhPWMPin = 8;
const int yellowLEDPin = 12; //Hungry
const int greenLEDPin = 13; //Happy

int mood = 1; //Mood starts as happy(1), will become hungry(2) over time, and will get angry(3) if not fed.

int dec;

unsigned long lastBeep = 0;
long randBeep;

unsigned long lastFeed = 0;

unsigned long lastWag = 0;
boolean wag = false;

Servo myservo;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void setup() {
  pinMode(redLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  pinMode(LWhFwdPin, OUTPUT); //Define left forward pin
  pinMode(LWhBwdPin, OUTPUT); //Define left backward pin
  pinMode(LWhPWMPin, OUTPUT); //Define left pwm pin
  pinMode(RWhFwdPin, OUTPUT); //Define right forward pin
  pinMode(RWhBwdPin, OUTPUT); //Define right backward pin
  pinMode(RWhPWMPin, OUTPUT); //Define right pwm pin

  digitalWrite(LWhFwdPin, LOW);
  digitalWrite(LWhBwdPin, LOW);
  digitalWrite(LWhPWMPin, 0);
  //All motors off
  digitalWrite(RWhFwdPin, LOW);
  digitalWrite(RWhBwdPin, LOW);
  digitalWrite(RWhPWMPin, 0);

  //Servo
  myservo.attach(servoPin);

  //IR sensor
  pinMode(IR_Pin, INPUT);
  IrReceiver.begin(IR_Pin);

  //LCD
  lcd.init();
  lcd.backlight();
  lcd.println("Hello!");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.println("I am Goddard.");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);

  //Serial monitor
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  IrReceiver.decode();
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    dec = IrReceiver.decodedIRData.command;
    Serial.print(dec);
    IrReceiver.resume();
  }
  
  //Hunger
  if (millis() - lastFeed > 60000) { //Gets hungry after 1 min
    mood = 2;
  }
  if (millis() - lastFeed > 120000) { //Get angry after not being fed for 2 min
    mood = 3;
  }

  //Movement
  if (dec == 82) { //Forward Command
    Serial.println(dec);
    if (mood == 1) {
      digitalWrite(LWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, HIGH);   //run left wheel forward
      digitalWrite(LWhPWMPin, LSPD1);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(RWhFwdPin, HIGH);  //run right wheel forward
      digitalWrite(RWhPWMPin, RSPD1);
    } else if (mood == 2) {
      digitalWrite(LWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, HIGH);   //run left wheel forward at half speed
      digitalWrite(LWhPWMPin, LSPD2);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(RWhFwdPin, HIGH);  //run right wheel forward at half speed
      digitalWrite(RWhPWMPin, RSPD2);
    } else {
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, HIGH);   //run left wheel backward
      digitalWrite(LWhPWMPin, LSPD1);
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);  //run right wheel backward
      digitalWrite(RWhPWMPin, RSPD1);
    }
  } else if (dec == 24) { //Backward Command
    Serial.println(dec);
    if (mood == 1) {
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, HIGH);   //run left wheel backward
      digitalWrite(LWhPWMPin, LSPD1);
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);  //run right wheel backward
      digitalWrite(RWhPWMPin, RSPD1);
    } else if (mood == 2) {
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, HIGH);   //run left wheel backward at half speed
      digitalWrite(LWhPWMPin, LSPD2);
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);  //run right wheel backward at half speed
      digitalWrite(RWhPWMPin, RSPD2);
    } else {
      digitalWrite(LWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, HIGH);   //run left wheel forward
      digitalWrite(LWhPWMPin, LSPD1);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(RWhFwdPin, HIGH);  //run right wheel forward
      digitalWrite(RWhPWMPin, RSPD1);
    }
  } else if (dec == 28) { //Stop Command
    Serial.println(dec);
    digitalWrite(LWhFwdPin, LOW);   //stop left wheel
    digitalWrite(LWhBwdPin, LOW);
    digitalWrite(LWhPWMPin, 0);
    digitalWrite(RWhFwdPin, LOW);   // stop right wheel
    digitalWrite(RWhBwdPin, LOW);
    digitalWrite(RWhPWMPin, 0);
  } else if (dec == 8) { //Left Command
    Serial.println(dec);
    if (mood == 1) {
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, HIGH);   //run left wheel backward
      digitalWrite(LWhPWMPin, LSPD1);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(RWhFwdPin, HIGH);  //run right wheel forward
      digitalWrite(RWhPWMPin, RSPD1);
      delay(250);
    } else if (mood == 2) {
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, HIGH);   //run left wheel backward at half speed
      digitalWrite(LWhPWMPin, LSPD2);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(RWhFwdPin, HIGH);  //run right wheel forward at half speed
      digitalWrite(RWhPWMPin, RSPD2);
      delay(500);
    } else {
      digitalWrite(LWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, HIGH);   //run left wheel forward
      digitalWrite(LWhPWMPin, LSPD1);
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);  //run right wheel backward
      digitalWrite(RWhPWMPin, RSPD1);
      delay(250);
    }
    dec = 28;
  } else if (dec == 90) { //Right Command
    Serial.println(dec);
    if (mood == 1) {
      digitalWrite(LWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, HIGH);   //run left wheel forward
      digitalWrite(LWhPWMPin, LSPD1);
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);  //run right wheel backward
      digitalWrite(RWhPWMPin, RSPD1);
      delay(250);
    } else if (mood == 2) {
      digitalWrite(LWhBwdPin, LOW);
      digitalWrite(LWhFwdPin, HIGH);   //run left wheel forward at half speed
      digitalWrite(LWhPWMPin, LSPD2);
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);  //run right wheel backward at half speed
      digitalWrite(RWhPWMPin, RSPD2);
      delay(500);
    } else {
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(LWhBwdPin, HIGH);   //run left wheel backward
      digitalWrite(LWhPWMPin, LSPD1);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(RWhFwdPin, HIGH);  //run right wheel forward
      digitalWrite(RWhPWMPin, RSPD1);
      delay(250);
    }
    dec = 28;
  } else if (dec == 25) { //Feeding
    digitalWrite(LWhFwdPin, LOW);   //stop left wheel
    digitalWrite(LWhBwdPin, LOW);
    digitalWrite(LWhPWMPin, 0);
    digitalWrite(RWhFwdPin, LOW);   // stop right wheel
    digitalWrite(RWhBwdPin, LOW);
    digitalWrite(RWhPWMPin, 0);
    if (mood == 1) {
      dec = 0;
    } else if (mood == 2) {
      mood = 1;
      dec = 0;
    } else {
      mood = 2;
      dec = 0;
    }
    lastFeed = millis();
  } else if (dec == 68 && mood == 1) { //Spin left
    digitalWrite(LWhFwdPin, LOW);
    digitalWrite(LWhBwdPin, HIGH);   //run left wheel backward
    digitalWrite(LWhPWMPin, LSPD1);
    digitalWrite(RWhBwdPin, LOW);
    digitalWrite(RWhFwdPin, HIGH);  //run right wheel forward
    digitalWrite(RWhPWMPin, RSPD1);
    delay(1000);
    dec = 28;
  } else if (dec == 64 && mood == 1) { //Spin right
    digitalWrite(LWhBwdPin, LOW);
    digitalWrite(LWhFwdPin, HIGH);   //run left wheel forward
    digitalWrite(LWhPWMPin, LSPD1);
    digitalWrite(RWhFwdPin, LOW);
    digitalWrite(RWhBwdPin, HIGH);  //run right wheel backward
    digitalWrite(RWhPWMPin, RSPD1);
    delay(1000);
    dec = 28;
  }

  if (mood == 1) {
    happySubRoutine();
  } else if (mood == 2) {
    hungrySubRoutine();
  } else if (mood == 3) {
    angrySubRoutine();
  }
}

void happySubRoutine() {
  digitalWrite(greenLEDPin, HIGH);
  digitalWrite(redLEDPin, LOW);
  digitalWrite(yellowLEDPin, LOW);
  lcd.clear();

  //Personality beeps every 45 sec
  if (millis() - lastBeep > 30000) {
    beep();
  }

  //Wag tail
  if (millis() - lastWag > 750) {
    lastWag = millis();
    if (wag) {
      myservo.write(135);
    } else {
      myservo.write(45);
    }
    wag = !wag;
  }
}

void hungrySubRoutine() {
  //LED indicators
  digitalWrite(greenLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);
  digitalWrite(yellowLEDPin, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("I am hungry.");

  //Personality beeps every 40 sec
  if (millis() - lastBeep > 15000) {
    beep();
  }

  //Wag tail slowly
  if (millis() - lastWag > 1500) {
    lastWag = millis();
    if (wag) {
      myservo.write(135);
    } else {
      myservo.write(45);
    }
    wag = !wag;
  }
}

void angrySubRoutine() {
  digitalWrite(greenLEDPin, LOW);
  digitalWrite(redLEDPin, HIGH);
  digitalWrite(yellowLEDPin, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("I am ANGRY!!!");

  //Personality beeps every 35 sec
  if (millis() - lastBeep > 5000) {
    beep();
  }
}

void beep() {
  lastBeep = millis();
  randBeep = random(6);
    if (randBeep == 1) {
      tone(buzzerPin, 100);
      delay(100);
      tone(buzzerPin, 300);
      delay(200);
      tone(buzzerPin, 150);
      delay(100);
      noTone(buzzerPin);
    } else if (randBeep == 2) {
      tone(buzzerPin, 200);
      delay(200);
      tone(buzzerPin, 250);
      delay(100);
      tone(buzzerPin, 390);
      delay(300);
      tone(buzzerPin, 100);
      delay(100);
      tone(buzzerPin, 300);
      delay(300);
      noTone(buzzerPin);
    } else if (randBeep == 3) {
      tone(buzzerPin, 300);
      delay(50);
      tone(buzzerPin, 100);
      delay(100);
      tone(buzzerPin, 300);
      delay(50);
      noTone(buzzerPin);
    } else if (randBeep == 4) {
      tone(buzzerPin, 400);
      delay(175);
      tone(buzzerPin, 100);
      delay(200);
      tone(buzzerPin, 600);
      delay(70);
      tone(buzzerPin, 100);
      delay(500);
      tone(buzzerPin, 500);
      delay(100);
      tone(buzzerPin, 100);
      delay(200);
      tone(buzzerPin, 200);
      delay(100);
      tone(buzzerPin, 100);
      delay(100);
      noTone(buzzerPin);
    } else {
      tone(buzzerPin, 500);
      delay(150);
      noTone(buzzerPin);
  }
  IrReceiver.begin(IR_Pin);
}
