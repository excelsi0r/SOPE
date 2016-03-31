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

  if(argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    return 1;
  }

    //mkstemp(filepath);
    file = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0600);

  


  if(file == -1) 
  {
      perror("/tmp");
      //perror("Redirecting to console...\n");
      //file = STDOUT_FILENO;
  }
/*
  char *header = "Serial number | File size | Modification Date | File name\n";
  write(file, header, strlen(header));
  //print_directory(argv[1], file);
  return 0;
*/
  return 0;
}

