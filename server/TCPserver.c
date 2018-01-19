#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include<signal.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int listenfd = 0, connfd = 0;
DIR *d;

void clean_quit()
{
  if(d != NULL)
    closedir(d);
  if(connfd != 0)
    close(connfd);
  sleep(1);
  exit(0);
}

void sig_handler(int signo)
{
  if (signo == SIGINT)
  {
    printf("\033[1;31m");
    printf("\nClosing server...\n");
    printf("\033[0m\n");
    clean_quit();
  }
}

int main(int argc, char *argv[])
{
  const char song_dir[] = "./songs/";
  signal(SIGINT, sig_handler);
  struct sockaddr_in serv_addr;

  char sendBuff[1025];
  time_t ticks;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5005);

  if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("Error binding...\n");
    clean_quit();
  }

  system("clear");
// printf("ip: %s\n", inet_ntoa(serv_addr.sin_addr));
  printf("\033[1;32m");
  printf("*********************************\n");
  printf("******** Server Active **********\n");
  printf("*********************************\n");
  printf("\033[0m\n");

  struct dirent *dir;
  d = opendir("./songs/");

  printf("\033[36m");
  printf("List of songs : \n");
  printf("\033[0m");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG)
      {
         printf("\t%s\n", dir->d_name);
      }
    }
  }
  listen(listenfd, 10);

  int quit=0;

  while(1)
  {
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

      printf("\033[32m");
      printf("## A new client joined ##\n");
      printf("\033[0m");

    rewinddir(d);
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG)
      {
        write(connfd, dir->d_name, sizeof(dir->d_name));
        usleep(10);
      }
    }
    write(connfd, "end", 4);

    while (1) { // For every song request
      char songRequest[512] = {0,};

      int n = read(connfd, songRequest, sizeof(songRequest)-1);
      if (n<=0)
        break;
      printf("%s\n", songRequest);
      if(!strcmp(songRequest, "quit"))
      {
        quit = 1;
        break;
      }

      FILE * pFile;
      long lSize;
      char * buffer;
      size_t result;
      char file_name[1024] = {};

      strcat(file_name, song_dir);
      strcat(file_name, songRequest);

      pFile = fopen (file_name, "rb" );
      if (pFile==NULL)
      {
        printf("\033[1;31m");
        printf("\n Error opening the file \"%s\"... \n\n", songRequest);
        printf("\033[0m\n");
        write(connfd, "ErrOF", 6);
        continue;
      }

      fseek (pFile , 0 , SEEK_END);
      lSize = ftell (pFile);

      rewind (pFile);
      buffer = (char*) malloc (sizeof(char)*lSize);
      if (buffer == NULL)
      {
        printf("\033[1;31m");
        printf("\n Error allocating memory... \n \n");
        printf("\033[0m\n");
        write(connfd, "ErrAM", 6);
        fclose (pFile);
        continue;
      }

      result = fread (buffer,1,lSize,pFile);
      if (result != lSize)
      {
        printf("\033[1;31m");
        printf("\n Error reading the file... \n\n");
        printf("\033[0m\n");
        write(connfd, "ErrRF", 6);
        fclose (pFile);
        continue;
      }

      fclose (pFile);
      write(connfd, "OK", 3);
      usleep(500000);
      write(connfd, buffer, lSize);
      sleep(1);
      write(connfd, "end", 4);
      free (buffer);

      if(quit)
        break;
    }
    printf("\033[32m");
    printf("## Client disconnected ##\n");
    printf("\033[0m");
    close(connfd);
    sleep(1);
  }
  closedir(d);
}
