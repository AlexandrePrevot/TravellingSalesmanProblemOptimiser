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
    print("Greeter client received: " + str(solution))

    return solution

"""
not crossed
[[44.0, 240.0], [92.0, 191.0], [134.0, 172.0], [181.0, 220.0], [175.0, 174.0], [160.0, 114.0], [133.0, 82.0], 
[117.0, 109.0], [108.0, 109.0], [117.0, 148.0], [77.0, 171.0], [38.0, 138.0]]
26480


crossed but better solution ?
[[181.0, 220.0], [175.0, 174.0], [160.0, 114.0], [133.0, 82.0], [117.0, 109.0], [108.0, 109.0], [117.0, 148.0], 
[134.0, 172.0], [92.0, 191.0], [44.0, 240.0], [77.0, 171.0], [38.0, 138.0]]
26553

"""