#include "grafo.h"
#include <stdio.h>
#include <assert.h>

void test_quantities(grafo g)
{
    assert(n_vertices(g) == 5);
    assert(n_arestas(g) == 10);
}

void test_graus(grafo g)
{
    vertice m_1 = agnode(g, "ERC", FALSE);
    vertice m_2 = agnode(g, "ALC", FALSE);
    vertice m_3 = agnode(g, "WH1", FALSE);
    vertice m_4 = agnode(g, "IH1", FALSE);
    vertice m_5 = agnode(g, "HO1", FALSE);

    assert(grau(m_5, g) == 4);
    assert(grau(m_4, g) == 4);
    assert(grau(m_3, g) == 4);
    assert(grau(m_2, g) == 4);
    assert(grau(m_1, g) == 4);

    assert(grau_maximo(g) == 4);
    assert(grau_minimo(g) == 4);
    assert(grau_medio(g) == 4);
}

void test_completo(grafo g)
{
    assert(completo(g) == 1);

    vertice m_1 = agnode(g, "ERC", FALSE);
    vertice m_2 = agnode(g, "ALC", FALSE);

    agdeledge(g, agedge(g, m_1, m_2, NULL, FALSE));

    assert(completo(g) == 0);

    agedge(g, m_1, m_2, NULL, TRUE);
}

void test_complemento(grafo g)
{
    grafo c = complemento(g);

    assert(grau_maximo(c) == 0);
    assert(grau_minimo(c) == 0);
    assert(grau_medio(c) == 0);

    escreve_grafo(c);

    destroi_grafo(c);
}

void test_regular(grafo g)
{
    assert(regular(g) == 1);

    vertice m_1 = agnode(g, "ERC", FALSE);
    vertice m_2 = agnode(g, "ALC", FALSE);

    agdeledge(g, agedge(g, m_1, m_2, NULL, FALSE));

    assert(regular(g) == 0);

    agedge(g, m_1, m_2, NULL, TRUE);
}

void test_conexo(grafo g)
{
    assert(conexo(g) == 1);

    vertice m_1 = agnode(g, "ERC", FALSE);
    vertice m_2 = agnode(g, "ALC", FALSE);
    vertice m_3 = agnode(g, "WH1", FALSE);
    vertice m_4 = agnode(g, "IH1", FALSE);
    vertice m_5 = agnode(g, "HO1", FALSE);

    agdeledge(g, agedge(g, m_1, m_2, NULL, FALSE));
    assert(conexo(g) == 1);

    agdeledge(g, agedge(g, m_1, m_3, NULL, FALSE));
    assert(conexo(g) == 1);

    agdeledge(g, agedge(g, m_1, m_4, NULL, FALSE));
    assert(conexo(g) == 1);

    agdeledge(g, agedge(g, m_1, m_5, NULL, FALSE));
    assert(conexo(g) == 0);

    agedge(g, m_1, m_2, NULL, TRUE);
    assert(conexo(g) == 1);

    agedge(g, m_1, m_3, NULL, TRUE);
    agedge(g, m_1, m_4, NULL, TRUE);
    agedge(g, m_1, m_5, NULL, TRUE);
}

void test_bipartido(grafo g)
{
    assert(bipartido(g) == 0);

    vertice m_1 = agnode(g, "ERC", FALSE);
    vertice m_2 = agnode(g, "ALC", FALSE);
    vertice m_3 = agnode(g, "WH1", FALSE);
    vertice m_4 = agnode(g, "IH1", FALSE);
    vertice m_5 = agnode(g, "HO1", FALSE);

    agdeledge(g, agedge(g, m_1, m_4, NULL, FALSE));
    assert(bipartido(g) == 0);

    agdeledge(g, agedge(g, m_2, m_4, NULL, FALSE));
    assert(bipartido(g) == 0);

    agdeledge(g, agedge(g, m_3, m_5, NULL, FALSE));
    assert(bipartido(g) == 0);

    agdeledge(g, agedge(g, m_1, m_2, NULL, FALSE));
    assert(bipartido(g) == 1);

    agedge(g, m_1, m_2, NULL, TRUE);
    assert(bipartido(g) == 0);

    agedge(g, m_3, m_5, NULL, TRUE);
    assert(bipartido(g) == 0);

    agedge(g, m_2, m_4, NULL, TRUE);
    assert(bipartido(g) == 0);

    agedge(g, m_1, m_4, NULL, TRUE);
    assert(bipartido(g) == 0);
}

int main(int argc, char const *argv[])
{
    FILE *f = fopen("complete.dot", "r");
    grafo g = agread(f, NULL);
    fclose(f);

    escreve_grafo(g);

    test_quantities(g);
    test_graus(g);
    test_completo(g);
    test_complemento(g);
    test_regular(g);
    test_conexo(g);
    test_bipartido(g);

    destroi_grafo(g);

    return 0;
}