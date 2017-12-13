FROM python
RUN pip install flask gunicorn

RUN useradd webapp
USER webapp
ADD ./src/ /usr/src/webapp
ENV HOME /usr/src/webapp
WORKDIR /usr/src/webapp


