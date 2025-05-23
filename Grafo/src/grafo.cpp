 /*
 *  GRAFO.CPP - Plantilla para la implementaci�n de la clase GRAFOS
 *
 *
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Cursos   : 2012-2021
 */

#include "grafo.h"

void GRAFO :: destroy() {
	for (unsigned i=0; i< n; i++) {
		LS[i].clear();
		A[i].clear();
		if (dirigido == 1) {
      LP[i].clear();
    };
	}
	LS.clear();
	LP.clear();
	A.clear();
}

void GRAFO::build (string nombrefichero, int &errorapertura) {
  ElementoLista dummy;
	ifstream textfile;
	textfile.open(nombrefichero);
	if (textfile.is_open()) {
		unsigned i, j, k;
		// leemos por conversion implicita el numero de nodos, arcos y el atributo dirigido
		textfile >> (unsigned & ) n >> (unsigned &) m >> (unsigned &) dirigido;
		// los nodos internamente se numeran desde 0 a n-1
		// creamos las n listas de sucesores
		LS.resize(n);
		A.resize(n);
		LP.resize(n);
        // leemos los m arcos
		for (k = 0; k < m ; k++) {
			//damos los valores a dummy.j y dummy.c
			textfile >> (unsigned &) i  >> (unsigned &) j >> (int &) dummy.c;
      dummy.j = j - 1;
				//situamos en la posici�n del nodo i a dummy mediante push_back
				//pendiente de hacer un segundo push_back si es no dirigido. O no.
      if (dirigido == 1) {
			  //pendiente la construcci�n de LP, si es dirigido
      	LS[i - 1].push_back(dummy);
        dummy.j = i - 1;
        LP[j - 1].push_back(dummy);
      }  
			if (dirigido == 0) {
        LS[i - 1].push_back(dummy);
        dummy.j = i - 1;
				if ( (j - 1) != (i - 1)) {
					LS[j - 1].push_back(dummy);
				}
      }
    }
		// Cerramos el fichero
		textfile.close();
		//pendiente del valor a devolver en errorapertura
    errorapertura = 0;
		//...

 	} else if ( !textfile.is_open()) {
		errorapertura = 1;
	}
}


GRAFO::~GRAFO() {
	destroy();
}

GRAFO::GRAFO(string nombrefichero, int &errorapertura) {
	build(nombrefichero, errorapertura);
}

void GRAFO:: actualizar (string nombrefichero, int &errorapertura) {
    //Limpiamos la memoria dinamica asumida en la carga previa, como el destructor
    destroy();
    //Leemos del fichero y actualizamos G con nuevas LS y, en su caso, LP
    build(nombrefichero, errorapertura);
}

unsigned GRAFO::Es_dirigido() {
	return dirigido;
}

void GRAFO::Info_Grafo() {
	cout << endl;
	cout << "Número de nodos: " << n << endl;
	cout << "Número de arcos: " << m << endl;
	cout << "Tipo de grafo" << (dirigido == 1 ? " Dirigido " : " No dirigido");
	cout << endl << endl;
}

void Mostrar_Lista(vector<LA_nodo> L) {
	for (unsigned i = 0; i < L.size(); i++) {
		cout << "Nodo " << i + 1 << ": ";
		for (unsigned j = 0; j < L[i].size(); j++) {
			cout << L[i][j].j + 1 << " (coste:" << L[i][j].c << ") ";
		}
		cout << endl;
	}
	cout << endl;
}

void GRAFO :: Mostrar_Listas (int l) {
	switch(l) {
		case 0:
			Mostrar_Lista(LS);
			break;
		case 1:
			Mostrar_Lista(LS);
			break;
		case -1:
			Mostrar_Lista(LP);
			break;
	}	
}

