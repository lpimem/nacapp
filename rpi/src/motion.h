#include <wiringPi.h>
#include "pca9685/Adafruit_PWMServoDriver.h"

//i2c address of PCA9685
#define PCA_ADDRESS 0x40

//Directions -f not correct
//experiment with them a little bit
//example +5/-5 - careful not to drive
//servo to the extreme
#define LEFT 345
#define RIGHT 545
#define HOME 420

class Motion {

public:
  Motion();

  void setSpeed( int );
  int getSpeed() const;
  void forward();
  void stop();
  //just turn the front wheel right
  void steerLeft();
  //includes the forward motion required
  //to turn
  void turnLeft();
  void steerRight();
  void turnRight();
  void steerHome();

  //take distance in mm, int
  void fwdDist( int );

private:
  void setup();

private:
  Adafruit_PWMServoDriver m_PWM;
  //Should all these be declared static
  //Raspberry Pi pins connected to L298N driver
  int m_Motor0_A = 0;  //11 in GPIO python, do gpio readall
  int m_Motor0_B = 1;  //12
  int m_Motor1_A = 2;  //13
  int m_Motor1_B = 3;  //15

  // Not any pins on Raspberry Pi, pin numbers that we need to send from RPI to
  // PCA driver via i2C, will generate PWM for L298N
  uint8_t m_EN_M0 = 4;  //servo driver IC CH4, not any pins on the PI
  uint8_t m_EN_M1 = 5;  //servo driver IC CH5

  //For controlling the servo that controls direction
  uint8_t m_dirServo = 0;  //servo driver IC CH0

  int m_speed;
  //int leftPWM = 375;
  //int homePWM = 450;
  //int rightPWM = 575;
};
