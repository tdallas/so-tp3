//#include "include/stdint.h"
#include "mutex.h"
//#include "include/lib.h"
#include <stdlib.h>
#include <string.h>
//#include "include/processes.h"
//#include "include/scheduler.h"
//#include "include/videoDriver.h"

#define MAX_PROCESSES 20

static mutexADT mutex[255];
static int id = 0;
static int numberOfMutexes = 0;

typedef struct mutex_t
{
	char* name;
	int value;
	int id;
	int* blockedProcesses[MAX_PROCESSES];
} mutex_t;

mutex_t *mutexInit(char *name)
{
	if((*name)!=0){
		for (int i = 0; i < numberOfMutexes; i++)
		{
			if (strcmp(name, mutex[i]->name) == 0){
				return mutex[i];
			}
		}
	}
	mutexADT newMutex = (mutexADT)malloc(sizeof(mutex_t));
	newMutex->name = (char *)malloc(strlen(name) + 1);
	strcpy(newMutex->name, name);
	newMutex->value = 1;
	newMutex->id = id;
	for(int i = 0; i < MAX_PROCESSES; i++)
	{
		newMutex->blockedProcesses[i] = NULL;
	}

	id++;
	numberOfMutexes++;
	//mutex = (mutexADT *)malloc(numberOfMutexes * sizeof(mutexADT));
	mutex[numberOfMutexes - 1] = newMutex;

	return newMutex;
}

int mutexLock(mutex_t *mut)
{
	while(mut->value==0)
	{
		//process *p = getCurrentProcess();
		//blockProcess(p);
		//mut->blockedProcesses[getProcessPid(p)]= p;
		//yieldProcess();
	}
	mut->value = 0;

	return 0;
}

int mutexUnlock(mutex_t *mut)
{

	for(int i = 0; i < MAX_PROCESSES; i++){
		//unblockProcess(mut->blockedProcesses[i]);
	}
	mut->value = 1;
	//yieldProcess();

	return mut->value;
}

int mutexListSize()
{
	return numberOfMutexes;
}

int mutexClose(mutex_t *mut)
{
	int i;
	for (i = 0; i < numberOfMutexes; i++)
	{
		if (mutex[i]->id == mut->id)
		{
			free(mutex[i]->name);
			free(mutex[i]);

			int j;
			for (j = i; j < numberOfMutexes - 1; j++)
			{
				mutex[j] = mutex[j + 1];
			}

			numberOfMutexes--;

			return 0;
		}
	}
	return 1;
}

void closeAllMutex()
{
	int i;
	for (i = numberOfMutexes; i > 0; i--)
	{
		mutexClose(mutex[i]);
	}
}

// void freeMutexList()
// {
// 	free(mutex);
// }
