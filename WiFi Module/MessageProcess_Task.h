#include "MyUDP.h"
#include "Ques.h"
#include "c:\Users\DELL\Documents\Arduino\libraries\ESP8266Scheduler\src\Scheduler.h"

#pragma once
#ifndef PROCESS_H
#define PROCESS_H

class MessageProcessTask : public Task
{
private:
	Message temp;
	MyUDP* channel;
public:
	MessageProcessTask(MyUDP* udp);
	void loop();

};
#endif