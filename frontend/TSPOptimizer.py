import sys
import os

request_dir = os.path.abspath('generated')
sys.path.insert(0, request_dir)

import grpc
import request_pb2
import request_pb2_grpc

def run():
    channel = grpc.insecure_channel('localhost:50051')
    stub = request_pb2_grpc.GreeterStub(channel)
    response = stub.SayHello(request_pb2.HelloRequest(name='1'))
    print("Greeter client received: " + response.message)
    response = stub.SayHello(request_pb2.HelloRequest(name='2'))
    print("Greeter client received: " + response.message)
    response = stub.SayHello(request_pb2.HelloRequest(name='3'))
    print("Greeter client received: " + response.message)
    response = stub.SayHello(request_pb2.HelloRequest(name='4'))
    print("Greeter client received: " + response.message)

print("hello world!")

if __name__ == '__main__':
    run()