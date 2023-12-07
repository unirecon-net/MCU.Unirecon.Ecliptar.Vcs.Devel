
#include "System/analog.h"

AdcData::AdcData(ADC_HandleTypeDef *adc, uint32_t channel)
{
	Init(adc, channel, 2);
}
AdcData::AdcData(ADC_HandleTypeDef *adc, uint32_t channel, size_t dataSize)
{
	Init(adc, channel, dataSize);
}
AdcData::AdcData(ADC_HandleTypeDef *adc, uint8_t channelsCount, uint32_t channels[])
{
	Init(adc, channelsCount, channels, channelsCount * 2);
}
AdcData::AdcData(ADC_HandleTypeDef *adc, uint8_t channelsCount, uint32_t channels[], size_t dataSize)
{
	Init(adc, channelsCount, channels, dataSize);
}

void AdcData::Init(ADC_HandleTypeDef *adc, uint32_t channel, size_t dataSize)
{
	uint32_t arr[1] { channel };
	Init(adc, 1, arr, dataSize);
}
void AdcData::Init(ADC_HandleTypeDef *adc, uint8_t channelsCount, uint32_t channels[], size_t dataSize)
{
	this->adc = adc;
	this->dma = adc->DMA_Handle;
	this->channelsCount = channelsCount;
	this->channels = channels;
	conversionsLength = dataSize;
	conversions = new Ring(dataSize * 2);
}

void 				AnalogConverterCollection::Add(AnalogConverter *converter)
{
	int id = hashtableId(converter->ConverterAddress(), 17);
	if (AnalogConverters->Table[id] == NULL)
		AnalogConverters->Table[id] = converter;
	else
	{
		for (int i = 0; i < AnalogConverters->Count; i++)
		{
			if (AnalogConverters->Table[i] == NULL)
				AnalogConverters->Table[i] = converter;
		}
	}
	AnalogConverters->Count++;
}
AnalogConverter* AnalogConverterCollection::Get(uint32_t converterAddress)
{
	int id = hashtableId(converterAddress, 17);
	AnalogConverter *result = AnalogConverters->Table[id];

	if (result->ConverterAddress() == converterAddress)
		return result;

	for (int i = 0; i < AnalogConverters->Count; i++)
	{
		result = AnalogConverters->Table[i];
		if (result->ConverterAddress() == converterAddress)
			return result;
	}

	return NULL;
}

uint32_t AnalogConverterCollection::Remove(uint32_t converterAddress)
{
	int id = hashtableId(converterAddress, 17);
	AnalogConverter *result = AnalogConverters->Table[id];

	if (result->ConverterAddress() == converterAddress)
	{
		AnalogConverters->Table[id] = NULL;
		return 1;
	}
	for (int i = 0; i < AnalogConverters->Count; i++)
	{
		result = AnalogConverters->Table[i];
		if (result->ConverterAddress() == converterAddress)
		{
			AnalogConverters->Table[i] = NULL;
			return 1;
		}
	}
	return 0;
}

AnalogConverterCollection* AnalogConverters = new AnalogConverterCollection();

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	AnalogConverter *ac = AnalogConverters->Get((uint32_t) hadc);
	AdcData *conv = ac->Converter;
	u16 len = conv->conversionsLength;
	conv->conversions->Step(len);
	ac->ConversionComplete(conv->conversions, len);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	AnalogConverter *ac = AnalogConverters->Get((uint32_t) hadc);
	AdcData *conv = ac->Converter;
	u16 len = conv->conversionsLength;
	conv->conversions->Step(len);
	ac->ConversionHalfComplete(conv->conversions, len);
}

