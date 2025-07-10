#include <stdlib.h>

#include "global.h"
#include "debug.h"

static double str_to_double(char* s, int size)
{
    double res = 0.0;
    double scale = 0.0;

    if (size > 1 && *s == '0' && *(s + 1) != '.')
        return -1.0;

    for (int i = 0; i < size; ++i)
    {
        if (*s >= '0' && *s <= '9')
        {
            res = 10.0 * res + (double)(*s - '0');
            scale = scale * 10.0;
        }
        else if (*s == '.')
            if (scale != 0)
                return -1.0;
            else
                scale = 1.0;
        else
            return -1.0;

        s++;
    }

    return res / (scale == 0.0 ? 1.0 : scale);
}

static int validate_distances()
{
    //Diag Zeros
    for (int i = 0; i < num_taxa; ++i)
        if (*(*(distances + i) + i) != 0)
            return -1;

    //Symmetry
    for (int i = 0; i < num_taxa; ++i)
        for (int j = 0; j < num_taxa; ++j)
            if (*(*(distances + i) + j) != *(*(distances + j) + i))
                return -1;
    return 0;
}

static void calc_node_name(int num, char *str)
{
    *str++ = '#';

    int scale = 1;
    while (num / scale > 9)
        scale = scale * 10;

    while (scale >= 1) {
        *str++ = (num / scale) + '0';
        num = num % scale;
        scale = scale / 10;
    }
}

static void update_nodes(int node_1, int node_2, int joining_node)
{
    char *name;
    NODE *n;

    n = nodes + node_1;
    name = *(node_names + node_1);
    if (*name == '#')
        *(n->neighbors + 2) = (nodes + joining_node);
    else
        *n->neighbors = (nodes + joining_node);

    n = nodes + node_2;
    name = *(node_names + node_2);
    if (*name == '#')
        *(n->neighbors + 2) = (nodes + joining_node);
    else
        *n->neighbors = (nodes + joining_node);

    n = nodes + joining_node;
    NODE **t = n->neighbors;
    *t++ = (nodes + node_1);
    *t = (nodes + node_2);
}

static int is_equal(char *name, char *other)
{
    while (*name != '\0' && *other != '\0')
        if (*name++ != *other++)
            return 0;
    return 1;
}

static int is_unique(char *name)
{
    for (int i = 0; i < num_taxa - 1; ++i)
        if (is_equal(name, *(node_names + i)))
            return 0;
    return 1;
}

static double get_distance(char *name_1, char *name_2)
{
    for (int i = 0; i < num_all_nodes; ++i)
        if (is_equal(*(node_names + i), name_1))
            for (int j = 0; j < num_all_nodes; ++j)
                if (is_equal(*(node_names + j), name_2))
                    return *(*(distances + i) + j);

    return -1.0;
}

static void construct_newick(NODE *n, char *ignore, FILE *out)
{
    if (*(n->name) != '#')
    {
        fprintf(out, "%s:%.2f", n->name, get_distance(n->name, ignore));
        return;
    }

    fprintf(out, "(");
    if (is_equal((*(n->neighbors))->name, ignore))
    {
        construct_newick(*(n->neighbors + 2), n->name, out);
        fprintf(out, ",");
        construct_newick(*(n->neighbors + 1), n->name, out);
    }
    else if (is_equal((*(n->neighbors + 1))->name, ignore))
    {
        construct_newick(*(n->neighbors + 2), n->name, out);
        fprintf(out, ",");
        construct_newick(*(n->neighbors), n->name, out);
    }
    else
    {
        construct_newick(*(n->neighbors + 1), n->name, out);
        fprintf(out, ",");
        construct_newick(*(n->neighbors), n->name, out);
    }
    fprintf(out, ")%s:%.2f", n->name, get_distance(n->name, ignore));
}

