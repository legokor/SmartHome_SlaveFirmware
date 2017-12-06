#include "MyUDP.h"
#include "Ques.h"
#include "Scheduler.h"

#pragma once
#ifndef SENDER_H
#define SENDER_H

class SenderTask : public Task
{
private:
	MyUDP* channel;
	IPAddress temp;
public:
	SenderTask(MyUDP* udp) : Task(), channel(udp)
	{}
	void loop()
	{
		Serial.println("SndTask");
		Message toSend("", channel->GetHostIP());
		if (Ques::SendQue.Pop(toSend))
		{
			temp.fromString(toSend.GetSourceIP());
			Serial.println("Sending message: " + String(toSend.GetFullMsg()));
			channel->SendUDPPacket(toSend.GetFullMsg(), temp, channel->GetPort());
			//delete toSend;
		}
	}
};

#endif