#include <messageQueueADT.h>
#include <mutex.h>
#include <videoDriver.h>



pipeADT newPipe(){
  return newMessageQueue();
}

void sendMessagePipe(pipeADT messageQueue, char * text, int length){

  sendMessage((messageQueueADT)messageQueue, 0, text, length);
}

void receiveMessagePipe(pipeADT messageQueue, char * dest, int length){

  receiveMessage((messageQueueADT)messageQueue, 0, dest, length);

}

void deletePipe(pipeADT messageQueue){
  deleteMessageQueue((messageQueueADT)messageQueue);
}
