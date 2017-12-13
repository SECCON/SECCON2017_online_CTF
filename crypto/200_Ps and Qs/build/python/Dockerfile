FROM python:2
RUN pip install pycrypto


RUN useradd rsa
USER rsa
ADD app /usr/src/app
WORKDIR /usr/src/app

