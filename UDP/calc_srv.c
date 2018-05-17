/** Calc_SRV.C **/

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

int main(void)
{
	struct sockaddr_in me, client;
	int adl=sizeof(me);
	char linha[81], *aux;
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
	
#ifdef WIN32
	memset(&me,0,adl);
#else
	bzero((char *)&me,adl);
#endif
	me.sin_family=AF_INET;
	me.sin_addr.s_addr=htonl(INADDR_ANY); /* endereco IP local */
	me.sin_port=htons(8450); /* porta local  */

	if(-1!=bind(sock,(struct sockaddr *)&me,adl))
	do
    {
        recvfrom(sock,linha,74,0,(struct sockaddr *)&client,&adl);
        //aux=linha;while(*aux) {*aux=toupper(*aux);aux++;}
        aux = strdup(linha);
        sprintf(linha, "Oscar: %s", aux);
        free(aux);
        sendto(sock,linha,81,0,(struct sockaddr *)&client,adl);
    } while(strcmp(linha+7,"EXIT"));
	else puts("Porta ocupada");

#ifdef WIN32
	closesocket(sock);
	WSACleanup( );
#else
	close(sock);
#endif
}
