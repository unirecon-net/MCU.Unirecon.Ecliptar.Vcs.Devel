/*
 * Drive/ecliptor.cpp
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#include "Drive/ecliptor.h"

EcliptorDriver::EcliptorDriver()
{
	riseLowThold = MIN_HALLSENSOR_THRESHOLD_VALUE;
	riseHighThold = MAX_HALLSENSOR_THRESHOLD_VALUE;
	fallLowThold = MIN_HALLSENSOR_THRESHOLD_VALUE;
	fallHighThold = MAX_HALLSENSOR_THRESHOLD_VALUE;
	Rotator.pulseWidthMin = MIN_SERVO_SIGNAL_US;
	Rotator.pulseWidthMax = MAX_SERVO_SIGNAL_US;
	Rotator.pulseWidthMid = (uint16_t) ((float) (MAX_SERVO_SIGNAL_US
			- MIN_SERVO_SIGNAL_US) / 2) + MIN_SERVO_SIGNAL_US;
	Rotator.pulseDeltaMax = (Rotator.pulseWidthMid - MIN_SERVO_SIGNAL_US) / 2;
	Rotator.pulsePeriod = SERVO_TIMER_COUNTER;
	Rotator.rotationAngleRange = SERVO_ANGLE_RANGE_DEG;
	Rotator.rotationAngleMin = (SERVO_ANGLE_RANGE_DEG / 2) * -1;
	Rotator.rotationAngleMax = (SERVO_ANGLE_RANGE_DEG / 2);
	Rotator.trimByDecimalPart = 0;
	Rotator.direction = normal;
	Rotator.rotationPercents = -2;
	Rotator.rotationAngle = -180;
	Eccentrator.overThold = false;
	Eccentrator.running = false;
	Eccentrator.powerOn = false;
	Eccentrator.powerPwmOn = false;
	Eccentrator.powerLevel = 0;
	Eccentrator.innerPowerLevel = 0;
}

void EcliptorDriver::Attach(EcliptorSide side, PwmTimer *powerPwm,
		PwmTimer *rotatorPwm, AdcData *hallsensor)
{
	this->Attach(side, powerPwm, rotatorPwm, hallsensor,
	MIN_HALLSENSOR_THRESHOLD_VALUE, MAX_HALLSENSOR_THRESHOLD_VALUE,
	MIN_HALLSENSOR_THRESHOLD_VALUE, MAX_HALLSENSOR_THRESHOLD_VALUE);
}
void EcliptorDriver::Attach(EcliptorSide side, PwmTimer *powerPwm,
		PwmTimer *rotatorPwm, AdcData *hallsensor, int riseLowThold,
		int riseHighThold)
{
	this->Attach(side, powerPwm, rotatorPwm, hallsensor, riseLowThold,
			riseHighThold,
			MIN_HALLSENSOR_THRESHOLD_VALUE, MAX_HALLSENSOR_THRESHOLD_VALUE);
}
void EcliptorDriver::Attach(EcliptorSide side, PwmTimer *powerPwm,
		PwmTimer *rotatorPwm, AdcData *hallsensor, int riseLowThold,
		int riseHighThold, int fallLowThold, int fallHighThold)
{
	this->Attach(side, powerPwm, rotatorPwm, hallsensor, riseLowThold,
			riseHighThold, fallLowThold, fallHighThold, MIN_ECCNTR_POWER,
			MAX_ECCNTR_POWER, MAX_ECCNTR_POWER);
}
void EcliptorDriver::Attach(EcliptorSide side, PwmTimer *powerPwm,
		PwmTimer *rotatorPwm, AdcData *hallsensor, int riseLowThold,
		int riseHighThold, int fallLowThold, int fallHighThold, int minPower,
		int maxPower, int period)
{
	Side = side;
	Converter = hallsensor;
	AnalogConverters->Add((AnalogConverter*) this);

	Rotator.RotatorPwm = rotatorPwm;
	Eccentrator.PowerPwm = powerPwm;
	PowerPwm = Eccentrator.PowerPwm;
	RotatorPwm = Rotator.RotatorPwm;

	this->riseLowThold = riseLowThold;
	this->riseHighThold = riseHighThold;
	this->fallLowThold = fallLowThold;
	this->fallHighThold = fallHighThold;
	curLowThold = riseLowThold;
	curHighThold = riseHighThold;

	Eccentrator.minPowerDuty = minPower;
	Eccentrator.maxPowerDuty = maxPower;

	HAL_TIM_PWM_Start(PowerPwm->timer, PowerPwm->channel);
	HAL_TIM_PWM_Start(RotatorPwm->timer, RotatorPwm->channel);
	HAL_ADC_Start_DMA(Converter->adc,
			(uint32_t*) Converter->conversions->Data.buffer,
			Converter->conversionsLength);
}

void EcliptorDriver::Detach()
{

}
bool EcliptorDriver::Attached()
{
	if (Rotator.RotatorPwm != NULL && Rotator.RotatorPwm->timer != NULL
			&& Eccentrator.PowerPwm != NULL
			&& Eccentrator.PowerPwm->timer != NULL && Converter != NULL
			&& Converter->adc != NULL)
		return true;
	return false;
}

void EcliptorDriver::DefineAreas(int lowHrz, int lowDgl, int lowVrt,
		int highHrz, int riseDgl, int highVrt, int fallDgl)
{
	AreaValues *pos = &(Position.DefinedAreas);
	pos->lowVertical = lowVrt;
	pos->lowDiagonal = lowDgl;
	pos->lowHorizontal = lowHrz;
	pos->highHorizontal = highHrz;
	pos->highVertical = highVrt;
	pos->riseDiagonal = riseDgl;
	pos->fallDiagonal = fallDgl;

	uint16_t highDelta = highVrt - highHrz;
	uint16_t lowDelta = lowVrt - lowHrz;

	for (int i = 1; i < 158; i++)
	{
		uint16_t max = ((highDelta * i) / 157) + highHrz;
		uint16_t min = ((lowDelta * i) / 157) + lowHrz;
		Position.AngularPositions[i - 1].SetRange(min, max);
		Dynamics.riseVectors[i - 1] = &Position.AngularPositions[i - 1];
	}
	int x = 0;
	for (int i = 313; i > 156; i--)
	{
		uint16_t max = Position.AngularPositions[i - 157].sensorMax;
		uint16_t min = Position.AngularPositions[i - 157].sensorMin;
		Position.AngularPositions[x + 157].SetRange(min, max);
		Dynamics.fallVectors[x] = &Position.AngularPositions[x + 157];
		x++;
	}
	x = 157;
	for (int i = 314; i < 471; i++)
	{
		uint16_t max = Position.AngularPositions[i - 314].sensorMax;
		uint16_t min = Position.AngularPositions[i - 314].sensorMin;
		Position.AngularPositions[i].SetRange(min, max);
		Dynamics.riseVectors[x] = &Position.AngularPositions[i];
		x++;
	}
	x = 157;
	for (int i = 471; i < 628; i++)
	{
		uint16_t max = Position.AngularPositions[i - 314].sensorMax;
		uint16_t min = Position.AngularPositions[i - 314].sensorMin;
		Position.AngularPositions[i].SetRange(min, max);
		Dynamics.fallVectors[x] = &Position.AngularPositions[i];
		x++;
	}
}
void EcliptorDriver::DefineRotator(uint16_t minWidth, uint16_t maxWidth,
		uint16_t midWidth, uint16_t period, float angleRange,
		RotatorDirection direction)
{
	Rotator.rotationAngleMin = (angleRange / 2) * -1;
	Rotator.rotationAngleMax = (angleRange / 2);

	uint16_t center = ((maxWidth - minWidth) / 2) + minWidth;
	uint16_t subTrim = (midWidth - center);
	uint16_t stdDeviation = ((maxWidth - minWidth) / 2);

	if (subTrim != 0)
	{
		if (subTrim < 0)
			subTrim *= -1;

		Rotator.trimByDecimalPart = (float) subTrim / (float) (stdDeviation);
		float trimAngle = Rotator.rotationAngleMin * Rotator.trimByDecimalPart;
		Rotator.rotationAngleMin -= trimAngle;
		Rotator.rotationAngleMax += trimAngle;

		stdDeviation = stdDeviation
				- (uint16_t) ((float) stdDeviation * Rotator.trimByDecimalPart);
	}

	Rotator.pulseDeltaMax = stdDeviation;

	Rotator.pulseWidthMin = minWidth;
	Rotator.pulseWidthMax = maxWidth;
	Rotator.pulseWidthMid = midWidth;

	Rotator.pulsePeriod = period;
	Rotator.rotationAngleRange = Rotator.rotationAngleMax
			- Rotator.rotationAngleMin;
	Rotator.direction = direction;

}

void EcliptorDriver::RotateAngle(float angle)
{
	angle *= Rotator.direction;
	if (angle < Rotator.rotationAngleMin)
		angle = Rotator.rotationAngleMin;
	else if (angle > Rotator.rotationAngleMax)
		angle = Rotator.rotationAngleMax;
	if (Rotator.rotationAngle != angle)
	{
		Rotator.rotationAngle = angle;
		Rotator.pulseWidth = Rotator.angleToMicroseconds(angle);
		Rotator.RotatorPwm->SetPulse(Rotator.pulseWidth);
	}
}
void EcliptorDriver::RotateRadians(float radians)
{
	RotateAngle(Rotator.radiansToAngle(radians));
}
void EcliptorDriver::RotatePercents(float percents)
{
	percents *= Rotator.direction;
	if (Rotator.rotationPercents != percents)
	{
		Rotator.rotationPercents = percents;
		Rotator.pulseWidth = Rotator.percentsToMicroseconds(percents);
		Rotator.RotatorPwm->SetPulse(Rotator.pulseWidth);
	}
}

bool EcliptorDriver::tholdPowerDuty()
{
	if (Eccentrator.powerOn)
	{
		innerPowerDuty(0);
	}
	return false;
}

bool EcliptorDriver::PowerLevel(uint32_t pwmValue)
{
	if (Eccentrator.powerLevel != pwmValue)
	{
		uint8_t running = pwmValue > 4;

		if (running)
			pwmValue = map<u32>(pwmValue, 910, 2090, Eccentrator.minPowerLevel,
					Eccentrator.maxPowerLevel);
		else
			pwmValue = 0;

		Eccentrator.running = running;
		Eccentrator.powerLevel = pwmValue;

		innerPowerLevel(pwmValue);

		return true;
	}
	return false;
}
bool EcliptorDriver::PowerDuty(float dutyValue)
{
	if (Eccentrator.dutyLevel != dutyValue)
	{
		uint8_t running = dutyValue > 4;
		if (running && !Eccentrator.overThold)
		{
			dutyValue = map<f32>(dutyValue, 0.00F, 100.00F,
					Eccentrator.minPowerDuty, Eccentrator.maxPowerDuty);
		}
		else
		{
			Eccentrator.powerLevel = 0;
			dutyValue = 0;
		}
		Eccentrator.running = running;
		Eccentrator.dutyLevel = dutyValue;

		innerPowerDuty(dutyValue);

		return true;
	}
	return false;
}
inline void EcliptorDriver::innerPowerLevel(uint16_t pwmValue)
{
	if (Eccentrator.innerPowerLevel != pwmValue)
	{
		Eccentrator.innerPowerLevel = pwmValue;
		Eccentrator.powerOn = pwmValue > 0;
		Eccentrator.PowerPwm->SetPulse(pwmValue);
	}
}
inline void EcliptorDriver::innerPowerDuty(float dutyValue)
{
	if (Eccentrator.innerDutyLevel != dutyValue)
	{
		Eccentrator.innerDutyLevel = dutyValue;
		Eccentrator.powerOn = dutyValue > 0;
		Eccentrator.innerPowerLevel = Eccentrator.PowerPwm->SetDuty(dutyValue);
	}
}

DynamicsData* EcliptorDriver::readSensorDynamics()
{
	if (newDynamicsData)
	{
		newDynamicsData = false;
		return &Dynamics;

	}
	return NULL;
}
MeasureData* EcliptorDriver::readSensorMeasure()
{
	if (newMeasureData)
	{
		newMeasureData = false;
		return &Measure;
	}
	return NULL;
}
PositionData* EcliptorDriver::readSensorPosition()
{
	if (newPositionData)
	{
		newPositionData = false;
		return &Position;
	}
	return NULL;
}
MotionData* EcliptorDriver::readEclipticMotion()
{
	if (newMotionData)
	{
		newMotionData = false;
		return &Motion;
	}
	return NULL;
}

EcliptorArea EcliptorDriver::OperativeComputeArea()
{
	return computeArea(Measure.operative.Value, Measure.operative.Delta);
}

inline EcliptorArea EcliptorDriver::computeArea(int16_t sensorValue,
		int16_t sensorDelta)
{
	EcliptorArea lastposition = Position.currentArea;
	uint8_t positionChanged = 0;

	if (!Position.secondQuarter && sensorDelta < -2
			&& (lastposition == HighVertical || lastposition == LowVertical))
	{
		Position.secondQuarter = 1;
		curLowThold = fallLowThold;
		curHighThold = fallHighThold;
		positionChanged = 1;
	}
	else if (Position.secondQuarter
			&& (lastposition == HighVertical || lastposition == LowVertical))
	{
		Position.secondQuarter = 0;
		curLowThold = riseLowThold;
		curHighThold = riseHighThold;
		positionChanged = 1;
	}

	EcliptorArea position = establishArea(sensorValue);

	if (!positionChanged)
		positionChanged = lastposition != position;

	if (positionChanged)
	{
		Position.currentArea = position;
		newPositionData = true;

		switch (position)
		{
		case FallDiagonal:
		{
			if (!Dynamics.fallComputing)
			{
				Dynamics.fallCollecting = true;
			}
			return position;
		}
		case RiseDiagonal:
		{
			if (!Dynamics.riseComputing)
			{
				Dynamics.riseCollecting = true;
			}
			return position;
		}
		default:
			break;
		}

		switch (lastposition)
		{
		case FallDiagonal:
		{
			if (!Dynamics.riseComputing)
			{
				Dynamics.fallCollecting = false;
			}
			return position;
		}
		case RiseDiagonal:
		{
			if (!Dynamics.riseComputing)
			{
				Dynamics.riseCollecting = false;
			}
			return position;
		}
		case LowHorizontal:
		{
			if (Position.secondHalf)
				Position.secondHalf = false;
			else
				Position.secondHalf = true;

			if (!Position.newHalf)
			{
				Position.newHalf = true;
			}

			return position;
		}
		case HighHorizontal:
			return position;
		case HighVertical:
			return position;
		case LowVertical:
			return position;
		case LowDiagonal:
			return position;
		case NoneArea:
			return position;
		}

		return position;
	}
	return NoneArea;
}
inline EcliptorArea EcliptorDriver::establishArea(int16_t sensorValue)
{
	AreaValues *pos = &Position.DefinedAreas;

	if (sensorValue > pos->lowHorizontal)
	{
		if (sensorValue > pos->lowDiagonal)
		{
			if (sensorValue > pos->lowVertical)
			{
				if (sensorValue < pos->highVertical)
				{
					if (!Position.secondQuarter)
					{
						if (sensorValue < pos->riseDiagonal)
							return RiseDiagonal;
					}
					else if (sensorValue < pos->fallDiagonal)
						return FallDiagonal;

					return HighVertical;
				}
				return LowVertical;
			}
			return LowVertical;
		}
		return LowHorizontal;
	}
	return LowHorizontal;
}

void EcliptorDriver::computeMotionDelta(int16_t delta)
{
	if (Position.secondQuarter)
		computeFallDelta(delta);
	else
		computeRiseDelta(delta);
}
void EcliptorDriver::computeRiseDelta(int16_t delta)
{
	Dynamics.riseDelta += delta;
	Dynamics.riseCounter++;

	if (!Dynamics.riseCollecting && Dynamics.riseCounter > 0)
	{
		Dynamics.riseComputing = true;
	}
}
void EcliptorDriver::computeFallDelta(int16_t delta)
{
	Dynamics.fallDelta += delta;
	Dynamics.fallCounter++;

	if (!Dynamics.fallCollecting && Dynamics.fallCounter > 0)
	{
		Dynamics.fallComputing = true;
	}
}

void EcliptorDriver::computeMotion()
{
	if (Position.newHalf)
	{
		computeAngularVelocity();
	}
	if (Dynamics.fallComputing)
	{
		computeFall();
	}
	if (Dynamics.riseComputing)
	{
		computeRise();
	}
}
void EcliptorDriver::computeRise()
{
	float deltasum = (float) Dynamics.riseDelta;
	float period_us = Dynamics.risePeriodMicros;
	float counter = (float) Dynamics.riseCounter;
	float velocity = deltasum / period_us;
	float accelleration = (velocity / counter) / (period_us / counter);
	Dynamics.riseDelta = 0;
	Dynamics.riseCounter = 0;
	Motion.riseVelocity = velocity;
	Motion.riseAcceleration = accelleration;
	Dynamics.riseComputing = false;
	newDynamicsData = true;
}
void EcliptorDriver::computeFall()
{
	float deltasum = (float) Dynamics.fallDelta;
	float period_us = Dynamics.fallPeriodMicros;
	float counter = (float) Dynamics.fallCounter;
	float velocity = deltasum / period_us;
	float accelleration = (velocity / counter) / (period_us / counter);
	Dynamics.fallDelta = 0;
	Dynamics.fallCounter = 0;
	Motion.fallVelocity = velocity;
	Motion.fallAcceleration = accelleration;
	Dynamics.fallComputing = false;
	newDynamicsData = true;
}
void EcliptorDriver::computeAngularVelocity()
{
/*	float angularVelocity = M_PI / period_s;
	Motion.angularAcceleration = (angularVelocity - Motion.angularVelocity)
			/ period_s;
	Motion.angularVelocity = angularVelocity;*/
	Position.newHalf = false;
}

inline void EcliptorDriver::OperativeMeasurements(uint16_t *buffer, uint16_t significantValueId)
{
	OperativeData *operative = &Measure.operative;

	int32_t accell = (buffer[4] - buffer[2]) - (buffer[2] - buffer[0]);
	operative->AccelerationDelta = accell - operative->Acceleration;
	operative->Acceleration = accell;

	if (significantValueId > 1)
	{
		operative->Delta = buffer[significantValueId] - operative->Value;

		ComputativeData *cd = &Measure.computative;
		operative->Counter = cd->Counter++;

		if(!cd->Operatives->Available())
			cd->Interval = operative->Counter;

		operative->Interval = cd->Interval;

		cd->Operatives->Write(operative, sizeof(OperativeData));

		newMeasureData = true;
	}
	else
		operative->Value = buffer[significantValueId];
}

inline void EcliptorDriver::ConversionComplete(Ring *buffer, u16 size)
{
	OperativeMeasurements((uint16_t*) buffer->Read(size), 3);
}

inline void EcliptorDriver::ConversionHalfComplete(Ring *buffer, u16 size)
{
	OperativeMeasurements((uint16_t*) buffer->Read(size), 1);
}
