#include "grafo.h"
#include <stdio.h>
#include <assert.h>

void test_quantities(grafo g)
{
    assert(n_vertices(g) == 22);
    assert(n_arestas(g) == 27);
}

void test_graus(grafo g)
{
    grafo m_5 = agnode(g, "HO1", FALSE);
    grafo m_4 = agnode(g, "IH1", FALSE);
    grafo m_3 = agnode(g, "MV", FALSE);
    grafo m_2 = agnode(g, "WH1", FALSE);
    grafo m_1 = agnode(g, "AN", FALSE);

    assert(grau(m_5, g) == 5);
    assert(grau(m_4, g) == 4);
    assert(grau(m_3, g) == 3);
    assert(grau(m_2, g) == 2);
    assert(grau(m_1, g) == 1);

    assert(grau_maximo(g) == 5);
    assert(grau_minimo(g) == 1);
}

test_completo(grafo g)
{
    assert(completo(g) == 0);
}

int main(int argc, char const *argv[])
{
    FILE *f = fopen("inet.dot", "r");
    grafo inet = agread(f, NULL);
    fclose(f);

    escreve_grafo(inet);

    test_quantities(inet);
    test_graus(inet);
    test_completo(inet);

    destroi_grafo(inet);

    return 0;
}
