# Module 26: Network & Socket Programming

## How the Internet Works

When you type `https://example.com` into a browser and press Enter, an astonishing
chain of events unfolds in milliseconds. This module breaks down every layer so you
understand exactly what happens — from the electrical signals on the wire to the
HTML that paints your screen.

---

## The Network Layer Model

Networks are organized in **layers**. Each layer has one job and talks only to the
layers directly above and below it. Think of it like mailing a letter:

```
┌─────────────────────────────────────────────────────────┐
│                   APPLICATION LAYER                     │
│  "Dear Alice, happy birthday!"  (HTTP, DNS, FTP, SSH)  │
├─────────────────────────────────────────────────────────┤
│                   TRANSPORT LAYER                       │
│  Envelope with tracking number   (TCP, UDP)             │
├─────────────────────────────────────────────────────────┤
│                   NETWORK LAYER                         │
│  Address on envelope             (IP)                   │
├─────────────────────────────────────────────────────────┤
│                   LINK LAYER                            │
│  The mail truck / postal system  (Ethernet, WiFi)       │
└─────────────────────────────────────────────────────────┘
```

**Why layers?** Each layer can change independently. WiFi replaced Ethernet cables
at the link layer, but TCP/IP above it didn't need to change at all. Your HTTP
request works the same over WiFi, fiber, or a phone's cellular connection.

### What each layer does

| Layer       | Job                                       | Key Protocols    |
|-------------|-------------------------------------------|------------------|
| Application | What you want to say (web page, email)    | HTTP, DNS, SSH   |
| Transport   | Reliable (or fast) delivery of messages   | TCP, UDP         |
| Network     | Routing packets across networks           | IP (IPv4, IPv6)  |
| Link        | Moving bits between directly-connected devices | Ethernet, WiFi |

---

## IP Addresses

Every device on the internet has an **IP address** — a unique number, like a
postal address for computers.

### IPv4

Four numbers (0–255), separated by dots:

```
192.168.1.42
 │    │  │ │
 │    │  │ └── Host number (your specific machine)
 │    │  └──── Subnet
 │    └─────── Private network range
 └──────────── First octet
```

Special addresses:
- `127.0.0.1` — **localhost** (your own machine, always)
- `0.0.0.0` — "any address" (used when a server listens on all interfaces)
- `192.168.x.x`, `10.x.x.x` — private addresses (your home network)

### IPv6

IPv4 has only ~4 billion addresses (not enough for every phone, laptop, fridge).
IPv6 uses 128 bits: `2001:0db8:85a3:0000:0000:8a2e:0370:7334`

---

## Ports

An IP address gets you to a **machine**. A **port** gets you to a specific
**program** on that machine. Think: IP = street address, port = apartment number.

```
    ┌──────────────────────────────────────────┐
    │          Machine: 93.184.216.34          │
    │                                          │
    │  Port 80  ──► Web server (HTTP)          │
    │  Port 443 ──► Web server (HTTPS)         │
    │  Port 22  ──► SSH server                 │
    │  Port 3306──► MySQL database             │
    │                                          │
    └──────────────────────────────────────────┘
```

- Ports 0–1023 are **well-known** (reserved for standard services)
- Ports 1024–49151 are **registered** (applications can claim them)
- Ports 49152–65535 are **ephemeral** (your OS picks these for outgoing connections)

When your browser connects to a web server, it picks a random high port (say 52431)
on your machine and connects to port 80 on the server.

---

## TCP vs UDP

Both are **transport layer** protocols. They solve different problems.

### TCP (Transmission Control Protocol)

**Reliable, ordered delivery.** Like a phone call — you establish a connection,
talk back and forth, then hang up.

- Guarantees every byte arrives, in order
- Retransmits lost packets
- Flow control (slows down if receiver is overwhelmed)
- Used for: HTTP, SSH, email, file transfer — anything where correctness matters

### UDP (User Datagram Protocol)

**Fast, no guarantees.** Like mailing postcards — you just send them. Some might
arrive late, out of order, or not at all.

- No connection setup (just fire and forget)
- No retransmission
- Very low overhead
- Used for: video streaming, online games, DNS lookups — where speed matters
  more than perfection

| Feature           | TCP            | UDP            |
|-------------------|----------------|----------------|
| Connection        | Yes (3-way handshake) | No       |
| Reliability       | Guaranteed     | Best effort    |
| Ordering          | Preserved      | Not guaranteed |
| Speed             | Slower         | Faster         |
| Overhead          | Higher         | Lower          |
| Use cases         | Web, SSH, email| Games, video, DNS |

---

## The TCP Three-Way Handshake

Before any data flows over TCP, client and server perform a "handshake" to
establish the connection. It takes exactly three messages:

