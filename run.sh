#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Error: Missing arguments."
  echo "Usage: $0 <cppbuild>"
  exit 1
fi

echo "starting optimizer building script"
echo "cpp build folder : $1"

protoc --proto_path=messages/ --cpp_out=backend/generated --grpc_out=backend/generated/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` messages/request.proto &&
python3 -m grpc_tools.protoc --proto_path=messages/ --python_out=frontend/generated/ --pyi_out=frontend/generated/ --grpc_python_out=frontend/generated/ messages/request.proto &&
cd $1 &&
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../TravellingSalesmanProblemOptimiser/backend/ &&
make -j 4 && 
echo "build is successfull, starting the program" &&
cd -
cd frontend/
python3 tsp_optimizer.py &
cd -
cd $1
./TravellingSalesmanProblemOptimiser
