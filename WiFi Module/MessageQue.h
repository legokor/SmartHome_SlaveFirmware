#include "Message.h"

#pragma once
#ifndef MSG_QUE_H
#define MSG_QUE_H

class MessageQue
{
private:
	Message* data[10];
	int elementNum;
	bool isUsed;
public:
	MessageQue();
	/*
	* Pushes the message into the FILO, to the last position.
	* Returns false, if message que is full.
	*/
	bool Push(const char*, const IPAddress&);
	bool Push(Message&);
	/*
	* Returns the first element in the row.
	* Returns false if no message is available.
	*/
	bool Pop(Message&);

	/*
	* Empties the message que, and fills it up with NULL pointers.
	*/
	void Empty();
};

#endif