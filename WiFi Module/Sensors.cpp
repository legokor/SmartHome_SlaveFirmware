#include "Sensor.h"

float Sensor::data[6] = { 0, 0, 0, 0, 0, 0 };

Sensor::Sensor() : Task(), mq2(A0), dht(DHTPIN, DHTTYPE)
{
	CoolDown = 0;
	for (int i = 0; i < 6; i++)
	{
		data[i] = 0;
	}
}

void Sensor::Init()
{
	Serial.println("mq2");
	//MQ-2 Sensor setup
	mq2.begin();

	Serial.println("dht");
	//DHT-22 Sensor Setup
	dht.begin();

	Serial.println("PIR");
	//PIR Sensor Setup
	pinMode(4, INPUT);

	Serial.println("FAN");
	//Fan Setup
	//pinMode(2, OUTPUT);
}

void Sensor::ReadSensors()
{
	//Reading gas sensor datas
	data[0] = mq2.readLPG();
	data[1] = mq2.readCO();
	data[2] = mq2.readSmoke();

	//DHT-22 main function
	// Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
	if (CoolDown == 4)
	{
		// Reading temperature or humidity takes about 250 milliseconds!
		data[3] = dht.readHumidity();
		data[4] = dht.readTemperature();// Read temperature as Celsius
		CoolDown = 0;
	}

	//PIR main function
	data[5] = digitalRead(4);
	CoolDown++;
}


float* Sensor::GetData()
{
	return data;
}

void Sensor::loop()
{
	Serial.println("SensTask");
	ReadSensors();
}