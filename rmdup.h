#ifndef RMDUP_H
#define RMDUP_H

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

#define MAX_BUFF 1024

struct File
{
  char name[MAX_BUFF];
  int size;
  int inode;
  int perm;
  int time;
  char path[MAX_BUFF];
};

/**
* list_dir will change STDOUT_FILENO to @param file in order to use
* printf to write in the file.
* It uses Multi-processing and calls ./lstdir with @param path as parameter
* Restores STDOUT_FILENO to terminal.
*/

int list_dir(const char * path, int file);

/**
* sort_file will change STDOUT_FILENO to @param file in order to use
* printf to write in the file.
* It calls sort program with @param path filepathaux as a parameter.
* Will sort filepathaux and writes it to file
* Restores STDOUT_FILENO to terminal.
*/

int sort_file(const char * filepathaux, int file);

/**
* str_split splits the line @param str in various elements and saves
* each accordingly to the @param stru struct elements
*/

void str_split(struct File* stru, char * str);

/**
* test_link will test the information passed through @param stru1 struct
* and @param stru2 struct and create the links acordingly
*/

int test_link(struct File* stru1, struct File* stru2);

/**
* searh_lines will change STDOUT_FILENO to @param hlinks in order to
* write every hardlink created to that file using printf
* It will read each line from file @param path and compare all possiblities
* using test_link.
* Uses str_split to split all possibile comparisons into file.
*/

int search_lines(int hlinks,const char * path);

#endif
