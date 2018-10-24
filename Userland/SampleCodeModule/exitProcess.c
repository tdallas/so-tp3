#include <stdio.h>
#include <processExec.h>

void exitProcess(){
  void*mut=mutexInit("micro");
  mutexLock(mut);
    printf("\n$>");
  mutexUnlock(mut);
  sysKillProcess();

}
