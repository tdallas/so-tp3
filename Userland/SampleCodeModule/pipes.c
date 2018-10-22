#include <systemCall.h>


int newPipe(){
  return (uint64_t)systemCall(22,0,0,0,0,0);
}

void sendMessagePipe(int pipe, char* msg, int length){
  systemCall(23, pipe, (uint64_t)msg, length,0,0);
}

void receiveMessagePipe(int pipe, char* dest, int length){
  systemCall(24, pipe, (uint64_t)dest, length,0,0);
}

void deletePipe(){
  systemCall(25, 0,0,0,0,0);
}
