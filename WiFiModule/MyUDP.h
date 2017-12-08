#include "Program.h"
#include "Message.h"
#include "Ques.h"
#include "PWM_Driver.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>


#pragma once
#ifndef MYUDP_H
#define MYUDP_H

#define VISIBLE  0
#define HIDDEN 1
#define CLIENT_NUM 16





class MyUDP
{
private:
	//Variables
	WiFiUDP UDP;
	WiFiServer server;
	int clientNum;
	IPAddress myIP;
	IPAddress HostIP;
	const IPAddress BROADCAST;
	char Host[20];
	const int Port;
	int Try_Num;
	char buff[255];
	int timer;
	static String currID;
public:
	/*
	* Getters
	*/
	IPAddress GetHostIP()
	{
		return HostIP;
	}

	IPAddress GetMyIP()
	{
		return myIP;
	}

	int GetPort() const
	{
		return Port;
	}

	/*
	* Constructor.
	* Sets clientNum to 0 and initializes the UDP
	* Sets Port to 1080
	*/
	MyUDP();

	/*
	* Sends the msg to the destination_IP and destination_Port adress/port
	* Returns true if successfull.
	*/
	bool SendUDPPacket(const char* msg, const IPAddress destination_IP, int destination_Port);


	/*
	*It sets up the WIFI ST_AP
	*Handles both the softAP creation and the connection to an existing AP
	*It requires the preset SSID and password for the Master PI
	*/
	bool WifiSetup(const char* Main_AP_ssid, const char* Main_AP_password, const char* Sec_AP_ssid, const char* Sec_AP_password);

	char* Listen(IPAddress&);

	/*
	* Processes messages if it has our ID.
	* If not returns false and then it is to be forwarded.
	*/
	bool HandleCommand(Message&);

	/*
	* Serves or denies an access request, dependig on the current clientNum
	*/
	void HandleNewClient(Message&);

	/*
	* Send out own data to current host.
	* Serves every kind of commands coming in.
	*/
	void CommunicationHandler(Message&);

private:
	/*
	* Tries to connect to given ssid with the given password.
	* Return true if successful.
	*/
	bool WifiConnect(const char* ssid, const char* password);

	/*
	* It broadcasts the REQIP command through the network which we could connect
	* The AP will either send back an ID (and an SVD command), or that it cannot have any more clients (DEN command)
	*/
	bool First_Comm(const char* InSSID);

	/*
	* Creates the Soft AP.
	* the ssid means the new AP's SSID
	* the psw the password. If NULL then there's no password
	* visibility 0 - visible; 1 - not visible
	* slave_size the amount of STA-s that can connect
	*/
	bool SoftAP_Setup(const char* ssid, const char* psw, int visibility, int slave_size);

	String* SearchVisibleSSIDs(int& String_size);

};


#endif
