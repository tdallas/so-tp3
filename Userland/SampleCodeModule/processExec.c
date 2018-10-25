#include "stdlib.h"
#include "shell.h"
#include "systemCall.h"
#include <exitProcess.h>
#include <fileDescriptors.h>

typedef void (*entry_point)(int, char **);
int sysExec(void *function, int argc, char **argv, char *name, int, int);
void sysSetForeground(int pid);

int execProcess(void *function, int argc, char **argv, char *name, int foreground, int stdin, int stdout)
{

	int pid = sysExec(function, argc, argv, name, stdin, stdout);
	if (foreground == 1)
	{
		sysSetForeground(pid);
	}

	return pid;
}

int sysExec(void *function, int argc, char **argv, char *name, int stdin, int stdout)
{
	struct fileDescriptors* fd;
	fd->stdin = stdin;
	fd->stdout = stdout;
  return (uint64_t)systemCall(13, (uint64_t)function, argc, (uint64_t)argv, (uint64_t)name, (uint64_t)fd);
}

void sysSetForeground(int pid)
{
  systemCall(19, (uint64_t)pid, 0, 0, 0, 0);
}

void sysKillProcess(){
	systemCall(14, 0, 0, 0, 0, 0);
}

int getPid(){
	return systemCall(20,0,0,0,0,0);
}

void printPids() {
	systemCall(15,0,0,0,0,0);
}

void setPriority(int pid, int priority){
	
	int a = my_getnbr(pid);
	int b = my_getnbr(priority);
	systemCall(26,(uint64_t)a ,(uint64_t)b ,0,0,0);
}

int my_getnbr(char *str)
{
  int result;
  int puiss;

  result = 0;
  puiss = 1;
  while (('-' == (*str)) || ((*str) == '+'))
  {
      if (*str == '-')
        puiss = puiss * -1;
      str++;
  }
  while ((*str >= '0') && (*str <= '9'))
  {
      result = (result * 10) + ((*str) - '0');
      str++;
  }
  return (result * puiss);
}
