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

#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#define CMD_NUM 7

/*
* The Program class hosts all the "global" functions and most of the variables
*/
class Program
{
public:
	//static MessageQue ReceviedQue;
	//static MessageQue SendQue;
	static String Own_ID;
	static char* cmds[];
	static void ErrorLoop(/*MyUDP& udp*/)
	{
		while (1)
		{
			//udp.SendUDPPacket("ERR", udp.BROADCAsT, udp.GetPort());
		}
	}
	static char* ReadyOutput(float data[])
	{
		String Output = ("SND|" + Own_ID + "|" + String(data[0]) + "|" + String(data[1]) + "|" + String(data[2]) + "|" + String(data[3]) + "|" + String(data[4]) + "|" + String(data[5]) + "|" + "\r");
		char* temp = new char[Output.length() + 1];
		Output.toCharArray(temp, Output.length() + 1);
		return temp;
	}

	static bool StringCompare(const char* compare, const char* to)
	{
		if (strlen(compare) < strlen(to))
		{
			return false;
		}
		else{
			for (int i = 0; i < strlen(to); i++)
			{
				if (to[i] != compare[i])
					return false;
			}
			return true;
		}
	}

	static bool StringCompare(const String compare, const String to)
	{
		if (compare.length() < to.length())
		{
			return false;
		}
		else{
			for (int i = 0; i < to.length(); i++)
			{
				if (to[i] != compare[i])
					return false;
			}
			return true;
		}
	}

	/*
	* Processes an incoming massage and returns the position of the command it contains.
	* All commands are stored int the cmds global string table which size also strored in
	* the defined value of CMD_NUM.
	* Returns -1 if command not recognise.
	*/
	static int ProcessCommand(char* cmd_In, int len)
	{
		cmd_In[len] = '\0';

		for (int i = 0; i < CMD_NUM; i++)
		{
			if (StringCompare(cmd_In, cmds[i]))
			{
				/*Serial.print("Command code: ");
				Serial.println(String(i));*/
				return i;
			}
		}
		return -1;
	}
};

#endif