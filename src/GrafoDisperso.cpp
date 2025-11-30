#include "GrafoDisperso.h"
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <chrono>
#include <iomanip>

// Estructura para la cola personalizada
struct NodoCola {
    int nodo;
    int profundidad;
    NodoCola* siguiente;
};

//clase cola para el algoritmo de busqueda en amplitud
class Cola {
private:
    NodoCola* frente;
    NodoCola* final;
public:
    Cola() : frente(nullptr), final(nullptr) {}
    ~Cola() {
        while (!vacia()) {
            desencolar();
        }
    }
    
    void encolar(int n, int p) {
        NodoCola* nuevo = new NodoCola;
        nuevo->nodo = n;
        nuevo->profundidad = p;
        nuevo->siguiente = nullptr;
        
        if (vacia()) {
            frente = final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
    }
    
    void desencolar() {
        if (vacia()) return;
        NodoCola* temp = frente;
        frente = frente->siguiente;
        if (frente == nullptr) {
            final = nullptr;
        }
        delete temp;
    }
    
    bool vacia() {
        return frente == nullptr;
    }
    
    int obtenerFrenteNodo() {
        if (vacia()) return -1;
        return frente->nodo;
    }
    
    int obtenerFrenteProfundidad() {
        if (vacia()) return -1;
        return frente->profundidad;
    }
};


//contructor
GrafoDisperso::GrafoDisperso(int numNodosInicial) {
    numNodos = 0;
    numAristas = 0;
    
    std::cout << "[C++ Core] Inicializando GrafoDisperso..." << std::endl;
    
    // Inicializar tamaños
    sizevalor = 0;
    sizecolumna = 0;
    sizefila = 0;
    
    // Inicializar capacidades con un valor inicial razonable
    capacidadvalor = 1000;
    capacidadcolumna = 1000;
    capacidadfila = 1000;
    
    // Reservar memoria inicial
    arrayvalor = new int[capacidadvalor];
    arraycolumna = new int[capacidadcolumna];
    arrayfila = new int[capacidadfila];
    
    // Inicializar arrayfila con el primer elemento (0)
    arrayfila[0] = 0;
    sizefila = 1;
}

GrafoDisperso::~GrafoDisperso() {
    std::cout << "Destruyendo GrafoDisperso..." << std::endl;
    delete[] arraycolumna;
    delete[] arrayfila;
    delete[] arrayvalor;
}

void GrafoDisperso::agregar_array_dinamica(int *&original, int &size, int &capacidad, int valor) {
    if (size >= capacidad) {
        int nuevaCapacidad = capacidad * 2;
        int *aux = new int[nuevaCapacidad];

        //Me acuerdo que la idea que tenia el profe era de convertir el texto a binario, pero esta solución
        //me parece más simple, el problema que tenia era la cantidad de movimientos al agregar un nuevo valor al array dinamico
        //encontre memcpy de la libreria cstring que copia solo numeros basicos y arrays
        //borra el original y le agrega más tamaño, esto termina haciendolo como maximo 6 veces
        std::memcpy(aux, original, size * sizeof(int));
        delete[] original;
        original = aux;
        capacidad = nuevaCapacidad;
    }
    //luego esto seria un 3n, uno verifica el espacio y el otro agrega el valor y aumenta el tamaño
    // Agregar el nuevo valor
    original[size] = valor;
    size++;
}

/*
//usado para verificar la estructura del grafo
void GrafoDisperso::MostrarDatos() {
    for (int i = 0; i < sizefila - 1; i++) {
        std::cout << "Fila " << i << " (Nodo " << i + 1 << "): "; 
        int inicio = arrayfila[i];
        int fin = arrayfila[i + 1];

        if (inicio == fin) {
            std::cout << "(No tiene aristas de salida)" << std::endl;
        } else {
            for (int j = inicio; j < fin; j++) {
                std::cout << "(" << i + 1 << ", " << arraycolumna[j] << ", peso:" << arrayvalor[j] << ") ";
            }
            std::cout << std::endl;
        }
    }
}
*/

void GrafoDisperso::cargarDatos(const std::string& archivoPath) {
    std::cout << "[C++ Core] Cargando dataset '" << archivoPath << "'..." << std::endl;
    // Resetear datos anteriores
    numNodos = 0;
    numAristas = 0;
    sizevalor = 0;
    sizecolumna = 0;
    sizefila = 1;
    arrayfila[0] = 0;

    std::ifstream archivo(archivoPath);
    
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + archivoPath);
    }
    
    std::string linea;
    int origen, destino, peso;
    int nodoAnterior = -1;
    while (std::getline(archivo, linea)) {
        //si no tiene nada la salta
        if (linea.empty() || linea[0] == '#') {
            continue;
        }
        std::stringstream ss(linea);
        if (!(ss >> origen >> destino)) {
            continue; // Si no se pueden leer dos números, saltar línea
        }
        if (origen != nodoAnterior) {
            while (numNodos < origen) {
                agregar_array_dinamica(arrayfila, sizefila, capacidadfila, numAristas);
                numNodos++;
                nodoAnterior = origen;
            }
        }
        
        // Calcular peso como distancia absoluta
        peso = destino - origen;
        if (peso < 0) peso = -peso;
        agregar_array_dinamica(arraycolumna, sizecolumna, capacidadcolumna, destino);
        agregar_array_dinamica(arrayvalor, sizevalor, capacidadvalor, peso);
        numAristas++;
    }
    while (numNodos < origen) { // 'origen' es el último nodo leído
        agregar_array_dinamica(arrayfila, sizefila, capacidadfila, numAristas);
        numNodos++;
    }
    
    // Cerrar arrayfila agregando el último índice (que debe ser el total de aristas)
    // El tamaño final de arrayfila debe ser numNodos + 1
    agregar_array_dinamica(arrayfila, sizefila, capacidadfila, numAristas);
    numNodos = sizefila - 1; // El número final de nodos es sizefila - 1
    
    archivo.close();
    
    std::cout << "[C++ Core] Carga completa. Nodos: " << numNodos << " | Aristas: " << numAristas << std::endl;

    long long totalBytes = ((long long)numAristas * sizeof(int) * 2) + ((long long)(numNodos + 1) * sizeof(int));  // arrayfila

    double tamañoMB = (double)totalBytes / 1024.0 / 1024.0;

    std::cout << "[C++ Core] Estructura CSR construida. Memoria estimada: " << tamañoMB << " MB." << std::endl;
}

