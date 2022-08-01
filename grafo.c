#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

//------------------------------------------------------------------------------
// Macros para acesso aos atributos internos dos vértices definidos pelo usuario
// Mais informações em https://graphviz.org/pdf/cgraph.pdf, páginas 9-10
#define V_index(v) (((meuvertice_t *)AGDATA(v))->index)
#define V_done(v) (((meuvertice_t *)AGDATA(v))->done)
#define V_color(v) (((meuvertice_t *)AGDATA(v))->color)

//------------------------------------------------------------------------------
typedef enum cor
{
  NONE,
  RED,
  BLUE,
} cor;

// Atributos intenos de um vértice
typedef struct meuvertice_t
{
  Agrec_t h; // Exigido pela biblioteca
  int done;  // Flag para sinalizar se já passamos pelo vértice
  union
  {
    int index; // Cada vértice pode ser indexado por um índice ou
    cor color; // uma cor
  };
} meuvertice_t;

typedef Agedge_t *aresta;

//------------------------------------------------------------------------------
// "contamina" todos os vizinhos e descedentes dos vizinhos de v. Retorna o
// total de contaminados. Utilizado para saber se um grafo é conexo. Se não for,
// definitivamente não será possível contaminar todos os vértices a partir de v.
int contaminar(grafo g, vertice v);

// Tenta colorir todos os vértices descendentes de um vértice inicial v.
// Utilizado para testar se o grafo é bipartido.
// Retorna 0 caso não seja possível colorir o grafo utilizando 2 cores.
int colorir(grafo g, vertice v);

// Seja M uma matriz de nxn dimensões, retorna M^3 em M. Utilizado para contar
// triangulos de um grafo.
void matriz_cubo(int **m, int n);

//------------------------------------------------------------------------------
grafo le_grafo(void)
{
  grafo g = agread(stdin, NULL);

  if (g) // atribui atributos internos para todos os vértices de g
    aginit(g, AGNODE, "meuvertice_t", sizeof(meuvertice_t), TRUE);

  return g;
}
//------------------------------------------------------------------------------
void destroi_grafo(grafo g)
{
  agclean(g, AGNODE, "meuvertice_t"); // libera da memória atributos internos
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
  // Num grafo completo, o grau de um vértice é igual ao número de vértices do
  // grafo - 1.
  int grau_completo = n_vertices(g) - 1;

  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    if (grau(i, g) != grau_completo)
      return 0;

  return 1;
}

// -----------------------------------------------------------------------------
int conexo(grafo g)
{
  // todos os vértices começam "descontaminados"
  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    V_done(i) = 0;

  // se for possível contaminar todos os vértices do grafo a partir de um vértice
  // inicial v, o grafo é conexo.
  return contaminar(g, agfstnode(g)) == n_vertices(g);
}

// -----------------------------------------------------------------------------
int bipartido(grafo g)
{
  // Todos os vértices são inicialemnte sem cor.
  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
  {
    V_color(i) = NONE;
    V_done(i) = 0;
  }

  // como colorimos o grafo a partir de um vértice inicial qualquer, se o grafo
  // for desconexo não será possível colorir todos os vértices. Por isso,
  // iteramos sobre todos os vértices para garantir que todos serão coloridos.
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
  // algoritmo extraído de
  // https://www.geeksforgeeks.org/number-of-triangles-in-a-undirected-graph
  int **m = matriz_adjacencia(g);
  int n = n_vertices(g);

  matriz_cubo(m, n);

  int traco = 0;
  for (int i = 0; i < n; i++)
    traco += m[i][i];

  for (int i = 0; i < n; i++)
    free(m[i]);
  free(m);

  return traco / 6;
}

