FROM ubuntu:16.04
MAINTAINER @shiracamus
RUN apt update && apt install -y \
    python-pil \
    python-pip \
    libev-dev \
    python-zbar \
&& apt clean \
&& rm -rf /var/lib/apt/lists/*

RUN pip install --upgrade pip && pip install \
    bottle \
    bjoern \
    pyzbar

EXPOSE 33654

RUN mkdir /qubicrube
ADD QubicRube_server_SECCON2017OnlineCTF.tgz /qubicrube

WORKDIR /qubicrube/server
ENTRYPOINT /usr/bin/python /qubicrube/server/QubicRube.py

