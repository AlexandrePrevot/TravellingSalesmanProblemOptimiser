
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

EXPOSE 50052

COPY frontend /app/frontend
WORKDIR /app/frontend

CMD ["python3", "tsp_optimizer.py"]