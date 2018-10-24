#include "include/lib.h"
#include "include/scheduler.h"
#include "include/mutex.h"
#include "include/processes.h"
#include "include/defs.h"
#include "include/interrupts.h"

static void addProcess(process *p);
static void setNextCurrent();

// Procesos actualmente bloqueados
static blockedProcess *firstBlockedProcess;

// Proceso actualmente corriendo
static nodeList *current = NULL;
static nodeList *prev = NULL;

process *getCurrentProcess()
{
	return current->p;
}

uint64_t nextProcess(uint64_t current_rsp)
{

	if (current == NULL)
	{
		return current_rsp;
	}

	decreaseQuantum();

	if (current->quantum > 0)
		return current_rsp;

	current->quantum = QUANTUM;

	setProcessRsp(current->p, current_rsp);

	if(current->p->status == RUNNING){
		current->p->status = READY;
	}

	prev = current;
	current = current->next;

	setNextCurrent();

	current->p->status = RUNNING;

	return getProcessRsp(current->p);
}

uint64_t runProcess(process *new_process)
{
	int pid;

	addProcess(new_process);

	pid = getProcessPid(new_process);

	if (pid == 0)
		_changeProcess(getProcessRsp(current->p));

	return pid;
}

static void addProcess(process *p)
{
	nodeList *new_node = (nodeList *)malloc(sizeof(*new_node));

	new_node->p = p;
	new_node->quantum = QUANTUM;

	if (current == NULL)
	{
		current = new_node;
		current->next = current;
		prev = current;
	}
	else
	{
		new_node->next = current->next;
		current->next = new_node;
	}
}

void killProcess()
{
	nodeList *n = current;
	removeProcess(n->p);
	prev->next = current->next;
	current = current->next;
	setNextCurrent();
	free((void *)n);
	increaseQuantum();
	_changeProcess(getProcessRsp(current->p));
}

void yieldProcess()
{
	current->next->quantum += 1;
	current->quantum = 0;
	_yieldProcess();
}

int counter = 0;
int priorityHandling = 1;
int processes = 0;

static void handlePriority()
{
	if (processes != getProcessesNumber()){
		
		processes = getProcessesNumber();
		priorityHandling = 1;		
	}

	if( current->p->priority == priorityHandling){
		counter = 0;
		return;
	}
	else{
		current->p->starvation++;

		if (current->p->starvation <=50){
		
		prev = current;
		current = current->next;
		
		}
		else{
			current->p->starvation = 0;
			counter++;
			if ( processes <= counter){
				if (priorityHandling  != 3){
					priorityHandling++;
					counter = 0;
				}
			}
			return;
		}
	}

	counter++;

	if ( processes <= counter){
		if (priorityHandling  != 3){
			priorityHandling++;
			counter = 0;
		}
	}

	setNextCurrent();
}

static void setNextCurrent()
{
	while (isProcessBlocked(current->p) || isProcessDeleted(current->p))
	{
		nodeList *next = current->next;

		if (isProcessDeleted(current->p))
		{
			prev->next = next;
			removeProcess(current->p);
			free((void *)current);
		}
		else
			prev = current;

		current = next;
	}

	handlePriority();
}

void printBlockedProcessesList()
{
	blockedProcess *temp = firstBlockedProcess;
	while (temp != NULL)
	{
		printString("PID: ", 0, 155, 255);
		printDec(temp->process->pid);
		//printInt(temp->process->pid, 0, 155, 255); BORRAR
		printString("\n", 0, 155, 255);
		temp = temp->next;
	}
}

void increaseQuantum()
{
	current->quantum += 1;
}

void decreaseQuantum()
{
	current->quantum -= 1;
}

void block(queueADT queue)
{
  blockProcess(current->p);
  enqueue(queue, current);
}

void unblock(queueADT queue)
{
	nodeList *node = dequeue(queue);
	if(node != NULL)
	{
		if(node->p->status == DELETE)
		{
			unblock(queue);
		}
		unblockProcess(current->p);
		addProcess(current->p);
	}
}
