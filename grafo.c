#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------
grafo le_grafo(void)
{
  return agread(stdin, NULL);
}
//------------------------------------------------------------------------------
void destroi_grafo(grafo g)
{
  agclose(g);
}
//------------------------------------------------------------------------------
grafo escreve_grafo(grafo g)
{
  agwrite(g, stdout);
  return g;
}

// -----------------------------------------------------------------------------
int n_vertices(grafo g)
{

  return agnnodes(g);
}

// -----------------------------------------------------------------------------
int n_arestas(grafo g)
{

  return agnedges(g);
}

// -----------------------------------------------------------------------------
int grau(vertice v, grafo g)
{

  return agcountuniqedges(g, v, TRUE, TRUE);
}

// -----------------------------------------------------------------------------
int grau_maximo(grafo g)
{
  int max = grau(agfstnode(g), g);

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    if (grau(i, g) > max)
      max = grau(i, g);

  return max;
}

// -----------------------------------------------------------------------------
int grau_minimo(grafo g)
{
  int min = grau(agfstnode(g), g);

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    if (grau(i, g) < min)
      min = grau(i, g);

  return min;
}

// -----------------------------------------------------------------------------
int grau_medio(grafo g)
{
  int soma = 0;
  int vertices = n_vertices(g);

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    soma += grau(i, g);

  return soma / vertices;
}

// -----------------------------------------------------------------------------
int regular(grafo g)
{

  return 0;
}

// -----------------------------------------------------------------------------
int completo(grafo g)
{
  int grau_completo = n_vertices(g) - 1;

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    if (grau(i, g) != grau_completo)
      return 0;

  return 1;
}

// -----------------------------------------------------------------------------
int conexo(grafo g)
{

  return 0;
}

// -----------------------------------------------------------------------------
int bipartido(grafo g)
{

  return 0;
}

// -----------------------------------------------------------------------------
int n_triangulos(grafo g)
{

  return 0;
}

// -----------------------------------------------------------------------------
int **matriz_adjacencia(grafo g)
{

  return NULL;
}

// -----------------------------------------------------------------------------
grafo complemento(grafo g)
{

  return NULL;
}
