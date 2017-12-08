#include "MyUDP.h"

String MyUDP::currID = "11111111";

/*
* Constructor.
* Sets clientNum to 0 and initializes the UDP
* Sets Port to 1080
*/
MyUDP::MyUDP() : Port(1080), BROADCAST(255, 255, 255, 255), server(1080)
{
	//Connection timeout in 500ms
	Try_Num = 20;
	//client num to 0
	clientNum = 0;
}

/*
* Sends the msg to the destination_IP and destination_Port adress/port
* Returns true if successfull.
*/
bool MyUDP::SendUDPPacket(const char* msg, const IPAddress destination_IP, int destination_Port)
{
	//UDP.begin(Port);
	UDP.beginPacket(destination_IP, destination_Port); +
		UDP.write(msg, strlen(msg));
	return UDP.endPacket();
}

char* MyUDP::Listen(IPAddress& out)
{
	if (UDP.parsePacket())
	{
		//Receiving a Packet
		int len = UDP.read(buff, 255);
		buff[len] = '\0';
		out = UDP.remoteIP();
		return buff;
	}else{
		return NULL;
	}
}

bool MyUDP::HandleCommand(Message& cmd)
{
	String arg = cmd.GetFullMsg();
	String ID = "........";
	String num = "...";
	int i;
	Serial.println("Handle command");
	//If the length of the argument isn't long enough
	if (arg.length() < 10)
	{
		return false;
	}

	for (i = 4; (arg[i] != '|'); i++)
	{
		//We put the ID field into the
		ID[i-4] = arg[i];
	}

	/*
	* We check if our ID is in the message and if the length of the ID is valid
	* If any above is so, we return false. 
	*/
	Serial.println(ID);
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
* Serves or denies an access request, dependig on the current clientNum
*/
void MyUDP::HandleNewClient(Message& request)
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
	}
	else{
		//Serial.println("Pushing DEN into SND Que");
		SendUDPPacket("DEN", temp, Port);
	}
}

/*
* Send out own data to current host.
* Serves every kind of commands coming in.
*/
void MyUDP::CommunicationHandler(Message& toSend)
{
	IPAddress ip;
	//HostIP.fromString(Host);
	//If we received no massages
	ip.fromString(toSend.GetSourceIP());
		switch (toSend.GetCmdCode())
		{
		case 0: 
			Serial.println("Serving" + String(toSend.GetFullMsg()));
			timer = 0;
			break;

		case 1: 
			Serial.println("Serving" + String(toSend.GetFullMsg()));
			break;

		case 2: 
			Serial.println("Serving" + String(toSend.GetFullMsg()));
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
		default: break;
		}
		timer++;
		//WiFi.mode(WIFI_AP_STA);
		//UDP.stop();     
}
/*
* Tries to connect to given ssid with the given password.
* Return true if successful.
*/
bool MyUDP::WifiConnect(const char* ssid, const char* password)
{
	Serial.print("\nConnecting to ");
	Serial.println(ssid);
	
	//We try 20 times to connect to a specific wifi server.
	//Between every try there's a 500ms delay during which we turn on&off a LED
	WiFi.begin(ssid, password);
	//WiFi.mode(WIFI_AP_STA);
	for (; ((WiFi.status() != WL_CONNECTED) && (Try_Num >0)); Try_Num--)
	{
		delay(100);
		Serial.print(".");
		digitalWrite(D0, HIGH); // turn the LED on (HIGH is the voltage level)
		delay(200);              // wait for a second
		digitalWrite(D0, LOW);    // turn the LED off by making the voltage LOW
		delay(200);
	}
	Serial.print("\n");
	if (Try_Num != 0)
	{
		Try_Num = 20;
		return true;
	}
	else{
		Try_Num = 20;
		return false;
	}
}

