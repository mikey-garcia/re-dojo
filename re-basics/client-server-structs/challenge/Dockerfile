FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y libc6-i386 && \
    rm -rf /var/lib/apt/lists/*

RUN mkdir -p /challenge

COPY client /challenge/client
COPY server /challenge/server
COPY flag /flag

RUN chmod 755 \
    /challenge/client \
    /challenge/server

CMD ["/challenge/server"]