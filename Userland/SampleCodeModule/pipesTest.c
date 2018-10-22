#include <exitProcess.h>
#include <stdio.h>
#include <pipes.h>
#include <processExec.h>

#include <shell.h>

void pipetest1Proc(int argc, char**argv);

void pipetest1(){
  int pipe = newPipe();
  int processes = 4;
  printf("Running %d processes.\n", processes);

  int * parameter = malloc(sizeof(int));
  *parameter = pipe;

  int processesPids[processes];
  for(int i=0; i< processes; i++){
    processesPids[i] = execProcess(pipetest1Proc, 1, &parameter, "hijo", 0);
  }


  char *msg = "holaaa\n";
  printf("Pipe: %d\n", pipe);
  printf("Sending: %s\n", msg);
  for(int i=0; i< processes; i++){

    sendMessagePipe(pipe, msg, 7);

  }

}

void pipetest1Proc(int argc, char**argv){
  int **p1_pointer = ((void**)argv);
  int p1 = **p1_pointer;
  char msg[8];
  int index=7;
  printf("Pipe: %d\n", p1);
  receiveMessagePipe(p1, msg, index);

  msg[index]=0;

  printf("Proceso %d: %s\n", getPid(), msg);
  exitProcess();
}

void pipeTest(int argc, char **argv){
    printf("Running pipe test.\n");
    char c;
    while(1){
      c = getchar();
      switch (c) {
        case '1':
          printf("Running test 1.\n");
          pipetest1();
          break;
        case '2':
          printf("Running test 2");
          //test2();
          break;
        case 'q':
          printf("Exiting pipeTest.\n");
          exitProcess();
          break;

        default:
        break;
      }
    }
}
