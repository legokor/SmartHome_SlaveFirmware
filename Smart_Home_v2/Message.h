#include <IPAddress.h>
#include "Program.h"

#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#define MSG_MAX_LEN 70
#define MSG_MIN_LEN 4
#define CMD_MAX_LEN 5

class Message
{
private:
	int cmdCode;
	String from;
	char fullMsg[71];
	char cmdMsg[6];
	char cmdArg[65];

private:
	/*
	* Sets the full received message.
	* Returns false if message length is invalid.
	*/
	bool SetFullMsg(const char*);
	bool SetCmdArg(const char*);
	/*
	* Sets both the command message and the command code (using SetCmdCode())
	* Returns false if either the message is invalid of length or command is none existent
	*/
	bool SetCmdMsg(const char*);
	bool SetCmdCode();
public:
	Message(const char*, IPAddress);
	Message(Message&);
	//~Message();
	const char* GetCmdMsg();
	const char* GetFullMsg();
	const char* GetCmdArg();
	String GetSourceIP();
	int GetCmdCode();
	Message& operator=(Message&);
};
#endif