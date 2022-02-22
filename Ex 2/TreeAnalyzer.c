#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

//linux thing
#define ENDOFLINE "\n"
//clock for DFS run
int clock = 0;

#define PATHINDEX 1
#define INDEXVERTEX1 2
#define INDEXVERTEX2 3
#define NUMOFVARS 4
#define BASE 10
#define READ "r"
#define MAXLINE 1024
#define NEWLINE '\n'
#define ENDOFSTRING '\0'
#define SPACE ' '
#define ZEROCHAR '0'
#define INVALIDERROR "Invalid input\n"
#define FISRT 0
#define SECOND 1
#define CHILDLESS '-'
#define EMPTYSTRING ""
#define TRUE 1
#define TREEERROR "The given graph is not a tree\n"
#define VISITED 1
#define NOTVISITED 0
#define NOROOT -1

/**
 * struct to represent a node in the tree graph
 */
typedef struct TreeGraph
{
    int key;            // the data of the node
    int *neighbors;     // array of indexes of neighbors
    int n;              // number of arguments in the neighbors array
    int visited;        // flag of visit
    int pre;            // pre time stamp
    int post;           // post time stamp
    int dist;           // distance in BFS run from certain vertex
    int prev;           // previous vertex in BFS path
} TreeGraph;

/**
 *function to create a new node in the tree
 * @param key data of node
 * @param n number of children(later will become neighbors)
 * @param arr array of children
 * @return pointer to new node
 */
TreeGraph *newNode(int key, int n, const int *arr)
{
    // Allocate memory for the new node in the tree
    TreeGraph *node = (TreeGraph *) malloc(sizeof(TreeGraph));

    // Assign vals to the structs fields
    node->key = key;
    node->n = n;
    node->neighbors = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i)
    {
        node->neighbors[i] = arr[i];
    }

    // default vals for the rest:
    node->visited = 0;
    node->post = 0;
    node->pre = 0;
    node->dist = INT_MAX;
    node->prev = -1;

    return node;
}


/**
 * pre visit clock stamp the vertex
 * @param node node in graph
 */
void preVisit(TreeGraph *node)
{
    node->pre = clock;
    clock++;
}

/**
 * post visit clock stamp the vertex
 * @param node node in graph
 */
void postVisit(TreeGraph *node)
{
    node->post = clock;
    clock++;
}

/**
 * explore the graph though vertexes and edges
 * @param tree the tree graph
 * @param n number of children of node
 * @param node node in tree
 */
void explore(TreeGraph **tree, int n, TreeGraph *node)
{
    node->visited = 1;
    preVisit(node);
    for (int i = 0; i < node->n; ++i)
    {
        if (tree[(node->neighbors[i])]->visited == 0)
        {
            explore(tree, n, tree[(node->neighbors[i])]);
        }
    }
    postVisit(node);

}

/**
 * DFS algorithm
 * @param tree tree graph
 * @param n number of vertices
 */
void DFS(TreeGraph **tree, int n)
{
    for (int i = 0; i < n; ++i)
    {
        tree[i]->visited = 0;
    }
    clock = 0;
    for (int i = 0; i < n; ++i)
    {
        if (tree[i]->visited == 0)
        {
            explore(tree, n, tree[i]);
        }
    }
}

/**
 * calculation of min/max branch on tree
 * @param tree the tree graph
 * @param nodePointer current node
 * @param d depth
 * @param max max deoth
 * @param min min depth
 */
void calcMinMaxlengthOfTree(TreeGraph **tree, TreeGraph *nodePointer, int d, int *max, int *min)
{
    int i = 0;
    for (i = 0; i < nodePointer->n; ++i)
    {
        //recursive call to go deeper
        calcMinMaxlengthOfTree(tree, tree[nodePointer->neighbors[i]], d + 1, max, min);
    }
    // raise only if deeper
    if (d > *max)
    {
        *max = d;
    }
    // lower only if shallower
    if (i == 0 && d < *min)
    {
        *min = d;
    }
}

/**
 * BFS algorithm
 * @param tree tree graph
 * @param n number of nodes
 * @param s current node
 */
