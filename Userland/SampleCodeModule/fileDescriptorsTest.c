#include <stdio.h>
#include <pipes.h>
#include <mutex.h>
#include <exitProcess.h>


fileDescriptorsTestProcProd(int argc, char** argv){
  for(int i=0; i < 8; i++){
    printf("Item");
  }
  putchar('\0');
  exitProcess();
}

fileDescriptorsTestProcCons(int argc, char** argv){
  char item[5];
  item[4]=0;
  for(int i=0; i < 11; i++){
    printf("Consumido: ");
    for(int j=0; j<4; j++){
      char c = getchar();
      if(c == '\0'){
        //putchar(' ');
        printf("No hay mas.");
        exitProcess();
      }
      putchar(c);
    }

    printf("\n");
  }
  putchar('%');
exitProcess();
}

fileDescriptorsTest(int argc, char** argv){
  int pipe = newPipe();
  putchar(' ');
  execProcess(fileDescriptorsTestProcProd, 0, 0, "prod", 0,0,pipe);
  execProcess(fileDescriptorsTestProcCons, 0, 0, "cons", 0,pipe,0);
  exitProcess();
}
