import sys
import os
import threading

communication_dir = os.path.abspath('communication')
display_dir = os.path.abspath('display')

sys.path.insert(0, communication_dir)
sys.path.insert(1, display_dir)

import Communicator
import MainWindow

if __name__ == '__main__':
    root = MainWindow.MainWindow()
    root.setOptimizeAction(Communicator.optimize)
    
    grpc_server_thread = threading.Thread(target = lambda : Communicator.serve(root.updateAction), daemon = True)
    grpc_server_thread.start()
    
    root.display()
    
    