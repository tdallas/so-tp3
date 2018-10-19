#include <systemCall.h>


int newPipe(){
  return (int)systemCall(22,0,0,0,0,0);
}

void sendMessagePipe(int pipe, char* msg, int length){
  systemCall(23, pipe, msg, length,0,0);
}

void receiveMessagePipe(int pipe, char* dest, int length){
  systemCall(24, pipe, dest, length,0,0);
}

void deletePipe(){
  systemCall(25, 0,0,0,0,0);
}
