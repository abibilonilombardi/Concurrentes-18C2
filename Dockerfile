FROM debian:testing

MAINTAINER Xero-Hige <Gaston.martinez.90@gmail.com>
WORKDIR /

RUN apt-get update && \
    apt-get install  -y --allow-unauthenticated --no-install-recommends build-essential && \
    apt-get install -y --allow-unauthenticated \
	gcc \
        g++ \
	make \
	cmake && \
    rm -rf /var/lib/apt/lists/* && \
    apt-get clean && \
    mkdir /workspace

WORKDIR /workspace
