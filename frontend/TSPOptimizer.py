import sys
import os

request_dir = os.path.abspath('generated')
sys.path.insert(0, request_dir)

import grpc
import request_pb2
import request_pb2_grpc

def run():
    channel = grpc.insecure_channel('localhost:50051')
    stub = request_pb2_grpc.OptimizationStub(channel)
    optimization_req = request_pb2.OptimizationRequest()

    coord1 = optimization_req.coordinates.add()
    coord2 = optimization_req.coordinates.add()
    coord3 = optimization_req.coordinates.add()

    coord1.coordX = 12
    coord1.coordY = 11

    coord2.coordX = -5
    coord2.coordY = 2

    coord3.coordX = 17
    coord3.coordY = 6

    response = stub.Optimize(optimization_req)
    print("Greeter client received: " + str(response.accepted))

print("hello world!")

if __name__ == '__main__':
    #run()
    