void BFS(TreeGraph **tree, int n, int s)
{
    for (int i = 0; i < n; ++i)
    {
        tree[i]->dist = INT_MAX;
    }
    tree[s]->dist = 0;
    tree[s]->prev = -1;
    Queue *q = allocQueue();
    enqueue(q, s);
    int u = 0;
    int w = 0;
    while ((u = (int) dequeue(q)) != UINT_MAX)
    {
        for (int i = 0; i < tree[u]->n; ++i)
        {
            w = tree[u]->neighbors[i];
            if (tree[w]->dist == INT_MAX)
            {
                enqueue(q, w);
                tree[w]->prev = u;
                tree[w]->dist = tree[u]->dist + 1;
            }
        }
    }
    freeQueue(&q);
}

/**
 * returns error
 * @return
 */
int errorFunc()
{
    fprintf(stderr, INVALIDERROR);
    return (EXIT_FAILURE);
}

/**
 * function to check the unput from user
 * @param argc num of vars
 * @param argv array of vars
 * @param myFile the file
 * @param vertex1 input vertex number 1
 * @param vertex2 input vertex number 2
 * @return valid input - 0, 1 otherwise
 */
int checkInput(int argc, const char *argv[], FILE **myFile, int *vertex1, int *vertex2)
{
    // var to holds the vertex input
    char *stringPart = (char *) argv[INDEXVERTEX1];
    if (argc != NUMOFVARS)
    {
        return errorFunc();
    }
    //path holder var
    const char *path = argv[PATHINDEX];
    *vertex1 = (int) strtol(stringPart, &stringPart, BASE);
    // validate there was only one number there
    if (strlen(stringPart) != 0)
    {
        return errorFunc();
    }
    // exact same deal with the second vertex:
    stringPart = (char *) argv[INDEXVERTEX2];
    *vertex2 = (int) strtol(stringPart, &stringPart, BASE);
    if (strlen(stringPart) != 0)
    {
        return errorFunc();
    }
    //opening the file for reading:
    *myFile = fopen(path, READ);
    if (*myFile == NULL)
    {
        // error handling
        fclose(*myFile);
        return errorFunc();
    }
    return EXIT_SUCCESS;
}

/**
 * validating and extracting the number of vertices presented in the top of the file
 * @param myFile the file
 * @param numberOfVertices place result into
 * @return number Of Vertices
 */
int getNumOfVer(FILE **myFile, int *numberOfVertices)
{
    char line[MAXLINE];
    char *stringPart = NULL;
    // reading the top line
    if (fgets(line, sizeof(line), *myFile) != NULL)
    {
        stringPart = line;
        *numberOfVertices = (int) strtol(stringPart, &stringPart, BASE);
        if (strcmp(stringPart, ENDOFLINE) != 0)
        {
            fclose(*myFile);
            return errorFunc();
        }
    }
    else
    {
        //it is an empty file..... or is it just an illegal file?
        fclose(*myFile);
        return errorFunc();
    }
    return EXIT_SUCCESS;
}

/**
 * checks if we have a number x in the line that is in the format 0x
 * @param line line of file
 * @param n len of string
 * @return is it valid
 */
