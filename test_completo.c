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

    agdeledge(g, agedge(g, m_1, m_4, NULL, FALSE)); // ERC -- IH1
    assert(bipartido(g) == 0);

    agdeledge(g, agedge(g, m_2, m_4, NULL, FALSE)); // ALC -- IH1
    assert(bipartido(g) == 0);

    agdeledge(g, agedge(g, m_3, m_5, NULL, FALSE)); // WH1 -- HO1
    assert(bipartido(g) == 0);

    agdeledge(g, agedge(g, m_1, m_2, NULL, FALSE)); // ERC -- ALC
    assert(bipartido(g) == 1);

    agdeledge(g, agedge(g, m_3, m_4, NULL, FALSE)); // IH1 -- WH1
    agdeledge(g, agedge(g, m_2, m_5, NULL, FALSE)); // HO1 -- ALC
    agdeledge(g, agedge(g, m_1, m_3, NULL, FALSE)); // WH1 -- ERC
    assert(bipartido(g) == 1);

    agedge(g, m_3, m_4, NULL, TRUE);
    agedge(g, m_2, m_5, NULL, TRUE);
    agedge(g, m_1, m_3, NULL, TRUE);
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

void print_matriz(int **m, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

void test_matriz(grafo g)
{
    int nv = n_vertices(g);
    int **matriz = matriz_adjacencia(g);

    for (size_t i = 0; i < nv; i++)
    {
        for (size_t j = 0; j < nv; j++)
        {
            if (i == j)
                assert(matriz[i][j] == 0);
            else
                assert(matriz[i][j] == 1);
        }
    }

    print_matriz(matriz, nv);
    printf("\n");

    vertice m_1 = agnode(g, "ERC", FALSE);
    vertice m_2 = agnode(g, "ALC", FALSE);
    vertice m_3 = agnode(g, "WH1", FALSE);
    vertice m_4 = agnode(g, "IH1", FALSE);
    vertice m_5 = agnode(g, "HO1", FALSE);

    agdeledge(g, agedge(g, m_1, m_4, NULL, FALSE));
    matriz = matriz_adjacencia(g);
    print_matriz(matriz, nv);
    printf("\n");

    agdeledge(g, agedge(g, m_1, m_5, NULL, FALSE));
    matriz = matriz_adjacencia(g);
    print_matriz(matriz, nv);
    printf("\n");

    agdeledge(g, agedge(g, m_2, m_3, NULL, FALSE));
    matriz = matriz_adjacencia(g);
    print_matriz(matriz, nv);
    printf("\n");

    agedge(g, m_1, m_4, NULL, TRUE);
    agedge(g, m_1, m_5, NULL, TRUE);
    agedge(g, m_2, m_3, NULL, TRUE);

    matriz = matriz_adjacencia(g);

    for (size_t i = 0; i < nv; i++)
    {
        for (size_t j = 0; j < nv; j++)
        {
            if (i == j)
                assert(matriz[i][j] == 0);
            else
                assert(matriz[i][j] == 1);
        }
    }

    print_matriz(matriz, nv);
    printf("\n");
}

void test_triangulos(grafo g)
{
    assert(n_triangulos(g) == 10);
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
    test_matriz(g);
    test_triangulos(g);

    destroi_grafo(g);

    return 0;
}