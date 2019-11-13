
FROM debian
RUN apt-get update
RUN apt-get upgrade -y
RUN apt install -y gdb
RUN apt install -y git
RUN apt install -y python
RUN apt install -y curl
#RUN apt-get install -y gcc-multilib g++-multilib libtesseract-dev vim man gcc make binutils
RUN apt-get install -y gcc-multilib g++-multilib
RUN apt-get install -y libtesseract-dev
RUN apt-get install -y vim
RUN apt-get install -y man
RUN apt-get install -y gcc
RUN apt-get install -y make
RUN apt-get install -y binutils
RUN git clone https://github.com/longld/peda.git ~/peda
RUN echo "source ~/peda/peda.py" >> ~/.gdbinit
