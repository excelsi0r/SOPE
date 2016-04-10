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
 
void print_file(const char* path, int output)
{
    struct stat file_info;
    if(stat(path, &file_info) == 1) 
    {
        perror(strerror(errno));
        perror("Error getting data about file.");
        return;
    } 
}
 
int main(int argc, char* argv[])
{

  char* filepath = "/tmp/files.txt";
  int file;
  
  DIR *dirp; 
  struct dirent *direntp; 
  struct stat stat_buf; 
  char *str; 
  char name[200]; 

  if(argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    return 1;
  }
  
  file = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0600);
   
  if((dirp = opendir( argv[1])) == NULL) 
   { 
      perror(argv[1]); 
      exit(2); 
     } 
    while
     ((direntp = readdir( dirp)) != NULL) 
     { 
      sprintf(name,"%s/%s",argv[1],direntp->d_name); 
    // <----- NOTAR 
                              // alternativa a chdir(); ex: anterior 
    if
     (
    lstat
    (name, &stat_buf)==-1)   
    // testar com 
    stat()
      { 
       perror("lstat ERROR"); 
       exit(3); 
      } 
    //      printf("%10d - ",(int) stat_buf.st_ino); 
    if
     (S_ISREG(stat_buf.st_mode)) str = "regular"; 
    else
    if
     (S_ISDIR(stat_buf.st_mode)) str = "directory"; 
    else
     str = "other"; 
      printf("%-25s - %s\n", direntp->d_name, str); 
     } 
     closedir(dirp); 
     exit(0);   
  return 0;
}

