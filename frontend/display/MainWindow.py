import tkinter as tk
from tkinter import ttk

import random
import threading

window_min_size = 1000
window_max_size = 2000
solution_window_ratio = 0.33
point_size = 5
line_width = 1
default_mutation_rate = 0.2
default_individual_number = 200

class MainWindow:
    def __createAndAddPointWithEvent(self, event):
        posX = event.x
        posY = event.y

        self.__createAndAddPoint(posX, posY)

    def __createAndAddPoint(self, posX, posY):
        self.__coordinates.append([posX, posY])
        self.solution_canvas.create_rectangle(posX, posY, posX + point_size, posY + point_size, fill="black")


    def __clearSolution(self):
        self.solution_canvas.delete("line")

    def __clear(self):
        self.solution_canvas.delete("all")
        self.__coordinates = []

    def __showResult(self, solution):
        size = len(solution)
        for i in range(size - 1):
            self.solution_canvas.create_line(solution[i][0],solution[i][1], solution[i+1][0],solution[i+1][1], width=line_width, tag="line")
        self.solution_canvas.create_line(solution[size - 1][0],solution[size - 1][1], solution[0][0],solution[0][1], width=line_width, tag="line")

    def __getNumberOfIndividual(self):
        try:
            self.individual_number = int(self.individual_entry.get())
        except ValueError:
            print("Please enter a valid number")

    def __getMutationRate(self):
        try:
            self.mutation_rate = float(self.mutation_rate_entry.get())
        except ValueError:
            print("Please enter a valid number")

    def __generateRandomCoordinates(self):
        to_generate = 0
        try:
            to_generate = int(self.random_generation_entry.get())
        except ValueError:
            print("Please enter a valid number")

        if to_generate == 0:
            return

        self.__clear()

        for i in range(to_generate):
            posX = random.randint(0, self.solution_canvas.winfo_reqwidth())
            posY = random.randint(0, self.solution_canvas.winfo_reqheight())

            self.__createAndAddPoint(posX, posY)

    def __init__(self):
        self.__coordinates = []
        self.__optimize_action = None
        self.individual_number = default_individual_number
        self.mutation_rate = default_mutation_rate

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
        self.solution_canvas.bind("<Button-1>", self.__createAndAddPointWithEvent)
        self.solution_canvas.pack(pady=100)  # Centered vertically

        graph_canvas = tk.Canvas(graph_frame, width=graph_canvas_width, height=graph_canvas_height, bg="white", highlightthickness=0)
        graph_canvas.pack(pady=100)  # Centered vertically

        optimize_button = tk.Button(self.main_frame, 
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

        optimize_button.pack(padx=20, pady=20)

        self.individual_entry = tk.Entry(self.main_frame)
        self.individual_entry.pack(pady=10)
        individual_entry_button = tk.Button(self.main_frame, text="submit number of individual", command=self.__getNumberOfIndividual)
        individual_entry_button.pack()

        self.mutation_rate_entry = tk.Entry(self.main_frame)
        self.mutation_rate_entry.pack(pady=10)
        mutation_rate_button = tk.Button(self.main_frame, text="submit mutation rate", command=self.__getMutationRate)
        mutation_rate_button.pack()


        self.random_generation_entry = tk.Entry(self.main_frame)
        self.random_generation_entry.pack(pady=10)
        random_generation_button = tk.Button(self.main_frame, text="generate random coordinates", command=self.__generateRandomCoordinates)
        random_generation_button.pack()

        clear_button = tk.Button(self.main_frame, text="clear", command=self.__clear)
        clear_button.pack()

        #here add matplot lib figure

    def optimizeAction(self):
        self.__clearSolution()
        threading.Thread(target = lambda : self.__optimize_action(self.__coordinates, self.mutation_rate, self.individual_number)).start()
        #self.__showResult(solution)
    
    def updateAction(self, solution):
        self.__clearSolution()
        self.__showResult(solution)
    
    def setOptimizeAction(self, action):
        self.__optimize_action = action

    def display(self):
        self.main_frame.mainloop()