void GRAFO::Mostrar_Matriz() {
	 vector<vector<int>> matriz_ady(n, vector<int>(n, 0));
  vector<vector<int>> matriz_costes(n, vector<int>(n, -1));

  for(unsigned i = 0; i < LS.size(); ++i) {
    for(auto adyacente : LS[i]) {
      matriz_ady[i][adyacente.j] = 1;
      matriz_costes[i][adyacente.j] = adyacente.c;
      if(!dirigido) {
        matriz_ady[adyacente.j][i] = 1;
        matriz_costes[adyacente.j][i] = adyacente.c;
      }
    }
  }

  cout << "Matriz de adyacencia" << endl;
  for(const auto& fila : matriz_ady) {
    for(int ady : fila) {
      cout << ady << " ";
    }
    cout << endl;
  }

  cout << endl << "Matriz de costes" << endl;
  for(const auto& fila : matriz_costes) {
    for (int coste : fila) {
      if (coste == -1) {
        cout << " X ";
      } else {
        cout <<  " " << coste << " ";
      }
    }
  cout << endl;
  }
}	


void GRAFO::dfs_num(unsigned i, vector<LA_nodo>  L, vector<bool> &visitado, vector<unsigned> &prenum, unsigned &prenum_ind, vector<unsigned> &postnum, unsigned &postnum_ind) { //Recorrido en profundidad recursivo con recorridos enum y postnum
	visitado[i] = true;
  prenum[prenum_ind++]=i + 1;//asignamos el orden de visita prenum que corresponde el nodo i
  for (unsigned j=0;j<L[i].size();j++)
    if (!visitado[L[i][j].j]) {
      dfs_num(L[i][j].j, L, visitado, prenum, prenum_ind, postnum, postnum_ind);
  	}
  postnum[postnum_ind++]=i + 1 ;//asignamos el orden de visita posnum que corresponde al nodo i
}

void GRAFO::RecorridoProfundidad() {
  vector<bool> visitado;
  visitado.resize(n, false);

  vector<unsigned> prenum;
  prenum.resize(n, 0);    
    
  vector<unsigned> postnum;
  postnum.resize(n, 0);

  unsigned prenum_ind = 0;
  unsigned postnum_ind = 0;
  unsigned nodo_solicitado = 0;

  cout << "Introduzca el nodo inicial: [1 - " << n << "]" << endl;
  cin >> (unsigned &) nodo_solicitado;

	while (nodo_solicitado < 1 || nodo_solicitado > n) {
		cout << "El nodo introducido no es válido. Introduzca un nodo entre [1 y " << n << "]" << endl;
		cin >> (unsigned &) nodo_solicitado;
	}

  dfs_num(nodo_solicitado - 1, LS, visitado, prenum, prenum_ind, postnum, postnum_ind);
    //Imprimimos por pantalla prenum y postnum
	cout << "Recorrido en preorden: [";
	for (unsigned i = 0; i < prenum_ind; i++) {
		cout << prenum[i]; 
		if (i < prenum_ind - 1) {
			cout << "] -> [";
		}
	}
	cout << "]" << endl;
	cout << endl;

	cout << "Recorrido en postorden: [";
	for (unsigned i = 0; i < postnum_ind;  i++) {
			cout << postnum[i]; 
		if (i < postnum_ind - 1) {
			cout << "] -> [";
		}
	}
	cout << "]" << endl;

}


 void GRAFO::bfs_num(	unsigned i, //nodo desde el que realizamos el recorrido en amplitud
				vector<LA_nodo>  L, //lista que recorremos, LS o LP; por defecto LS
				vector<unsigned> &pred, //vector de predecesores en el recorrido
				vector<unsigned> &d) //vector de distancias a nodo i+1
//Recorrido en amplitud con la construcci�n de pred y d: usamos la cola
{
  vector<bool> visitado; //creamos e iniciamos el vector visitado
  visitado.resize(n, false);
  visitado[i] = true;

  pred.resize(n, 0); //creamos e inicializamos pred y d
  d.resize(n, 0);
  pred[i] = i;
  d[i] = 0;

  queue<unsigned> cola; //creamos e inicializamos la cola
  cola.push(i);//iniciamos el recorrido desde el nodo i+1

  while (!cola.empty()) { //al menos entra una vez al visitar el nodo i+1 y contin�a hasta que la cola se vac�e
   unsigned k = cola.front(); //cogemos el nodo k+1 de la cola
    cola.pop(); //lo sacamos de la cola
    //Hacemos el recorrido sobre L desde el nodo k+1
      for (unsigned j = 0; j < L[k].size(); j++) {
      //Recorremos todos los nodos u adyacentes al nodo k+1
      //Si el nodo u no est� visitado
				if (!visitado[L[k][j].j]) {
					//Lo visitamos
					visitado[L[k][j].j] = true;
					//Lo metemos en la cola
					cola.push(L[k][j].j);
					//le asignamos el predecesor
					pred[L[k][j].j] = k;
					//le calculamos su etiqueta distancia
					d[L[k][j].j] = d[k] + 1;
      	} 
      }
        //Hemos terminado pues la cola est� vac�a
    }
}

