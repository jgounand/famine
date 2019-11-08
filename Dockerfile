FROM i386/debian
RUN apt-get update
RUN apt-get upgrade -y
RUN apt install -y gdb
RUN apt install -y git
RUN apt install -y python
RUN apt install -y curl
RUN git clone https://github.com/longld/peda.git ~/peda
RUN echo "source ~/peda/peda.py" >> ~/.gdbinit
RUN apt install -y binutils
RUN apt install -y make
RUN apt install -y gcc
RUN apt install -y man
