import tkinter as tk
from tkinter import ttk

import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

import random
import threading

window_min_size = 1000
window_max_size = 2000
solution_window_ratio = 0.8
point_size = 1
line_width = 1
default_mutation_rate = 0.2
default_individual_number = 200

matplotlib.use("tkAgg") # make plotlib using tkinter


"""
    use private attributes when ever possible
    use python google style guide
    make a binder to bind the communicator and the FE (delete the function pointer logic)
    
    create a file for solution canva and graph canva
    make a class for the communicator

    /!\ review each function for special case /!\
"""

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
        if size == 0:
            return
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

    def __generateStaticCoordinates(self):
        self.__clear()
        points = [
            [400, 100], [410, 110], [420, 120], [430, 130], [440, 140], [450, 150], [460, 160],
            [470, 170], [480, 180], [490, 190], [500, 200], [510, 210], [520, 220], [530, 230],
            [540, 240], [550, 250], [560, 260], [570, 270], [580, 280], [590, 290], [600, 300],
            [610, 310], [620, 320], [630, 330], [640, 340], [650, 350], [660, 360], [670, 370],
            [680, 380], [690, 390], [700, 400], [690, 410], [680, 420], [670, 430], [660, 440],
            [650, 450], [640, 460], [630, 470], [620, 480], [610, 490], [600, 500], [590, 510],
            [580, 520], [570, 530], [560, 540], [550, 550], [540, 560], [530, 570], [520, 580],
            [510, 590], [500, 600], [490, 610], [480, 620], [470, 630], [460, 640], [450, 650],
            [440, 660], [430, 670], [420, 680], [410, 690], [400, 700], [390, 690], [380, 680],
            [370, 670], [360, 660], [350, 650], [340, 640], [330, 630], [320, 620], [310, 610],
            [300, 600], [290, 590], [280, 580], [270, 570], [260, 560], [250, 550], [240, 540],
            [230, 530], [220, 520], [210, 510], [200, 500], [190, 490], [180, 480], [170, 470],
            [160, 460], [150, 450], [140, 440], [130, 430], [120, 420], [110, 410], [100, 400],
            [110, 390], [120, 380], [130, 370], [140, 360], [150, 350], [160, 340], [170, 330],
            [180, 320], [190, 310], [200, 300]
        ]

        for i in range(len(points)):
            self.__createAndAddPoint(points[i][0], points[i][1])

    def __init__(self):
        self.__coordinates = []
        self.scores = []
        self.generation = []
        self.worst_score = 0

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
        contour_button = tk.Button(self.main_frame, 
                   text="contour", 
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

        contour_button.pack(padx=20, pady=20)

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
        contour_button = tk.Button(self.main_frame, text="generate contour of a drawing", command=self.__generateStaticCoordinates)
        contour_button.pack()


        clear_button = tk.Button(self.main_frame, text="clear", command=self.__clear)
        clear_button.pack()

        # matplotlib
        self.figure = Figure(figsize=(5,4), dpi=100) # dpi = dots per inch (resolution)
        self.axis = self.figure.add_subplot(111)
        self.line = self.axis.plot([],[], "r-")[0]
        self.axis.set_ylim(0, 100)
        self.plot_canvas = FigureCanvasTkAgg(self.figure, master = graph_canvas)
        self.plot_canvas.get_tk_widget().pack(fill="both", expand=True)

    def optimizeAction(self):
        self.__clearSolution()
        self.scores = []
        self.generation = []
        self.worst_score = 0
        self.line.set_data([], [])
        self.axis.set_ylim(0, 10)
        self.plot_canvas.draw()
        threading.Thread(target = lambda : self.__optimize_action(self.__coordinates, self.mutation_rate, self.individual_number)).start()
    
    def updateAction(self, solution, score, generation):
        self.worst_score = max(score, self.worst_score)
        self.scores.append(score)
        self.generation.append(generation) # maybe should add individual generations here
        self.line.set_data(self.generation, self.scores)
        self.axis.set_xlim(0, generation)
        self.axis.set_ylim(0, max(score, self.worst_score))
        self.plot_canvas.draw()
        self.__clearSolution()
        self.__showResult(solution)
    
    def setOptimizeAction(self, action):
        self.__optimize_action = action

    def display(self):
        self.main_frame.mainloop()