void GRAFO::RecorridoAmplitud() { //Construye un recorrido en amplitud desde un nodo inicial 

  unsigned i = 0;
	vector<unsigned> pred(n, UERROR);
	vector<unsigned> d(n, maxint);

  cout << "Introduzca el nodo inicial (1 <= nodo_introducido <= n) " << endl;
  cin >> (unsigned &) i;

	while (i < 1 || i > n) {
		cout << "El nodo introducido no es válido. Introduzca un nodo entre [1 y " << n << "]" << endl;
		cin >> (unsigned &) i;
	}
    
  bfs_num(i - 1, LS, pred, d);
	cout << "Nodos segun distancia al nodo inicial en numero de aristas" << endl;
	for (unsigned dist = 0; dist < n ; dist++) {
		bool distanciaVacia = true; // Variable para verificar si la distancia está vacía
		for (unsigned nodo = 0; nodo < n; nodo++) {
			if (d[nodo] == dist) {
				if (distanciaVacia) {
					cout << "Distancia " << dist << " aristas: ";
					distanciaVacia = false; // La distancia no está vacía
				}
				cout << nodo + 1 << " ";
			}
		}
		if (!distanciaVacia) {
			cout << endl;
		}
	}

	cout << "Ramas de conexiones" << endl;
	for (unsigned i = 1; i < pred.size(); i++) {
		if (pred[i] != UERROR) {
			vector<unsigned> ramas;
			unsigned nodo_actual = i;
			while (nodo_actual != pred[nodo_actual]) {
				ramas.push_back(nodo_actual + 1);
				nodo_actual = pred[nodo_actual];
			}
			ramas.push_back(nodo_actual + 1);
			for (int j = ramas.size() - 1; j >= 0; j--) {
				cout << ramas[j];
				if (j != 0) {
					cout << " - ";
				}
			}
			cout << endl;
		}
	}
	cout << endl;
}

void GRAFO::DFS_Postnum(unsigned i, vector<bool>& visitado, vector<unsigned>& postnum, unsigned& num ) {
	visitado[i] = true;
	for (unsigned k{0}; k < LS.size(); k++)
		for (unsigned j{0}; j < LS[k].size(); j++)
			if(visitado[j] == false){
				DFS_Postnum(j, visitado, postnum, num);
			}
	postnum[num] = i;
	num = num + 1; 
}

void GRAFO::DFS_Inv(unsigned i, vector<bool>& visitado) {
	visitado[i] = true;
	for (unsigned k{0}; k < LS.size(); k++)
		for (unsigned j {0}; j < LS[k].size(); j++) 
			if (visitado[j] == false ) {
				DFS_Inv(j, visitado);
			}

}

void GRAFO::ComponentesFuertementeConexas() {
	vector<bool> visitado;
	visitado.resize(n, false);
	vector<unsigned> postnum;
	postnum.resize(n, -1); 
	unsigned num{1}, i{1}, cfc{0}; 

	while (i <= n ){
		if (visitado[i] == false){
			DFS_Postnum(i, visitado, postnum, num);
		}
		i = i + 1;
	}

	visitado.resize(n, false);
	i = n;

	while ( i >= 1) {
		if (visitado[postnum[i]] == false) {
			cfc++;
			cout << "Componente fuertemente conexa numero: " << cfc;
			DFS_Inv(postnum[i], visitado);
		}
		i = i - 1;
	}
}

