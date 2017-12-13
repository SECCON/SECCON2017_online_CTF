FROM python
RUN apt-get update
RUN apt-get install -y socat

RUN pip install cryptography

RUN useradd ecdh
USER ecdh
ADD app /usr/src/app/
WORKDIR /usr/src/app/