```
     Client                                  Server
       │                                       │
       │  ──── SYN (seq=100) ──────────────►   │   1. "I want to connect"
       │                                       │      (client picks initial
       │                                       │       sequence number)
       │                                       │
       │  ◄── SYN-ACK (seq=300, ack=101) ───   │   2. "OK, I acknowledge your
       │                                       │      SYN and here's mine"
       │                                       │      (server picks its own
       │                                       │       sequence number)
       │                                       │
       │  ──── ACK (ack=301) ──────────────►   │   3. "Got it. Connection
       │                                       │      established!"
       │                                       │
       │  ═══════ DATA FLOWS BOTH WAYS ══════  │
       │                                       │
       │  ──── FIN ────────────────────────►   │   Closing: similar exchange
       │  ◄── FIN-ACK ─────────────────────    │   with FIN/FIN-ACK
       │  ──── ACK ────────────────────────►   │
       │                                       │
```

**Why three steps?** Both sides need to agree on initial sequence numbers.
The SYN says "I'll start counting bytes from X." The SYN-ACK says "I
acknowledge your X, and I'll start from Y." The final ACK says "I got your Y."
Now both sides are synchronized.

---

## The Sockets API

A **socket** is a file descriptor that represents one end of a network connection.
In Unix, everything is a file — sockets are no exception. You read and write
to them just like files.

### The Key Functions

```
socket()   ──► Create a socket (like opening a file)
bind()     ──► Assign an address and port to it
listen()   ──► Mark it as a server socket (ready to accept connections)
accept()   ──► Wait for and accept an incoming connection
connect()  ──► Connect to a remote server (client side)
send()     ──► Send data through the connection
recv()     ──► Receive data from the connection
close()    ──► Close the socket (like closing a file)
```

### Client-Server Socket Flow

This is the complete flow showing which calls happen on each side:

```
         SERVER                              CLIENT
    ┌──────────────┐                   ┌──────────────┐
    │  socket()    │                   │  socket()    │
    │  (create)    │                   │  (create)    │
    └──────┬───────┘                   └──────┬───────┘
           │                                  │
    ┌──────▼───────┐                          │
    │  bind()      │                          │
    │  (assign     │                          │
    │   address)   │                          │
    └──────┬───────┘                          │
           │                                  │
    ┌──────▼───────┐                          │
    │  listen()    │                          │
    │  (wait for   │                          │
    │   clients)   │                          │
    └──────┬───────┘                          │
           │                                  │
    ┌──────▼───────┐                   ┌──────▼───────┐
    │  accept()    │◄══ 3-way ════════►│  connect()   │
    │  (blocks     │   handshake       │  (initiate)  │
    │   until      │                   │              │
    │   client)    │                   │              │
    └──────┬───────┘                   └──────┬───────┘
           │                                  │
    ┌──────▼───────┐                   ┌──────▼───────┐
    │  recv()      │◄─── data ────────│  send()      │
    │  (read       │                   │  (write      │
    │   request)   │                   │   request)   │
    └──────┬───────┘                   └──────┬───────┘
           │                                  │
    ┌──────▼───────┐                   ┌──────▼───────┐
    │  send()      │──── data ────────►│  recv()      │
    │  (write      │                   │  (read       │
    │   response)  │                   │   response)  │
    └──────┬───────┘                   └──────┬───────┘
           │                                  │
    ┌──────▼───────┐                   ┌──────▼───────┐
    │  close()     │                   │  close()     │
    └──────────────┘                   └──────────────┘
```

### Socket Code Skeleton (Server)

You won't run this code in the exercises (we're doing string parsing instead),
but here's what a real TCP server looks like in C so you can see how the API
fits together:

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void)
{
    /* 1. Create a socket */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. Bind to an address and port */
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,  /* listen on all interfaces */
        .sin_port = htons(8080)          /* port 8080, network byte order */
    };
    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    /* 3. Listen — allow up to 5 queued connections */
    listen(server_fd, 5);

    /* 4. Accept a connection (blocks until a client connects) */
    int client_fd = accept(server_fd, NULL, NULL);

    /* 5. Read the client's request */
    char buf[4096];
    int n = recv(client_fd, buf, sizeof(buf) - 1, 0);
    buf[n] = '\0';

    /* 6. Send a response */
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nHello";
    send(client_fd, response, strlen(response), 0);

    /* 7. Close both sockets */
    close(client_fd);
    close(server_fd);

    return 0;
}
```

**Key insight:** `accept()` returns a *new* socket for the specific client
connection. The original `server_fd` keeps listening for more clients.

---

## HTTP Protocol

HTTP (Hypertext Transfer Protocol) is how web browsers and servers communicate.
It's a **text-based protocol** — you could literally type it by hand.

### HTTP Request Format

```
┌─────────────────────────────────────────────────────────────┐
│  GET /index.html HTTP/1.1\r\n              ← Request line   │
│  Host: www.example.com\r\n                 ← Header         │
│  User-Agent: Mozilla/5.0\r\n               ← Header         │
│  Accept: text/html\r\n                     ← Header         │
│  \r\n                                      ← Empty line     │
│  (optional body for POST requests)         ← Body           │
└─────────────────────────────────────────────────────────────┘

Structure:
  METHOD SP PATH SP VERSION CRLF
  Header-Name: Header-Value CRLF
  Header-Name: Header-Value CRLF
  ...
  CRLF                          ← blank line = end of headers
  [body]

Where:
  SP   = space character
  CRLF = \r\n (carriage return + line feed)
