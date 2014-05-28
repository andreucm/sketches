#ifndef COMMCONSTANTS_H
#define COMMCONSTANTS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <iostream>
#include <limits.h> //PIPE_BUF constant

static const char FIFONAME[20] = "/tmp/my_fifo"; //fifo name in the computer file system
static const unsigned int UINTSIZE = sizeof(unsigned int); //size of unsigned int in Bytes
static const unsigned int FLOATSIZE = sizeof(float); //size of float in Bytes
static const unsigned int NFLOATS = 1022; //number of floats to send per packet
static const unsigned int PACKETSIZE = 1 + UINTSIZE + NFLOATS * FLOATSIZE; //packet size = serviceId + clientId + floatLoad 
static const unsigned int NREQUESTS = 10; //number of requests the client will perform

union uniFloat {
  float f;
  char c[FLOATSIZE];
};

union uniUint {
  float i;
  char c[UINTSIZE];
};

#endif
