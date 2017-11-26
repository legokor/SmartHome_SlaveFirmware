/*
 Name:		MiniPro.ino
 Created:	9/25/2017 5:15:30 PM
 Author:	Maxentius
*/
#include <Arduino_FreeRTOS.h>
#include "Sensor.h"
#include "MessageProcess_Task.h"
#include "UARTListener.h"
#include "UARTSender.h"


// the setup function runs once when you press reset or power the board
void setup() 
{
	Serial.begin(115200);
	Serial.println("DEBUG|MiniProStarts");
	xTaskCreate(MessageProcessTask::MessageProcess
		, (const portCHAR *)"MessageProcess"   // A name just for humans
		, 128  // This stack size can be checked & adjusted by reading the Stack Highwater
		, NULL
		, 2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		, NULL);

	xTaskCreate(Sensor::SensorTask
		, (const portCHAR *)"SensorProcess"   // A name just for humans
		, 128  // This stack size can be checked & adjusted by reading the Stack Highwater
		, NULL
		, 2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		, NULL
	);
	xTaskCreate(UARTListener::UARTListenerTask
		, (const portCHAR *)"UARTListener" 
		, 128  // This stack size can be checked & adjusted by reading the Stack Highwater
		, NULL
		, 3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		, NULL
	);
	xTaskCreate(UARTSender::UARTSenderTask
		, (const portCHAR *)"UARTListener"
		, 128  // This stack size can be checked & adjusted by reading the Stack Highwater
		, NULL
		, 2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		, NULL
	);
}

// the loop function runs over and over again until power down or reset
void loop() {

}
