#include <exitProcess.h>
#include <stdio.h>
#include <messages.h>
#include <processExec.h>

#include <shell.h>

void msgtest1Proc(int argc, void**argv);

void msgtest1(){
  int processes = 7;
  printf("Running %d processes.\n", processes);
  int p1 = getPid();
  int * parameter = malloc(sizeof(int));
  *parameter = p1;

  int processesPids[processes];
  for(int i=0; i< processes; i++){
    processesPids[i] = execProcess(msgtest1Proc, 1, (char**)&parameter, "hijo", 0,0,0);
  }


  char *msg = "holaaa\n";

  printf("Sending: %s\n", msg);
  for(int i=0; i< processes; i++){

    send(processesPids[i], msg, strlenUserland(msg));

  }

}

void msgtest1Proc(int argc, void**argv){
  int **p1_pointer = (argv);
  int p1 = **p1_pointer;
  char msg[BUFFERSIZE+1];
  int index=6;

  receive(p1, msg, index);


  msg[index]=0;

  printf("Proceso %d: %s\n", getPid(), msg);
  exitProcess();
}

void messageTest(int argc, char **argv){
    printf("Running message test. Press '1' to run test and 'q' to quit.\n");
    char c;
    while(1){
      c = getchar();
      switch (c) {
        case '1':
          printf("Running test 1.\n");
          msgtest1();
          break;
        case '2':
          printf("Running test 2");
          //test2();
          break;
        case 'q':
          printf("Exiting messageTest.\n");
          exitProcess();
          break;

        default:
        break;
      }
    }
}
