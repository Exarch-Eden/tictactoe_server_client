#include <stdio.h>
#include <string.h>
// FORKS
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#include "./helper.h"
#include "./shared.h"
#include "./server.h"

// HTTP helper functions
void init_reply_header(char *str)
{
    str = malloc(sizeof(char) * BUF_SIZE);
    strcat(str, "HTTP/1.1 ");
}

// HTTP functions
// void head(char *buf, int *cfd)
// {
//     printf("inside HEAD method\n");
//     char *reply_header = malloc(sizeof(char) * BUF_SIZE);

//     printf("BUFFER:\n%s", buf);

//     // substring to find in buffer
//     char *http_find = "HTTP/";

//     strcat(reply_header, http_find);

//     // find "HTTP/" in buffer
//     if (strstr(buf, http_find) != NULL)
//     {
//         char *found = strstr(buf, http_find);

//         int position = found - buf;
//         int substrlen = 5;

//         char *version = malloc(sizeof(char) * 3);
//         int begin_version = position + substrlen;
//         version = strncpy(version, buf + begin_version, 3);
//         printf("version: %s\n", version);

//         strcat(reply_header, version);
//         free(version);
//     }
//     else
//     {
//         perror("strstr");
//         exit(EXIT_FAILURE);
//     }

//     strcat(reply_header, " ");

//     printf("reply_header (before verification): %s\n", reply_header);

//     char *filetype = malloc(sizeof(char) * BUF_SIZE + 1);

//     // check data the server wants
//     char *file = strtok_r(buf, " ", &buf);

//     if (file[0] == '/')
//         file++;

//     // get file type
//     // concatenate to content-type in reply header
//     // obtain file type
//     if (file_exists(file))
//     {
//         strcat(reply_header, "200 OK\r\nContent-Type: ");

//         printf("file is %s\n", file);

//         filetype = strchr(file, '.');
//         filetype = strtok_r(filetype, ".", &filetype);
//         printf("file type: %s\n", filetype);

//         if (strcmp(filetype, "html") == 0)
//         {
//             strcat(reply_header, "text/html;");
//             printf("concat text/html\n");
//         }
//         else
//         {
//             fprintf(stderr, "File type not supported");
//         }

//         printf("reply_header: %s\n", reply_header);

//         // get content length
//         char send_buffer[BUF_SIZE];
//         FILE *sendFile = fopen(file, "r");

//         printf("before reading file\n");
//         int numread = 0;
//         while (!feof(sendFile))
//         {
//             numread += fread(send_buffer, sizeof(unsigned char), BUF_SIZE, sendFile);
//             if (numread < 1) // EOF
//                 break;
//         }
//         printf("numread: %d\n", numread);

//         char *con_length = malloc(sizeof(char) * BUF_SIZE);
//         sprintf(con_length, "%d", numread);

//         // concatenate numread to reply_header
//         strcat(reply_header, "\r\nContent-Length: ");
//         strcat(reply_header, con_length);
//         strcat(reply_header, ";");

//         printf("finished reading\n");
//     }
//     else
//     {
//         strcat(reply_header, "404 NOT OK\r\nContent-Type: ");
//     }

//     printf("reply_header (final): %s\n", reply_header);
//     write(*cfd, reply_header, strlen(reply_header));

//     printf("data inside reply header is: %s\n", reply_header);
// }

void get(char *buf, int *cfd)
{
    char *filetype = malloc(sizeof(char) * BUF_SIZE + 1);

    printf("data insde buf is\n%s\n", buf);
    // reply

    // check data the server wants
    // char *file = strtok_r(buf, " ", &buf);

    char *file = malloc(sizeof(char) * 14);
    strcat(file, "./index2.html");

    if (file[0] == '/')
        file++;

    char *file_path = malloc(sizeof(char) * BUF_SIZE);

    strcat(file_path, "./");
    strcat(file_path, file);
    // if (file_exists(file))
    if (file_exists(file_path) && strcmp(file, "") != 0)
    {
        printf("file is %s\n", file);

        // obtain file type
        filetype = strchr(file, '.');
        filetype = strtok_r(filetype, ".", &filetype);
        printf("file type: %s\n", filetype);

        char *reply_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";

        char reply[BUF_SIZE];

        char send_buffer[BUF_SIZE];
        FILE *sendFile = fopen(file_path, "r");

        if (sendFile == NULL)
        {
            printf("value is null");
            sendFile = fopen(DEFAULT_404, "r");
        }

        while (!feof(sendFile))
        {
            int numread = fread(send_buffer, sizeof(unsigned char), BUF_SIZE, sendFile);
            if (numread < 1)
                break;

            printf("send buffer data: %s\n", send_buffer);
        }
        fclose(sendFile);

        strcat(reply, reply_header);
        strcat(reply, send_buffer);
        write(*cfd, reply, strlen(reply));
        dup2(fileno(stdin), STDIN_FILENO);
    }
    else
    {
        printf("file not found\n");
        
        char send_buffer[BUF_SIZE];
        FILE *sendFile = fopen(DEFAULT_404, "r");

        if (sendFile == NULL)
        {
            sendFile = fopen(DEFAULT_404, "r");
        }

        while (!feof(sendFile))
        {
            int numread = fread(send_buffer, sizeof(unsigned char), BUF_SIZE, sendFile);
            if (numread < 1)
                break; 

            printf("send buffer data: %s\n", send_buffer);
        }
        fclose(sendFile);
        char *reply_header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
        char reply1[BUF_SIZE];
        printf("reply1 : %s\n", reply1);
        strcat(reply1, reply_header);
        strcat(reply1, send_buffer);
        printf("reply: %s\n", reply1);
        write(*cfd, reply1, strlen(reply1));
    }
    free(file_path);
}

