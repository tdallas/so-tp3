#include "messageQueueADT.h"
#include "mutex.h"
#include "pipesADT.h"
//#include <videoDriver.h>



pipeADT newPipe(){
  return newMessageQueue();
}

void sendMessagePipe(pipeADT pipe, char * text, int length){
  //printString("Enviando mensaje.\n", 200, 200, 200);
  sendMessage((messageQueueADT)pipe, 1, text, length);
}

void receiveMessagePipe(pipeADT pipe, char * dest, int length){
  	//printString("Pidiendo mensaje: ", 200, 200, 200);
    //printDec(messageQueue);
    //printString("\n", 200, 200, 200);
  receiveMessage((messageQueueADT)pipe, 1, dest, length);
  //printString("Mensaje recibido.\n", 200, 200, 200);
}

void deletePipe(pipeADT pipe){
  deleteMessageQueue((messageQueueADT)pipe);
}
