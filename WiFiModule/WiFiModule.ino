/*
 Name:		Smart_Home_v2.ino
 Created:	4/10/2017 2:51:00 PM
 Author:	Richard Schantzl
*/

//Includes
#include <ArduinoOTA.h>
#include "stdlib.h"
#include "DHT.h"
#include "MQ2.h"
#include "Sensor.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include "Scheduler.h"
#include "MyUDP.h"
#include "Program.h"
#include "Listener_Task.h"
#include "MessageProcess_Task.h"
#include "Sender_Task.h"
#include "PWM_Driver.h"
#include <Schedule.h>

//Wifi Default Settings

const char* Main_AP_ssid = "Chinese Bela";
const char* Main_AP_password = "paSSword";
char* Sec_AP_ssid = "STAP_";
const char* Sec_AP_password = "CHXYSW2:)64";
//SchedulerClass Scheduler;

MyUDP udp;
Listener LisTask(&udp);
SenderTask SndTask(&udp);
MessageProcessTask MsgProcTask(&udp);
Sensor sens;
Driver pwm;


void setup() {
	//Starting Serial Port with 115200 baud-rate
	Serial.begin(115200);
	//LED setup
	pinMode(D0, OUTPUT);

	WiFi.softAPdisconnect(true);
	if (udp.WifiSetup(Main_AP_ssid, Main_AP_password, Sec_AP_ssid, Sec_AP_password))
	{
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
		Serial.println(udp.GetMyIP().toString());
		Serial.println(udp.GetHostIP().toString());
	}
	else{
		Serial.println("Failed to connect to any network!");
		Program::ErrorLoop();
	}

	//Initializing sensors
	Serial.println("Initializing Sensors");
	sens.Init();

	Serial.println("Creating tasks");
	//Initializing Tasks
	Scheduler.Init();
	Scheduler.start(&LisTask);
	Scheduler.start(&SndTask);
}

void loop() {
	// put your main code here, to run repeatedly:
	//run_scheduled_functions();
	Serial.println("Starting tasks");
	//Starting the tasks
	Scheduler.begin();
	int i;
}
