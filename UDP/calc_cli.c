/** Calc_CLI.C **/

#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>

int main(int argc, char **argv)
{
	system("color F1");
	char nome[30];
	printf("Digite seu nome: ");
	gets (nome);
	system("cls");
	printf("Bem vindo %s ", nome);
	//system("pause");
	
    struct sockaddr_in me, server;
    int adl=sizeof(me);
    char linha[81];
    
#ifdef WIN32
	WSADATA wsaData;
	WORD wVersionRequested;
	SOCKET sock=0;
 
	wVersionRequested = MAKEWORD( 2, 2 );

	WSAStartup(wVersionRequested, &wsaData);
#else
	int sock=0;
#endif

    sock=socket(AF_INET,SOCK_DGRAM,0);

    if(argc!=2) 
	{
#ifdef WIN32
		closesocket(sock);
#else
        close(sock);
#endif
		puts("Utilizar o end. IP do servidor na linha de comando");
		exit(1);
	}
    
#ifdef WIN32
	memset(&me,0,adl);
	memset(&server,0,adl);
#else
	bzero((char *)&me,adl);
    bzero((char *)&server,adl);
#endif

    me.sin_family=AF_INET;
    me.sin_addr.s_addr=htonl(INADDR_ANY); /* endereco IP local */
    me.sin_port=htons(0); /* porta local (0=auto assign) */
    bind(sock,(struct sockaddr *)&me,adl);

    server.sin_family=AF_INET;
    /* endereco IP de destino */
    server.sin_addr.s_addr=inet_addr(argv[1]); /* host local */
    server.sin_port=htons(8450); /* porta do servidor */
    do
    {
        gets(linha);
        sendto(sock,linha,81,0,(struct sockaddr *)&server,adl);
        recvfrom(sock,linha,81,0,(struct sockaddr *)&server,&adl);
        puts(linha);
    } while(strcmp(linha+7,"EXIT"));

#ifdef WIN32
	closesocket(sock);
	WSACleanup( );
#else
	close(sock);
#endif
}