/**
 * @brief  Read genetic distance data and initialize data structures.
 * @details  This function reads genetic distance data from a specified
 * input stream, parses and validates it, and initializes internal data
 * structures.
 *
 * The input format is a simplified version of Comma Separated Values
 * (CSV).  Each line consists of text characters, terminated by a newline.
 * Lines that start with '#' are considered comments and are ignored.
 * Each non-comment line consists of a nonempty sequence of data fields;
 * each field is terminated either by ',' or else newline for the last
 * field on a line.  The constant INPUT_MAX specifies the maximum number
 * of data characters that may be in an input field; fields with more than
 * that many characters are regarded as invalid input and cause an error
 * return.  The first field of the first data line is empty;
 * the subsequent fields on that line specify names of "taxa", which comprise
 * the leaf nodes of a phylogenetic tree.  The total number N of taxa is
 * equal to the number of fields on the first data line, minus one (for the
 * blank first field).  Following the first data line are N additional lines.
 * Each of these lines has N+1 fields.  The first field is a taxon name,
 * which must match the name in the corresponding column of the first line.
 * The subsequent fields are numeric fields that specify N "distances"
 * between this taxon and the others.  Any additional lines of input following
 * the last data line are ignored.  The distance data must form a symmetric
 * matrix (i.e. D[i][j] == D[j][i]) with zeroes on the main diagonal
 * (i.e. D[i][i] == 0).
 *
 * If 0 is returned, indicating data successfully read, then upon return
 * the following global variables and data structures have been set:
 *   num_taxa - set to the number N of taxa, determined from the first data line
 *   num_all_nodes - initialized to be equal to num_taxa
 *   num_active_nodes - initialized to be equal to num_taxa
 *   node_names - the first N entries contain the N taxa names, as C strings
 *   distances - initialized to an NxN matrix of distance values, where each
 *     row of the matrix contains the distance data from one of the data lines
 *   nodes - the "name" fields of the first N entries have been initialized
 *     with pointers to the corresponding taxa names stored in the node_names
 *     array.
 *   active_node_map - initialized to the identity mapping on [0..N);
 *     that is, active_node_map[i] == i for 0 <= i < N.
 *
 * @param in  The input stream from which to read the data.
 * @return 0 in case the data was successfully read, otherwise -1
 * if there was any error.  Premature termination of the input data,
 * failure of each line to have the same number of fields, and distance
 * fields that are not in numeric format should cause a one-line error
 * message to be printed to stderr and -1 to be returned.
 */

int read_distance_data(FILE *in) {
    char c;
    char *p;
    double *d;
    int *a = active_node_map;
    NODE *n = nodes;
    num_taxa = 0;

    while ((c = fgetc(in)) == '#')
        while ((c = fgetc(in)) != '\n');

    //Parse First Line
    while (c != '\n')
    {
        if (++num_taxa > MAX_TAXA)
            return  -1;

        int field_size = 0;
        p = input_buffer;

        while ((c = fgetc(in)) != ',' && c != '\n')
        {
            if (++field_size > INPUT_MAX)
                return -1;

            *p++ = c;
        }

        if (!is_unique(input_buffer))
            return -1;

        p = input_buffer;
        char *name = *(node_names + num_taxa - 1);
        (n++)->name = name;

        for (int i = 0; i < field_size; ++i)
            *name++ = *p++;
    }

    num_all_nodes = num_active_nodes = num_taxa;

    //Parse Distance Data
    for (int i = 0; i < num_taxa; ++i)
    {
        while ((c = fgetc(in)) == '#')
            while ((c = fgetc(in)) != '\n');

        d = *(distances + i);
        *a++ = i;

        char *name = *(node_names + i);
        while (c != ',')
            if (c != *name++)
                return -1;
            else
                c = fgetc(in);

        for (int j = 0; j < num_taxa; ++j)
        {
            p = input_buffer;
            while ((c = fgetc(in)) != ',' && c != '\n' && p - input_buffer <= INPUT_MAX)
                *p++ = c;

            if (p - input_buffer > INPUT_MAX)
                return -1;

            *d = str_to_double(input_buffer, p - input_buffer);
            if (*d++ == -1.0)
                return -1;
        }
    }

    return validate_distances();
}

/**
 * @brief  Emit a representation of the phylogenetic tree in Newick
 * format to a specified output stream.
 * @details  This function emits a representation in Newick format
 * of a synthesized phylogenetic tree to a specified output stream.
 * See (https://en.wikipedia.org/wiki/Newick_format) for a description
 * of Newick format.  The tree that is output will include for each
 * node the name of that node and the edge distance from that node
 * its parent.  Note that Newick format basically is only applicable
 * to rooted trees, whereas the trees constructed by the neighbor
 * joining method are unrooted.  In order to turn an unrooted tree
 * into a rooted one, a root will be identified according by the
 * following method: one of the original leaf nodes will be designated
 * as the "outlier" and the unique node adjacent to the outlier
 * will serve as the root of the tree.  Then for any other two nodes
 * adjacent in the tree, the node closer to the root will be regarded
 * as the "parent" and the node farther from the root as a "child".
 * The outlier node itself will not be included as part of the rooted
 * tree that is output.  The node to be used as the outlier will be
 * determined as follows:  If the global variable "outlier_name" is
 * non-NULL, then the leaf node having that name will be used as
 * the outlier.  If the value of "outlier_name" is NULL, then the
 * leaf node having the greatest total distance to the other leaves
 * will be used as the outlier.
 *
 * @param out  Stream to which to output a rooted tree represented in
 * Newick format.
 * @return 0 in case the output is successfully emitted, otherwise -1
 * if any error occurred.  If the global variable "outlier_name" is
 * non-NULL, then it is an error if no leaf node with that name exists
 * in the tree.
 */
