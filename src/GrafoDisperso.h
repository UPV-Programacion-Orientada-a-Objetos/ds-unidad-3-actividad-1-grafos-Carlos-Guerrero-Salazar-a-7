#ifndef GRAFO_DISPERSO_H
#define GRAFO_DISPERSO_H
#include <string>
#include "GrafoBase.h"

class GrafoDisperso : public GrafoBase {
    public:
    //variables para la matriz dispersa
        int *arrayvalor;
        int *arraycolumna;
        int *arrayfila;
        int numNodos;
        int numAristas;
        
        // Tamaño actual (elementos usados)
        int sizevalor;
        int sizecolumna;
        int sizefila;
        
        // Capacidad total (memoria reservada)
        int capacidadvalor;
        int capacidadcolumna;
        int capacidadfila;
        
        void agregar_array_dinamica(int *&original, int &size, int &capacidad, int valor);

        GrafoDisperso(int numNodos);
        ~GrafoDisperso();
        
        // Implementación de GrafoBase
        void cargarDatos(const std::string& archivo) override;
        NodoArista* BFS(int nodoInicio, int profundidadMax) override; 
        int obtenerPeso(int nodo) override;
        int obtenerNodoMayorGrado() override;
        int obtenerNumNodos() override;
        int obtenerNumAristas() override;

        //usado para verificar la estructura del grafo
        //void MostrarDatos();
        int obtenerNodosPorProfundidad(int nodoInicial, int profundidadMaxima) const;
        void obtenerNumNodosDFS(int nodoActual, int profundidad, int profundidadMaxima, bool visitados[], int &contador) const;
};

#endif