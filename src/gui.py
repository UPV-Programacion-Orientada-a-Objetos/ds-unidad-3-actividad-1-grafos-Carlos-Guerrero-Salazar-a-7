import tkinter as tk
from tkinter import filedialog, messagebox
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import networkx as nx
import sys
import os

sys.path.append(os.getcwd())

try:
    from neuronet import PyGrafo
except ImportError as e:
    messagebox.showerror("Error", f"Failed to import 'neuronet': {e}")
    sys.exit(1)

class NeuroNetGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("NeuroNet: Análisis de Grafos Masivos")
        self.root.geometry("1200x800")

        self.grafo = PyGrafo()
        self.graph_nx = nx.Graph()
        # para hacer un area de visualizacion se usa el frame
        # su estructura es nombre del area, ancho, alto, padding 
        # --- Sidebar ---
        self.sidebar = tk.Frame(root, width=300, padx=10, pady=10)
        self.sidebar.pack(side=tk.LEFT, fill=tk.Y)
        
        # se crea un area para la carga de datos
        tk.Label(self.sidebar, text="Carga de Datos").pack(pady=(0, 5))
        self.btn_load = tk.Button(self.sidebar, text="Cargar Dataset", command=self.load_dataset)
        self.btn_load.pack(fill=tk.X, pady=5)
        
        self.lbl_status = tk.Label(self.sidebar, text="Estado: Esperando...", wraplength=280)
        self.lbl_status.pack(pady=5)
        
        # se crea un area para las metricas
        tk.Label(self.sidebar, text="Métricas").pack(pady=(20, 5))
        self.lbl_nodes = tk.Label(self.sidebar, text="Nodos: 0")
        self.lbl_nodes.pack(anchor="w")
        self.lbl_edges = tk.Label(self.sidebar, text="Aristas: 0")
        self.lbl_edges.pack(anchor="w")
        
        # Es un boton para encontrar el nodo con mayor grado
        tk.Label(self.sidebar, text="Análisis").pack(pady=(20, 5))
        self.btn_critical = tk.Button(self.sidebar, text="Calcular el nodo con mayor grado", command=self.find_critical_node)
        self.btn_critical.pack(fill=tk.X, pady=5)
        self.lbl_critical = tk.Label(self.sidebar, text="-", wraplength=280)
        self.lbl_critical.pack(pady=5)
        
        #asignadores de valores para el bfs
        tk.Label(self.sidebar, text="Simulación BFS").pack(pady=(20, 5))
        
        tk.Label(self.sidebar, text="Nodo Inicio:").pack(anchor="w")
        self.entry_start_node = tk.Entry(self.sidebar)
        self.entry_start_node.pack(fill=tk.X)
        
        tk.Label(self.sidebar, text="Profundidad:").pack(anchor="w")
        self.entry_depth = tk.Entry(self.sidebar)
        self.entry_depth.pack(fill=tk.X)
        
        self.btn_bfs = tk.Button(self.sidebar, text="Ejecutar BFS", command=self.run_bfs)
        self.btn_bfs.pack(fill=tk.X, pady=10)
        
        # --- Main Area (Visualization) ---
        self.main_area = tk.Frame(root)
        # se crea un area para el grafo que agarra el resto de la pantalla
        self.main_area.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
        self.figure = plt.Figure(figsize=(5, 4), dpi=100)
        self.ax = self.figure.add_subplot(111)
        self.ax.set_title("Visualización de Grafo")
        self.ax.axis('off')
        
        self.canvas = FigureCanvasTkAgg(self.figure, master=self.main_area)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

    def load_dataset(self):
        file_path = filedialog.askopenfilename(
            title="Seleccionar Dataset",
            filetypes=(("Archivos de Texto", "*.txt"), ("Todos los archivos", "*.*"))
        )
        
        if not file_path:
            return

        try:
            filename = os.path.basename(file_path)
            self.lbl_status.config(text=f"Cargando {filename}...")
            self.root.update()
            
            self.grafo.cargar_datos(file_path)
            
            num_nodos = self.grafo.obtener_num_nodos()
            self.lbl_nodes.config(text=f"Nodos: {num_nodos}")
            
            num_aristas = self.grafo.obtener_num_aristas()
            self.lbl_edges.config(text=f"Aristas: {num_aristas}")
            
            self.lbl_status.config(text="Carga completa.")
            
            # Reset visualization
            self.ax.clear()
            self.ax.set_title(f"Grafo {filename} Cargado (Vista Vacía)")
            self.ax.axis('off')
            self.canvas.draw()
            
        except Exception as e:
            messagebox.showerror("Error", f"Error al cargar datos: {e}")
            self.lbl_status.config(text="Error en carga.")

    def find_critical_node(self):
        node = self.grafo.obtener_nodo_mayor_grado()
        weight = self.grafo.obtener_peso(node)
        self.lbl_critical.config(text=f"Nodo: {node}\nGrado: {weight}")

    def run_bfs(self):
        start_node = int(self.entry_start_node.get())
        depth = int(self.entry_depth.get())
            
        edges = self.grafo.bfs(start_node, depth)
            
        if not edges:
            messagebox.showinfo("Info", "No se encontraron nodos en el rango especificado.")
            return

        self.graph_nx.clear()
        for u, v in edges:
            self.graph_nx.add_edge(u, v)
            
        self.ax.clear()
        self.ax.set_title(f"BFS desde {start_node} (Profundidad {depth})")
        self.ax.axis('off')
            
        pos = nx.spring_layout(self.graph_nx, seed=42) 
        nx.draw_networkx(self.graph_nx, pos, ax=self.ax, with_labels=True, node_size=300, node_color='skyblue', font_size=8)
            
        self.canvas.draw()

if __name__ == "__main__":
    root = tk.Tk()
    app = NeuroNetGUI(root)
    root.mainloop()
