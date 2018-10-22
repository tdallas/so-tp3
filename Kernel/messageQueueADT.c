#include "include/messageQueueADT.h"
#include "include/message.h"
#include "include/processes.h"
#include "include/lib.h"
#include "include/scheduler.h"
//#include <videoDriver.h>
#include "include/mutex.h"


struct queueHeader{
  mutexADT mutex;
  struct messageNode * first;
  struct messageNode * last;
  struct blockedProcessNode * blockedProcesses;
};

struct messageNode{
  struct messageNode * tail;
  struct messageNode * head;
  struct msg * message;
};

struct blockedProcessNode{
  struct blockedProcessNode * tail;
  struct blockedProcessNode * head;
  int processPid;
  int waitingCategory;
  int messageSize;
};


int isMessageAvailable(struct messageNode * curr, int category, int length){
  int aux = 0;
  for(; curr != NULL; curr = curr->tail){
    if(curr->message->category == category){
      aux += curr->message->length;
      if(aux >= length){
        return 1;
      }
    }
  }
  return 0;
}

struct messageNode * searchMessageR(struct messageNode* curr, messageQueueADT queue, struct messageNode* prev, int category, int length, char * dest){
  if(curr == NULL)
    return curr;

  curr->head = prev;
  if(curr->message->category == category){
    if(length >= curr->message->length){
      memcpy(dest, curr->message->msg, curr->message->length);
      length-=curr->message->length;
      dest += curr->message->length;

      struct messageNode * aux = searchMessageR(curr->tail, queue, prev, category, length, dest);
      free(curr->message->msg);
      free(curr->message);
      free(curr);
      return aux;

    }else{
      memcpy(dest, curr->message->msg, length);
      memcpy(curr->message->msg, curr->message->msg+length, curr->message->length-length);
      curr->message->length -= length;
      return curr;
    }
  }else{
    curr->tail = searchMessageR(curr->tail,queue, curr, category, length, dest);
    if(curr->tail == NULL)
      queue->last = curr;
    return curr;
  }


}

void searchMessage(messageQueueADT queue, int category, int length, char* dest){
  queue->first = searchMessageR(queue->first, queue,  NULL, category, length, dest);
}

void unblockProcesses(messageQueueADT queue, int category, int length){
  struct blockedProcessNode* curr = queue->blockedProcesses;
  while(curr != NULL){
    if( (category == -1) || (curr->waitingCategory == category && isMessageAvailable(queue->first, category, curr->messageSize - length) )){
      //*** Unblock process ***
      process *p = getProcessByPid(curr->processPid);
      unblockProcess(p);

      if(curr->head != NULL)
        curr->head->tail = curr->tail;
      else
        queue->blockedProcesses = curr->tail;

      if(curr->tail != NULL)
        curr->tail->head = curr->head;

      struct blockedProcessNode* aux = curr;
      curr = curr->tail;
      free(aux);
    }else{
      curr = curr->tail;
    }
  }
}

//public

messageQueueADT newMessageQueue(){
  struct queueHeader* newQueue = malloc(sizeof(struct queueHeader));
  newQueue->mutex = mutexInit("");

  newQueue->first = NULL;
  newQueue->last = NULL;
  newQueue->blockedProcesses = NULL;
  return (messageQueueADT)newQueue;
}

void deleteMessageQueue(messageQueueADT queue){
  mutexADT mut = queue->mutex;

  mutexLock(mut);

  //delete messages
  struct messageNode * curr = queue->first;
  while(curr != NULL){
    free(curr->message);
    struct messageNode * aux = curr;
    curr=curr->tail;
    free(aux);
  }

  //unblock all processes
  unblockProcesses(queue, -1, 0);

  free(queue);


  //mutexUnlock(mut);
  mutexClose(mut);
}

void sendMessage(messageQueueADT queue, int category, char * text, int length){
  mutexLock(queue->mutex);
  char * message = malloc(length);
  memcpy(message, text, length);
  struct messageNode *newNode = malloc(sizeof(struct messageNode));
  newNode->tail = NULL;
  newNode->head = queue->last;
  newNode->message = malloc(sizeof(struct msg));
  newNode->message->category = category;
  newNode->message->msg = message;
  newNode->message->length = length;

  if(queue->first == NULL){
    queue->first = newNode;
    queue->last = newNode;
  }else{
    queue->last->tail = newNode;
    queue->last = newNode;
  }


  unblockProcesses(queue, category, length);
  mutexUnlock(queue->mutex);
}

void receiveMessage(messageQueueADT queue, int category, char* dest, int length){
  mutexLock(queue->mutex);
  if(isMessageAvailable(queue->first, category, length) == 0){
    process *p = getCurrentProcess();
    //*** Block process ***

    struct blockedProcessNode *newBlockedProcess = malloc(sizeof(struct blockedProcessNode));
    newBlockedProcess->waitingCategory = category;
    newBlockedProcess->messageSize = length;
    newBlockedProcess->processPid =  getProcessPid(p);
    newBlockedProcess->head = NULL;
    newBlockedProcess->tail = queue->blockedProcesses;
    if(queue->blockedProcesses != NULL)
      queue->blockedProcesses->tail = newBlockedProcess;
    queue->blockedProcesses = newBlockedProcess;

    blockProcess(p);
    mutexUnlock(queue->mutex);
    yieldProcess();
    receiveMessage(queue, category, dest, length);
    return;
  }else{
    searchMessage(queue, category, length, dest);
  }
  mutexUnlock(queue->mutex);
}
