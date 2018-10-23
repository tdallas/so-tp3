
#ifndef EXECPROCESS_H_
#define EXECPROCESS_H_

int execProcess(void* function,int argc, char** argv, char* name, int foreground, int stdin, int stdout);
void sysSetForeground(int pid);
void sysKillProcess();
void printPids();
int getPid();
#endif
