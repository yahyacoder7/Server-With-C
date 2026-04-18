# 🖥️ Custom TCP Server in C — From Scratch

> A lightweight, educational TCP server written in pure C that demonstrates low-level socket programming, HTTP protocol handling, and a custom protocol built on top of raw TCP.

Built as a learning project to deeply understand the **TCP/IP stack**, the **Socket API**, and how real servers work — before jumping into high-level frameworks.

---

## 🎯 Project Goals

- Understand the full TCP connection lifecycle: `socket()` → `bind()` → `listen()` → `accept()`
- Serve static HTML pages over HTTP (Application Layer)
- Implement a **custom protocol** on top of raw TCP
- Learn protocol detection: distinguish between HTTP and custom messages on the **same port**
- Understand the difference between **Raw TCP** and **HTTP**

---

## ✨ Features

| Feature | Details |
|---|---|
| TCP Socket Server | Listening on port `2222` |
| HTTP/1.1 Support | `GET /` serves a welcome HTML page |
| Custom Protocol | Send `Y:w` → server replies with a welcome message |
| SO_REUSEADDR | Enabled for quick restarts during development |
| Protocol Detection | Distinguishes HTTP vs custom protocol on the same port |
| Iterative Handling | Handles one client at a time (no threading yet) |

---

## 📁 Project Structure

```
tcp-custom-server/
├── server.c        # Main server implementation
├── welcome.html    # Static HTML page served via HTTP
├── server          # Compiled binary (after build)
└── README.md       # This file
```

---

## ⚙️ How to Build and Run

### Prerequisites

- Linux / macOS (or WSL on Windows)
- GCC compiler

### Compilation

```bash
gcc server.c -o server
```

### Running the Server

```bash
./server
```

The server will listen on `192.168.198.149:2222`.  
> **Note:** To listen on all interfaces, change `inet_addr("192.168.198.149")` to `INADDR_ANY` in the code.

---

## 🧪 Testing the Server

### 1. Test HTTP (Browser or curl)

```bash
curl http://192.168.198.149:2222/
```
Or open directly in your browser:  
```
http://192.168.198.149:2222/
```
You should see a styled welcome HTML page.

### 2. Test Custom Protocol (using netcat)

```bash
nc 192.168.198.149 2222
```

Then type:

```
Y:w
```

Server replies:

```
Welcome user to my own server in c language from scratch! 👋
```

### 3. Unknown Requests

Any request that doesn't match `GET /` or `Y:w` returns:

```
HTTP/1.1 404 Not Found
```

---

## 🧠 Code Overview (Learning Path)

1. **Socket Creation** → `socket(AF_INET, SOCK_STREAM, 0)`
2. **Binding & Listening** → Preparing the passive socket on a specific IP/Port
3. **Accepting Connections** → `accept()` creates a new connected socket per client
4. **Protocol Detection**:
   - Request starts with `GET /` → HTTP handler
   - Request contains `Y:w` → Custom protocol handler
   - Else → `404 Not Found`
5. **Serving HTML** → Reading file + building proper HTTP headers (`Content-Length`, `Content-Type`)
6. **Custom Protocol** → Simple text-based command (easily extendable)

---

## 🔮 Future Improvements (Roadmap)

- [ ] Concurrent client handling using `fork()`, threads, or `epoll()`
- [ ] Full custom protocol with message length prefix (to avoid partial `read()` issues)
- [ ] Better HTTP parser (support `POST`, headers, query params, routes)
- [ ] Client application in C for the custom protocol
- [ ] Commands: `TIME`, `ECHO`, `LOGIN` with 4-byte header format
- [ ] Logging and structured error handling
- [ ] Configuration file for port and IP

---

## 📚 Learning Resources Used

- `man` pages: `man socket`, `man bind`, `man accept`, `man read`
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) *(highly recommended)*
- Understanding TCP Three-Way Handshake and socket queues

---

## 👤 Author

**Yahya**  
Simple educational project built from scratch to master low-level network programming in C.

📸 Instagram: [@onl.yahya](https://instagram.com/onl.yahya)

---

*Made with ❤️ for learning low-level networking*
