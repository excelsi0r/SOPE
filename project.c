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
#include"lstdir.c"
 
int main(int argc, char* argv[])
{

  //LOCAL VARIABLES
  char* filepath = "/tmp/files.txt"; //files.txt dir
  int file;                          //file descriptor
  
  DIR *dirp;                         //directory file


  //ARGUMENTS TEST
  if(argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    exit(1);
  }
  
  //CREATING files.txt IN TEMP AREA
  if((file = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0600)) < 0)
  {
    perror(argv[1]); 
    exit(2); 
  }
  
  if((dirp = opendir( argv[1])) == NULL) 
  { 
      perror(argv[1]); 
      exit(3); 
  } 
  
  if(lstdir(dirp,argv[1]) == 0)
    exit(4);
  
  //CHANGING DESCRYPTOR OF STDOUT_FILENO TO file.txt IN ORDER TO WRITE THE DIR
  //dup2(file, STDOUT_FILENO); 
   
  //OPENING DIRECTORY
  /*
  if((dirp = opendir( argv[1])) == NULL) 
  { 
      perror(argv[1]); 
      exit(3); 
  } 
  
  
  
  while((direntp = readdir(dirp)) != NULL) 
  { 
    sprintf(filestr,"%s/%s",argv[1],direntp->d_name); 
    
    if(lstat(filestr, &stat_buf)==-1)   
    { 
     perror("lstat ERROR"); 
     exit(3); 
    } 

    
    if (S_ISREG(stat_buf.st_mode)) 
    {
      printf("%-35s - %6li - ", direntp->d_name, stat_buf.st_size); 
      char permission[100];
      sprintf(permission, "%o", stat_buf.st_mode);
      printf("%s - %li - %s\n", permission, stat_buf.st_mtime,filestr);
      
    }
    
    
  } 
  
  //CLOSING AND EXITING
  closedir(dirp); 
  */
  exit(0);  
}

