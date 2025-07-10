#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "debug.h"

int main(int argc, char **argv)
{
    if(validargs(argc, argv))
        USAGE(*argv, EXIT_FAILURE);
    if(global_options == HELP_OPTION)
        USAGE(*argv, EXIT_SUCCESS);
    if (read_distance_data(stdin))
        return EXIT_FAILURE;

    if (global_options)
        build_taxonomy(NULL);
    else
        build_taxonomy(stdout);

    if (global_options == MATRIX_OPTION && emit_distance_matrix(stdout))
        return EXIT_FAILURE;
    if (global_options == NEWICK_OPTION && emit_newick_format(stdout))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
