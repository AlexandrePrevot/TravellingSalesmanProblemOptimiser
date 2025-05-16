""" MainWindow of the FE"""
import tkinter as tk
from tkinter import ttk

import random
import threading

import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

WINDOW_MIN_SIZE = 1000
WINDOW_MAX_SIZE = 2000
WINDOW_TITLE = "Travelling Salesman Problem Optimiser"
SOLUTION_WINDOW_RATIO = 0.8
POINT_SIZE = 5
LINE_WIDTH = 1
DEFAULT_SELECTION_RATE = 0.2
DEFAULT_INDIVIDUAL_NUMBER = 200

matplotlib.use("tkAgg")  # make plotlib using tkinter


class MainWindow:
    """ MainWindow of the FE
    
    Composed of 3 frames: 
    
    - the main Frame composed of 2 frames
    - Solution frame
    - Graphic frame

    Solution frame has a canva that display the current solution
    Graphic frame display the result score
    """

    def optimize_action(self):
        """ action to do clicking on optimize action button
        
        currently start a thread on clicking event to avoid waiting for 
        a synchronous reply to update the FE. 
        client code must specify what function to call when clicking the button
        """
        self.__clear_solution()
        self.__scores = []
        self.__generation = []
        self.__worst_score = 0
        self.__line.set_data([], [])
        self.__axis.set_ylim(0, 10)
        self.__plot_canvas.draw()
        self.__optimising_tag_id = self.__solution_canvas.create_window(
            10, 10, window=self.__optimising_tag, anchor="nw")

        self.__set_algo_blocker_button(False)
        threading.Thread(
            target=self.__trigger_optimisation_then_update).start()

    def update_action(self, solution, score, generation, update_graph):
        """ action to do when the FE should be updated
        
        update the solution and the graph.
        """
        self.__worst_score = max(score, self.__worst_score)
        if update_graph:
            self.__scores.append(score)
            self.__generation.append(
                generation)  # maybe should add individual generations here
            self.__line.set_data(self.__generation, self.__scores)
            self.__axis.set_xlim(0, generation)
            self.__axis.set_ylim(0, max(score, self.__worst_score))
            self.__plot_canvas.draw()
        self.__clear_solution()
        self.__show_result(solution)

    def set_optimize_action(self, action):
        """set the function call when clicking on optimize"""
        self.__optimize_action = action

    def display(self):
        """start displaying the FE"""
        self.__main_frame.mainloop()

    def __create_and_add_point_with_event(self, event):
        pos_x = event.x
        pos_y = event.y

        self.__create_and_add_point(pos_x, pos_y)

    def __create_and_add_point(self, pos_x, pos_y):
        self.__coordinates.append([pos_x, pos_y])
        self.__solution_canvas.create_rectangle(pos_x,
                                                pos_y,
                                                pos_x + POINT_SIZE,
                                                pos_y + POINT_SIZE,
                                                fill="black")

    def __clear_solution(self):
        self.__solution_canvas.delete("line")

    def __clear(self):
        self.__solution_canvas.delete("all")
        self.__coordinates = []

    def __show_result(self, solution):
        size = len(solution)
        if size == 0:
            return
        for i in range(size - 1):
            self.__solution_canvas.create_line(solution[i][0],
                                               solution[i][1],
                                               solution[i + 1][0],
                                               solution[i + 1][1],
                                               width=LINE_WIDTH,
                                               tag="line")
        self.__solution_canvas.create_line(solution[size - 1][0],
                                           solution[size - 1][1],
                                           solution[0][0],
                                           solution[0][1],
                                           width=LINE_WIDTH,
                                           tag="line")

    def __get_number_of_individual(self):
        try:
            number = int(self.__individual_entry.get())
            if number < 0:
                raise ValueError
            self.__individual_number = number
        except ValueError:
            print("Please enter a valid number")

    def __get_selection_rate(self):
        try:
            rate = float(self.__selection_rate_entry.get())
            if rate > 0 and rate <= 1:
                self.__selection_rate = float(
                    self.__selection_rate_entry.get())
            else:
                raise ValueError
        except ValueError:
            print("Please enter a valid number")

    def __generate_random_coordinates(self):
        to_generate = 0
        try:
            to_generate = int(self.__random_generation_entry.get())
        except ValueError:
            print("Please enter a valid number")

        if to_generate <= 0:
            return

        self.__clear()

        for _ in range(to_generate):
            pos_x = random.randint(0, self.__solution_canvas.winfo_reqwidth())
            pos_y = random.randint(0, self.__solution_canvas.winfo_reqheight())

            self.__create_and_add_point(pos_x, pos_y)

    def __terminate_process_action(self, solution):
        if self.__optimising_tag_id is not None:
            self.__solution_canvas.delete(self.__optimising_tag_id)
            self.__optimising_tag_id = None
        self.__set_algo_blocker_button(True)
        self.update_action(solution, -1, -1, False)

    def __trigger_optimisation_then_update(self):
        result = self.__optimize_action(self.__coordinates,
                                        self.__selection_rate,
                                        self.__individual_number,
                                        self.__real_time_update.get() == 1)
        self.__terminate_process_action(result)

    def __set_algo_blocker_button(self, activate):
        if activate:
            self.__optimize_button.config(state="normal")
            self.__clear_button.config(state="normal")
        else:
            self.__optimize_button.config(state="disabled")
            self.__clear_button.config(state="disabled")

    def __init__(self):
        self.__coordinates = []
        self.__scores = []
        self.__generation = []
        self.__worst_score = 0

        self.__optimize_action = None
        self.__individual_number = DEFAULT_INDIVIDUAL_NUMBER
        self.__selection_rate = DEFAULT_SELECTION_RATE

        self.__main_frame = tk.Tk()
        self.__main_frame.title(WINDOW_TITLE)
        self.__main_frame.configure(background="light grey")
        self.__main_frame.minsize(WINDOW_MIN_SIZE, WINDOW_MIN_SIZE)
        self.__main_frame.maxsize(WINDOW_MAX_SIZE, WINDOW_MAX_SIZE)

        result_frame = tk.Frame(self.__main_frame)
        result_frame.pack(anchor="center", expand=True)

        solution_frame = tk.Frame(result_frame,
                                  bg="light grey",
                                  highlightbackground="black",
                                  highlightthickness=1)
        solution_frame.pack(side=tk.LEFT, expand=True, fill=tk.Y)

        graph_frame = tk.Frame(result_frame,
                               bg="light grey",
                               highlightbackground="black",
                               highlightthickness=1)
        graph_frame.pack(side=tk.RIGHT, expand=True, fill=tk.Y)

        self.__graph_label = ttk.Label(graph_frame,
                                       background="light grey",
                                       text="Result",
                                       font=("Arial", 20))
        self.__graph_label.pack(pady=20)
        self.__solution_label = ttk.Label(solution_frame,
                                          background="light grey",
                                          text="Solution",
                                          font=("Arial", 20))
        self.__solution_label.pack(pady=20)

        solution_canvas_height = WINDOW_MIN_SIZE * SOLUTION_WINDOW_RATIO
        solution_canvas_width = WINDOW_MIN_SIZE * SOLUTION_WINDOW_RATIO

        graph_canvas_height = WINDOW_MIN_SIZE * SOLUTION_WINDOW_RATIO
        graph_canvas_width = WINDOW_MIN_SIZE * SOLUTION_WINDOW_RATIO

        self.__solution_canvas = tk.Canvas(solution_frame,
                                           width=solution_canvas_width,
                                           height=solution_canvas_height,
                                           bg="white",
                                           highlightthickness=0)
        self.__solution_canvas.bind("<Button-1>",
                                    self.__create_and_add_point_with_event)
        self.__solution_canvas.pack()  # Centered vertically
        self.__optimising_tag = ttk.Label(self.__solution_canvas,
                                          background="white",
                                          text="optimising...",
                                          font=("Arial", 10))
        self.__optimising_tag_id = None
        graph_canvas = tk.Canvas(graph_frame,
                                 width=graph_canvas_width,
                                 height=graph_canvas_height,
                                 bg="white",
                                 highlightthickness=1)
        graph_canvas.pack()  # Centered vertically

        self.__real_time_update = tk.IntVar(value=1)
        real_time_update_checkbox = tk.Checkbutton(
            graph_frame,
            text='real-time update',
            variable=self.__real_time_update,
            onvalue=1,
            offvalue=0)
        real_time_update_checkbox.pack(side=tk.LEFT)
        self.__optimize_button = tk.Button(graph_frame,
                                           text="Optimise",
                                           command=self.optimize_action,
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

        self.__optimize_button.pack(padx=20, pady=20)

        self.__individual_entry = tk.Entry(graph_frame)
        self.__individual_entry.pack(pady=10)
        individual_entry_button = tk.Button(
            graph_frame,
            text="submit number of individual",
            command=self.__get_number_of_individual)
        individual_entry_button.pack()

        self.__selection_rate_entry = tk.Entry(graph_frame)
        self.__selection_rate_entry.pack(pady=10)
        mutation_rate_button = tk.Button(graph_frame,
                                         text="submit selection rate",
                                         command=self.__get_selection_rate)
        mutation_rate_button.pack()

        self.__random_generation_entry = tk.Entry(graph_frame)
        self.__random_generation_entry.pack(pady=10)
        random_generation_button = tk.Button(
            graph_frame,
            text="generate random coordinates",
            command=self.__generate_random_coordinates)
        random_generation_button.pack()

        self.__clear_button = tk.Button(graph_frame,
                                        text="clear",
                                        command=self.__clear)
        self.__clear_button.pack()

        # matplotlib
        self.__figure = Figure(figsize=(5, 4),
                               dpi=100)  # dpi = dots per inch (resolution)
        self.__axis = self.__figure.add_subplot(111)
        self.__line = self.__axis.plot([], [], "r-", marker='o')[0]
        self.__axis.set_ylim(0, 100)
        self.__axis.grid(True)
        self.__axis.set(xlabel="Number of generations", ylabel="Best score")
        self.__plot_canvas = FigureCanvasTkAgg(self.__figure,
                                               master=graph_canvas)
        self.__plot_canvas.get_tk_widget().pack(fill="both", expand=True)
