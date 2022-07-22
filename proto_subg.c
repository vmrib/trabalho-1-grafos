#include <stdio.h>
#include "grafo.h"

int main(int argc, char const *argv[])
{
    FILE *f = fopen("inet.dot", "r");
    grafo g = agread(f, NULL);
    fclose(f);

    escreve_grafo(g);

    for (grafo i = agfstsubg(g); i != NULL; i = agnxtsubg(i))
    {
        escreve_grafo(i);
    }

    return 0;
}
