FROM ubuntu:14.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        valgrind && \
    rm -rf /var/lib/apt/lists/* && \
    apt-get clean

# Set the working directory to /root
WORKDIR /root

# Copy the current directory contents into the container at /root
COPY . /root


ENTRYPOINT ["valgrind"]
