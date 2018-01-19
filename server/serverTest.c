#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;
  FILE * pFile;
  unsigned int nsongs=0;


    if(argc != 2)
    {
        printf("\n Utilize o formato:   %s <ip do servidor> \n\n",argv[0]);
        return 1;
    }

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Erro ao tentar abrir socket de rede \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5005);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n Erro lendo IP fornecido \n");
        return 1;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Erro ao tentar se conectar (verifique o servidor) \n \n");
       return 1;
    }


    while (1)
    {
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    if(n<=0)
    exit(0);
    if(!strcmp(recvBuff, "end"))
    break;
    nsongs++;
    printf("%s\n", recvBuff);
    }

    printf ("Received %d songs\n", nsongs);


    printf("\n\nNow requesting \"Obfuse.mp3\"\n");
    write(sockfd, "02 - Obfuse.mp3", 16);

    pFile = fopen ( "music.mp3", "wb");
    if(pFile == NULL)
      exit(0);

    while (1)
    {
      n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
      if(n<=0)
        exit(0);
      if(!strcmp(recvBuff, "end"))
        break;
      fwrite (recvBuff, sizeof(char), n, pFile);
    }

    printf("Now requesting \"Stolen Dog.mp3\"\n");
    write(sockfd, "03 - Ston Dog.mp3", 20);
    read(sockfd, recvBuff, sizeof(recvBuff)-1);
    if(!strcmp(recvBuff, "ErrOF"))
      printf("Error file name\n");
    else if(!strcmp(recvBuff, "OK"))
    {
      printf("Receiving file...\n");
      pFile = fopen ( "music2.mp3", "wb");
      if(pFile == NULL)
        exit(0);

      while (1)
      {
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        if(n<=0)
          exit(0);
        if(!strcmp(recvBuff, "end"))
          break;
        fwrite (recvBuff, sizeof(char), n, pFile);
      }
    }
    close(sockfd);
    //write(sockfd, "02 - Obfuse.mp3", 16);
    return 0;
}
