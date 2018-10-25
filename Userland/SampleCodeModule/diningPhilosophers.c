#include <exitProcess.h>
#include <mutex.h>
#include <stdio.h>
#include <processExec.h>

#define N 20
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + numberOfPhilosophers-1) % numberOfPhilosophers
#define RIGHT (phnum + 1) % numberOfPhilosophers

int state[N];
int numberOfPhilosophers;

void * mutex;
void * S[N];

int finished =0;

void test(int phnum)
{
    if (state[phnum] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {
        // state that eating
        state[phnum] = EATING;


        printf("Philosopher %d takes fork %d and %d\n",
                      phnum + 1, LEFT + 1, phnum + 1);

        printf("Philosopher %d is Eating\n", phnum + 1);

        mutexUnlock(S[phnum]);
    }
}

// take up chopsticks
void take_fork(int phnum)
{

    mutexLock(mutex);

    // state that hungry
    state[phnum] = HUNGRY;

    printf("Philosopher %d is Hungry\n", phnum + 1);

    // eat if neighbours are not eating
    test(phnum);

    mutexUnlock(mutex);

    // if unable to eat wait to be signalled
    mutexLock(S[phnum]);


}

// put down chopsticks
void put_fork(int phnum)
{

    mutexLock(mutex);

    // state that thinking
    state[phnum] = THINKING;

    printf("Philosopher %d putting fork %d and %d down\n",
           phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);

    test(LEFT);
    test(RIGHT);

    mutexUnlock(mutex);
}

void philosopher(int argc, char**argv)
{
    int * num = &argc;
    while(1) {

        int* i = num;

        mutexLock(mutex);
        if(numberOfPhilosophers < *i+1 || finished){
          printf("Philosopher %d is leaving the table. (%d)\n", *i+1, numberOfPhilosophers);
          if(numberOfPhilosophers == 0 && *i == 0 && finished==1){
            mutexClose(mutex);
            exitProcess();
          }
          mutexUnlock(mutex);
          exitProcess();
        }
        mutexUnlock(mutex);

        take_fork(*i);



        put_fork(*i);
    }

}

void addPhilosopher(){
  mutexLock(mutex);
  S[numberOfPhilosophers] = mutexInit("");
  mutexLock(S[numberOfPhilosophers]);
  execProcess(philosopher, numberOfPhilosophers, 0, "philosopher", 0,0,0);
  printf("Philosopher %d is thinking\n", numberOfPhilosophers + 1);
  numberOfPhilosophers++;
  mutexUnlock(mutex);
}

int diningPhilosophers(int argc, char** argv)
{
  printf("'a'/'r' to add/remove a philosopher.\n");
  finished=0;
    int i;
    numberOfPhilosophers=0;
    // initialize
    mutex = mutexInit("");

    char c;
    while(1){
      c = getchar();
      switch (c) {
        case 'a':
          if(numberOfPhilosophers == 0){
            addPhilosopher();
            addPhilosopher();
          }else if(numberOfPhilosophers < N){
            addPhilosopher();
          }
          break;
        case 'r':
          mutexLock(mutex);
          if(numberOfPhilosophers==2 ){
            numberOfPhilosophers=0;
            printf("Table is empty.\n");
            putchar(' ');
          }else{
            numberOfPhilosophers--;
            mutexClose(S[numberOfPhilosophers]);
          }

          mutexUnlock(mutex);
          break;
        case 'q':
          printf("Exiting philosophers.\n");
          mutexLock(mutex);
          numberOfPhilosophers=0;
          finished = 1;
          mutexUnlock(mutex);
          exitProcess();

          break;
      }
    }
}
