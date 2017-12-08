#include <string.h>
#include "Message.h"

bool Message::SetFullMsg(const char* msg)
{
	/*
	* We check if the message given has correct length.
	* The maximum length is specified to be 14 char-s & the \0.
	* The minimum requriment is that a message contains at least one command message that is at least 3 char-s long and the | ending it.
	*/
	if ((strlen(msg) < MSG_MIN_LEN) || (strlen(msg) > MSG_MAX_LEN))
	{
		return false;
	}

	//Copying the message into our fullMsg variable
	for (int i = 0; i <= strlen(msg); i++)
	{
		fullMsg[i] = msg[i];
	}
	return SetCmdMsg(msg) && SetCmdArg(msg);
}

/*
*We will rely on the fact that the SetFullMsg() function is previously called, thus no need to check the valid length of the message.
*/
bool Message::SetCmdMsg(const char* msg)
{
	int i;
	for (i = 0; (msg[i] != '|') && (i < 5); i++)
	{
		//There can't be any command longer than 4 char-s!
		if (i > CMD_MAX_LEN)
		{
			return false;
		}else{
			cmdMsg[i] = msg[i];
		}
	}

	//The message must be at least 3 char-s long (plus the last iteration when we find the | )
	if (i < 3)
	{
		return false;
	}

	//Set the end of the cmdMsg with the \0.
	cmdMsg[i] = '\0';
	Serial.println("Setting command code!");
	return SetCmdCode();
}

/*
* Sets the command argument variable.
* If no argument was given it fills it with a \0.
*/
bool Message::SetCmdArg(const char* msg)
{
	int i;
	//We search the end of the command
	for (i = 0; (msg[i] != '|') && (i < strlen(msg)); i++);

	//Skipping the |
	i++;

	//If there aren't any command args this will only copy the \0.
	for (int j = i; j <= strlen(msg); j++)
	{
		cmdArg[j - i] = msg[j];
	}
	return true;
}

bool Message::SetCmdCode()
{
	int temp = Program::ProcessCommand(cmdMsg, strlen(cmdMsg));
	Serial.print("Code: ");
	Serial.println(temp);
	if (temp == -1)
	{
		return false;
	}
	cmdCode = temp;
	return true;
}

Message::Message(const char* msg, IPAddress from) : from(from.toString())
{
	if (SetFullMsg(msg))
	{
		
	}else{
		Serial.println("Wrong msg code!");
	}
}

Message::Message(Message& theOther) : from(theOther.from)
{
	if (SetFullMsg(theOther.GetFullMsg()))
	{
	}
	else{
		Serial.println("Wrong msg code!");//throw 1;
	}

}

const char* Message::GetCmdMsg()
{
	return cmdMsg;
}

const char* Message::GetFullMsg()
{
	return fullMsg;
}

const char* Message::GetCmdArg()
{
	return cmdArg;
}

int Message::GetCmdCode(){
	return cmdCode;
}

Message& Message::operator=(Message& theOther)
{
	SetFullMsg(theOther.GetFullMsg());
	this->from = theOther.from;
	return *this;
}

String Message::GetSourceIP()
{
	return from;
}