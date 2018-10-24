#include <stdio.h>
#include <pipes.h>
#include <mutex.h>
#include <exitProcess.h>


fileDescriptorsTestProcProd(int argc, char** argv){
  for(int i=0; i < 5; i++){
    printf("Item");
  }
  exitProcess();
}

fileDescriptorsTestProcCons(int argc, char** argv){
  char item[5];
  item[4]=0;
  for(int i=0; i < 10; i++){
    printf("Consumido: ");
    for(int j=0; j<4; j++){
      char c = getchar();
      if(c == 0){
        printf("No hay mas.");
        exitProcess();
      }
      putchar(c);
    }

    printf("\n");
    //printf("Consumido: %s\n");
  }
exitProcess();
}

fileDescriptorsTest(int argc, char** argv){
  int pipe = newPipe();
  putchar(' ');
  execProcess(fileDescriptorsTestProcProd, 0, 0, "prod", 0,0,pipe);
  execProcess(fileDescriptorsTestProcCons, 0, 0, "cons", 0,pipe,0);
  exitProcess();
}
