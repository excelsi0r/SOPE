#include<dirent.h>
#include<errno.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

 
int main(int argc, char* argv[])
{

  //LOCAL VARIABLES
  char* filepath = "/tmp/files.txt"; //files.txt dir
  int file;                          //file descriptor
  
  //ARGUMENTS TEST
  if(argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    exit(1);
  }
  
  //ELIMINATING FILE
  remove(filepath);
  
  //CREATING files.txt IN TEMP AREA
  if((file = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(argv[1]); 
    exit(2); 
  }
  
  //CHANGING DESCRYPTOR OF STDOUT_FILENO TO file.txt IN ORDER TO WRITE THE DIR
  dup2(file, STDOUT_FILENO); 
  
  //CALLING LSTDIR
  
  if(execl("./lstdir","./lstdir",argv[1],NULL) == -1)
    printf("error");
    
  execl("sort", "sort", filepath, NULL);
  exit(0);  
}

