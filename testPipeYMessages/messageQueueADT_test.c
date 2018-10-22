#include "messageQueueADT.h"
#include "message.h"
#include "pipesADT.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>


void test1();
void test2();
void test3();
void test4();

int main(){
  test1();
  test2();
  test3();
  test4();
  printf("Fin\n");
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

}

void test4(){
  pipeADT pipe = newPipe();

  char * msg1 = "hola como andas.";
  sendMessagePipe(pipe, msg1, strlen(msg1));

  char response1[6];
  response1[5]=0;
  receiveMessagePipe(pipe, response1, 5);
  printf("Respuesta 1: %s\n", response1);
  assert(strcmp("hola ", response1) == 0);

  char * msg2 = "pepe.";
  sendMessagePipe(pipe, msg2, strlen(msg2));

  char response2[5];
    response2[5]=0;
  receiveMessagePipe(pipe, response2, 5);
  printf("Respuesta 2: %s\n", response2);
  assert(strcmp("como ", response2) == 0);

  char response3[7];
    response3[6]=0;
  receiveMessagePipe(pipe, response3, 6);
  printf("Respuesta 3: %s\n", response3);
  assert(strcmp("andas.", response3) == 0);

  char response4[5];
    response4[4]=0;
  receiveMessagePipe(pipe, response4, 4);
  printf("Respuesta 4: %s\n", response4);
  assert(strcmp("pepe", response4) == 0);
}

void test5(){

}
