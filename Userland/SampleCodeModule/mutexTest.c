#include <mutex.h>
#include <processExec.h>
#include <stdio.h>
#include <exitProcess.h>

void test1Proc(int argc, char ** argv){
  //void * mutex = (void*)argc;
  printf("Starting test1.\n");
  void * mutex = mutexInit("mutexTest1");
  for(int i=0; i < 6; i++){
    //printf("Asking for lock.\n");
    mutexLock(mutex);
    printf("Locking.\n");
    //printPids();
    printf("Unlocking.\n");
    mutexUnlock(mutex);
    //printf("Lock ended.\n");
  }

  printf("Exiting. \n");
  exitProcess();
}

void test1(){
  void * mutex = mutexInit("mutexTest1");

  execProcess(test1Proc, (int)mutex, NULL, "test1-1", 0, 0, 0);
  execProcess(test1Proc, (int)mutex, NULL, "test1-2", 0, 0, 0);

}

void test2Proc(int argc, char ** argv){
  printf("Starting test2.\n");
  void * mutex = mutexInit("mutexTest2");
  //printPids();
  mutexLock(mutex);
  printf("Locked.\n");
  for(int i=0; i < 1000000000; i++){

  }
  printf("Unlocked.\n");
  mutexUnlock(mutex);
  exitProcess();
}

void test2(){
  void * mutex = mutexInit("mutexTest2");
  execProcess(test2Proc, (int)mutex, NULL, "test2-1", 0, 0, 0);
  execProcess(test2Proc, (int)mutex, NULL, "test2-2", 0, 0, 0);
  execProcess(test2Proc, (int)mutex, NULL, "test2-3", 0, 0, 0);

}



void mutexTest(int argc, char ** argv){
  printf("Running mutex test. Press '1' to run test1, '2' to run test2 and 'q' to quit.\n");
  char c;
  while(1){
    c = getchar();
    switch (c) {
      case '1':
        printf("Running test 1.\n");
        test1();
        break;
      case '2':
        printf("Running test 2");
        test2();
        break;
      case 'q':
        printf("Exiting mutexTest.\n");
        exitProcess();
        break;

      default:
      break;
    }
  }
}
