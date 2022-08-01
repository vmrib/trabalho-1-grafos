Após a chamada de matriz_adjacencia() é necessário liberar a memória quando a matriz retornada
por essa função não for mais ser utilizada.
Se M é uma matriz nxn retornada por essa função, então o seguinte código libera a mariz da memória:
  for (int i = 0; i < n; i++)
    free(M[i]);
  free(M);