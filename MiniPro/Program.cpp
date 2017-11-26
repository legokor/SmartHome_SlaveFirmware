#include <Arduino.h>
#include "Program.h"
#include "PWM_Driver.h"

String Program::Own_ID = "--------";
char* Program::cmds[] = { "ACK", "DEN", "SND", "CMD", "REQIP", "SVD", "ERR" };

unsigned Driver::sourcePercentage = 0;
bool Driver::isUsed = false;
