#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SUBPROCESS 0
#define THREADS 1

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;
typedef pthread_t thread;

// HTTP helper functions
void init_reply_header(char * str);

// HTTP functions
void head(char * buf, int * cfd);

void get(char * buf, int * cfd);

// Client Function
void * client_func(void * args);

// Thread functions
void serv_run_threads(int * sfd, SA_IN * addr, int * addrl);

// Subprocess functions
void serv_run_subprocess(int * sfd, SA_IN * addr, int * addrl);

// server main function
void start_server(int process_mode);

#endif
