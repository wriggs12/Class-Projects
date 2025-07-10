void Signal(int sig, void (*sig_handler)(int));
void block_signal(int sig);
void unblock_signal(int sig);
void sigchld_handler(int sig);
void sigint_handler(int sig);
void suspend();
