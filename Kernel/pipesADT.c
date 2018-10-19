#include <messageQueueADT.h>
#include <mutex.h>
#include <pipesADT.h>




struct pipeNode{

  messageQueueADT messageQueue;

};



pipeADT newPipe(){
  struct pipeNode *newNode = malloc(sizeof(struct pipeNode));

  newNode->messageQueue = newMessageQueue();
  return newNode;
}

void sendMessagePipe(pipeADT pipe, char * text, int length){
  sendMessage(pipe->messageQueue, 0, text, length);
}

void receiveMessagePipe(pipeADT pipe, char * dest, int length){
  receiveMessage(pipe->messageQueue, 0, dest, length);
}

void deletePipe(pipeADT pipe){
  deleteMessageQueue(pipe->messageQueue);
  free(pipe);
}
