#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <errno.h>

#include "handlers.h"
#include "deet.h"
#include "help.h"
#include "quit.h"
#include "show.h"
#include "run.h"
#include "stop.h"
#include "cont.h"
#include "release.h"
#include "wait.h"
#include "kill.h"
#include "peek.h"
#include "poke.h"
#include "trace.h"

#define HELP "help"
#define QUIT "quit"
#define SHOW "show"
#define RUN "run"
#define STOP "stop"
#define CONTINUE "cont"
#define RELEASE "release"
#define WAIT "wait"
#define KILL "kill"
#define PEEK "peek"
#define POKE "poke"
#define TRACE "bt" 

#define PROMPT "deet> "
#define DELIMS " \n\t"

#define NUM_CMDS 12
#define NUM_STATES 7
#define ARG_SIZE 10

typedef struct Process {
    int deet_id;
    pid_t pid;
    char trace;
    PSTATE state;
    int exit_status;
    char *cmd;
} Process;

typedef struct Node {
    Process *proc;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct State_Info {
    pid_t pid;
    int exit_status;
    PSTATE state;
    struct State_Info *next;
} State_Info;

extern Node *proc_list;
extern char *input;
extern char *states[];
extern size_t n;
extern int d_errno;
extern int HANDLER_RAN;

enum errors {
    NONE,
    INVAL_DEET_ID
};

int handle_input();
Process *get_process();
void error();
void async_print();
void shell(int prompt);
void rm_node(Node *node);
void add_node(Node *node);
void print_process(Process *p);
void update_state(pid_t pid, PSTATE new_state);
void log_state(pid_t pid, int exit_status, PSTATE state);
void setup_print(pid_t pid, PSTATE state, int exit_status);

#endif
