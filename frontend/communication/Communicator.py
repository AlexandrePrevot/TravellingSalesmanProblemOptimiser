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

    response = stub.Optimize(optimization_req)

    solution = []
    for coord in response.coordinates:
        solution.append([coord.coordX, coord.coordY])
    print("Greeter client received: " + str(response.accepted))

    return solution

