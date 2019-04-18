#include <stdio.h>
#include <time.h>
#include <unistd.h>
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
#include <errno.h>
#include <stdlib.h>

#define PORT 8080
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"


int main() {
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "ls"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } else {
        char command[10];
        char argument[256];
        char second_argument[256];
        char input[266];
        printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);

        while(fgets(input, 266, stdin) != 0){
            int num_arguments = sscanf(input, "%s %s %s", command, argument, second_argument);
            if (!strcmp(command, "ls")){
                send(sock , command , strlen(command) , 0 );
                valread = read( sock , buffer, 1024);
		        strncat(buffer,"\0",1);
                printf("%s\n",buffer );
                printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
            }

            else if (!strcmp(command, "mkdir")) {
                if (num_arguments != 2) {
                    printf("Wrong number of arguments\n");
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
                else {
                    char * temp = strcat(strcat(command," "), argument);
                    send(sock , temp , strlen(temp) , 0 );
                    valread = read( sock , buffer, 1024);
                    strncat(buffer,"\0",1);
                    printf("%s\n",buffer );
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
            }

            else if (!strcmp(command, "cd")) {
                if (num_arguments != 2) {
                    printf("Wrong number of arguments\n");
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
                else {
                    char * temp = strcat(strcat(command," "), argument);
                    send(sock , temp , strlen(temp) , 0 );
                    valread = read( sock , buffer, 1024);
                    strncat(buffer,"\0",1);
                    printf("%s\n",buffer );
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
            }

            else if (!strcmp(command, "touch")) {
                if (num_arguments != 2) {
                    printf("Wrong number of arguments\n");
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
                else {
                    char * temp = strcat(strcat(command," "), argument);
                    send(sock , temp , strlen(temp) , 0 );
                    valread = read( sock , buffer, 1024);
                    strncat(buffer,"\0",1);
                    printf("%s\n",buffer );
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
            }

            else if (!strcmp(command, "cat")) {
                if (num_arguments != 2) {
                    printf("Wrong number of arguments\n");
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
                else {
                    char * temp = strcat(strcat(command," "), argument);
                    send(sock , temp , strlen(temp) , 0 );
                    valread = read( sock , buffer, 1024);
                    strncat(buffer,"\0",1);
                    printf("%s\n",buffer );
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
            }

            else if (!strcmp(command, "rm")) {
                if (num_arguments != 2) {
                    printf("Wrong number of arguments\n");
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
                else {
                    char * temp = strcat(strcat(command," "), argument);
                    send(sock , temp , strlen(temp) , 0 );
                    valread = read( sock , buffer, 1024);
                    strncat(buffer,"\0",1);
                    printf("%s\n",buffer );
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
            }

            else if (!strcmp(command, "import")) {
                if (num_arguments != 3) {
                    printf("Wrong number of arguments\n");
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
                else {
                    char * temp = strcat(strcat(strcat(command," "), argument), second_argument);
                    send(sock , temp , strlen(temp) , 0 );
                    valread = read( sock , buffer, 1024);
                    strncat(buffer,"\0",1);
                    printf("%s\n",buffer );
                    printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
                }
            }


            else if (!strcmp(command, "help")) {
                printf("Avaliable commands:\n"
                       "ls - list directory\n"
                       "mkdir [name] - make directory\n"
                       "cd [path] - change directory\n"
                       "touch [name] - create empty file\n"
                       "cat [name] - display contents of file\n"
                       "rm [file/dir] - delete file of directory\n"
                       "import [inner_name] [outer_name] - brings copy of outer_name file to your file system"
                       "to inner_name file. If inner_name file doesn't exist then it will be created in current directory\n"
                       "exit - use it to finish working with file system and save all changes\n"
                       "!!! WITHOUT EXIT COMMAND ALL CHANGES WILL BE LOST\n"
                );
                printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
            }
            else if (!strcmp(command, "exit")) {
                send(sock , command , strlen(command) , 0 );
                valread = read( sock , buffer, 1024);
                strncat(buffer,"\0",1);
                printf("%s\n",buffer );
                break;
            }

            else {
                printf(ANSI_COLOR_RED "Wrong command, use help to know about avaliable commands\n" ANSI_COLOR_RESET);
                printf(ANSI_COLOR_YELLOW "myFS$ " ANSI_COLOR_RESET);
            }
        }
    }


    return 0;
}

