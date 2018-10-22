#ifndef PIPES_H
#define PIPES_H

int newPipe();

void sendMessagePipe(int pipe, char* msg, int length);

void receiveMessagePipe(int pipe, char * dest, int length);

void deletePipe();

#endif