int emit_newick_format(FILE *out) {
    if (num_taxa == 1)
        fprintf(out, "%s:%.2f", nodes->name, 0.0);
    if (num_taxa == 2)
        fprintf(out, "(%s:%.2f, %s:%.2f)", nodes->name, get_distance(nodes->name, (nodes + 1)->name), (nodes + 1)->name, get_distance(nodes->name, (nodes + 1)->name));


    if (!outlier_name)
    {
        double *sums;
        for (int i = 0; i < num_taxa; ++i)
        {
            sums = row_sums + i;
            *sums = 0.0;
            for (int j = 0; j < num_taxa; ++j)
                *sums += *(*(distances + i) + j);
        }

        double max = *row_sums;
        outlier_name = *node_names;
        for (int i = 0; i < num_taxa; ++i)
            if (*(row_sums + i) > max)
            {
                max = *(row_sums + i);
                outlier_name = *(node_names + i);
            }
    }
    else
    {
        int found = 0;
        for (int i = 0; i < num_taxa; ++i)
            if (is_equal(outlier_name, *(node_names + i)))
            {
                found = 1;
                break;
            }

        if (!found)
            return -1;
    }

    for (int i = num_taxa; i < num_all_nodes; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            NODE *n = *((nodes + i)->neighbors + j);
            if (is_equal(n->name, outlier_name))
            {
                construct_newick((nodes + i), outlier_name, out);
                break;
            }
        }
    }

    fprintf(out, ";\n");

    return 0;
}

/**
 * @brief  Emit the synthesized distance matrix as CSV.
 * @details  This function emits to a specified output stream a representation
 * of the synthesized distance matrix resulting from the neighbor joining
 * algorithm.  The output is in the same CSV form as the program input.
 * The number of rows and columns of the matrix is equal to the value
 * of num_all_nodes at the end of execution of the algorithm.
 * The submatrix that consists of the first num_leaves rows and columns
 * is identical to the matrix given as input.  The remaining rows and columns
 * contain estimated distances to internal nodes that were synthesized during
 * the execution of the algorithm.
 *
 * @param out  Stream to which to output a CSV representation of the
 * synthesized distance matrix.
 * @return 0 in case the output is successfully emitted, otherwise -1
 * if any error occurred.
 */
int emit_distance_matrix(FILE *out) {
    for (int i = 0; i < num_all_nodes; ++i)
        fprintf(out, ",%s", *(node_names + i));
    fprintf(out, "\n");

    double *d;
    for (int i = 0; i < num_all_nodes; ++i)
    {
        fprintf(out, "%s,", *(node_names + i));
        d = *(distances + i);

        for (int j = 0; j < num_all_nodes; ++j)
        {
            fprintf(out, "%.2f", *d++);
            if (num_all_nodes - j > 1)
                printf(",");
        }
        fprintf(out, "\n");
    }

    return 0;
}

/**
 * @brief  Build a phylogenetic tree using the distance data read by
 * a prior successful invocation of read_distance_data().
 * @details  This function assumes that global variables and data
 * structures have been initialized by a prior successful call to
 * read_distance_data(), in accordance with the specification for
 * that function.  The "neighbor joining" method is used to reconstruct
 * phylogenetic tree from the distance data.  The resulting tree is
 * an unrooted binary tree having the N taxa from the original input
 * as its leaf nodes, and if (N > 2) having in addition N-2 synthesized
 * internal nodes, each of which is adjacent to exactly three other
 * nodes (leaf or internal) in the tree.  As each internal node is
 * synthesized, information about the edges connecting it to other
 * nodes is output.  Each line of output describes one edge and
 * consists of three comma-separated fields.  The first two fields
 * give the names of the nodes that are connected by the edge.
 * The third field gives the distance that has been estimated for
 * this edge by the neighbor-joining method.  After N-2 internal
 * nodes have been synthesized and 2*(N-2) corresponding edges have
 * been output, one final edge is output that connects the two
 * internal nodes that still have only two neighbors at the end of
 * the algorithm.  In the degenerate case of N=1 leaf, the tree
 * consists of a single leaf node and no edges are output.  In the
 * case of N=2 leaves, then no internal nodes are synthesized and
 * just one edge is output that connects the two leaves.
 *
 * Besides emitting edge data (unless it has been suppressed),
 * as the tree is built a representation of it is constructed using
 * the NODE structures in the nodes array.  By the time this function
 * returns, the "neighbors" array for each node will have been
 * initialized with pointers to the NODE structure(s) for each of
 * its adjacent nodes.  Entries with indices less than N correspond
 * to leaf nodes and for these only the neighbors[0] entry will be
 * non-NULL.  Entries with indices greater than or equal to N
 * correspond to internal nodes and each of these will have non-NULL
 * pointers in all three entries of its neighbors array.
 * In addition, the "name" field each NODE structure will contain a
 * pointer to the name of that node (which is stored in the corresponding
 * entry of the node_names array).
 *
 * @param out  If non-NULL, an output stream to which to emit the edge data.
 * If NULL, then no edge data is output.
 * @return 0 in case the output is successfully emitted, otherwise -1
 * if any error occurred.
 */
