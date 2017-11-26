#include "MessageProcess_Task.h"
#include "Sensor.h"
#include "PWM_Driver.h"
#include <IPAddress.h>

unsigned MessageProcessTask::ackCounter = 0;
const IPAddress MessageProcessTask::BROADCAST = IPAddress(255,255,255,255);
IPAddress MessageProcessTask::myIP = IPAddress();
Message MessageProcessTask::temp;
int MessageProcessTask::clientNum = 0;
IPAddress MessageProcessTask::HostIP = IPAddress();
String MessageProcessTask::currID = "00000000";


void MessageProcessTask::MessageProcess(void* taskParam)
{
	Serial.println("MsgProc");

	if (Ques::ReceviedQue.Pop(temp))
	{
		Serial.print("Processing message: " + String(temp.GetFullMsg()));
		Serial.print(", with code: " + String(temp.GetCmdCode())+ ", from: ");
		Serial.println(temp.GetSourceIP());
		CommunicationHandlerTask(temp);
	}
	Ques::SendQue.Push(Program::ReadyOutput(Sensor::GetData()), IPAddress().fromString(temp.GetSourceIP()));
}

/*
* Serves or denies an access request, dependig on the current clientNum
*/
bool MessageProcessTask::HandleNewClient(Message& request)
{
	IPAddress temp;
	temp.fromString(request.GetSourceIP());
	//We check if we have room for a new client.
	if (clientNum < CLIENT_NUM)
	{
		/*
		* The barely written built-in string hiking to make it work...
		*/
		String msgString = "SVD|" + currID + '|' + myIP.toString() + "|\0";
		/*The whole massage can be maximum command + ID + IP address + 3 imes the '|' + '\0' long.
		* That adds up to 3 + 1 + 8 + 1 + (3*5 + 3) + 2*1 = 30
		*/
		char msgTemp[30];
		//thanks to the idiot who did this...
		msgString.toCharArray(msgTemp, msgString.length() + 1);
		/*
		* Sending an OK tho the joining request.
		* After sending the massage weincrease the clientNum and the currID
		* to make sure the next one who joins get a different ID.
		*/

		Ques::SendQue.Push(msgTemp, temp);
		clientNum++;
		//If we are full and can't handle anymore client    
		return true;
	}
	else {
		//Serial.println("Pushing DEN into SND Que");
		Ques::SendQue.Push("DEN", temp);
		return false;
	}
}

bool MessageProcessTask::HandleCommand(Message& cmd)
{
	String arg = cmd.GetFullMsg();
	String ID = "........";
	String num = "...";
	int i;
	Serial.println("DEBUG|MiniPro Handle command");
	//If the length of the argument isn't long enough
	if (arg.length() < 10)
	{
		return false;
	}

	for (i = 4; (arg[i] != '|'); i++)
	{
		//We put the ID field into the
		ID[i - 4] = arg[i];
	}

	/*
	* We check if our ID is in the message and if the length of the ID is valid
	* If any above is so, we return false.
	*/
	Serial.println("DEBUG|" + ID);
	if (!Program::StringCompare(ID, Program::Own_ID))
	{
		return false;
	}

	//We have to skip the |
	++i;

	//We copy the argument numbers into our number string (num).
	for (int j = 0; (j < 3) && (arg[i] != '|'); j++, i++)
	{
		num[j] = arg[i];
		num[j + 1] = '\0';
	}

	//We set the PWM percentage.
	Serial.println(num);
	Driver::SetPercentage(num.toInt());

	return true;
}

/*
* Send out own data to current host.
* Serves every kind of commands coming in.
*/
void MessageProcessTask::CommunicationHandlerTask(Message& toSend)
{
	IPAddress ip;
	//HostIP.fromString(Host);
	//If we received no massages
	ip.fromString(toSend.GetSourceIP());
	switch (toSend.GetCmdCode())
	{
	case 0:
		ackCounter = 0;
		break;

	case 1:
		break;

	case 2:
		Ques::SendQue.Push(toSend.GetFullMsg(), HostIP);
		Ques::SendQue.Push("ACK", ip);
		break;

	case 3:
		Serial.println("Serving" + String(toSend.GetFullMsg()));
		if (!HandleCommand(toSend))
		{
			Ques::SendQue.Push(toSend.GetFullMsg(), BROADCAST);
		}
		break;

	case 4:
		Serial.println("Serving " + String(toSend.GetFullMsg()));
		HandleNewClient(toSend);
		Serial.println("Request served!");
		break;
	case 5:
		Serial.println("Got SVD msg");
		break;
	case 99:
		HostIP = ip;
		break;
	default: break;
	}
	ackCounter++;
	//WiFi.mode(WIFI_AP_STA);
	//UDP.stop();     
}
