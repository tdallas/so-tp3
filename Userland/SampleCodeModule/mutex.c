#include <systemCall.h>

void * mutexInit(char *name){
  return (void*)systemCall(16, (uint64_t)name, 0,0,0,0);
}

int mutexLock(void * mutex){
  return systemCall(18, (uint64_t)mutex, 0,0,0,0);
}

int mutexUnlock(void * mutex){
  return systemCall(17, (uint64_t)mutex, 0,0,0,0);
}

int mutexClose(void * mutex){
  return systemCall(21, (uint64_t)mutex, 0,0,0,0);
}
