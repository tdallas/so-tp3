
#include <mutex.h>
#include <videoDriver.h>
#include <pipesADT.h>
#include <lib.h>
#include <scheduler.h>
#include <processes.h>

#define MESSAGE_BLOCK_SIZE 100

struct pipeHeader{
  struct messageBlock * firstBlock;
  struct messageBlock * lastBlock;
  mutexADT mutex;
  struct blockedProcessNode * blockedProcesses;
  int size;
};

struct messageBlock{
  struct messageBlock * tail;

  int readIndex;
  int writeIndex;
  char msg[MESSAGE_BLOCK_SIZE];
};

struct blockedProcessNode{
  struct blockedProcessNode * tail;
  struct blockedProcessNode * head;
  int processPid;
  int messageSize;
};

pipeADT newPipe(){
  struct pipeHeader * newPipe = malloc(sizeof(struct pipeHeader));
  newPipe->firstBlock=NULL;
  newPipe->lastBlock=NULL;
  newPipe->mutex=mutexInit("");
  newPipe->blockedProcesses=NULL;
  newPipe->size = 0;
  return (pipeADT)newPipe;
}

struct messageBlock* newBlock(){
  struct messageBlock * newMessageBlock = malloc(sizeof(struct messageBlock));
  newMessageBlock->tail = NULL;
  newMessageBlock->readIndex = 0;
  newMessageBlock->writeIndex = 0;
  return newMessageBlock;
}

void unblockProcessesPipe(pipeADT pipe, int all){
  struct blockedProcessNode* curr = pipe->blockedProcesses;
  int length = pipe->size;
  while(curr != NULL){
    if( curr->messageSize <= length || all != 0){
      //*** Unblock process ***
      process *p = getProcessByPid(curr->processPid);
      unblockProcess(p);

      if(curr->head != NULL)
        curr->head->tail = curr->tail;
      else
        pipe->blockedProcesses = curr->tail;

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

void sendMessagePipe(pipeADT pipe, char * text, int length){
  mutexLock(pipe->mutex);
  pipe->size += length;

  if(pipe->lastBlock == NULL){
    struct messageBlock * newMessageBlock = newBlock(pipe);
    pipe->firstBlock = newMessageBlock;
    pipe->lastBlock = newMessageBlock;
  }
  struct messageBlock * curr = pipe->lastBlock;
  while(length != 0){
    if(curr->writeIndex == MESSAGE_BLOCK_SIZE){
      struct messageBlock * newMessageBlock = newBlock();
      pipe->lastBlock->tail = newMessageBlock;
      pipe->lastBlock = newMessageBlock;
      curr = newMessageBlock;
    }else{
      int writeLength = (length > (MESSAGE_BLOCK_SIZE - curr->writeIndex)? MESSAGE_BLOCK_SIZE-curr->writeIndex : length);


      memcpy(&curr->msg[curr->writeIndex], text, writeLength);


      length-=writeLength;
      text+=writeLength;
      curr->writeIndex += writeLength;
    }

  }

  unblockProcessesPipe(pipe, 0);
  mutexUnlock(pipe->mutex);
}

void receiveMessagePipe(pipeADT pipe, char * text, int length){
  mutexLock(pipe->mutex);
  if(pipe->size < length){

    process *p = getCurrentProcess();
    //*** Block process ***

    struct blockedProcessNode *newBlockedProcess = malloc(sizeof(struct blockedProcessNode));
    newBlockedProcess->messageSize = length;
    newBlockedProcess->processPid =  getProcessPid(p);
    newBlockedProcess->head = NULL;
    newBlockedProcess->tail = pipe->blockedProcesses;
    if(pipe->blockedProcesses != NULL)
      pipe->blockedProcesses->head = newBlockedProcess;
    pipe->blockedProcesses = newBlockedProcess;

    blockProcess(p);
    mutexUnlock(pipe->mutex);
    yieldProcess();
    receiveMessagePipe(pipe, text, length);
    return;
  }

  pipe->size -= length;

  struct messageBlock * curr = pipe->firstBlock;
  while(length != 0){
    if(curr->readIndex == curr->writeIndex){
      struct messageBlock * next = curr->tail;
      free(curr);
      curr = next;
      pipe->firstBlock = curr;
    }else{
      int readLength = (length >= (curr->writeIndex - curr->readIndex)? curr->writeIndex-curr->readIndex : length);
      memcpy(text, &curr->msg[curr->readIndex], readLength);
      length-=readLength;
      text+=readLength;
      curr->readIndex += readLength;
    }
  }

  mutexUnlock(pipe->mutex);
}

void deletePipe(pipeADT pipe){
  mutexLock(pipe->mutex);
  struct messageBlock * curr = pipe->firstBlock;
  while(curr!=NULL){
    struct messageBlock * next = curr->tail;
    free(curr);
    curr = next;
  }
  unblockProcessesPipe(pipe, 1);
  free(pipe);
  mutexClose(pipe->mutex);
}

int lengthAvailablePipe(pipeADT pipe){
  return pipe->size;
}
