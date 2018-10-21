#include <messageQueueADT.h>
#include <mutex.h>
#include <videoDriver.h>



messageQueueADT newPipe(){
  return newMessageQueue();
}

void sendMessagePipe(messageQueueADT messageQueue, char * text, int length){
  printString("Enviando mensaje.\n", 200, 200, 200);
  sendMessage(messageQueue, 0, text, length);
}

void receiveMessagePipe(messageQueueADT messageQueue, char * dest, int length){
  	printString("Pidiendo mensaje: ", 200, 200, 200);
    printDec(messageQueue);
    printString("\n", 200, 200, 200);
  receiveMessage(messageQueue, 0, dest, length);
  printString("Mensaje recibido.\n", 200, 200, 200);
}

void deletePipe(messageQueueADT messageQueue){
  deleteMessageQueue(messageQueue);
}
