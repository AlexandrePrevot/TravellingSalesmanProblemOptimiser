import sys
import os
import grpc

parent_dir = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
request_dir = os.path.abspath(os.path.join(parent_dir, "generated"))
sys.path.insert(0, request_dir)

import request_pb2
import request_pb2_grpc

def run(coordinate_list):
    print("sending coordinates : " + str(coordinate_list))
    channel = grpc.insecure_channel('localhost:50051')
    stub = request_pb2_grpc.OptimizationStub(channel)
    optimization_req = request_pb2.OptimizationRequest()

    for coord in coordinate_list:
        msg_coord = optimization_req.coordinates.add()
        msg_coord.coordX = coord[0]
        msg_coord.coordY = coord[1]

    """
    coord1 = optimization_req.coordinates.add()
    coord2 = optimization_req.coordinates.add()
    coord3 = optimization_req.coordinates.add()

    coord1.coordX = 12
    coord1.coordY = 11

    coord2.coordX = -5
    coord2.coordY = 2

    coord3.coordX = 17
    coord3.coordY = 6
    """

    response = stub.Optimize(optimization_req)
    print("Greeter client received: " + str(response.accepted))
