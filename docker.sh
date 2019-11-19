mkdir /goinfre/${USER}/ 2> /dev/null
mkdir /goinfre/${USER}/docker 2> /dev/null
ln -s /goinfre/${USER}/docker ~/Library/Containers/com.docker.docker
chmod /goinfre/${USER} 0500
docker build -t debian .
docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -ti -v ~/:/root debian
