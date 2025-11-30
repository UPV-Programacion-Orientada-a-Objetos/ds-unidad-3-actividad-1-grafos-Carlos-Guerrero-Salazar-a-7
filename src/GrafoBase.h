#ifndef GRAFOBASE_H
#define GRAFOBASE_H

#include <string>

struct NodoArista {
    int origen;
    int destino;
    NodoArista* siguiente;
};

class GrafoBase {
public:
    virtual ~GrafoBase() {}
    
    virtual void cargarDatos(const std::string& archivo) = 0;
    
    virtual NodoArista* BFS(int nodoInicio, int profundidadMax) = 0;
    
    virtual int obtenerPeso(int nodo) = 0;
    
    virtual int obtenerNodoMayorGrado() = 0;
    
    virtual int obtenerNumNodos() = 0;
    
    virtual int obtenerNumAristas() = 0;
};

#endif // GRAFOBASE_H
