#include "Scheduler.h"
#include <Arduino.h>

extern "C" {
    #include "cont.h"

    void yield();
}

SchedulerClass Scheduler;

Task SchedulerClass::main;
Task *SchedulerClass::current = &SchedulerClass::main;
int SchedulerClass::count = 0;

SchedulerClass::SchedulerClass() {
    main.next = &main;
    main.prev = &main;
	current = &main;
}

void SchedulerClass::start(Task *task) {
	//Serial.println(count);
	if (count == 0)
	{
		//Serial.println("First input");
		current = task;
		current->next = task;
		current->prev = task;
	} else if (count == 1)
	{
		current->prev = task;
		task->next = current;
		current->next = task;
		task->prev = current;
	}else{
		//Serial.println("Other Input");
		task->next = current;
		task->prev = current->prev;
		//setting the previous task's next to the new task
		current->prev->next = task;
		current->prev = task;
		//Serial.println("And we're out");
	}
	count++;
}

void SchedulerClass::Init()
{
	main.next = &main;
	main.prev = &main;
	current = &(main);
	count = 0;
}

void SchedulerClass::begin() {
	//Serial.println("inside the beast again");
	/*current = main.next;
	main.prev->next = main.next;
	main.next->prev = main.prev;*/
    while (1) {
		//current = current->next;
		//Serial.println("While");
		//Serial.println(String(main.delay_ms));
		if (current->shouldRun())
		{
			//Serial.println("So is it here?");
			cont_run(&current->context, task_tramponline);
		}
		//Serial.println("Are we yielding?");
        yield();
		current = current->next;
    }
}

void task_tramponline() {
    SchedulerClass::current->loopWrapper();
}
