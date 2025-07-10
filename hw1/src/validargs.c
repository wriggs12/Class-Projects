#include <stdlib.h>

#include "global.h"
#include "debug.h"

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the various options that were specified will be
 * encoded in the global variable 'global_options', where it will be
 * accessible elsewhere in the program.  For details of the required
 * encoding, see the assignment handout.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * @modifies global variable "global_options" to contain an encoded representation
 * of the selected program options.
 */
int validargs(int argc, char **argv)
{
    while (--argc > 0)
    {
        char *flag = *++argv;
        if (*++flag == 'h')
        {
            if (global_options == MATRIX_OPTION || global_options == NEWICK_OPTION)
                return -1;
            global_options = HELP_OPTION;
            return 0;
        }
        else if (*flag == 'n')
        {
            if (global_options == MATRIX_OPTION)
                return -1;
            global_options = NEWICK_OPTION;
        }
        else if (*flag == 'm')
        {
            if (global_options == NEWICK_OPTION)
                return -1;
            global_options = MATRIX_OPTION;
        }
        else if (*flag == 'o')
        {
            if (global_options != NEWICK_OPTION)
                return -1;
            if (--argc == 0)
                return -1;
            outlier_name = *++argv;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}