```

### Common HTTP Methods

| Method | Purpose                    | Has Body? |
|--------|----------------------------|-----------|
| GET    | Retrieve a resource        | No        |
| POST   | Submit data to server      | Yes       |
| PUT    | Replace a resource         | Yes       |
| DELETE | Remove a resource          | No        |
| HEAD   | Like GET but headers only  | No        |

### HTTP Response Format

```
┌─────────────────────────────────────────────────────────────┐
│  HTTP/1.1 200 OK\r\n                      ← Status line     │
│  Content-Type: text/html\r\n              ← Header          │
│  Content-Length: 45\r\n                   ← Header          │
│  \r\n                                     ← Empty line      │
│  <html><body>Hello, World!</body></html>  ← Body            │
└─────────────────────────────────────────────────────────────┘

Structure:
  VERSION SP STATUS-CODE SP REASON-PHRASE CRLF
  Header-Name: Header-Value CRLF
  ...
  CRLF
  [body]
```

### Common Status Codes

| Code | Meaning            | When you see it                  |
|------|--------------------|----------------------------------|
| 200  | OK                 | Everything worked                |
| 301  | Moved Permanently  | URL changed, follow redirect     |
| 400  | Bad Request        | Malformed request                |
| 403  | Forbidden          | You don't have permission        |
| 404  | Not Found          | Resource doesn't exist           |
| 500  | Internal Server Error | Server crashed                |

---

## DNS (Domain Name System)

Humans remember `example.com`. Computers need `93.184.216.34`. DNS translates
between the two.

```
   You type: www.example.com
              │
              ▼
   ┌─────────────────┐
   │  DNS Resolver    │  (usually your ISP's server)
   │  "Do you know    │
   │   this name?"    │
   └────────┬────────┘
            │ if not cached...
            ▼
   ┌─────────────────┐     ┌─────────────────┐
   │  Root Server    │────►│  .com Server    │
   │  "Ask .com"     │     │  "Ask example's │
   │                 │     │   name server"  │
   └─────────────────┘     └────────┬────────┘
                                    │
                                    ▼
                           ┌─────────────────┐
                           │ example.com NS  │
                           │ "It's at        │
                           │  93.184.216.34" │
                           └─────────────────┘
```

This result gets cached at every level so the next lookup is instant.

---

## Client-Server Model

The **client-server model** is the foundation of networked applications:

- **Server**: runs continuously, waits for connections, serves many clients
- **Client**: initiates the connection, makes a request, gets a response

```
  ┌─────────┐         ┌─────────┐         ┌─────────┐
  │ Client  │────────►│         │◄────────│ Client  │
  │ (phone) │         │ Server  │         │ (laptop)│
  └─────────┘         │         │         └─────────┘
                      │         │
  ┌─────────┐         │         │
  │ Client  │────────►│         │
  │ (tablet)│         └─────────┘
  └─────────┘
```

The server must handle **multiple clients** — often simultaneously. Common
approaches:
1. **Fork a process** per client (simple, heavy)
2. **Thread per client** (lighter, shared memory risks)
3. **Event loop / non-blocking I/O** (efficient, complex code)

---

## Blocking vs Non-Blocking I/O

### Blocking (default)

When you call `recv()`, your program **stops and waits** until data arrives.
Like standing at the mailbox waiting for the letter carrier.

```
recv(client_fd, buf, size, 0);   /* program freezes here until data arrives */
printf("Got data!\n");            /* only runs after data received */
```

Problem: if you have 100 clients and one is slow, everyone waits.

### Non-Blocking

You tell the OS: "don't block — if there's no data, just tell me immediately."
Like checking the mailbox and walking away if it's empty.

```
/* Set socket to non-blocking */
fcntl(client_fd, F_SETFL, O_NONBLOCK);

int n = recv(client_fd, buf, size, 0);
if (n == -1 && errno == EAGAIN) {
    /* No data right now, do something else */
}
```

Real servers use **`select()`**, **`poll()`**, or **`epoll()`** to efficiently
monitor many sockets at once: "wake me up when ANY of these 1000 sockets has
data ready."

---

## What You'll Build in the Exercises

Since actual networking requires OS support and running servers, our exercises
focus on the **parsing side** — which is actually the hardest part of network
programming. Real web servers spend most of their code parsing HTTP, not on
socket calls.

1. **HTTP Parser** (`http_parser.c`) — Parse HTTP requests and responses, extract
   headers, build response strings. This is exactly what nginx/Apache do internally.

2. **Protocol Parser** (`protocol.c`) — Design and parse a simple text protocol.
   Every network application needs a protocol — a shared language that both sides
   understand.

These exercises build the skills you need for a real web server. The socket
calls (`socket`, `bind`, `listen`, `accept`) are just plumbing — parsing the
protocol is where the logic lives.

---

## Exercises

| File              | Functions                                                    | Tests |
|-------------------|--------------------------------------------------------------|-------|
| `http_parser.c`   | `http_parse_request`, `http_parse_header`, `http_build_response` | 14    |
| `protocol.c`      | `proto_parse`, `proto_build`, `proto_validate`               | 8     |
| **Total**         |                                                              | **22**|
