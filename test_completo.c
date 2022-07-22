#include "grafo.h"
#include <stdio.h>
#include <assert.h>

void test_quantities(grafo g)
{
    assert(n_vertices(g) == 5);
    assert(n_arestas(g) == 11);
}

void test_graus(grafo g)
{
    vertice m_1 = agnode(g, "ERC", FALSE);
    vertice m_2 = agnode(g, "ALC", FALSE);
    vertice m_3 = agnode(g, "WH1", FALSE);
    vertice m_4 = agnode(g, "IH1", FALSE);
    vertice m_5 = agnode(g, "HO1", FALSE);

    assert(grau(m_5, g) == 5);
    assert(grau(m_4, g) == 5);
    assert(grau(m_3, g) == 5);
    assert(grau(m_2, g) == 5);
    assert(grau(m_1, g) == 5);

    assert(grau_maximo(g) == 5);
    assert(grau_minimo(g) == 5);
    assert(grau_medio(g) == 5);
}

test_completo(grafo g)
{
    assert(completo(g) == 1);
}

test_complemento(grafo g)
{
    grafo c = complemento(g);
}