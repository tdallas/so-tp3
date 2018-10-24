#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "message.h"

typedef struct queueHeader * messageQueueADT;

messageQueueADT newMessageQueue();

void sendMessage(messageQueueADT queue, int pid, char* text, int length);

void receiveMessage(messageQueueADT queue, int pid, char* dest, int length);

void deleteMessageQueue(messageQueueADT queue);

int lengthAvailable(messageQueueADT queue, int category);

#endif