/*
* It broadcasts the REQIP command through the network which we could connect
* The AP will either send back an ID (and an SVD command), or that it cannot have any more clients (DEN command)
*/
bool MyUDP::First_Comm(const char* InSSID)
{
	UDP.begin(Port);
	Serial.println("dasd");
	char PacketIn[255];
	int TimeOut = 0;
	/*
	* We are looping broadcasting and waiting for a correct answer.
	* If we find a correct msg we return from the loop.
	* TODO: TimeOut from Loop to detect a fail host!
	*/
	while (true)
	{
		/*
		* Creating and sending the request msg to our host-to-be
		*/
		Serial.println("Sending message: " + String(InSSID));

		UDP.beginPacket(BROADCAST, Port);
		UDP.write("REQIP|");
		if (UDP.endPacket())
		{
			Serial.println("Message sent!");
		}
		//Waiting for an answer to come or timeout after 1 sec
		while (!UDP.parsePacket() && (TimeOut < 100))
		{
			TimeOut++;
			delay(100);
		}
		//If we time out we try to broadcast again
		if (TimeOut == 100)
		{
			Serial.println("Timed Out!");
			TimeOut = 0;
			//If we got message we check it and if corrrect we parse it
		}
		else{
			//Setting back TimeOut in case of a wrong incoming message
			TimeOut = 0;
			/*
			* If we received a Packet we read it to a buffer (PacketIn),
			* and then decide if it's a correct answer or not.
			*/
			int len = UDP.read(PacketIn, 255);
			Serial.print("\nGot message: ");
			//DEBUG
			for (int i = 0; i < len; i++)
				Serial.print(String(PacketIn[i]));
			Serial.print(" " + String(len) + "\n");
			//DEBUG_END

			//Checking for incoming Packet Validity
			//If valid, then we extract the information in it
			if (Program::StringCompare(PacketIn, "SVD"))
			{
				//iterators
				int i, j;
				/*
				* Extracting the ID from the packet
				* We know that the first 4 characters are parts of the command
				* only then comes 8 char-s of the ID.
				*/
				for (i = 4; PacketIn[i] != '|'; i++)
				{
					Serial.print(Program::Own_ID[i - 4] = PacketIn[i]);
				}

				//DEBUG
				Serial.println("ID: " + Program::Own_ID);
				//DEBUG_END

				//We must add the \0 manually
				Program::Own_ID[i - 4] = '\0';

				/*
				* The next part of the packet is the IP address of our host.
				* We read it from i+1 since the i now showing an | char.
				* The destination string needs to be indexed from 0,
				* so we must subtract the i from the j!
				*/
				i++;
				for (int j = i; PacketIn[j] != '|'; j++)
				{
					Host[j - i] = PacketIn[j];
				}

				//We must add the \0 manually
				Host[j - i] = '\0';
				//DEBUG
				Serial.println("Host IP: " + String(Host));
				HostIP = UDP.remoteIP();
				//DEBUG_END
				//UDP.stop();
				return true;
			}
			else{ //If our connect request was denied.
				if (Program::StringCompare(PacketIn, "DEN"))
					return false;
			}
		}
	}
}

/*
* Creates the Soft AP.
* the ssid means the new AP's SSID
* the psw the password. If NULL then there's no password
* visibility 0 - visible; 1 - not visible
* slave_size the amount of STA-s that can connect
*/
bool MyUDP::SoftAP_Setup(const char* ssid, const char* psw, int visibility, int slave_size)
{
	Serial.println("\nConfiguring SoftAP");
	//Creating a SofAP over our WIFI. 
	//WiFi.begin(ssid, password);
	if (WiFi.softAP(ssid, psw, visibility, slave_size))
	{
		//setting up the global variable myIP
		myIP = WiFi.softAPIP();
		//WiFi.mode(WIFI_AP_STA);
		Serial.println("WiFi AP created");
		// Starting the server API
		server.begin();
		return true;
	}
	else{
		return false;
	}
}

String* MyUDP::SearchVisibleSSIDs(int& String_size)
{
	int sum = WiFi.scanNetworks();
	String_size = 0;
	String* SSIDs = new String[sum];

	for (int i = 0; i<sum; i++)
	{
		String temp = WiFi.SSID(i);
		//Serial.println(temp);
		SSIDs[String_size] = temp;
		Serial.println(SSIDs[String_size]);
		String_size++;
	}
	return SSIDs;
}

/*
*It sets up the WIFI ST_AP
*Handles both the softAP creation and the connection to an existing AP
*It requires the preset SSID and password for the Master PI
*/
bool MyUDP::WifiSetup(const char* Main_AP_ssid, const char* Main_AP_password, const char* Sec_AP_ssid, const char* Sec_AP_password)
{
	if (WifiConnect(Main_AP_ssid, Main_AP_password))
	{
		Serial.println("\nObtaining ID:");
		if (First_Comm(Main_AP_ssid))
		{
			Serial.print("\nConfiguring SoftAP: ");
			Serial.println(Sec_AP_ssid);
			//Creating a SofAP over our WIFI. The third parameter means it's not hidden
			//and the fourth parameter means the max amount of clients that can connect. 
			if (SoftAP_Setup(Sec_AP_ssid, Sec_AP_password, VISIBLE, CLIENT_NUM))
			{
				return true;
			}
			else{
				return false;
			}
		}
	}
	Serial.println("Connecting to secondary AP:");
	int String_size;
	String* SSIDs = SearchVisibleSSIDs(String_size);
	for (int i = 0; i < String_size; i++)
	{
		Serial.println("dasd");
		char* temp = new char[SSIDs[i].length() + 1];
		SSIDs[i].toCharArray(temp, SSIDs[i].length() + 1);
		Serial.println(temp);
		if (WifiConnect(temp, Sec_AP_password))
		{
			if (First_Comm(temp))
			{
				if (SoftAP_Setup(Sec_AP_ssid, Sec_AP_password, VISIBLE, CLIENT_NUM))
				{
					return true;
				}
				else{
					return false;
				}
			}
		}
	}
	return false;
}