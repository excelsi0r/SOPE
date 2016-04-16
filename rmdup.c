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
  
  //time
  token = strtok(NULL, search);
  stru->time = atoi(token);
  
  //size
  token = strtok(NULL, search);
  stru->size = atoi(token);
   
  //permission
  token = strtok(NULL, search);
  stru->perm = atoi(token);
  
  //inode
  token = strtok(NULL, search);
  stru->inode = atoi(token);
  
  //path
  token = strtok(NULL, search);
  strcpy(stru->path, token);
}

int test_link(struct File* stru1, struct File* stru2)
{
  if(strcmp(stru1->path, stru2->path) == 0)
  {
    return 0;
  }
    
  //equal files  
  if((strcmp(stru1->name, stru2->name) == 0) && (stru1->size == stru2->size) && (stru1->perm == stru2->perm))
  {
    if(stru1->time < stru2->time)
    {
      if(remove(stru1->path) == -1)
        perror(stru1->path);
      link(stru2->path, stru1->path);
      printf("Deleted %s and created hardlink to %s\n", stru1->path, stru2->path);
      return 0;
    }
    
    else if(stru1->time > stru2->time)
    {
      
      if(remove(stru2->path) == -1)
        perror(stru2->path);
      link(stru1->path, stru2->path);
      printf("Deleted %s and created hardlink to %s\n", stru2->path, stru1->path);
      return 0;
    }
  }
  
  return 0;


}

int search_lines(char path[])
{
   FILE *fr1;  
   FILE *fr2;         
   char line1[512];
   char line2[512];
   
  struct File * Test1 = malloc (sizeof(*Test1)); //struct for file
  struct File * Test2 = malloc (sizeof(*Test2)); //struct for file
  

   fr1 = fopen (path, "rt"); 

   while(fgets(line1, 512, fr1) != NULL)
   {
      str_split(Test1, line1);
   
	    fr2 = fopen (path, "rt"); 
	    while(fgets(line2, 512, fr2) != NULL)
      {
        str_split(Test2, line2);
        test_link(Test1, Test2);
      }
   }
   fclose(fr1);
   fclose(fr2);
   return 0;  
}

int main(int argc, char* argv[])
{

  //LOCAL VARIABLES
  char* filepath = "/tmp/files.txt";        //files.txt path dir
  char* filepathaux = "/tmp/filesaux.txt";  //filesaux.txt path dir
  char *hlink = "/hlinks.txt";              //hardlinks
  int file;                                 //files.txt descriptor
  int fileaux;                              //filesaux.txt descriptor
  int filelink;
  int stdout_fileno;                        //stdout_fileno saved
  
  char linkspath[strlen(argv[1]) + strlen(hlink) + 1];
	strcpy(linkspath, argv[1]);
	strcat(linkspath, hlink);
  

  //ARGUMENTS TEST
  if(argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    exit(1);
  }
  

  //ELIMINATING FILES (FORMATING)
  remove(hlink);
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
  
  
  //CREATING files.txt IN TEMP AREA
  if((filelink = open(linkspath, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(linkspath); 
    exit(2); 
  }
  
  dup2(filelink, STDOUT_FILENO);
  
  //CREATING HARDLINKS
  search_lines(filepath);
  
  
  //==========================================================================================================================
  //CREATING filesaux.txt IN TEMP AREA
  if((fileaux = open(filepathaux, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(argv[1]); 
    exit(2); 
  }
  
  dup2(fileaux, STDOUT_FILENO); 
  
  //CALLING LSTDIR
  //CHILD WILL EXECUTE LSTDIR WHILE FATHER WAITS UNTIL CHILD IS DONE
  pid = fork();
  
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
  
  //RESTORE STDOUT_FILENO
  dup2(stdout_fileno, STDOUT_FILENO);

  
  //Closing and exiting
  close(file);
  close(fileaux);
  exit(0);  
  
}

