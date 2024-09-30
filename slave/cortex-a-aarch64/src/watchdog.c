void WatchdogInitEarly() __attribute__ ((naked));
void WatchdogTrigger();

void WatchdogInitEarly()
{
	// please use only inline assembly here
	// intentionally left empty
	__asm__ volatile ("ret");
}

void WatchdogInit()
{
	// intentionally left empty
}

void WatchdogTrigger()
{
	// intentionally left empty
}