void GRAFO::Kruskal(){
	// Crear un vector de aristas para almacenar todas las aristas del grafo
	vector <AristaPesada> Aristas;
	Aristas.resize(m);
	unsigned k = 0;
	// Recorrer todos los nodos del grafo
	for (unsigned i = 0; i < n; i++) {
		// Recorrer todas las aristas de cada nodo
		for (unsigned j = 0; j < LS[i].size(); j++) {
			// Asegurarnos de que cada arista se meta solo una vez 
			if ( i < LS[i][j].j) {
				// Almacenar los extremos y el peso de la arista
				Aristas[k].extremo1 = i;
				Aristas[k].extremo2 = LS[i][j].j;
				Aristas[k].peso = LS[i][j].c;
				k++; // Incrementar el índice de las aristas
			}
		}
	}

	// Crear un vector para almacenar la raíz de cada nodo
	vector<unsigned> Raiz;
	Raiz.resize(n);
	for (unsigned q = 0; q < n; q++) {
		Raiz[q] = q;
	}

	// Ordenar las aristas en orden descendente de peso
	AristaPesada aux;
	for (unsigned i = 0; i < m - 1; i++) {
		for (unsigned j = 0; j < m - 1; j++) {
			if (Aristas[j].peso < Aristas[j + 1].peso) {
				aux = Aristas[j];
				Aristas[j] = Aristas[j + 1];
				Aristas[j + 1] = aux;
			}
		}
	}

	// Variables para contar las aristas incorporadas y el peso total del MST
	unsigned aristas_incorporadas = 0;
	int contador_aristas = 0;
	int pesoMST = 0;
	unsigned numero_aristas = Aristas.size();

	// Aplicar el algoritmo de Kruskal
	while (aristas_incorporadas < n - 1 && numero_aristas > 0) {
		int indice_minimo = -1;
		int peso_minimo = maxint;

		// Aseguramos que no se formen bucles
		for (unsigned i = 0; i < numero_aristas; i++) {
			if (Raiz[Aristas[i].extremo1] != Raiz[Aristas[i].extremo2] && Aristas[i].peso < peso_minimo) {
				peso_minimo = Aristas[i].peso;
				indice_minimo = i; 
			}
		}

		if (indice_minimo == -1) {
			break;
		}

		// Unir los conjuntos de los extremos de la arista seleccionada
		unsigned raiz1 = Raiz[Aristas[indice_minimo].extremo1];
		unsigned raiz2 = Raiz[Aristas[indice_minimo].extremo2];
		for (unsigned k = 0; k < n; k++) {
			if ( Raiz[k] == raiz2) {
				Raiz[k] = raiz1;
			}
		}

		// Incrementar el contador de aristas incorporadas y el peso total del MST
		aristas_incorporadas++;
		pesoMST += Aristas[indice_minimo].peso;
		// Imprimir la arista incorporada
		cout << "Arista numero " << ++contador_aristas << " incorporada (" << Aristas[indice_minimo].extremo1 + 1 << "," << Aristas[indice_minimo].extremo2 + 1 << ") con peso " << Aristas[indice_minimo].peso << endl;
		// Eliminar la arista seleccionada del vector de aristas
		Aristas[indice_minimo] = Aristas[numero_aristas - 1];
		numero_aristas--;
	}

	// Comprobar si el grafo es conexo
	// Creamos un vector para almacenar las componentes conexas
	vector<unsigned> componentes_conexas;
	componentes_conexas.resize(n);
	unsigned num_componentes = 0;
	
	// Recorremos todos los nodos del grafo
	for (unsigned i = 0; i < n; i++) {
		bool componente_nueva = true;
		
		// Comprobamos si la raíz del nodo i ya está en las componentes conexas
		for (unsigned j = 0; j < num_componentes; j++) {
			if (Raiz[i] == componentes_conexas[j]) {
				componente_nueva = false;
			}
		}
		
		// Si la raíz del nodo i no está en las componentes conexas, la añadimos
		if (componente_nueva) {
			componentes_conexas[num_componentes] = Raiz[i];
			num_componentes++;
		}
	}

	//Imprimir si el grafo es conexo o no
	if (num_componentes == 1) {
		cout << "El grafo es conexo" << endl;
		cout << "El peso del arbol generador de minimo coste es " << pesoMST << endl;
	} else {
		cout << "El grafo no es conexo, tiene " <<  num_componentes <<  " componentes conexas" << endl;
	}

	// Imprimir el número de componentes conexas


}


