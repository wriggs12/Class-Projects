#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "debug.h"
#include "client_registry.h"
#include "transaction.h"
#include "store.h"
#include "server.h"

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

static void terminate(int status);
void sighup_handler();

CLIENT_REGISTRY *client_registry; 

int main(int argc, char* argv[]){ 
    int c;
    int port;
    while ((c = getopt(argc, argv, "p:")) != -1)
        switch (c)
        {
            case 'p':
                port = atoi(optarg);
                break;
        }

    // Perform required initializations of the client_registry,
    // transaction manager, and object store.
    client_registry = creg_init();
    trans_init();
    store_init();

    // Setup SIGHUP handler
    struct sigaction action;
    struct sigaction old_action;

    sigset_t sigset;
    sigemptyset(&sigset);

    action.sa_handler = &sighup_handler;
    action.sa_mask = sigset;
    action.sa_flags = 0;

    sigaction(SIGHUP, &action, &old_action);

    // Open server socket
    int server_socket, *connfdp;
    SA_IN server_addr;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (SA *)&server_addr, sizeof(server_addr));
    listen(server_socket, 1024);
    
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfdp = malloc(sizeof(int));
        *connfdp = accept(server_socket, (SA *)&clientaddr, &clientlen);
        pthread_create(&tid, NULL, xacto_client_service, connfdp);
    }

    fprintf(stderr, "You have to finish implementing main() "
	    "before the Xacto server will function.\n");

    terminate(EXIT_FAILURE);
}

/*
 * Function called to cleanly shut down the server.
 */
void terminate(int status) {
    // Shutdown all client connections.
    // This will trigger the eventual termination of service threads.
    creg_shutdown_all(client_registry);
    
    debug("Waiting for service threads to terminate...");
    creg_wait_for_empty(client_registry);
    debug("All service threads terminated.");

    // Finalize modules.
    creg_fini(client_registry);
    trans_fini();
    store_fini();

    debug("Xacto server terminating");
    exit(status);
}

void sighup_handler()
{
    terminate(EXIT_SUCCESS);
}
