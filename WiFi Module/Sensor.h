#include "DHT.h"
#include "MQ2.h"
#include "Scheduler.h"

#pragma once
#ifndef SENSOR_H
#define SENSOR_H

#define DHTPIN 5     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
class Sensor : public Task
{
private:
	MQ2 mq2;
	DHT dht;
	static float data[6];
	int CoolDown;
public:
	Sensor();
	void Init();
	static float* GetData();
	void ReadSensors();
	void loop();
};
#endif