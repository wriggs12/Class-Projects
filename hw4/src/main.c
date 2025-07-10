#include <stdlib.h>

#include "helpers.h"

int main(int argc, char *argv[]) {
    int c;
    int prompt = 1;

    while ((c = getopt(argc, argv, "p")) != -1)
        switch(c)
        {
            case 'p':
                prompt = 0;
                break;
        }

    shell(prompt);
    return EXIT_SUCCESS;
}
