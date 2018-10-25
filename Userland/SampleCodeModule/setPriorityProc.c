#include <processExec.h>
#include <exitProcess.h>

void setPriorityProc(int arcg, char * argv[]){
  setPriority(argv[1], argv[2]);
  exitProcess();
}
