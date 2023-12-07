/*
 * ecliptar_drive.c
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */


#include "Drive/motor.h"


MotorDriver::MotorDriver()
{
  Properties.powerLocked = 0;
  Properties.running = 0;
  Properties.powerOn = 0;
  Properties.powerPwmOn = 0;
  Properties.powerLevel = 0;
  Properties.innerPowerLevel = 0;
}
void                    MotorDriver::Attach(PwmTimer* motorPwm)
{
  Attach(motorPwm, MIN_MOTOR_POWER, MAX_MOTOR_POWER, MAX_MOTOR_POWER);
}
void                    MotorDriver::Attach(PwmTimer* motorPwm, int minPower, int maxPower, int period)
{
  Properties.MotorPwm = motorPwm;
  MotorPwm = Properties.MotorPwm;
  Properties.minPowerDuty = minPower;
  Properties.maxPowerDuty = maxPower;

  HAL_TIM_PWM_Start(MotorPwm->timer, MotorPwm->channel);
}

bool                    MotorDriver::Attached()
{
  return  Properties.MotorPwm != NULL && Properties.MotorPwm->timer != NULL ? true : false;
}

void                    MotorDriver::PowerLevel(u32 pwmValue)
{
  if (Properties.powerLevel != pwmValue)
  {
    bool running = pwmValue > 3;
    if (running)
      pwmValue = map<u32>(pwmValue, 910, 2090, Properties.minPowerLevel, Properties.maxPowerLevel);
    else
      pwmValue = 0;

    Properties.running = running;
    Properties.powerLevel = pwmValue;
    innerPowerLevel(pwmValue);
  }
}
inline void            MotorDriver::innerPowerLevel(u32 pwmValue)
{
  if (Properties.innerPowerLevel != pwmValue)
  {
    Properties.innerPowerLevel = pwmValue;
    Properties.powerOn = pwmValue > 0;
    Properties.MotorPwm->SetPulse(pwmValue);
  }
}
void                    MotorDriver::PowerDuty(float dutyValue)
{
  if (Properties.dutyLevel != dutyValue)
  {
    bool running = dutyValue > 3;
	if (running)
	{
		dutyValue = map<f32>(dutyValue, 0.00F, 100.00F, Properties.minPowerDuty,	Properties.maxPowerDuty);
	}
	else
	{
		Properties.powerLevel = 0;
		dutyValue = 0;
	}

    Properties.running = running;
    Properties.dutyLevel = dutyValue;
    innerPowerDuty(dutyValue);
  }
}
inline void            MotorDriver::innerPowerDuty(float dutyValue)
{
  if (Properties.innerDutyLevel != dutyValue)
  {
    Properties.innerDutyLevel = dutyValue;
    Properties.powerOn = dutyValue > 0;
    Properties.innerPowerLevel = Properties.MotorPwm->SetDuty(dutyValue);
  }
}


void                    MotorDriver::revolutionSpeed(uint16_t rpm)
{

}


