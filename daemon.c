#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <resolv.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#define PORT 8080
//changing
/*
struct sockaddr_in client_name;//--структура sockaddr_in клиентской машины (параметры ее нам неизвестны. Мы не знаем какая машина к нам будет подключаться)
int size = sizeof(client_name);//--размер структуры (тоже пока неизвестен)
int client_socket_fd;          //--идентификатор клиентского сокета
//--вобще теория сокетов более подробно будет рассмотрена в следующей статье
*/
int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
char *hello = "Hello from server";

char sockbuff[1024]; //--наш буфер обмена информацией с клиентом
time_t now;
struct tm *ptr;
char tbuf[80];

int Daemon();

int main(int argc, char* argv[])
{
	pid_t parpid;

	if (argc < 2)
	{
		printf("Usage ./daemon -d for daemon or ./daemon -i for interactive\n");
		exit(1);
	}
	if (strcmp(argv[1],"-i")==0)
		Daemon();
	else if (strcmp(argv[1],"-d")==0)
	{
		if((parpid=fork())<0)
		{
			printf("\ncan't fork");
			exit(1);
		}
		else if (parpid!=0)
			exit(0);
		setsid();
		Daemon();
	}
	else
	{
		printf("Usage ./daemon -d for daemon or ./daemon -i for interactive\n");
		exit(1);
	}
	return 0;
}

int Daemon()
{
	
	void sig_child(int);//--объявление функции ожидания завершения дочернего процесса
	pid_t pid;
	
	
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //listen(socket_fd,20);
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }   
	for(;;){
		signal(SIGCHLD, sig_child); //--если клиент уже поработал и отключился ждем завершение его дочернего процесса
		
        	new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket > 0) 
		{
			if ((pid=fork())==0) //--то мы создаем копию нашего сервера для работы с другим клиентом(то есть один сеанс уде занят дублируем свободный процесс)
			{
				do
				{
                    		valread = read( new_socket , buffer, 1024); 
				strncat(sockbuff,"\0",1);
   			 	printf("%s\n",buffer ); 
   			 	send(new_socket , buffer , strlen(buffer) , 0); 
    				printf("Hello message sent\n"); 
            
				} 
				while(valread > 0 && strncmp("bye",buffer,3)!=0); //--выполняем цикл пока клиент не закроет сокет или пока не прилетит сообщение от клиента "bye"
				
				close(new_socket); //--естествено закрываем сокет
				exit(0); //--гасим дочерний процесс
			}
			else close(new_socket);
		}
	}
}

void sig_child(int sig)
{
	pid_t pid;
	int stat;
	while ((pid=waitpid(-1, &stat, WNOHANG))>0){}
	return;
}