int build_taxonomy(FILE *out) {
    double *d = *distances;
    double *sums = row_sums;
    char *name = *node_names;

    while (num_active_nodes > 2)
    {
        //Calculate Row Sums
        for (int i = 0; i < num_active_nodes; ++i)
        {
            sums = row_sums + *(active_node_map + i);
            *sums = 0.0;
            for (int j = 0; j < num_active_nodes; ++j)
                *sums += *(*(distances + *(active_node_map + i)) + *(active_node_map + j));
        }

        double min = num_active_nodes * (*row_sums);
        int node_1, node_2, joining_node, node_1_index, node_2_index;

        //Calculate Lowest Q-Value
        for (int i = 0; i < num_active_nodes; ++i)
        {
            for (int j = 0; j < num_active_nodes; ++j)
            {
                int active_node_1 = *(active_node_map + i);
                int active_node_2 = *(active_node_map + j);

                d = *(distances + active_node_1) + active_node_2;

                double q = (num_active_nodes - 2) * (*d) - *(row_sums + active_node_1) - *(row_sums + active_node_2);
                if (i != j && q < min)
                {
                    node_1 = active_node_1;
                    node_2 = active_node_2;
                    node_1_index = i;
                    node_2_index = j;
                    min = q;
                }
            }
        }

        joining_node = num_all_nodes;
        num_all_nodes++;

        //Add New Node to Distance Matrix
        for (int i = 0; i < num_active_nodes; ++i)
        {
            double distance = 0.0;
            int node = *(active_node_map + i);

            if (node == node_1)
            {
                double temp = *(*(distances + node_1) + node_2);
                double sum_1 = *(row_sums + node_1);
                double sum_2 = *(row_sums + node_2);

                distance = (temp + (sum_1 - sum_2) / (num_active_nodes - 2)) / 2;
            }
            else if (node == node_2)
            {
                double temp = *(*(distances + node_1) + node_2);
                double sum_1 = *(row_sums + node_1);
                double sum_2 = *(row_sums + node_2);

                distance = (temp + (sum_2 - sum_1) / (num_active_nodes - 2)) / 2;
            }
            else if (node == joining_node)
            {
                distance = 0.0;
            }
            else
            {
                double d_1 = *(*(distances + node_1) + node);
                double d_2 = *(*(distances + node_2) + node);
                double temp = *(*(distances + node_1) + node_2);

                distance = (d_1 + d_2 - temp) / 2;
            }

            d = *(distances + joining_node) + node;
            *d = distance;

            d = *(distances + node) + joining_node;
            *d = distance;
        }

        //Output Distance Data
        if (out)
        {
            double dist_1 = *(*(distances + node_1) + joining_node);
            double dist_2 = *(*(distances + node_2) + joining_node);

            fprintf(out, "%d,%d,%.2f\n", node_1, joining_node, dist_1);
            fprintf(out, "%d,%d,%.2f\n", node_2, joining_node, dist_2);
        }

        //Update Active Nodes Map
        *(active_node_map + node_1_index) = joining_node;
        *(active_node_map + node_2_index) = *(active_node_map + num_active_nodes - 1);
        num_active_nodes--;

        name = *(node_names + joining_node);
        calc_node_name(joining_node, name);
        (nodes + joining_node)->name = name;

        update_nodes(node_1, node_2, joining_node);
    }

    //Create Final Edge
    *((nodes + *active_node_map)->neighbors + 2) = (nodes + *(active_node_map + 1));
    *((nodes + *(active_node_map + 1))->neighbors + 2) = (nodes + *active_node_map);

    if (out)
    {
        int node_1 = *active_node_map;
        int node_2 = *(active_node_map + 1);
        double dist = *(*(distances + node_1) + node_2);

        if (node_1 < node_2)
            fprintf(out, "%d,%d,%.2f\n", node_1, node_2, dist);
        else
            fprintf(out, "%d,%d,%.2f\n", node_2, node_1, dist);
    }

    return 0;
}
