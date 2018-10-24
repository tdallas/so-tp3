#ifndef FILEDESCRIPTORS_H
#define FILEDESCRIPTORS_H

#include <pipesADT.h>

struct fileDescriptors{
  uint64_t stdin;
  uint64_t stdout;
  //pipeADT stderr;
};

#endif