// Client Function
void *client_func(void *args)
{
    size_t num_read;
    char *buf = malloc(sizeof(char) * BUF_SIZE + 1);
    int cfd = *((int *)args);
    printf("client - %d\n", cfd);

    // while there are bytes to read
    while ((num_read = read(cfd, buf, BUF_SIZE)) > 0)
    {
        printf("I break here: inside and numread is %d\n", (int)num_read);
        // write data

        // check client message
        printf("%s\n", buf);

        char *method = strtok_r(buf, " ", &buf);
        // if (strcmp(method, "HEAD") == 0)
        // {
        //     printf("data inside buf is\n%s\n", buf);
        //     head(buf, &cfd);
        // }
        if (strcmp(method, "GET") == 0)
        {
            get(buf, &cfd);
            break;
        }

        // close socket
    }
    close(cfd);
    pthread_exit(NULL);
}

// Thread functions
void serv_run_threads(int *sfd, SA_IN *addr, int *addrl)
{
    int server_fd = *sfd;
    SA_IN address = *addr;
    int addrlen = *addrl;
    int new_socket;

    check(listen(server_fd, 10), "Listen Error");

    thread tid[MAX_CLIENTS];
    int i = 0;
    int num_clients = 0;
    while (1)
    {
        check((new_socket = accept(server_fd, (SA *)&address, (socklen_t *)&addrlen)),
              "Accept Error");

        check(pthread_create(&tid[i++], NULL, client_func, &new_socket),
              "Failed to create thread");

        if (i >= MAX_CLIENTS)
        {
            i = 0;
            while (i < MAX_CLIENTS)
            {
                printf("checking thread %d\n", i);
                thread t = tid[i++];

                pthread_join(t, NULL);
                printf("num clients: %d\n", num_clients++);
            }
            num_clients = 0;
            i = 0;
        }
    }
}

// Subprocess functions
void serv_run_subprocess(int *sfd, SA_IN *addr, int *addrl)
{
    int status = 1;
    pid_t wpid, cpid;

    cpid = fork();

    int server_fd = *sfd;
    SA_IN address = *addr;
    int addrlen = *addrl;
    int new_socket;

    check(listen(server_fd, 10), "Listen Error");

    while (1)
    {
        check((new_socket = accept(server_fd, (SA *)&address, (socklen_t *)&addrlen)),
              "Accept Error");

        if ((cpid = fork()) == -1)
        {
            close(new_socket);
            continue;
        }

        // child pid
        if (cpid == 0)
        {
            client_func(&new_socket);
        }
        else // parent
        {
            do
            {
                wpid = waitpid(cpid, &status, WUNTRACED
#ifdef WCONTINUED /* Not all implementations support this */
                                                  | WCONTINUED
#endif
                );
                if (wpid == -1)
                {
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }

                if (WIFEXITED(status))
                {
                    fprintf(stderr, "child exited, status=%d\n", WEXITSTATUS(status));
                }
                else if (WIFSIGNALED(status))
                {
                    fprintf(stderr, "child killed (signal %d)\n", WTERMSIG(status));
                }
                else if (WIFSTOPPED(status))
                {
                    fprintf(stderr, "child stopped (signal %d)\n", WSTOPSIG(status));

#ifdef WIFCONTINUED /* Not all implementations support this */
                }
                else if (WIFCONTINUED(status))
                {
                    fprintf(stderr, "child continued\n");
#endif
                }
                else
                { /* Non-standard case -- may never happen */
                    fprintf(stderr, "Unexpected status (0x%x)\n", status);
                }
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            // close socket since it's done
            close(new_socket);
        }
    }
}

// server main function

void start_server(int process_mode)
{
    // configs
    // 1 for pthreads; 0 for sub processes

    printf("Listening on port %d\n", PORT);

    int server_fd;

    SA_IN address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    check((server_fd = socket(AF_INET, SOCK_STREAM, 0)),
          "Failed to create socket");

    // to fix server port in use
    int enable = 1;
    check(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)),
          "setsockopt(SO_REUSEADDR)");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // binding the socket
    check(bind(server_fd, (struct sockaddr *)&address, sizeof(address)), "In Bind");

    // run server using a certain process
    if (process_mode == THREADS)
    {
        serv_run_threads(&server_fd, &address, &addrlen);
    }
    else if (process_mode == SUBPROCESS)
    {
        serv_run_subprocess(&server_fd, &address, &addrlen);
    }
}
