/***************************************************
  This is a library for our Adafruit 16-channel PWM & Servo driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_PWMServoDriver.h"

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>  //for usleep
#include <iostream>
#include <math.h>
#include <stdio.h>

// Set to true to print some debug messages, or false to disable them.
#define ENABLE_DEBUG_OUTPUT false

using namespace std;

Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(uint8_t addr) {
  _i2caddr = addr;
  //setup wiring pi pin layout
  wiringPiSetup();
  fd = wiringPiI2CSetup(0x40);
  if (fd < 0) {
       cout << "Error setting up I2C: n" << endl;
       exit(0);
  }
  reset();
}

//void Adafruit_PWMServoDriver::begin(void) {
//  reset();
//}
//
void Adafruit_PWMServoDriver::reset(void) {
  setAllPWM(0, 0);
  wiringPiI2CWriteReg8(fd, PCA9685_MODE2, OUTDRV);
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, ALLCALL);
  //sleep for 5 ms
  usleep(5000);

  int mode1 = wiringPiI2CReadReg8(fd, PCA9685_MODE1);
  mode1 = mode1 & ~SLEEP;
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, mode1);
  usleep(5000);
}

void Adafruit_PWMServoDriver::setPWMFreq(float freq) {
  //freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;

  uint8_t oldmode = wiringPiI2CReadReg8(fd, PCA9685_MODE1);
  uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep

  uint8_t prescale = floor(prescaleval + 0.5);

  cout << "hello" << int(floor(prescale)) << endl;

  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, newmode); // go to sleep
  wiringPiI2CWriteReg8(fd, PCA9685_PRESCALE, int(floor(prescale)) ); // set the prescaler
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, oldmode);

  //delay(5);
  usleep(5000);
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, oldmode | 0x80);  //  This sets the MODE1 register to turn on auto increment.
                                          // This is why the beginTransmission below was not working.
}

void Adafruit_PWMServoDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
  wiringPiI2CWriteReg8(fd, LED0_ON_L + 4*num, on & 0xFF);
  wiringPiI2CWriteReg8(fd, LED0_ON_H + 4*num, on >> 8);
  wiringPiI2CWriteReg8(fd, LED0_OFF_L + 4*num, off & 0xFF);
  wiringPiI2CWriteReg8(fd, LED0_OFF_H + 4*num, off >> 8);
}

void Adafruit_PWMServoDriver::setAllPWM(uint16_t on, uint16_t off) {
  wiringPiI2CWriteReg8(fd, ALLLED_ON_L, on & 0xFF);
  wiringPiI2CWriteReg8(fd, ALLLED_ON_H, on >> 8);
  wiringPiI2CWriteReg8(fd, ALLLED_OFF_L, off & 0xFF);
  wiringPiI2CWriteReg8(fd, ALLLED_OFF_H, off >> 8);
}
