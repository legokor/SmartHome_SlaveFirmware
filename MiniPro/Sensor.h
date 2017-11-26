#include "DHT.h"
#include "MQ2.h"

#pragma once
#ifndef SENSOR_H
#define SENSOR_H

#define DHTPIN 5     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define BUTTONPIN 12
class Sensor
{
private:
	MQ2 mq2;
	DHT dht;
	static float data[6];
	static bool isLocked;
	int CoolDown;
public:
	Sensor();
	void Init();
	const static float* GetData();
	void ReadSensors();
	static void SensorTask(void* taskParams);
};
#endif