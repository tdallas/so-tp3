#include "messageQueueADT.h"
#include "message.h"
#include "pipesADT.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();

int main(){
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  printf("\nFin\n");
  test8();
}

void test1(){
  messageQueueADT queue = newMessageQueue();

  char * msg1 = "hola como andas.";
  sendMessage(queue, 9, msg1, strlen(msg1));

  char response1[5];
  receiveMessage(queue, 9, response1, 5);
  assert(strcmp("hola ", response1) == 0);

  char response2[5];
  receiveMessage(queue, 9, response2, 5);
  assert(strcmp("como ", response2) == 0);

  char response3[6];
  receiveMessage(queue, 9, response3, 6);
  assert(strcmp("andas.", response3) == 0);

  deleteMessageQueue(queue);
}

void test2(){
  messageQueueADT queue = newMessageQueue(10);

  char * msg1 = "hola";
  sendMessage(queue, 9, msg1, strlen(msg1));

  char response[9];
  char responseBackup[9];
  responseBackup[8]=0;
  response[8]=0;
  memcpy(responseBackup, response, 8);
  receiveMessage(queue, 9, response, 8);
  assert(strcmp(responseBackup, response) == 0);

  char * msg2 = "como";
  sendMessage(queue, 9, msg2, strlen(msg2));

  receiveMessage(queue, 9, response, 8);
  assert(strcmp("holacomo", response) == 0);

  deleteMessageQueue(queue);
}

void test3(){
  messageQueueADT queue1 = newMessageQueue();

  char * msg1 = "hola   ";
  sendMessage(queue1, 9, msg1, strlen(msg1));

  char response[9];
  char responseBackup[9];
  responseBackup[8]=0;
  response[8]=0;
  memcpy(responseBackup, response, 8);
  receiveMessage(queue1, 8, response, 4);
  assert(strcmp(responseBackup, response) == 0);

  deleteMessageQueue(queue1);
}

void test4(){
  pipeADT pipe = newPipe();

  char * msg1 = "hola como andas.";
  sendMessagePipe(pipe, msg1, strlen(msg1));

  char response1[6];
  response1[5]=0;
  receiveMessagePipe(pipe, response1, 5);
  assert(strcmp("hola ", response1) == 0);

  char * msg2 = "pepe.";
  sendMessagePipe(pipe, msg2, strlen(msg2));

  char response2[5];
    response2[5]=0;
  receiveMessagePipe(pipe, response2, 5);
  assert(strcmp("como ", response2) == 0);

  char response3[7];
    response3[6]=0;
  receiveMessagePipe(pipe, response3, 6);
  assert(strcmp("andas.", response3) == 0);

  char response4[5];
    response4[4]=0;
  receiveMessagePipe(pipe, response4, 4);
  assert(strcmp("pepe", response4) == 0);

  deletePipe(pipe);
}

void test5(){
  pipeADT pipe = newPipe();

  char * msg1 = "hola como andas.Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  sendMessagePipe(pipe, msg1, strlen(msg1));

  char response;
  while(*msg1 != 0){
    receiveMessagePipe(pipe, &response, 1);
    assert(*msg1 == response);
    msg1++;
  }


  deletePipe(pipe);
}

void test6(){
  messageQueueADT queue = newMessageQueue();

  char * msg1 = "hola como andas.";
  sendMessage(queue, 9, msg1, strlen(msg1));

  char * msg2 = " Yo ando muy bien, gracias.";
  sendMessage(queue, 9, msg2, strlen(msg2));

  char * msg3 = " Hola hola hola.";
  sendMessage(queue, 9, msg3, strlen(msg3));

  int length = strlen(msg1)+strlen(msg2)+ strlen(msg3);
  char response3[length + 1];
  response3[length]=0;
  receiveMessage(queue, 9, response3, length);
  assert(strcmp("hola como andas. Yo ando muy bien, gracias. Hola hola hola.", response3) == 0);

  deleteMessageQueue(queue);
}

void test7(){
  messageQueueADT queue = newMessageQueue();

  char * msg1 = "hola como andas.Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  sendMessage(queue, 9, msg1, strlen(msg1));

  char response;
  while(*msg1 != 0){
    receiveMessage(queue, 9, &response, 1);
    assert(*msg1 == response);
    msg1++;
  }


  deleteMessageQueue(queue);
}


pipeADT pipe;

void* test8Cons(void*arg){
  char response[4];
  response[3]=0;
  for(int j=0; j <11; j++){
    response[0]='a';
    response[1]='a';
    response[2]='a';
    receiveMessagePipe(pipe, response, 3);
    printf("Consuming item: %s\n", response);
  }

  pthread_exit(NULL);
}

void *test8Prod(void*arg){
  for(int j=0; j < 10; j++){
    sendMessagePipe(pipe, "ABC", 3);
    printf("Inserting item: %d\n", j);
  }

  pthread_exit(NULL);
}

void test8(){
  int division = 1;
  pthread_t threads[division*2];
  //prodcons
  pipe = newPipe();

  for(int i=0; i < division; i++){
    pthread_create(&threads[i], NULL, test8Prod, NULL);
  }

  for(int i=0; i < division; i++){
    pthread_create(&threads[i+division], NULL, test8Cons, NULL);
  }

  for(int i=0; i < division*2; i++){
    pthread_join(threads[i], NULL);
  }

  deletePipe(pipe);

}
