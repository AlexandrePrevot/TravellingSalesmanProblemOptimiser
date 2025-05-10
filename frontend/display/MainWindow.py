import tkinter as tk
from tkinter import ttk

window_min_size = 1000
window_max_size = 2000
solution_window_ratio = 0.33
point_size = 5
line_width = 1

class MainWindow:
    def __createAndAddPoint(self, event):
        posX = event.x
        posY = event.y

        self.__coordinates.append([posX, posY])
        self.solution_canvas.create_rectangle(posX, posY, posX + point_size, posY + point_size, fill="black")
    
    def __showResult(self, solution):
        size = len(solution)
        for i in range(size - 1):
            self.solution_canvas.create_line(solution[i][0],solution[i][1], solution[i+1][0],solution[i+1][1], width=line_width)
        self.solution_canvas.create_line(solution[size - 1][0],solution[size - 1][1], solution[0][0],solution[0][1], width=line_width)

    def __init__(self):
        self.__coordinates = []
        self.__optimize_action = None

        self.main_frame = tk.Tk()
        self.main_frame.title("Travelling Salesman Problem Optimizer")
        self.main_frame.configure(background="grey")
        self.main_frame.minsize(window_min_size, window_min_size)
        self.main_frame.maxsize(window_max_size, window_max_size)

        result_frame = tk.Frame(self.main_frame)
        result_frame.pack(anchor="center", expand=True)

        solution_frame = tk.Frame(result_frame, bg="white")
        solution_frame.pack(padx=20, side=tk.LEFT, expand=True, fill=tk.Y)

        graph_frame = tk.Frame(result_frame, bg="white")
        graph_frame.pack(padx=20,side=tk.RIGHT, expand=True, fill=tk.Y)

        self.graph_label = ttk.Label(graph_frame, text="Result: 0.000", font=("Arial", 20))
        self.graph_label.pack(pady=20)
        self.solution_label = ttk.Label(solution_frame, text="Solution", font=("Arial", 20))
        self.solution_label.pack(pady=20)

        solution_canvas_height = window_min_size * solution_window_ratio
        solution_canvas_width = window_min_size * solution_window_ratio

        graph_canvas_height = window_min_size * solution_window_ratio
        graph_canvas_width = window_min_size * solution_window_ratio

        self.solution_canvas = tk.Canvas(solution_frame, width=solution_canvas_width, height=solution_canvas_height, bg="white", highlightthickness=0)
        self.solution_canvas.bind("<Button-1>", self.__createAndAddPoint)
        self.solution_canvas.pack(pady=100)  # Centered vertically

        graph_canvas = tk.Canvas(graph_frame, width=graph_canvas_width, height=graph_canvas_height, bg="white", highlightthickness=0)
        graph_canvas.pack(pady=100)  # Centered vertically

        button = tk.Button(self.main_frame, 
                   text="Optimize", 
                   command=self.optimizeAction,
                   activebackground="blue", 
                   activeforeground="white",
                   anchor="center",
                   bd=3,
                   bg="lightgray",
                   cursor="hand2",
                   disabledforeground="gray",
                   fg="black",
                   font=("Arial", 12),
                   height=2,
                   highlightbackground="black",
                   highlightcolor="green",
                   highlightthickness=2,
                   justify="center",
                   overrelief="raised",
                   padx=10,
                   pady=5,
                   width=15,
                   wraplength=100)
        button.pack(padx=20, pady=20)

        #here add matplot lib figure

    def optimizeAction(self):
        solution = self.__optimize_action(self.__coordinates)
        self.__showResult(solution)
    
    def setOptimizeAction(self, action):
        self.__optimize_action = action

    def display(self):
        self.main_frame.mainloop()