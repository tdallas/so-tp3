#include <messageQueueADT.h>

struct messageNode{
  struct messageNode * tail;
  struct messageNode * head;
  struct msg * message;
};

struct pipeListHeader{
  mutexADT mutex;
  struct pipeNode * first;
}


struct pipeNode{
  struct pipeNode * tail;

  messageQueueADT messageList;
  uint64_t inId;
  uint64_t outId;
}

struct pipeHeader pipeList;


pipeADT newPipe(int fd[2]){

}