int GrafoDisperso::obtenerNumNodos() {
    return numNodos;
}

NodoArista* GrafoDisperso::BFS(int nodoInicio, int profundidadMax) {
    std::cout << "[C++ Core] Ejecutando BFS nativo..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    if (nodoInicio < 0 || nodoInicio >= numNodos) return nullptr;

    bool* visitados = new bool[numNodos](); // Inicializa a false
    Cola cola; 
    
    cola.encolar(nodoInicio, 0);
    visitados[nodoInicio] = true;
    
    NodoArista* head = nullptr;
    
    while (!cola.vacia()) {
        int u = cola.obtenerFrenteNodo();
        int depth = cola.obtenerFrenteProfundidad();
        cola.desencolar();
        
        if (depth >= profundidadMax) continue;
        
        int inicio = arrayfila[u];
        int fin = arrayfila[u + 1];
        
        for (int i = inicio; i < fin; ++i) {
            int v = arraycolumna[i];
            if (!visitados[v]) {
                visitados[v] = true;
                cola.encolar(v, depth + 1);
                
                // Agregar arista a la lista enlazada
                NodoArista* nuevo = new NodoArista;
                nuevo->origen = u;
                nuevo->destino = v;
                nuevo->siguiente = head;
                head = nuevo;
            }
        }
    }
    
    delete[] visitados;
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    
    // Contar nodos encontrados
    int count = 0;
    NodoArista* temp = head;
    while(temp) {
        count++;
        temp = temp->siguiente;
    }
    
    std::cout << "[C++ Core] Nodos encontrados: " << count << ". Tiempo ejecución: " << duration.count() << "ms." << std::endl;
    
    return head;
}

int GrafoDisperso::obtenerPeso(int nodo) {
    if (nodo < 0 || nodo >= numNodos) return 0;
    return arrayfila[nodo + 1] - arrayfila[nodo];
}

int GrafoDisperso::obtenerNodoMayorGrado() {
    std::cout << "Calculando nodo de mayor grado..." << std::endl;
    int maxGrado = -1;
    int nodoMax = -1;
    
    for (int i = 0; i < numNodos; ++i) {
        int grado = obtenerPeso(i);
        if (grado > maxGrado) {
            maxGrado = grado;
            nodoMax = i;
        }
    }
    return nodoMax;
}

int GrafoDisperso::obtenerNodosPorProfundidad(int nodoInicial, int profundidadMaxima) const {
    std::cout << "Iniciando DFS para contar nodos desde " << nodoInicial << " con profundidad " << profundidadMaxima << "..." << std::endl;
    bool *visitados = new bool[numNodos + 1]();
    int contador = -1;
    
    obtenerNumNodosDFS(nodoInicial, 0, profundidadMaxima, visitados, contador);
    
    delete[] visitados;
    return contador;
}
void GrafoDisperso::obtenerNumNodosDFS(int nodoActual, int profundidad, int profundidadMaxima, bool visitados[], int &contador) const {
    if (profundidad > profundidadMaxima) {
        return;
    }
    if (visitados[nodoActual]) {
        return;
    }
    visitados[nodoActual] = true;
    contador++;
    
    int idx_crs = nodoActual; 
    if (idx_crs >= sizefila - 1) { 
        return; 
    }
    
    int inicio = arrayfila[idx_crs];
    int fin = arrayfila[idx_crs + 1];

    // Recorrer las aristas de salida del nodo actual
    for (int j = inicio; j < fin; j++) {
        int vecino = arraycolumna[j];
        //si el vecino no ha sido visitado se llama para buscar dentro
        obtenerNumNodosDFS(vecino, profundidad + 1, profundidadMaxima, visitados, contador);
    }
}

int GrafoDisperso::obtenerNumAristas() {
    return numAristas;
}