"""Handle communication with the BE"""
import sys
import os
from concurrent import futures
import grpc

parent_dir = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
request_dir = os.path.abspath(os.path.join(parent_dir, "generated"))
sys.path.insert(0, request_dir)

import request_pb2
import request_pb2_grpc
from google.protobuf.empty_pb2 import Empty


# gRPC generation is not necessarily style guide friendly
# pylint: disable=too-few-public-methods
# pylint: disable=invalid-name
class OptimizationServicer(request_pb2_grpc.OptimizationServicer):
    """ Notification Service
    receives messages from the backend that are asynchronous
    """

    def Update(self, request, _):
        """call the update call back action"""
        solution = []
        for coord in request.coordinates:
            solution.append([coord.coordX, coord.coordY])
        self.__update_callback(solution, request.score, request.generation,
                               True)
        return Empty()

    def __init__(self, update_callback):
        self.__update_callback = update_callback


# pylint: enable=too-few-public-methods
# pylint: enable=invalid-name


def serve(update_action):
    """Create the python server with the update function pointer given
    
    The server will later on receive asynchronous update from the BE
    """
    # need to process all request sequentially anyway, no need to have
    # more workers
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=1))
    request_pb2_grpc.add_OptimizationServicer_to_server(
        OptimizationServicer(update_action), server)
    server.add_insecure_port('[::]:50052')
    server.start()
    server.wait_for_termination()


def optimize(coordinate_list, selection_rate, individual_number,
             real_time_update):
    """Sends an optimization request to the BE"""
    channel = grpc.insecure_channel('localhost:50051')
    stub = request_pb2_grpc.OptimizationStub(channel)
    optimization_req = request_pb2.OptimizationRequest()
    optimization_req.individualNumber = individual_number
    optimization_req.selectionRate = selection_rate
    optimization_req.realTimeUpdate = real_time_update
    for coord in coordinate_list:
        msg_coord = optimization_req.coordinates.add()
        msg_coord.coordX = coord[0]
        msg_coord.coordY = coord[1]

    response = None
    try:
        response = stub.Optimize(optimization_req)
    except grpc.RpcError as err:
        print("communication with B.E. failed : " + str(err))

    if response is None:
        return []

    solution = []
    for coord in response.coordinates:
        solution.append([coord.coordX, coord.coordY])
    return solution
