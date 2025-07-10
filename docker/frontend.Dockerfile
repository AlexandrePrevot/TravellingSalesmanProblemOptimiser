
# it is necessary to run "xhost +local:docker" before building
# this image then run with these options
#
# to display the frontend, add options
# -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix [image id]


# -------- stage 1 : install python deps --------
FROM python:3.12 AS tsp_python_deps_builder

WORKDIR /app

RUN apt-get update && apt-get install -y python3-tk

WORKDIR /app/frontend

COPY frontend/requirements.txt /app/frontend
RUN pip install --no-cache-dir -r requirements.txt

# -------- stage 2 : install source code --------

FROM tsp_python_deps_builder AS tsp_python_builder


COPY frontend /app/frontend

WORKDIR /app

# workaround
# for bigger projects, avoid re-generating proto files
# when modifying python src
# do so by just exporting generated files outside
# the source folder of the code
COPY messages /app/messages
RUN python3 -m grpc_tools.protoc --proto_path=messages/ \
    --python_out=frontend/generated/ --pyi_out=frontend/generated/ \
    --grpc_python_out=frontend/generated/ messages/request.proto

WORKDIR /app/frontend

EXPOSE 50052

CMD ["python3", "tsp_optimizer.py"]