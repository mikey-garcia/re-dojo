FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
        gcc \
        gcc-multilib \
        make \
        libc6-i386 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY . .

RUN make

RUN mkdir -p /challenge

RUN cp server /challenge/server
RUN cp client /challenge/client

RUN chmod 755 \
    /challenge/server \
    /challenge/client
COPY flag /flag

CMD ["/challenge/server"]