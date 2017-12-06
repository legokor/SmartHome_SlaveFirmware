#include "MessageProcess_Task.h"
#include "Sensor.h"

MessageProcessTask::MessageProcessTask(MyUDP* udp) : Task(), channel(udp), temp(" ",udp->GetHostIP())
{
	int i = 2;
}
void MessageProcessTask::loop()
{
	Serial.println("MsgProc");
	//Message* toProcess;
	if (Ques::ReceviedQue.Pop(temp))
	{
		Serial.print("Processing message: " + String(temp.GetFullMsg()));
		Serial.print(", with code: " + String(temp.GetCmdCode())+ ", from: ");
		Serial.println(temp.GetSourceIP());
		channel->CommunicationHandler(temp);
		//delete toProcess;
	}
	Ques::SendQue.Push(Program::ReadyOutput(Sensor::GetData()), channel->GetHostIP());
}