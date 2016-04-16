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

#define MAX_BUFF 256

struct File
{
  char name[MAX_BUFF];
  int size;
  int inode;
  int perm;
  int time;
  char path[MAX_BUFF];
};

void str_split(struct File* stru, char str[])
{

  char *token;
  char *search = " - ";
    
  //name
  token = strtok(str, search);
  strcpy(stru->name, token);
  
  //size
  token = strtok(NULL, search);
  stru->size = atoi(token);
  
  //inode
  token = strtok(NULL, search);
  stru->inode = atoi(token);
  
  //permission
  token = strtok(NULL, search);
  stru->perm = atoi(token);
  
  //time
  token = strtok(NULL, search);
  stru->time = atoi(token);
  
  //path
  token = strtok(NULL, search);
  strcpy(stru->path, token);
}




int main(int argc, char* argv[])
{

  //LOCAL VARIABLES
  char* filepath = "/tmp/files.txt";        //files.txt path dir
  char* filepathaux = "/tmp/filesaux.txt";  //filesaux.txt path dir
  int file;                                 //files.txt descriptor
  int fileaux;                              //filesaux.txt descriptor
  int stdout_fileno;                        //stdout_fileno saved
  
  struct File * Test1 = malloc (sizeof(*Test1)); //struct for file
  struct File * Test2 = malloc (sizeof(*Test2)); //struct for file
  
  //ARGUMENTS TEST
  if(argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    exit(1);
  }
  
  //ELIMINATING FILES (FORMATING)
  remove(filepathaux);
  remove(filepath);
  
  //CREATING filesaux.txt IN TEMP AREA
  if((fileaux = open(filepathaux, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(argv[1]); 
    exit(2); 
  }
  
  //CREATING files.txt IN TEMP AREA
  if((file = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(argv[1]); 
    exit(2); 
  }
  
  //CHANGING DESCRYPTOR OF STDOUT_FILENO TO fileaux.txt IN ORDER TO WRITE THE DIR
  stdout_fileno = dup(STDOUT_FILENO);
  dup2(fileaux, STDOUT_FILENO); 
  
  //CALLING LSTDIR
  //CHILD WILL EXECUTE LSTDIR WHILE FATHER WAITS UNTIL CHILD IS DONE
  pid_t pid = fork();
  
  if(pid == 0)
  {
    if(execl("./lstdir","./lstdir",argv[1],NULL) < 0)
      exit(3);
    exit(0);
  }
  else if(pid > 0)
  {
     waitpid(pid, NULL, 0);
  }
  
  //CHANGING DESCRYPTOR OF STDOUT_FILENO TO files.txt IN ORDER TO WRITE THE DIR
  dup2(file, STDOUT_FILENO); 
 
  //SORTING filesaux.txt AND SAVING TO files.txt
  pid = fork();
  
  if(pid == 0)
  {
    execlp("sort", "sort", filepathaux, NULL); 
    exit(0);
  }
  else if(pid > 0)
  {
     waitpid(pid, NULL, 0);
  }
   
  //Removing filepath txt file
  remove(filepathaux);
  
  //CREATING HARDLINKS
  //str_split(Test1, str);
  
  //RESTORE STDOUT_FILENO
  dup2(stdout_fileno, STDOUT_FILENO);
  printf("HELOO");
  
  
  close(file);
  close(fileaux);
  exit(0);  
  
}

