/*
 * ecliptar_drive.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef __DRIVE_MOTOR_H
#define __DRIVE_MOTOR_H

#include <inttypes.h>

#include "Drive/base.h"

#define MIN_MOTOR_POWER                 2999
#define MAX_MOTOR_POWER                 7999
#define MIN_MOTOR_DUTY                  40
#define MAX_MOTOR_DUTY                  100

typedef struct
{
  PwmTimer*		  		    MotorPwm;
  volatile u32                powerLevel;
  volatile float              dutyLevel;
  volatile u32                innerPowerLevel;
  volatile float              innerDutyLevel;
  volatile int                maxPowerLevel;
  volatile int                minPowerLevel;
  volatile float              maxPowerDuty;
  volatile float              minPowerDuty;
  volatile uint16_t          revolutionSpeed;
  volatile uint8_t           running : 1;
  volatile uint8_t           started : 1;
  volatile uint8_t           powerOn : 1;
  volatile uint8_t           powerPwmOn : 1;
  volatile uint8_t           powerLocked : 1;
} MotorProperties;

class MotorDriver
{
public:
  MotorDriver();

  void                    Attach(PwmTimer* motorPwm);
  void                    Attach(PwmTimer* motorPwm, int minPower, int maxPower, int period);

  bool                    Attached();

  void                    PowerLevel(u32 pwmValue);
  void                    PowerDuty(float dutyValue);

  void                    revolutionSpeed(uint16_t rpm);

  MotorProperties        Properties;

  PwmTimer*               MotorPwm = NULL;

private:

  void                    innerPowerLevel(u32 pwmValue);
  void                    innerPowerDuty(float dutyValue);

};

#endif /* __DRIVE_MOTOR_H */
