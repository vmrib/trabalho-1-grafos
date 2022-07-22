#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "debug.h"

#define V_index(v) (((meuvertice_t *)AGDATA(v))->index)
#define V_done(v) (((meuvertice_t *)AGDATA(v))->done)
#define V_color(v) (((meuvertice_t *)AGDATA(v))->color)

typedef enum cor
{
  NONE,
  RED,
  BLUE,
} cor;

typedef struct meuvertice_t
{
  Agrec_t h;
  int index;
  int done;
  cor color;
} meuvertice_t;

typedef Agedge_t *aresta;

int contaminar(grafo g, vertice v);
int colorir(grafo g, vertice v);
int v_triangulos(grafo g, vertice v);

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
  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    V_done(i) = 0;

  return contaminar(g, agfstnode(g)) == n_vertices(g);
}

// -----------------------------------------------------------------------------
int bipartido(grafo g)
{
  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
  {
    V_color(i) = NONE;
    V_done(i) = 0;
  }

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
  {
    if (colorir(g, i) == 0)
      return 0;
  }

  return 1;
}

// -----------------------------------------------------------------------------
int n_triangulos(grafo g)
{
  int triangulos = 0;

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    triangulos += v_triangulos(g, i);

  return triangulos;
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

int contaminar(grafo g, vertice v)
{
  int contaminados = 1;
  V_done(v) = 1;

  for (aresta a = agfstedge(g, v); a != NULL; a = agnxtedge(g, a, v))
  {
    if (V_done(a->node))
      continue;

    contaminados += contaminar(g, a->node);
  }

  return contaminados;
}

int colorir(grafo g, vertice v)
{
  cor candidato = NONE;
  for (aresta a = agfstedge(g, v); a != NULL; a = agnxtedge(g, a, v))
  {
    if (V_color(a->node) != NONE)
      if (candidato == NONE)
        candidato = V_color(a->node);
      else
        return 0;
  }

  V_color(v) = candidato != NONE ? candidato : RED;
  V_done(v) = 1;

  for (aresta a = agfstedge(g, v); a != NULL; a = agnxtedge(g, a, v))
  {
    if (V_done(a->node))
      continue;

    if (colorir(g, a->node) == 0)
      return 0;
  }

  return 1;
}

int v_triangulos(grafo g, vertice v)
{
  int triangulos = 0;

  for (aresta a = agfstedge(g, v); a != NULL; a = agnxtedge(g, a, v))
  {
    for (aresta b = agfstedge(g, a->node); b != NULL; b = agnxtedge(g, b, a->node))
    {
      for (aresta c = agfstedge(g, b->node); c != NULL; c = agnxtedge(g, c, b->node))
      {
        if (c == b)
          continue;

        if (V_done(a) && V_done(b) && V_done(c))
          continue;

        if (ageqedge(a, c))
          triangulos++;

        V_done(c) = 1;
      }
      V_done(b) = 1;
    }
    V_done(a) = 1;
  }

  return triangulos;
}