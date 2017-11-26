#include <Arduino.h>

#pragma once
#ifndef PWM_H
#define PWM_H

#define FAN_PIN 14

class Driver
{
private:
	static unsigned sourcePercentage;
	static bool isUsed;
	unsigned percentage;
public:
	Driver() 
	{
		percentage = 0;
		pinMode(FAN_PIN, OUTPUT);
	}

	static bool SetPercentage(unsigned param)
	{
		if (!isUsed)
			return false;
		isUsed = true;
		if ((0 <= param) && (param <= 100))
		{
			sourcePercentage = param;
			isUsed = false;
			return true;
		}else{
			isUsed = false;
			return false;
		}
	}

	void loop()
	{
		Serial.println("PWM loop");
		percentage = sourcePercentage;
	}
};

#endif // !PWM_H
