# TCP

A simple HTTP/1.1 server and client written from scratch in C, built as a practice project to explore low-level networking on Linux.

---

## Features

- HTTP/1.1 request/response handling
- Routing — `GET`, `POST`, and other HTTP methods
- Static file serving with MIME type detection
- Thread pool for concurrent connections
- Non-blocking sockets with `epoll`
- Request queue
- Graceful shutdown on `SIGINT` / `SIGTERM`

---

## Performance

Roughly **2× faster** than an equivalent Node.js HTTP server in both throughput (req/s) and latency, thanks to zero runtime overhead and direct use of Linux system calls.

---

## Building

```bash
git clone https://github.com/yourname/tcp.git
cd tcp
make
```

### Run the server

```bash
cd build/server
./server
```

### Run the client

```bash
cd build/client
./client
```

---

## Requirements

- Linux
- GCC / Clang
- GNU Make

---

## License

MIT