FROM debian:buster
RUN apt-get update
RUN apt-get upgrade -y
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get install -y g++ cmake vim-tiny
RUN apt-get install -y libmsgpack-dev
RUN apt-get install -y libwebsocketpp-dev
RUN apt-get install -y libasio-dev
RUN apt-get install -y libgtest-dev
RUN apt-get install -y libboost-all-dev
RUN mkdir -p /opt/x5e/ink
WORKDIR /opt/x5e/ink
COPY . .
RUN cmake . && make -j 4 && ./ink.out '--gtest_filter=*'

