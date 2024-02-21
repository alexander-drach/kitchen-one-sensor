#include <GyverStepper.h>
#include "GyverButton.h"

GButton stopDownSensor(2);
GButton slowDownSpeed(3);

GButton slowUpSpeed(4);
GButton stopUpSensor(5);

GButton hand(6);

// GButton moveUp(7);
// GButton moveDown(8);
// GButton buttonSensor(12);

// byte buttonSensor;

GStepper<STEPPER2WIRE> stepper(200, 11, 10);

int speed = 800;

bool flag = false;

bool motionDetected = false;
bool firstMotionDetected = false;
bool motorMovingUp = false;
bool motorMovingDown = false;

bool statePositionUp = false;
bool statePositionDown = false;

bool moving = false;

void setup() {
  Serial.begin(9600);

  stepper.setRunMode(KEEP_SPEED);
  stepper.setMaxSpeed(1500);

  pinMode(7, OUTPUT);
}

void loop() {

  stopDownSensor.tick();
  slowDownSpeed.tick();

  slowUpSpeed.tick();
  stopUpSensor.tick();

  hand.tick();

  // moveUp.tick();
  // moveDown.tick();

  stepper.tick();

  motionDetected = !digitalRead(12);

  if (hand.isPress()) {
    Serial.println("hand sunul ruku STOP"); // проверка на руку
    stepper.brake();
    // stepper.autoPower(true); // снятие напряжения
    flag = false;
  }

  if (stopDownSensor.isPress()) {// остановка фартука внизу ДАТЧИК - 0
    stepper.brake();
    digitalWrite(7, HIGH); // снятие напряжения без библиотеки
    flag = false;
    Serial.println("doehal vniz");
    statePositionDown = true;
    statePositionUp = false;
    moving = false;
    digitalWrite(7, HIGH);
  }  

  if (stopUpSensor.isPress()) { // остановка фартука вверху ДАТЧИК - 3
    stepper.brake();
    digitalWrite(7, HIGH); // снятие напряжения без библиотеки
    flag = false;
    Serial.println("doehal vverh");
    statePositionUp = true;
    statePositionDown = false;
    moving = false;
    digitalWrite(7, HIGH);
  }

  if (motionDetected) {
    Serial.println("moveUp"); // движение фартука вверх по сенсору вверх/вниз

    digitalWrite(7, LOW); 

    if (!firstMotionDetected) {
      stepper.setSpeed(speed);
      firstMotionDetected = true;
      moving = true;
    }

    if (firstMotionDetected && statePositionDown) {
      stepper.setSpeed(speed);
      moving = true;
    }

    if (firstMotionDetected && statePositionUp) {
      stepper.setSpeed(-speed);
      moving = true;
    }

    if (stopUpSensor.isHold() && !motionDetected) {
      stepper.brake();
      digitalWrite(7, HIGH); // снятие напряжения без библиотеки
      flag = false;
      Serial.println("doehal vverh");
      statePositionUp = true;
      statePositionDown = false;
      moving = false;
    }

    if (stopUpSensor.isHold() && motionDetected) {
      stepper.setSpeed(-speed);
      moving = true;
    }

    if (stopDownSensor.isHold() && !motionDetected) {
      stepper.brake();
      digitalWrite(7, HIGH); // снятие напряжения без библиотеки
      flag = false;
      Serial.println("doehal vniz");
      statePositionDown = true;
      statePositionUp = false;
      moving = false;
    }

    if (stopDownSensor.isHold() && motionDetected) {
      stepper.setSpeed(speed);
      moving = true;
    }
  }

  if (statePositionDown && moving) {
    if (slowUpSpeed.isPress()) { // замедление скорости вверху ДАТЧИК - 2
      stepper.setSpeed(200);
      Serial.println("slowUpSpeed");
    }
  }

  if (statePositionUp && moving) {
    if (slowDownSpeed.isPress()) { // замедление скорости внизу ДАТЧИК - 1  
      stepper.setSpeed(-200);
      Serial.println("slowDownSpeed");
    }
  }
}