// -----------------------------------------------------------------------------
int **matriz_adjacencia(grafo g)
{
  int n_vert = n_vertices(g);
  int **matriz;

  // Alocação da matriz
  matriz = malloc(n_vert * sizeof(int *));
  for (int i = 0; i < n_vert; i++)
  {
    matriz[i] = malloc(n_vert * sizeof(int));
    memset(matriz[i], 0, n_vert * sizeof(int));
  }

  // Atribuição de um índice para cada vértice, na ordem de agnxtnode()
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

  // complemento possui todos os vértices do original, com o mesmo nome
  for (vertice i = agfstnode(g); i != NULL; i = agnxtnode(g, i))
    agnode(c, agnameof(i), TRUE);

  for (vertice i = agfstnode(c); i != NULL; i = agnxtnode(c, i))
    for (vertice j = agfstnode(c); j != NULL; j = agnxtnode(c, j))
    {
      if (i == j) // Aresta do tipo {u, u} não existe
        continue;

      // Dados dois vértices i e j do complemento, obtemos os vértices
      // equivalentes u e v do grafo oiginal.
      vertice u = agnode(g, agnameof(i), FALSE);
      vertice v = agnode(g, agnameof(j), FALSE);

      // se não existir aresta entre os vértices u e v do grafo original,
      // inserimos ela no complemento.
      if (!agedge(g, u, v, NULL, FALSE))
        agedge(c, j, i, NULL, TRUE);
    }

  return c;
}

// -----------------------------------------------------------------------------
// "contamina" todos os vizinhos e descedentes dos vizinhos de v. Retorna o
// total de contaminados. Utilizado para saber se um grafo é conexo. Se não for,
// definitivamente não será possível contaminar todos os vértices a partir de v.
int contaminar(grafo g, vertice v)
{
  int contaminados = 1; // vértice v inicial contaminado
  V_done(v) = 1;

  for (aresta a = agfstedge(g, v); a != NULL; a = agnxtedge(g, a, v))
  {
    if (V_done(a->node)) // já contaminamos o vértice anteriormente
      continue;

    contaminados += contaminar(g, a->node);
  }

  return contaminados;
}

// -----------------------------------------------------------------------------
// Tenta colorir todos os vértices descendentes de um vértice inicial v.
// Utilizado para testar se o grafo é bipartido.
// Retorna 0 caso não seja possível colorir o grafo utilizando 2 cores.
int colorir(grafo g, vertice v)
{
  // cor "candidata" ao vértice v é inicialmente nula
  cor candidata = NONE;

  // Checagem para verificar se vizinhos já não possuem alguma cor
  for (aresta a = agfstedge(g, v); a != NULL; a = agnxtedge(g, a, v))
  {
    if (V_color(a->node) != NONE) // Vizinho já possui cor
    {
      // Se não temos cor definida, então somos coloridos com a cor
      // contrária ao vizinho
      if (candidata == NONE)
        candidata = V_color(a->node) == RED ? BLUE : RED;

      // Caso exista um vizinho com a mesma cor que a nossa, então é
      // Impossível colorir com 2 cores.
      else if (candidata == V_color(a->node))
        return 0;
    }
  }

  // Se nenhum vizinho tem cor definida, arbitrariamente definimos nossa
  // cor como vermelho.
  V_color(v) = candidata != NONE ? candidata : RED;
  V_done(v) = 1;

  // Colorimos os vizinhos
  for (aresta a = agfstedge(g, v); a != NULL; a = agnxtedge(g, a, v))
  {
    if (V_done(a->node)) // Vizinho já foi colorido previamente
      continue;

    if (colorir(g, a->node) == 0)
      return 0;
  }

  return 1;
}

// -----------------------------------------------------------------------------
// Seja M uma matriz de nxn dimensões, retorna M^3 em M. Utilizado para contar
// triangulos de um grafo.
void matriz_cubo(int **m, int n)
{
  int aux1[n][n], aux2[n][n];

  // aux1 = m * m;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      aux1[i][j] = 0;
      for (int k = 0; k < n; k++)
      {
        aux1[i][j] += (m[i][k] * m[k][j]);
      }
    }
  }

  // aux2 = m * aux1
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      aux2[i][j] = 0;
      for (int k = 0; k < n; k++)
      {
        aux2[i][j] += m[i][k] * aux1[k][j];
      }
    }
  }

  // m = aux2 = m^3
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      m[i][j] = aux2[i][j];
    }
  }
}