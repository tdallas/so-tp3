#ifndef PIPES_H
#define PIPES_H

typedef messageQueueADT pipeADT;

pipeADT newPipe();

void sendMessagePipe(pipeADT pipe, char * text, int length);

void receiveMessagePipe(pipeADT pipe, char * dest, int length);

void deletePipe(pipeADT pipe);


#endif
