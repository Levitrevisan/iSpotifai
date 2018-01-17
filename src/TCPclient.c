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
  unsigned int nrec=0;


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
    serv_addr.sin_port = htons(5000);

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

    pFile = fopen ( "music.mp3", "wb");

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        fwrite (recvBuff, sizeof(char), n, pFile);
  	nrec += n;

    }

    printf ("Recebidos %d bytes", nrec);

    if(n < 0)
    {
        printf("\n Erro na recepçao \n \n");
	return (0);
    }

    fclose(pFile);

    system(" which mpg321 > teste.txt ");

    pFile = fopen ( "teste.txt", "rt");
    n = fread (recvBuff, sizeof(char), 10, pFile);

    if (n != 10)
       printf("\n Player mpg321 não localizado. Instale usando \n \t\t sudo apt-get -y install mpg321 \n\n");
    else
       system(" mpg321 music.mp3 ");

    return 0;
}
