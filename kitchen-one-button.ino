#include <GyverStepper.h>
#include "GyverButton.h"

GButton stopDownSensor(2);
GButton slowDownSpeed(3);

GButton slowUpSpeed(4);
GButton stopUpSensor(5);

GStepper<STEPPER2WIRE> stepper(200, 11, 10);

int speed = 400;

bool flag = false;

bool motionDetected = false;
bool firstMotionDetected = false;
bool motorMovingUp = false;
bool motorMovingDown = false;

bool statePositionUp = false;
bool statePositionDown = false;

bool moving = false;

bool handSensor = false;

int direct = 0;

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

  stepper.tick();

  motionDetected = !digitalRead(12);
  handSensor = digitalRead(6);

  if (handSensor) {
    // Serial.println("hand sunul ruku STOP"); // проверка на руку
    stepper.brake();
    // stepper.autoPower(true); // снятие напряжения
    flag = false;
    statePositionDown = true;
    statePositionUp = false;
    direct = 0;
  }

  if (stopDownSensor.isPress()) {// остановка фартука внизу ДАТЧИК - 0
    stepper.brake();
    digitalWrite(7, HIGH); // снятие напряжения без библиотеки
    flag = false;
    // Serial.println("doehal vniz");
    statePositionDown = true;
    statePositionUp = false;
    moving = false;
    digitalWrite(7, HIGH);
    direct = 0;
  }  

  if (stopUpSensor.isPress()) { // остановка фартука вверху ДАТЧИК - 3
    stepper.brake();
    digitalWrite(7, HIGH); // снятие напряжения без библиотеки
    flag = false;
    // Serial.println("doehal vverh");
    statePositionUp = true;
    statePositionDown = false;
    moving = false;
    digitalWrite(7, HIGH);
    direct = 0;
  }

  if (statePositionDown && moving) {
    if (slowUpSpeed.isPress()) { // замедление скорости вверху ДАТЧИК - 2
      stepper.setSpeed(200);
      // Serial.println("slowUpSpeed");
    }
  }

  if (statePositionUp && moving) {
    if (slowDownSpeed.isPress()) { // замедление скорости внизу ДАТЧИК - 1  
      stepper.setSpeed(-200);
      // Serial.println("slowDownSpeed");
    }
  }

  if (motionDetected) {
    direct++;
    delay(500);

    if (direct > 2) {
      direct = 1;
    }
  }

  if (motionDetected && !handSensor) { // если коснулись датчик движения и не сработал датчик движения руки, выполняем разные действия ГЛАВНОЕ УСЛОВИЕ
    // Serial.println("moveUp"); // движение фартука вверх по сенсору вверх/вниз
    digitalWrite(7, LOW);

    speed = 400;

    if (direct == 2) {
      stepper.brake();
      // moving = false;
    }

    if (!firstMotionDetected && direct != 2) { // если это ПЕРВЫЙ СТАРТ то поехали наверх
      stepper.setSpeed(speed);
      firstMotionDetected = true;
      moving = true;
    }

    if (firstMotionDetected && statePositionDown && direct != 2) { // если НЕ ПЕРВЫЙ СТАРТ и фартук низу то поехали ВВЕРХ
      stepper.setSpeed(speed);
      moving = true;
    }

    if (firstMotionDetected && statePositionUp && direct != 2) { // если НЕ ПЕРВЫЙ СТАРТ и фартук вверху то поехали ВНИЗ
      stepper.setSpeed(-speed);
      moving = true;
    }

    if (stopUpSensor.isHold() && !motionDetected) { // если не касаемся датчика движения фартука и нажали датчик ВВЕРХУ то ОСТАНОВИЛИСЬ
      stepper.brake();
      digitalWrite(7, HIGH); // снятие напряжения без библиотеки
      flag = false;
      // Serial.println("doehal vverh");
      statePositionUp = true;
      statePositionDown = false;
      moving = false;
    }

    if (stopDownSensor.isHold() && !motionDetected) { // если не касаемся датчика движения фартука и нажали датчик ВНИЗУ то ОСТАНОВИЛИСЬ
      stepper.brake();
      digitalWrite(7, HIGH); // снятие напряжения без библиотеки
      flag = false;
      // Serial.println("doehal vniz");
      statePositionDown = true;
      statePositionUp = false;
      moving = false;
    }

    if (stopUpSensor.isHold() && motionDetected) { // если нажат и удерживается датчик вверху и коснулись датчик движения фартука то ПОЕХАЛ ВНИЗ
      stepper.setSpeed(-speed);
    }

    if (stopDownSensor.isHold() && motionDetected) { // если нажат нажний датчик и удерживается и коснулись датчика движения фартука, то ПОЕХАЛ ВВЕРХ
      stepper.setSpeed(speed);
    }
  }
}