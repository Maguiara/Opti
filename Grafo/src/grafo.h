/*
 *  GRAFO.h - plantilla para los alumnos y alumnas de definicion de la clase GRAFO
 *
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Cursos  : 2012-2021
 */

#ifndef _GRAPH_H
#define _GRAPH_H


#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <cassert>
#include <algorithm>

using namespace std;

const unsigned UERROR = 65000; //Para ciertos algoritmos es necesario codificar este valor de error
const int maxint = 1000000; //Para ciertos algoritmos es necesario codificar este valor de m�ximo

// definicion de un elemento de la lista de adyacencia

typedef struct
{
	unsigned j; // nodo
	int      c; // atributo para expresar los costes, pesos, las longitudes, las inversiones, etc...
} ElementoLista;

typedef struct {
	unsigned extremo1, extremo2;
	int peso;
} AristaPesada;

// definicion del tipo de una lista de adyacencia de un nodo
typedef vector<ElementoLista> LA_nodo;

class GRAFO
 {
private:
	unsigned            dirigido;  //0 si el grafo es no dirigido y 1 si es dirigido
	unsigned            n;         // numero de NODOS
	unsigned            m;         // numero de ARCOS
	vector<LA_nodo>     LS;        // Lista de adyacencia de los sucesores: es una matriz dode cada fila posiblemente es posiblemente de distinto tama�o
	vector<LA_nodo>     LP;        // Lista de adyacencia de los predecesores: es una matriz dode cada fila posiblemente es posiblemente de distinto tama�o
	vector<LA_nodo>     A;         // Matriz de adyacencia, tanto adyacencia como costes
	void destroy();                // Destructor del LS, A, y LP, en su caso
	void build (string nombrefichero, int &errorapertura); //Crea LS, A y LP en su caso, desde la lectura del fichero
	void dfs_num(unsigned i, vector<LA_nodo>  L, vector<bool> &visitado, vector<unsigned> &prenum, unsigned &prenum_ind, vector<unsigned> &postnum, unsigned &postnum_ind); //Recorrido en profundidad recursivo con recorridos enum y postnum
  void bfs_num(unsigned i, vector<LA_nodo>  L, vector<unsigned> &pred, vector<unsigned> &d); //Recorrido en amplitud con c�lculo de pred y d
	void DFS_Postnum(unsigned i, vector<bool>& visitado, vector<unsigned>& postnum, unsigned& num); //Calcula postnum para todo nodo i
	void DFS_Inv(unsigned i, vector<bool>& visitado); 
public:
     GRAFO(string nombrefichero, int &errorapertura);
     void actualizar (string nombrefichero, int &errorapertura);
     unsigned Es_dirigido(); // devuelve 0 si el grafo es no dirigido y 1 si es dirigido
     void Info_Grafo(); // devuelve informaci�n b�sica del grafo
     void Mostrar_Listas(int l);  //Muestra la lista de adyacencia
     void Mostrar_Matriz(); //Muestra la matriz de adyacencia: nodos y costes
     void RecorridoProfundidad(); //Construye un recorrido en profundidad desde un nodo inicial
     void RecorridoAmplitud(); //Construye un recorrido en amplitud desde un nodo inicial
		 void ComponentesFuertementeConexas(); //Algoritmo para el calculo de componentes fuertemente conexas
		 void Kruskal(); //Algoritmo de kruskal para construir el MST
     ~GRAFO(); //Destructor del objeto grafo
};

#endif