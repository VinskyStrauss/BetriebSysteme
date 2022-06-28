FROM debian:latest

RUN apt-get -y update && apt-get -y install

RUN cgroup_enable=memory

RUN apt-get -y install g++

COPY . /usr/src/P5

WORKDIR /usr/src/P5

RUN g++ main.cpp -pthread -o main

CMD ./main 
