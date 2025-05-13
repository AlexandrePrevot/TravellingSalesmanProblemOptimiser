import sys
import os
import grpc
from concurrent import futures

parent_dir = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
request_dir = os.path.abspath(os.path.join(parent_dir, "generated"))
sys.path.insert(0, request_dir)

import request_pb2
import request_pb2_grpc
from google.protobuf.empty_pb2 import Empty

#take a look at https://stackoverflow.com/questions/38387443/how-to-implement-a-async-grpc-python-server
# for asynchronous request

class OptimizationServicer(request_pb2_grpc.OptimizationServicer):
    def Update(self, request, context):
        solution = []
        for coord in request.coordinates:
            solution.append([coord.coordX, coord.coordY])
        self.__update_callback(solution, request.score, request.generation)
        return Empty()
    
    def __init__(self, update_callback):
        self.__update_callback = update_callback

def serve(update_action):
    # need to process all request sequentially anyway, no need to have
    # more workers
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=1))
    request_pb2_grpc.add_OptimizationServicer_to_server(OptimizationServicer(update_action), server)
    server.add_insecure_port('[::]:50052')
    server.start()
    print("frontend has started")
    server.wait_for_termination()

def optimize(coordinate_list, mutation_rate, individual_number):
    print("sending coordinates : " + str(coordinate_list))
    channel = grpc.insecure_channel('localhost:50051')
    stub = request_pb2_grpc.OptimizationStub(channel)
    optimization_req = request_pb2.OptimizationRequest()
    optimization_req.individualNumber = individual_number
    optimization_req.mutationRate = mutation_rate

    for coord in coordinate_list:
        msg_coord = optimization_req.coordinates.add()
        msg_coord.coordX = coord[0]
        msg_coord.coordY = coord[1]

    response = stub.Optimize(optimization_req)

    solution = []
    for coord in response.coordinates:
        solution.append([coord.coordX, coord.coordY])

    print("received: " + str(solution))

    return solution