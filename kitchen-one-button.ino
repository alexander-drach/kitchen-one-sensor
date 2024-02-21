#include <GyverStepper.h>
#include "GyverButton.h"

GButton stopDownSensor(2);
GButton slowDownSpeed(3);

GButton slowUpSpeed(4);
GButton stopUpSensor(5);

GButton hand(6);

GButton moveUp(7);
GButton moveDown(8);
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

void setup() {
  Serial.begin(9600);

  stepper.setRunMode(KEEP_SPEED);
  stepper.setMaxSpeed(1500);

  // buttonSensor.setType(LOW_PULL);
}

void loop() {

  stopDownSensor.tick();
  slowDownSpeed.tick();

  slowUpSpeed.tick();
  stopUpSensor.tick();

  hand.tick();

  moveUp.tick();
  moveDown.tick();

  stepper.tick();

  motionDetected = !digitalRead(12);

  if (hand.isPress()) {
    Serial.println("hand sunul ruku STOP"); // проверка на руку
    stepper.brake();
    flag = false;
  }

  // if (moveUp.isPress()) {
  //   Serial.println("moveUp"); // движение фартука вверх
  //   directionDown = false;

  //   if (flag) {
  //     stepper.stop();
  //     flag = false;
  //   } else {
  //     stepper.setSpeed(speed);
  //     flag = true;
  //   }

  //   if (stopUpSensor.isHold()) {
  //     stepper.brake();
  //     flag = false;
  //     Serial.println("doehal vverh");
  //   }   
  // }

  // if (moveDown.isPress()) { // движение фартука вниз
  //   Serial.println("moveDown");
  //   directionUp = false;
    
  //   if (flag) {
  //     stepper.stop();
  //     flag = false;
  //   } else {
  //     stepper.setSpeed(-speed);
  //     flag = true;
  //   };

  //   if (stopDownSensor.isHold()) {
  //     stepper.brake();
  //     flag = false;
  //     Serial.println("doehal vniz");
  //   }
  // }

  if (stopDownSensor.isPress()) {// остановка фартука внизу ДАТЧИК - 0
    stepper.brake();
    flag = false;
    Serial.println("doehal vniz");
    statePositionDown = true;
    statePositionUp = false;
  }

  if (slowDownSpeed.isPress()) { // замедление скорости внизу ДАТЧИК - 1  
    stepper.setSpeed(-200);
    Serial.println("slowDownSpeed");
  }  

  if (slowUpSpeed.isPress()) { // замедление скорости вверху ДАТЧИК - 2
    stepper.setSpeed(200);
    Serial.println("slowUpSpeed");
  }

  if (stopUpSensor.isPress()) { // остановка фартука вверху ДАТЧИК - 3
    stepper.brake();
    flag = false;
    Serial.println("doehal vverh");
    statePositionUp = true;
    statePositionDown = false;
  }

  if (motionDetected) {
    Serial.println("moveUp"); // движение фартука вверх по сенсору

    if (!firstMotionDetected) {
      stepper.setSpeed(speed);
      firstMotionDetected = true;
    }

    if (firstMotionDetected && statePositionDown) {
      stepper.setSpeed(speed);
    }

    if (firstMotionDetected && statePositionUp) {
      stepper.setSpeed(-speed);
    }

    if (stopUpSensor.isHold()) {
      stepper.brake();
      flag = false;
      Serial.println("doehal vverh");
      statePositionUp = true;
      statePositionDown = false;
    }

    if (stopUpSensor.isHold() && motionDetected) {
      stepper.setSpeed(-speed);
    }

    if (stopDownSensor.isHold()) {
      stepper.brake();
      flag = false;
      Serial.println("doehal vniz");
      statePositionDown = true;
      statePositionUp = false;
    }

    if (stopDownSensor.isHold() && motionDetected) {
      stepper.setSpeed(speed);
    }
  }
}
