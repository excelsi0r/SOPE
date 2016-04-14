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

int lstdir(const char * filepath)
{
  DIR *dirp;                         //directory file
  struct dirent *direntp;            //dirent struct
  struct stat stat_buf;              //stat struct
  
  
  if((dirp = opendir(filepath)) == NULL) 
  { 
      perror(filepath); 
      exit(3); 
  } 

  while((direntp = readdir(dirp)) != NULL) 
  { 
    char filestr[strlen(filepath)+strlen(direntp->d_name)];                 //file path
    sprintf(filestr,"%s/%s",filepath,direntp->d_name); 
    
    if(lstat(filestr, &stat_buf)==-1)   
    { 
     perror("lstat ERROR"); 
     exit(3); 
    } 
    
    if(strcmp(direntp->d_name, ".") !=0 && strcmp(direntp->d_name, "..") != 0)
    {
      if (S_ISREG(stat_buf.st_mode)) 
      {
        printf("%-35s - %6li - ", direntp->d_name, stat_buf.st_size); 
        printf("%04o - %li - %s\n", stat_buf.st_mode & 0777, stat_buf.st_mtime,filestr);
        
      }
      else if(S_ISDIR(stat_buf.st_mode))
      {
        pid_t pid = fork();
        
        if(pid > 0)
        {
          strcat(filestr, "/");
          lstdir(filestr);
          exit(0);
        }
        else if(pid == 0)
        {
          waitpid(pid, NULL,0);
        }
        
      }
    }
    
  } 

  //CLOSING AND EXITING
  closedir(dirp);
  return 0;
}

int main(int argc, char* argv[])
{
  //ARGUMENTS TEST
  if(argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    exit(1);
  }
  
  if(lstdir(argv[1]) > 0)
    exit(2);
    
  return 0; 
}
