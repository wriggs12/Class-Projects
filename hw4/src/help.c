#include <stdio.h>

int help()
{
    fprintf(stdout, "Available commands:\n"\
            "help -- Print this help message\n" \
            "quit (<=0 args) -- Quit the program\n" \
            "show (<=1 args) -- Show process info\n" \
            "run (>=1 args) -- Start a process\n" \
            "stop (1 args) -- Stop a running process\n" \
            "cont (1 args) -- Continue a stopped process\n" \
            "release (1 args) -- Stop tracing a process, allowing it to continue normally\n" \
            "wait (1-2 args) -- Wait for a process to enter a specified state\n" \
            "kill (1 args) -- Forcibly terminate a process\n" \
            "peek (2-3 args) -- Read from the address space of a traced process\n" \
            "poke (3 args) -- Write to the address space of a traced process\n" \
            "bt (1 args) -- Show a stack trace for a traced process\n");
    return 1;
}
