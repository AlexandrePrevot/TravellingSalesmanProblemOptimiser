"""bind the python FE and the communication module"""

import sys
import os
import threading

communication_dir = os.path.abspath('communication')
display_dir = os.path.abspath('display')

sys.path.insert(0, communication_dir)
sys.path.insert(1, display_dir)

import Communicator
import main_window

if __name__ == '__main__':
    root = main_window.MainWindow()
    root.set_optimize_action(Communicator.optimize)

    grpc_server_thread = threading.Thread(
        target=lambda: Communicator.serve(root.update_action), daemon=True)
    grpc_server_thread.start()

    root.display()
