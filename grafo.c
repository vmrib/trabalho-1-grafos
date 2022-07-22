#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "debug.h"

#define V_index(v) (((meuvertice_t *)AGDATA(v))->index)
#define V_done(v) (((meuvertice_t *)AGDATA(v))->done)

typedef struct meuvertice_t
{
  Agrec_t h;
  int index;
  int done;
} meuvertice_t;

typedef Agedge_t *aresta;

//------------------------------------------------------------------------------
grafo le_grafo(void)
{
  grafo g = agread(stdin, NULL);

  if (g)
    aginit(g, AGNODE, "meuvertice_t", sizeof(meuvertice_t), TRUE);

  return g;
}
//------------------------------------------------------------------------------
void destroi_grafo(grafo g)
{
  agclean(g, AGNODE, "meuvertice_t");
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
  int sample = grau(agfstnode(g), g);

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    if (grau(i, g) != sample)
      return 0;

  return 1;
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
  int n_vert = n_vertices(g);
  int **matriz;

  matriz = malloc(n_vert * sizeof(int *));
  for (size_t i = 0; i < n_vert; i++)
  {
    *matriz = malloc(n_vert * sizeof(int));
    memset(*matriz, 0, n_vert * sizeof(int));
  }

  int index = 0;
  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
  {
    V_index(i) = index;
    index++;
  }

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
  {
    for (aresta a = agfstedge(g, i); a != NULL; a = agnxtedge(g, a, i))
    {
      matriz[V_index(i)][V_index(a->node)] = 1;
      matriz[V_index(a->node)][V_index(i)] = 1;
    }
  }

  return matriz;
}

// -----------------------------------------------------------------------------
grafo complemento(grafo g)
{
  grafo c = agopen("Complemento", Agstrictundirected, NULL);
  if (!c)
    return NULL;

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
  {
    agnode(c, agnameof(i), TRUE);
  }

  for (vertice i = agfstnode(c); i != NULL; i = agnxtnode(c, i))
    for (vertice j = agfstnode(c); j != NULL; j = agnxtnode(c, j))
    {
      if (i == j)
        continue;

      vertice u = agnode(g, agnameof(i), FALSE);
      vertice v = agnode(g, agnameof(j), FALSE);

      if (!agedge(g, u, v, NULL, FALSE))
        agedge(c, j, i, NULL, TRUE);
    }

  return c;
}
