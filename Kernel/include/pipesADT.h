#ifndef PIPES_H
#define PIPES_H


messageQueueADT newPipe();

void sendMessagePipe(messageQueueADT pipe, char * text, int length);

void receiveMessagePipe(messageQueueADT pipe, char * dest, int length);

void deletePipe(messageQueueADT pipe);


#endif
