#include "../grafo.h"

int main(int argc, char const *argv[])
{
    grafo inet = agread("../inet.dot", NULL);

    escreve_grafo(inet);

    destroi_grafo(inet);

    return 0;
}
