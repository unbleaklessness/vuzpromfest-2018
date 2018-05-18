/**
 * Пасажиры.
 */

#include <Wire.h>
#include <I2CEncoder.h>

#define RANGE 352

I2CEncoder encoderLeft, encoderRight;

const int motorM1PinOne = 46;
const int motorM1PinTwo = 45;
const int motorM2PinOne = 6;
const int motorM2PinTwo = 7;

int motor1SetReversed = true;
int motor2SetReversed = false;

int motorSpeed = 150;

#define PI 3.1416

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
  encoderLeft.init(MOTOR_393_TORQUE_ROTATIONS, MOTOR_393_TIME_DELTA);
  encoderLeft.setReversed(true);
  encoderLeft.unTerminate();
  encoderRight.init(MOTOR_393_TORQUE_ROTATIONS, MOTOR_393_TIME_DELTA);
  
  pinMode(motorM1PinOne, OUTPUT);
  pinMode(motorM1PinTwo, OUTPUT);
  pinMode(motorM2PinOne, OUTPUT);
  pinMode(motorM2PinTwo, OUTPUT);
  
  delay(5000);
  
  start();
}

void start()
{
  forward(RANGE);
  delay(1500);
  
  backward(RANGE);
  delay(1500);
}

void loop()
{
  //
}

void forward(float l)
{
  double oldPosLeft = encoderLeft.getPosition();
  double oldPosRight = encoderLeft.getPosition();
  
  float u = 0.0;
  
  powerMotor1(motorSpeed);
  powerMotor2(motorSpeed);
  
  while (abs(oldPosLeft - encoderLeft.getPosition()) < 2.0 * 1 / (PI * 10.3))
  {
    // Добавить скорости, если мотор отстал.
    u = kp * ((oldPosLeft - encoderLeft.getPosition()) - (oldPosRight - encoderRight.getPosition()));
    powerMotor2(motorSpeed - u);
    powerMotor1(motorSpeed + u);
  }
}

void backward(float l)
{
  double oldPosLeft = encoderLeft.getPosition();
  double oldPosRight = encoderRight.getPosition();
  
  float u = 0.0;
  
  powerMotor1(-motorSpeed);
  powerMotor2(-motorSpeed);
  
  while (abs(oldPosLeft - encoderLeft.getPosition()) < 2.0 * 1 / (PI / 10.3))
  {
    u = kp * ((oldPosLeft - encoderLeft.getPosition()) - (oldPosRight - encoderRight.getPosition()))
    powerMotor2(-(motorSpeed + u));
    powerMotor1(-(motorSpeed - u));
  }
}

void powerMotor1(int speedValue)
{
  int direction = motor1SetReversed ? -1 : 1;
  speedValue = constrain(speedValue, -254, 254);
  
  if (speedValue == 0)
  {
    digitalWrite(motorM1PinOne, LOW);
    digitalWrite(motorM1PinTwo, LOW);
  }
  if (direction * speedValue > 0)
  {
    digitalWrite(motorM1PinOne, LOW);
    analogWrite(motorM1PinTwo, abs(speedValue));
  }
  if (direction * speedValue < 0)
  {
    analogWrite(motorM1PinOne, abs(speedValue));
    digitalWrite(motorM1PinTwo, LOW);
  }
}

void powerMotor2(int speedValue)
{
  int direction = motor2SetReversed ? -1 : 1;
  speedValue = constrain(speedValue, -254, 254);
  
  if (speedValue == 0)
  {
    digitalWrite(motorM2PinOne, LOW);
    digitalWrite(motorM2PinTwo, LOW);
  }
  if (direction * speedValue > 0)
  {
    digitalWrite(motorM2PinOne, LOW);
    analogWrite(motorM2PinTwo, abs(speedValue));
  }
  if (direction * speedValue < 0)
  {
    analogWrite(motorM2PinOne, abs(speedValue));
    digitalWrite(motorM2PinTwo, LOW);
  }
}