/*
 * EcliptarTools.cpp
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */
#include "System/time.h"
#include "COnfig/tim.h"

#define SYSTICK_TIMER 1

inline uint32_t tickms(void)
{
	return uwTick;
}

inline uint64_t tickus(void)
{

#ifdef SYSTICK_TIMER
	return (htim12.Instance->CNT * 0.1) + (uwTick * 1000);
#else
	return (HAL_GetTick() * 1000) + (1000 - (SysTick->VAL / 180)); // @suppress("Field cannot be resolved")
#endif
}

inline double tick(void)
{
#ifdef SYSTICK_TIMER
	return ((htim12.Instance->CNT) + (uwTick * 10000)) * 0.1f;
#else
	return (HAL_GetTick() * 180000) + (180000 - SysTick->VAL); // @suppress("Field cannot be resolved")
#endif
}




