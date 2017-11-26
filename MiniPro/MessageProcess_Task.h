#include "Ques.h"
#include "Message.h"

#pragma once
#ifndef PROCESS_H
#define PROCESS_H

#define CLIENT_NUM 16

class MessageProcessTask
{
private:
	static Message temp;
	static unsigned ackCounter;
	static int clientNum;
	static IPAddress HostIP;
	static const IPAddress BROADCAST;
	static IPAddress myIP;
	static String currID;

public:
	static void MessageProcess(void* taskParams);
	static bool HandleNewClient(Message& request);
	static bool HandleCommand(Message& cmd);
	static void CommunicationHandlerTask(Message& toSend);

};
#endif