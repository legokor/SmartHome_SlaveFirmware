#include "MessageQue.h"
#include <stdlib.h>

/*Message* MessageQue::data[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int MessageQue::elementNum = 0;
bool MessageQue::isUsed = false;*/

MessageQue::MessageQue()
{
	isUsed = false;
	elementNum = 0;
	for (size_t i = 0; i < 10; i++)
	{
		data[i] = NULL;
	}
}

bool MessageQue::Push(const char* msg, const IPAddress& adr)
{
	//Serial.println("Pushed message into Que: " + String(msg));
	if (isUsed == true)
	{
		return false;
	}else{
		isUsed = true;
	}
	if (elementNum == 10)
	{
		isUsed = false;
		return false;
	}
	Message* temp = new Message(msg, adr);
	//putting the message into the current end of our list.
	data[elementNum] = temp;
	Serial.print("Pushed message: ");
	Serial.print(String(data[elementNum]->GetFullMsg()) + ", with code: " + String(data[elementNum]->GetCmdCode()));
	Serial.println(", and with IP: " + String(data[elementNum]->GetSourceIP()));
	elementNum++;
	isUsed = false;
	return true;
}

bool MessageQue::Push(Message& msg)
{
	if (isUsed == true)
	{
		return false;
	}
	else{
		isUsed = true;
	}
	if (elementNum == 10)
	{
		isUsed = false;
		return false;
	}
	//putting the message into the current end of our list.
	data[elementNum] = new Message(msg);
	Serial.println(String(data[elementNum - 1]->GetFullMsg()));
	elementNum++;
	isUsed = false;
	return true;
}

bool MessageQue::Pop(Message& tmp)
{
	if (isUsed == true)
	{
		return false;
	}
	else{
		isUsed = true;
	}

	if (elementNum == 0)
	{
		isUsed = false;
		return false;
	}
	//Serial.println("msg: " + String(data[0]->GetFullMsg()));
	//Saving the first element to be returned.
	tmp = *data[0];

	//Deleting the first element to avoid any memory leak.
	delete data[0];
	data[0] = NULL;

	//Moving all previous messages one place "left"
	for (int i = 0; i < (elementNum - 1); i++)
	{
		data[i] = data[i + 1];
	}
	//Removing the last element from the list, and decreasing elementNum.
	data[elementNum - 1] = NULL;
	elementNum--;
	isUsed = false;
	Serial.println("Popped message: "+ String(tmp.GetFullMsg()) + ", with IP: " + String(tmp.GetSourceIP()));
	return true;
}


void MessageQue::Empty()
{
	for (int i = 0; i < elementNum; i++)
	{
		delete data[i];
		data[i] = NULL;
	}
}