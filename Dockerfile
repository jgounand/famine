FROM debian
RUN apt-get update
RUN apt-get upgrade -y
RUN apt install -y gdb
RUN apt-get install -y vim
RUN apt-get install -y gcc
RUN apt-get install -y make
