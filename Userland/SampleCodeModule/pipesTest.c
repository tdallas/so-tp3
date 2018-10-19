#include <exitProcess.h>
#include <stdio.h>
#include <pipes.h>
#include <processExec.h>

#include <shell.h>

void pipetest1Proc(int argc, char**argv);

void pipetest1(){
  int processes = 7;
  printf("Running %d processes.\n", processes);
  int p1 = getPid();
  int * parameter = malloc(sizeof(int));
  *parameter = p1;

  int processesPids[processes];
  for(int i=0; i< processes; i++){
    processesPids[i] = execProcess(pipetest1Proc, 1, &parameter, "hijo", 0);
  }


  char *pipe = "holaaa\n";

  printf("Sending: %s\n", pipe);
  for(int i=0; i< processes; i++){

    sendMessagePipe(processesPids[i], pipe, strlenUserland(pipe));

  }

}

void pipetest1Proc(int argc, char**argv){
  int **p1_pointer = ((void**)argv);
  int p1 = **p1_pointer;
  char pipe[BUFFERSIZE+1];
  int index=6;

  receive(p1, pipe, index);


  pipe[index]=0;

  printf("Proceso %d: %s\n", getPid(), pipe);
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
