FROM --platform=amd64 z88dk/z88dk  AS z88dk-builder
WORKDIR /build
COPY . .

RUN /build/build-docker.sh

FROM --platform=amd64 debian:bookworm-slim AS local-builder

WORKDIR /utils

RUN apt-get update


# now taptools, cpmtools, libdsk
RUN apt install  -y git autoconf automake libtool libdsk4-dev cpmtools make build-essential

RUN git clone https://github.com/jorgegv/taptools.git

WORKDIR /utils/taptools
RUN autoreconf -fi && ./configure --disable-dependency-tracking
RUN make

WORKDIR /utils

RUN apt-get clean

WORKDIR /src/
COPY --from=z88dk-builder /build /src
#RUN bas2tap -sdisk -a10 loader.bas loader.tap
RUN apt install libdsk-utils
RUN gcc /src/utils/bas2tap.c -o /utils/bas2tap -lm 
COPY run-docker.sh /src
RUN chmod +x /src/run-docker.sh

CMD ["./run-docker.sh"]
