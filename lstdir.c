#include "lstdir.h"

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
      if(access(filestr, W_OK) == 0 && access(filestr, R_OK) == 0)
        {
        printf("%-35s - ", direntp->d_name);        //name
        printf("%li - ", stat_buf.st_mtime);        //last modified time
        printf("%6li - ", stat_buf.st_size);        //size
        printf("%04o - ", stat_buf.st_mode & 0777); //permisiion
        printf("%li - ", stat_buf.st_ino);          //inode
        printf("%s - \n",filestr);                     //path 
        }
        
      }
      else if(S_ISDIR(stat_buf.st_mode))
      {
        if(access(filestr, W_OK) == 0 && access(filestr, R_OK) == 0)
        {
          pid_t pid = fork();
          
          if(pid == 0)
          {
            lstdir(filestr);
            exit(0);
          }
          else if(pid > 0)
          {
            waitpid(pid, NULL,0);
          }
        }
        
      }
    }
  } 

  //CLOSING AND EXITING
  closedir(dirp);
  exit(0);
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
    
  exit(0); 
}
