FROM ubuntu:latest

VOLUME /tmp
ADD build/Main Main

ENV SERVER_OPTS=""

ENTRYPOINT [ "/bin/sh", "-c", "Main" ]
