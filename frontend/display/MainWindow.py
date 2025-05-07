import tkinter as tk
from tkinter import ttk

class MainWindow:
    def __init__(self):
        self.main_frame = tk.Tk()
        # Setting some window properties
        self.main_frame.title("Travelling Salesman Problem Optimizer")
        self.main_frame.configure(background="grey")
        self.main_frame.minsize(1000, 1000)
        self.main_frame.maxsize(2000, 2000)

        result_frame = tk.Frame(self.main_frame, width=500, height=500)
        result_frame.pack(anchor="center", expand=True)

        solution_frame = tk.Frame(result_frame, bg="white", width=500, height=500)
        solution_frame.pack(padx=20, side=tk.LEFT, expand=True, fill=tk.Y)

        graph_frame = tk.Frame(result_frame, bg="white", width=500, height=500)
        graph_frame.pack(padx=20,side=tk.RIGHT, expand=True, fill=tk.Y)

        self.graph_label = ttk.Label(graph_frame, text="Result: 0.000", font=("Arial", 20))
        self.graph_label.pack(pady=20)
        self.solution_label = ttk.Label(solution_frame, text="Solution", font=("Arial", 20))
        self.solution_label.pack(pady=20)

        solution_canvas = tk.Canvas(solution_frame, width=100, height=100, bg="white", highlightthickness=0)
        solution_canvas.pack(pady=100)  # Centered vertically
        solution_canvas.create_rectangle(10, 10, 90, 90, fill="blue")

        graph_canvas = tk.Canvas(graph_frame, width=100, height=100, bg="white", highlightthickness=0)
        graph_canvas.pack(pady=100)  # Centered vertically
        graph_canvas.create_rectangle(10, 10, 90, 90, fill="blue")

        #here add matplot lib figure




    
    def display(self):
        self.main_frame.mainloop()


root = MainWindow()
root.display()