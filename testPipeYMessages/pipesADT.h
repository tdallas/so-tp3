#ifndef PIPES_H
#define PIPES_H


typedef struct pipeHeader * pipeADT;

pipeADT newPipe();

void sendMessagePipe(pipeADT pipe, char * text, int length);

void receiveMessagePipe(pipeADT pipe, char * dest, int length);

void deletePipe(pipeADT pipe);

int lengthAvailablePipe(pipeADT pipe);

#endif