int isZeroOccuring(const char *line, size_t n)
{
    for (size_t i = 0; i < n - 2; ++i)
    {
        if (line[i] == NEWLINE || line[i] == ENDOFSTRING)
        {
            // if we got to the end we can return
            return EXIT_SUCCESS;
        }
        if (line[i] == ZEROCHAR && line[i + 1] != SPACE)
        {
            //found a problem
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * returns error
 * @return
 */
void *buildError(FILE **myFile, int *arr)
{
    fprintf(stderr, INVALIDERROR);
    free(arr);
    fclose(*myFile);
}

/**
 * build the actual tree structure and return a pointer to it
 * @param numberOfVertices calimed number of vertices
 * @param myFile file
 * @return pointer to tree
 */
TreeGraph **buildTree(int numberOfVertices, FILE **myFile, int *notATree)
{
    // will hold the line we read
    char line[MAXLINE];
    // will hold the next part we read in the line
    char *stringPart = NULL;
    // array of values we collect of a single line
    int *arr = (int *) malloc(numberOfVertices * sizeof(int *));
    // counts the line number
    int indexI = 0;
    // counts the num of args in the array
    int indexJ = 0;
    // hold the numeric val of single reading
    int returnVal = 0;
    // len of the read string - to compare to each time so we know how many numeric chars we read
    size_t len = 0;
    // temporary holder for the last char in string
    char tempHolder = SPACE;
    // allocate the space
    TreeGraph **tree = (TreeGraph **) malloc(numberOfVertices * sizeof(TreeGraph));
    // while we get a next line to read
    while (fgets(line, sizeof(line), *myFile))
    {
        if (isZeroOccuring(line, strlen(line)))
        {
            return buildError(myFile, arr);
        }
        stringPart = line;
        indexJ = 0;
        tempHolder = stringPart[FISRT];
        len = strlen(stringPart);
        // if the line is empty
        if (len == 1 && stringPart[SECOND] != ENDOFSTRING)
        {
            return buildError(myFile, arr);
        }
        // the line is full and we need to parse it
        while ((returnVal = (int) strtol(stringPart, &stringPart, BASE)) != 0 || (TRUE))
        {
            if (returnVal > numberOfVertices - 1)
            {
                // invalid to have a non existing vertex
                return buildError(myFile, arr);
            }
            if (strlen(stringPart) - len == 0)
            {
                // non numeric char
                if (stringPart[FISRT] == CHILDLESS)
                {
                    // but a legal one - so we flag it
                    stringPart += 1;
                }
                break;
            }
            if (tempHolder == CHILDLESS)
            {
                tempHolder = stringPart[SECOND];
                continue;
            }
            // all is good and we need to add the valid arg to the array
            arr[indexJ] = returnVal;
            indexJ++;
            // need to know if the go down a line
            if (stringPart[SECOND] != NEWLINE)
            {
                tempHolder = stringPart[SECOND];
            }
            // update the len
            len = strlen(stringPart);
            if (returnVal == indexI)
            {
                *notATree = 1;
            }
        }
        if (strcmp(stringPart, ENDOFLINE) != 0 && strcmp(stringPart, EMPTYSTRING) != 0)
        {
            // non numeric char after reading - flagged
            return buildError(myFile, arr);
        }
        // add the node!!
        tree[indexI] = newNode(indexI, indexJ, arr);
        indexI++;
        // check that numberOfVertices is actually true
        if (indexI > numberOfVertices)
        {
            return buildError(myFile, arr);
        }
    }
    // free stuff we dont need anymore
    fclose(*myFile);
    free(arr);
    // check that numberOfVertices is actually true
    if (indexI < numberOfVertices)
    {
        fprintf(stderr, INVALIDERROR);
        return NULL;
    }
    return tree;
}

/**
 * check if the given vertices and exiting in the graph
 * @param tree the tree
 * @param numberOfVertices num of nodes
 * @param vertex vertex to check
 * @return whether we found it
 */
int findVertex(TreeGraph **tree, int numberOfVertices, int vertex)
{
    for (int i = 0; i < numberOfVertices; ++i)
    {
        if (tree[i]->key == vertex)
        {
            return EXIT_SUCCESS;
        }
    }
    fprintf(stderr, INVALIDERROR);
    return EXIT_FAILURE;
}

/**
 * checks if the graph is actually a tree
 * @param tree tree
 * @param numberOfVertices num of nodes
 * @return whether its a tree
 */
int checkIfTree(TreeGraph **tree, int numberOfVertices)
{
    // start checking that it is a tree!
    //test #1: check E = V -1!
    int E = 0;
    for (int i = 0; i < numberOfVertices; ++i)
    {
        E += tree[i]->n;
    }
    if (E != numberOfVertices - 1)
    {
        fprintf(stderr, TREEERROR);
        return EXIT_FAILURE;
    }

    // test #2: check that every one has only 1 parent!
    int *verticesCount = (int *) calloc(numberOfVertices, sizeof(int));
    int childrenIndex = 0;
    for (int i = 0; i < numberOfVertices; ++i)
    {
        for (int j = 0; j < tree[i]->n; ++j)
        {
            childrenIndex = tree[tree[i]->neighbors[j]]->key;
            if (verticesCount[childrenIndex] == VISITED)
            {
                fprintf(stderr, TREEERROR);
                return EXIT_FAILURE;
            }
            verticesCount[childrenIndex] = VISITED;
        }
    }
    free(verticesCount);

    // test #3: DFS run!
    DFS(tree, numberOfVertices);
    TreeGraph *nodeU = NULL;
    TreeGraph *nodeV = NULL;

    for (int i = 0; i < numberOfVertices; ++i)
    {
        for (int j = 0; j < tree[i]->n; ++j)
        {
            nodeU = tree[i];
            nodeV = tree[(tree[i]->neighbors[j])];
            if (nodeU->pre > nodeV->pre)
            {
                if (nodeV->post > nodeU->post)
                {
                    fprintf(stderr, TREEERROR);
                    return EXIT_FAILURE;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
 * calculates the root of the tree
 * @param tree tree
 * @param numberOfVertices num of nodes
 * @return root index
 */
int calcRootOfTree(TreeGraph **tree, int numberOfVertices)
{
    for (int i = 0; i < numberOfVertices; ++i)
    {
        tree[i]->visited = NOTVISITED;
    }
    // visit all children
    for (int i = 0; i < numberOfVertices; ++i)
    {
        for (int j = 0; j < tree[i]->n; ++j)
        {
            tree[(tree[i]->neighbors[j])]->visited = VISITED;
        }
    }
    // root was not a child of any node so it was not visited
    for (int i = 0; i < numberOfVertices; ++i)
    {
        if (tree[i]->visited == NOTVISITED)
        {
            return i;
        }
    }
    return NOROOT;
}

/**
 * calculates the diameter of tree
 * @param tree the tree
 * @param numberOfVertices num of nodes
 * @param treeRoot the root
 * @return the tree diameter
 */
int calcDiameterOfTree(TreeGraph **tree, int numberOfVertices, int treeRoot)
{
    //should run once from treeRoot, check for max dist,
    // run BFS again from there and then the max is the max
    int maxDist = 0;
    BFS(tree, numberOfVertices, treeRoot);
    int maxDistNode = 0;
    for (int i = 0; i < numberOfVertices; ++i)
    {
        if (tree[i]->dist > maxDist)
        {
            maxDist = tree[i]->dist;
            maxDistNode = i;
        }
    }
    BFS(tree, numberOfVertices, maxDistNode);
    for (int i = 0; i < numberOfVertices; ++i)
    {
        if (tree[i]->dist > maxDist)
        {
            maxDist = tree[i]->dist;
        }
    }
    return maxDist;
}

/**
 * calculates the Shortest Path between two nodes
 * @param tree tree
 * @param vertex2 end vertex
 * @param lengthOfPath the length of the path
 * @return pointer to path array
 */
int *calcShortestPath(TreeGraph **tree, int vertex2, int lengthOfPath)
{
    int *shortestPath = (int *) malloc((lengthOfPath) * sizeof(int));
    int jumpingNode = vertex2;
    int i = 0;
    // hold all nodes in the way(keeping in mind its in the reverse order)
    for (i = 0; i < lengthOfPath - 1; ++i)
    {
        shortestPath[i] = jumpingNode;
        jumpingNode = tree[tree[jumpingNode]->prev]->key;
    }
    // special treatment for the last
    shortestPath[i] = jumpingNode;
    return shortestPath;
}

/**
 * make the tree a- directional.
 * have the trees neighbors array point at the parent as well as the children
 * @param tree
 * @param numberOfVertices
 * @param treeRoot
 */
void adirectionalizeTheTree(TreeGraph **tree, int numberOfVertices, int treeRoot)
{
    // making the graph a-directed - everyone points to vertex they are connected to, or connected to them
    int tempN = 0;
    TreeGraph *child = NULL;
    for (int i = 0; i < numberOfVertices; ++i)
    {
        for (int j = 0; j < tree[i]->n; ++j)
        {
            child = tree[tree[i]->neighbors[j]];
            tempN = (child->n) + 1;
            child->neighbors = (int *) realloc(child->neighbors, tempN * sizeof(int));
            child->neighbors[tempN - 1] = i;
        }

    }
    // updating the length now that that are connected to parents
    for (int i = 0; i < numberOfVertices; ++i)
    {
        tree[i]->n++;
    }
    // expect treeRoot who has no parents :(
    tree[treeRoot]->n--;
}

/**
 * function to print all of the results of the program
 * @param treeRoot the root
 * @param numberOfVertices number of vertices
 * @param minBranch minimum branch
 * @param maxBranch maximum branch
 * @param maxDist diameter of tree
 * @param vertex1 vertex1
 * @param vertex2 vertex2
 * @param lengthOfPath the length of the path
 * @param shortestPath array of the path
 */
void printResults(int treeRoot, int numberOfVertices, int minBranch, int maxBranch, int maxDist,
                  int vertex1, int vertex2, int lengthOfPath, int *shortestPath)
{
    printf("Root Vertex: %d\n", treeRoot);
    printf("Vertices Count: %d\n", numberOfVertices);
    printf("Edges Count: %d\n", numberOfVertices - 1);
    printf("Length of Minimal Branch: %d\n", minBranch);
    printf("Length of Maximal Branch: %d\n", maxBranch);
    printf("Diameter Length: %d\n", maxDist);
    printf("Shortest Path Between %d and %d: ", vertex1, vertex2);
    // printing the path backwards
    for (int i = lengthOfPath - 1; i > 0; i--)
    {
        printf("%d ", shortestPath[i]);
    }
    printf("%d\n", shortestPath[FISRT]);
}

/**
 * @brief The main function that runs the program.
 * @param argc Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated
 * multibyte strings that represent the arguments passed to the program from the execution
 * environment. The value of argv[argc] is guaranteed to be a null pointer.
 * @return 0, to tell the system the execution ended without errors, or 1, to
 * tell the system that the code has executione errors.
 */
int main(int argc, const char *argv[])
{
    // vars for the start ;)
    FILE *myFile = NULL;
    int vertex1 = 0, vertex2 = 0;
    int numberOfVertices = 0;
    int notATree = 0;
    //check the dry inputs
    if (checkInput(argc, argv, &myFile, &vertex1, &vertex2))
    {
        return (EXIT_FAILURE);
    }
    // stating content of the file
    if (getNumOfVer(&myFile, &numberOfVertices))
    {
        return (EXIT_FAILURE);
    }
    // building of the tree object(file will be closed here)
    TreeGraph **tree = buildTree(numberOfVertices, &myFile, &notATree);
    // if we returned NULL from the buildTree means we have an error
    if (tree == NULL)
    {
        return (EXIT_FAILURE);
    }
    // checking vertices existence
    if (findVertex(tree, numberOfVertices, vertex1) || findVertex(tree, numberOfVertices, vertex2))
    {
        return (EXIT_FAILURE);
    }
    // validate its a tree
    if (notATree || checkIfTree(tree, numberOfVertices))
    {
        return (EXIT_FAILURE);
    }
    // calc root
    int treeRoot = calcRootOfTree(tree, numberOfVertices);
    TreeGraph *TreeRoot = tree[treeRoot];
    //calc min/max branch
    int maxBranch = 0, minBranch = numberOfVertices;
    calcMinMaxlengthOfTree(tree, TreeRoot, 0, &maxBranch, &minBranch);
    //now that we know for sure it is a tree, it will be easier from now on that we will
    // consider this graph to be a-directional.
    // thus the following
    adirectionalizeTheTree(tree, numberOfVertices, treeRoot);
    // calculate the diameter of the tree:
    int maxDist = calcDiameterOfTree(tree, numberOfVertices, treeRoot);
    BFS(tree, numberOfVertices, vertex1);
    //calculate the shortest path from v1 to v2:
    int lengthOfPath = tree[vertex2]->dist + 1;
    int *shortestPath = calcShortestPath(tree, vertex2, lengthOfPath);
    // free all the stuff:
    for (int i = 0; i < numberOfVertices; ++i)
    {
        free(tree[i]->neighbors);
        free(tree[i]);
    }
    free(tree);
    //print the results of the algorithm
    printResults(treeRoot, numberOfVertices, minBranch, maxBranch, maxDist, vertex1, vertex2,
                 lengthOfPath, shortestPath);
    // last to free :)
    free(shortestPath);//hi
}