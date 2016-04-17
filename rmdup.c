#include "rmdup.h"

int list_dir(const char * path, int file)
{
   int stdout_fileno = dup(STDOUT_FILENO);
   dup2(file, STDOUT_FILENO); 

  //calling lstdir
  //child will execute and father waits
  pid_t pid = fork();
  
  if(pid == 0)
  {
    if(execl("./lstdir","./lstdir",path,NULL) < 0)
      exit(3);
    fflush(stdout);
    exit(0);
  }
  else if(pid > 0)
  {
     waitpid(pid, NULL, 0);
     dup2(stdout_fileno, STDOUT_FILENO);
  }
  else
  {
    dup2(stdout_fileno, STDOUT_FILENO);
    perror(path);
    exit(-1);
  }
  
  dup2(stdout_fileno, STDOUT_FILENO);
  return 0;
}

int sort_file(const char * filepathaux, int file)
{
  int stdout_fileno = dup(STDOUT_FILENO);
  dup2(file, STDOUT_FILENO);

  //sorting filepathaux to file
  pid_t pid = fork();
  
  if(pid == 0)
  {
    execlp("sort", "sort", filepathaux, NULL); 
    fflush(stdout);
    exit(0);
  }
  else if(pid > 0)
  {
     waitpid(pid, NULL, 0);
     dup2(stdout_fileno, STDOUT_FILENO);
  }
  else
  {
    dup2(stdout_fileno, STDOUT_FILENO);
    perror(filepathaux);
    exit(-1);
  }
  dup2(stdout_fileno, STDOUT_FILENO);
  return 0;
}

void str_split(struct File* stru, char * str)
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
  
  struct stat stat_buf1;
  struct stat stat_buf2;
  lstat(stru1->path, &stat_buf1);
  lstat(stru2->path, &stat_buf2);
  
  if(stat_buf1.st_ino == stat_buf2.st_ino)
    return 0;
  
  //equal files  
  if((strcmp(stru1->name, stru2->name) == 0) && (stru1->size == stru2->size) && (stru1->perm == stru2->perm))
  {
    if(stru1->time < stru2->time)
    {
      if(remove(stru1->path) == -1)
        perror(stru1->path);
      link(stru2->path, stru1->path);
      printf("Deleted %s and created hardlink to %s\n", stru1->path, stru2->path);
      return 1;
    }
    
    else if(stru1->time > stru2->time)
    {
      
      if(remove(stru2->path) == -1)
        perror(stru2->path);
      link(stru1->path, stru2->path);
      printf("Deleted %s and created hardlink to %s\n", stru2->path, stru1->path);
      return 1;
    }
  }
  return 0;


}

int search_lines(int hlinks,const char * path)
{
   int retval = 0;
   FILE *fr1;  
   FILE *fr2;         
   char line1[MAX_BUFF];
   char line2[MAX_BUFF];
   int stdout_fileno = dup(STDOUT_FILENO);
   dup2(hlinks, STDOUT_FILENO);
   
  struct File * Test1 = malloc (sizeof(*Test1)); //struct for file
  struct File * Test2 = malloc (sizeof(*Test2)); //struct for file
  

   fr1 = fopen (path, "rt"); 

   while(fgets(line1, MAX_BUFF, fr1) != NULL)
   {
      str_split(Test1, line1);
   
	    fr2 = fopen (path, "rt"); 
	    while(fgets(line2, MAX_BUFF, fr2) != NULL)
      {
        str_split(Test2, line2);
        if(test_link(Test1, Test2) == 1)
        {
          retval = 1;
        }
      }
   }
   fflush(stdout);
   fclose(fr1);
   fclose(fr2);
   free(Test1);
   free(Test2);
   dup2(stdout_fileno, STDOUT_FILENO);
   return retval;  
}

int main(int argc, char* argv[])
{

  //local variables
  char* filepath = "/tmp/files.txt";        //files.txt path dir
  char* filepathaux = "/tmp/filesaux.txt";  //filesaux.txt path dir
  char* hlink = "/hlinks.txt";              //hardlinks.txt filespath
  int file;                                 //files.txt descriptor
  int fileaux;                              //filesaux.txt descriptor
  int filelink;                             //hlinks.txt descriptor
  struct stat mypath;                       //struct to test path
 
  //creating path for linkspath
  char linkspath[strlen(argv[1]) + strlen(hlink) + 1];
	strcpy(linkspath, argv[1]);
	strcat(linkspath, hlink);

  //argument test
  if(argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    exit(1);
  }
  
  if(linkspath[0] != '/')
  {
    printf("Invalid link, must start with semicolon \n");
    exit(1);
  }
  
  if (stat(argv[1], &mypath) < 0)
  {
    printf("Invalid path, unable to find path\n");
    exit(1);
  }
  else if (!S_ISDIR(mypath.st_mode))
  {
    printf("Invalid path\n");
    exit(1);
  }

  //formating existing files
  remove(linkspath);
  remove(filepathaux);
  remove(filepath);
  
  //creating filesaux in tmp area
  if((fileaux = open(filepathaux, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(filepathaux); 
    exit(2); 
  }
  
  //creating files.txt tmp area
  if((file = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(filepath); 
    exit(2); 
  }
  
  //creating hlinks.txt in tmp area
  if((filelink = open(linkspath, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(linkspath); 
    exit(2); 
  }
   

  //saving stdout_fileno descryptor and writing to file
  if(list_dir(argv[1], fileaux) != 0)
  {
    printf("List Directory error \n");
    exit(3); 
  }
  printf("Listed Directory\n");

  //sorting from filesaux.txt to files.txt
  if(sort_file(filepathaux, file) != 0)
  {
    printf("Sort File error \n"); 
    exit(4);
  }
  printf("Sorted File\n");
  
  //creating hardlinks 
  int ret =  search_lines(filelink, filepath);
  if(ret < 0)
  {
      printf("Error Searching Lines\n");
      exit(5);
  }
  else if(ret == 1)
  {
    printf("Hardlinks Created\n");
  }
  else if(ret == 0)
  {
    printf("No Hardlinks found\n");
  }
  
  
  remove(filepathaux);
  if((fileaux = open(filepathaux, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(argv[1]); 
    exit(2); 
  }
  
 //saving stdout_fileno descryptor and writing to file
  if(list_dir(argv[1], fileaux) != 0)
  {
    printf("List Directory error \n");
    exit(3); 
  }
  printf("Listed Directory\n");
  
  //temporary change to give enter in files.txt
  int stdout_fileno = dup(STDOUT_FILENO);
  dup2(file, STDOUT_FILENO);
  printf("\n.:New Listed\n");
  fflush(stdout);
  dup2(stdout_fileno, STDOUT_FILENO);
  

  //sorting from filesaux.txt to files.txt
  if(sort_file(filepathaux, file) != 0)
  {
    printf("Sort File error \n"); 
    exit(4);
  }
  printf("Sorted File");
   
  //Removing filepath txt file
  remove(filepathaux);
  
  //Closing and exiting
  close(file);
  close(fileaux);
  close(filelink);
  exit(0);  
}

