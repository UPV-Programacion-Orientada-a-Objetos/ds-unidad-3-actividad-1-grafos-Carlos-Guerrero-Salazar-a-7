# distutils: language = c++

from libc.stdlib cimport free
from libcpp.string cimport string

cdef extern from "GrafoBase.h":
    cdef struct NodoArista:
        int origen
        int destino
        NodoArista* siguiente

    cdef cppclass GrafoBase:
        void cargarDatos(string archivo)
        NodoArista* BFS(int nodoInicio, int profundidadMax)
        int obtenerPeso(int nodo)
        int obtenerNodoMayorGrado()
        int obtenerNumNodos()
        int obtenerNumAristas()

cdef extern from "GrafoDisperso.h":
    cdef cppclass GrafoDisperso(GrafoBase):
        GrafoDisperso(int numNodos)

cdef class PyGrafo:
    cdef GrafoBase* c_grafo

    def __cinit__(self, int numNodos=0):
        # Inicializamos con 0 nodos, se ajustará al cargar datos
        self.c_grafo = new GrafoDisperso(numNodos)

    def __dealloc__(self):
        if self.c_grafo != NULL:
            del self.c_grafo

    def cargar_datos(self, str archivo):
        self.c_grafo.cargarDatos(archivo.encode('utf-8'))

    def bfs(self, int nodo_inicio, int profundidad_max):
        print(f"[Cython] Solicitud recibida: BFS desde Nodo {nodo_inicio}, Profundidad {profundidad_max}.")
        cdef NodoArista* head = self.c_grafo.BFS(nodo_inicio, profundidad_max)
        cdef NodoArista* current = head
        cdef NodoArista* temp
        
        result = []
        while current != NULL:
            result.append((current.origen, current.destino))
            temp = current
            current = current.siguiente
            free(temp)
            

            
        print("[Cython] Retornando lista de adyacencia local a Python.")
        return result

    def obtener_peso(self, int nodo):
        return self.c_grafo.obtenerPeso(nodo)

    def obtener_nodo_mayor_grado(self):
        return self.c_grafo.obtenerNodoMayorGrado()
        
    def obtener_num_nodos(self):
        return self.c_grafo.obtenerNumNodos()

    def obtener_num_aristas(self):
        return self.c_grafo.obtenerNumAristas()
