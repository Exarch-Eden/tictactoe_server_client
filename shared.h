#ifndef SHARED_H
#define SHARED_H

// https://www.sciencedirect.com/topics/computer-science/registered-port#:~:text=Ports%200%20through%201023%20are,be%20used%20dynamically%20by%20applications.
// /etc/services
#define MAX_CLIENTS 2
#define BUF_SIZE 10000
#define LINESIZE 1024

#define DEFAULT_404 "./404.html"
#define DEFAULT_ROOT "./"
#define PORT 8000
#define IS_THREAD 0

#endif
