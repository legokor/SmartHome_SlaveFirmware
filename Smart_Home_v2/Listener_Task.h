#include "MyUDP.h"
#include "Ques.h"
#include "Scheduler.h"

#pragma once
#ifndef LISTENER_H
#define LISTENER_H


class Listener : public Task
{
private:
	MyUDP* channel;
	IPAddress adr;
public:
	Listener(MyUDP* udp) : Task(), channel(udp)
	{}
	void loop()
	{
		Serial.println("Listener task");
		char* msg = channel->Listen(adr);
		if (msg != NULL)
		{
			Serial.println(adr.toString());
			Serial.println("This came: " + String(msg));
		}
		if (msg != NULL)
		{
			if (Ques::ReceviedQue.Push(msg, adr))
			{
				Serial.println("Message was succesfully pushed!");
			}
		}
	}
};

#endif // !LISTENER_H
