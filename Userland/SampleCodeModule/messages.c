#include <systemCall.h>

void send(int pid, char* msg, int length){
  systemCall(11, pid, (uint64_t)msg, length,0,0);
}

void receive(int pid, char* msg, int length){
  systemCall(12, pid, (uint64_t)msg, length,0,0);
}
