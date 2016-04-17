#ifndef LSTDIR_H
#define LSTDIR_H

#include<dirent.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

/**
* Lists Directory given from @param filepath to STDOUT_FILENO
*/

int lstdir(const char * filepath);

#endif
