#include "UARTListener.h"
#include "Ques.h"
#include <IPAddress.h>

void UARTListener::ListenerTask(void * taskParams)
{
	while (true)
	{
		String incomingMessage = Serial.readStringUntil('\r');
		if (incomingMessage.length() > 0)
		{
			if (Program::StringCompare(incomingMessage, "UMSG|"))
			{
				String message;
				String address;
				IPAddress ip;

				int i;

				//Copying the mesage content to our message variable
				for (i = 4; (incomingMessage[i] != '|') && (i < incomingMessage.length()); i++)
				{
					message += incomingMessage[i];
				}

				//Skipping the '|'
				i++;

				//Copying the IPAddres to out adress variable
				for (; i < incomingMessage.length(); i++)
				{
					address += incomingMessage[i];
				}

				//if the adress is realy an IPaddress
				if (ip.fromString(address))
				{
					Ques::ReceviedQue.Push(incomingMessage, ip);
				}
			}
		}
	}
}
