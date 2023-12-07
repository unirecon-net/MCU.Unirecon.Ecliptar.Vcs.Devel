#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "System/analog.h"
#include "System/Data/ring.h"
#include "System/hash.h"
#include "System/print.h"
#include "System/pwm.h"
#include "System/scan.h"
#include "System/serial.h"
#include "System/time.h"

template<typename T>
T map(T y, T y_min, T y_max, T x_min, T x_max)
{
	return (y-y_min)/(y_max-y_min)*(x_max-x_min)+x_min;
}

template<typename T>
T* New()
{
	__disable_irq();
	T* x = (T*)calloc(1, sizeof(T));
	__enable_irq();
	return x;
}

template<typename T>
T* NewArray(u32 qty)
{
	__disable_irq();
	T* x = (T*)calloc(qty, sizeof(T));
	__enable_irq();
	return x;
}

#endif /* _SYSTEM_H */
