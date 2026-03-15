FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y build-essential cmake

WORKDIR /app
COPY . .

# build project
RUN mkdir build
WORKDIR /app/build
RUN cmake ..
RUN make

# move to the directory where the binary + public folder exist
WORKDIR /app/build/server

EXPOSE 8000

CMD ["./server"]
