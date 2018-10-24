#include "init.h"
#include "scheduler.h"
#include "videoDriver.h"
#include "processes.h"
#include "lib.h"

static void *const sampleCodeModuleAddress = (void *)0x400000;

void init()
{
	struct fileDescriptors *fd;
	fd->stdin=0;
	fd->stdout=0;
	process *shell = createProcess((uint64_t)sampleCodeModuleAddress, 0,0, "shell", fd);
	setProcessForeground(shell->pid);
	runProcess(shell);

	while (1)
	{
		_hlt();
	}
}
