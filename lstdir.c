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


int lstdir(DIR *dirp, char * folder)
{

  struct dirent *direntp;            //dirent struct
  struct stat stat_buf;              //stat struct
  char filestr[200];                 //file path
 

  //CHANGING DESCRYPTOR OF STDOUT_FILENO TO file.txt IN ORDER TO WRITE THE DIR
  //dup2(file, STDOUT_FILENO); 

  while((direntp = readdir(dirp)) != NULL) 
  { 
    sprintf(filestr,"%s/%s",folder,direntp->d_name); 
    
    if(lstat(filestr, &stat_buf)==-1)   
    { 
     perror("lstat ERROR"); 
     exit(3); 
    } 
 
    if (S_ISREG(stat_buf.st_mode)) 
    {
      printf("%-35s - %6li - ", direntp->d_name, stat_buf.st_size); 
      printf("%04o - %li - %s\n", stat_buf.st_mode & 0777, stat_buf.st_mtime,filestr);
      
    }  
  } 

  //CLOSING AND EXITING
  closedir(dirp);
  return 0; 
 }
