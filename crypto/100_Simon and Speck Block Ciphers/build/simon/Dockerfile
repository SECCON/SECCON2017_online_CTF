FROM python

RUN useradd simon
ADD src /usr/src/app/
RUN chown simon:simon -R /usr/src/app/
USER simon
WORKDIR /usr/src/app/

RUN git clone https://github.com/inmcm/Simon_Speck_Ciphers.git
RUN mv ./Simon_Speck_Ciphers/Python/simon.py .

