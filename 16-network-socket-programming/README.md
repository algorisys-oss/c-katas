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

## UDP Communication in Practice

TCP gets most of the attention, but UDP is just as important. It's **connectionless**
— there's no handshake, no connection state, no SYN/ACK dance. You just send packets
(called **datagrams**) and hope they arrive.

### UDP Workflow

```
  UDP Server:                        UDP Client:
  socket(AF_INET, SOCK_DGRAM)       socket(AF_INET, SOCK_DGRAM)
  bind(port)                         (no bind needed for client)
  recvfrom() ← waits                sendto(server_addr) →
  sendto(client_addr) →              recvfrom() ← gets reply
```

Notice the differences from TCP:
- **No `listen()` or `accept()`** — there's no connection to accept
- **`SOCK_DGRAM`** instead of `SOCK_STREAM`
- **`sendto()` / `recvfrom()`** instead of `send()` / `recv()` — because
  there's no connection, you must specify the destination address every time

### sendto() and recvfrom()

Unlike TCP's stream-based `send()` and `recv()`, UDP's calls work with
**complete messages**. Each `sendto()` sends one datagram. Each `recvfrom()`
receives one complete datagram. There's no partial read — you either get the
whole message or nothing.

```c
/* Server: receive a datagram and echo it back */
struct sockaddr_in client_addr;
socklen_t addr_len = sizeof(client_addr);

int n = recvfrom(sock, buf, sizeof(buf), 0,
                 (struct sockaddr *)&client_addr, &addr_len);

/* client_addr now contains the sender's IP and port */
sendto(sock, buf, n, 0,
       (struct sockaddr *)&client_addr, addr_len);
```

### Ephemeral Ports

When a UDP client doesn't call `bind()` to pick a specific port, the OS assigns
a random high port (called an **ephemeral port**, typically in the range
49152–65535). The server learns the client's address and ephemeral port from
`recvfrom()` — that's how it knows where to send the reply.

### Key Differences from TCP

| Aspect              | TCP                        | UDP                         |
|---------------------|----------------------------|-----------------------------|
| Delivery guarantee  | Every byte arrives         | Packets may be lost         |
| Ordering            | Preserved                  | Packets may arrive reordered|
| Flow control        | Yes (backpressure)         | None — sender can overwhelm|
| Message boundaries  | Stream (no boundaries)     | Each datagram is separate   |
| Connection overhead | 3-way handshake + teardown | Zero                        |

**When to use UDP:** DNS lookups (one question, one answer), online games
(latest position matters, old ones don't), video calls (a dropped frame is
better than a delayed one), and any situation where speed beats reliability.

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

**Why do we need sequence numbers at all?** TCP promises to deliver data
reliably and in order, but the network below it makes no such promises —
packets can arrive out of order, get duplicated, or get lost entirely.
Sequence numbers let the receiver:

1. **Put packets in the right order** — even if packet 3 arrives before packet 2,
   the receiver can sort them by sequence number
2. **Detect missing packets** — if you get sequence 100 and 300 but not 200,
   you know to request retransmission of the gap
3. **Detect and discard duplicates** — if the same sequence number arrives twice,
   the second copy is dropped

Each side picks a **random** starting sequence number (not 0 or 1) to prevent
confusion with packets from old, already-closed connections that might still
be floating around the network.

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
```

> **Tip: SO_REUSEADDR — Fixing "Address already in use"**
>
> When you restart a server, `bind()` may fail with "Address already in use"
> because the OS holds the port for ~60 seconds after the previous server
> exits (this is called the TIME_WAIT state). Fix this by setting the
> `SO_REUSEADDR` socket option before `bind()`:
>
> ```c
> int opt = 1;
> setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
> ```
>
> Put this between `socket()` and `bind()`. Almost every server does this
> — without it, you'll be waiting a minute every time you restart during
> development.

> **What is `htons()` and why do we need it?**
>
> Computers store multi-byte numbers in memory in different orders. Your
> Intel/AMD CPU uses **little-endian** (least significant byte first), but
> network protocols use **big-endian** (most significant byte first, also
> called "network byte order"). If both sides don't agree on the order,
> the number gets misread.
>
> `htons()` = **"host to network short"** — it converts a 16-bit number
> from your CPU's byte order to network byte order.
>
> ```
>     Port 8080 = 0x1F90
>
>     Little-endian (your CPU): 90 1F   (least significant byte first)
>     Big-endian (network):     1F 90   (most significant byte first)
>
>     htons(8080) does this swap for you.
> ```
>
> There's a family of these functions:
> - `htons()` / `ntohs()` — 16-bit (ports)
> - `htonl()` / `ntohl()` — 32-bit (IP addresses)
>
> The "h" = host, "n" = network, "s" = short (16-bit), "l" = long (32-bit).

### Binary Serialization: Length-Prefixed Messages

Most binary network protocols use a common pattern for framing messages:
**length-prefixed encoding**. The sender writes the message length first,
then the message itself. The receiver reads the length, then knows exactly
how many bytes to read next.

```
  ┌──────────┬───────────────────┐
  │ Length   │ Payload            │
  │ (4 bytes)│ (Length bytes)     │
  │ big-end  │                    │
  └──────────┴───────────────────┘
```

Here's how to write and read length-prefixed messages in C:

```c
/* Writing a length-prefixed message */
uint32_t len = htonl(payload_len);
write(fd, &len, 4);
write(fd, payload, payload_len);

/* Reading a length-prefixed message */
uint32_t net_len;
read(fd, &net_len, 4);
uint32_t len = ntohl(net_len);
char *buf = malloc(len);
read(fd, buf, len);
```

> **Warning: partial reads!**
>
> TCP is a **stream** protocol — a single `read()` might return fewer bytes
> than you asked for. If you call `read(fd, buf, 1000)`, you might only get
> 200 bytes back. The rest isn't lost — it just hasn't arrived yet. You must
> **always loop** until you've read the full message:
>
> ```c
> /* Read exactly n bytes — keep reading until we have them all */
> int read_exact(int fd, void *buf, size_t n) {
>     size_t total = 0;
>     while (total < n) {
>         ssize_t r = read(fd, (char *)buf + total, n - total);
>         if (r <= 0) return -1;  /* error or connection closed */
>         total += r;
>     }
>     return 0;
> }
> ```
>
> This is one of the most common bugs in network code. Always use a
> `read_exact`-style helper when reading binary protocol messages.

```c

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

## Real-World Protocol: Redis RESP

HTTP isn't the only text-based protocol. Let's look at how **Redis** — one of
the most popular in-memory databases — communicates over TCP. Understanding
real protocols shows you that everything on the network is just bytes following
agreed-upon rules.

Redis uses a protocol called **RESP** (Redis Serialization Protocol). It's
text-based, human-readable, and surprisingly simple.

### How Commands Are Encoded

Commands are sent as arrays of bulk strings:

```
*<number of arguments>\r\n
$<length of argument 1>\r\n
<argument 1>\r\n
$<length of argument 2>\r\n
<argument 2>\r\n
...
```

For example, the command `SET mykey hello` has 3 arguments (SET, mykey, hello):

```
*3\r\n
$3\r\n
SET\r\n
$5\r\n
mykey\r\n
$5\r\n
hello\r\n
```

### Response Types

Responses use a single-character prefix to indicate the type:

| Prefix | Type          | Example                  |
|--------|---------------|--------------------------|
| `+`    | Simple string | `+OK\r\n`                |
| `-`    | Error         | `-ERR unknown command\r\n` |
| `:`    | Integer       | `:42\r\n`                |
| `$`    | Bulk string   | `$5\r\nhello\r\n`        |
| `*`    | Array         | `*2\r\n:1\r\n:2\r\n`     |

So when you run `SET mykey hello`, the server responds `+OK\r\n`. When you
run `GET mykey`, the server responds `$5\r\nhello\r\n` (a bulk string with
5 bytes of data).

This is a great example of a text-based protocol — human-readable, easy to
debug with `netcat`, but still structured enough for machines to parse
efficiently. You could test a Redis server by hand:

```
$ echo -e "*3\r\n\$3\r\nSET\r\n\$5\r\nmykey\r\n\$5\r\nhello\r\n" | nc localhost 6379
+OK
```

---

## Real-World Protocol: PostgreSQL Wire Protocol (Overview)

While Redis uses a text-based protocol, **PostgreSQL** uses a **binary protocol**
over TCP. This is more complex but shows you how serious database systems
communicate at the byte level.

### Connection Startup

The client sends a **Startup Message**:

```
  ┌──────────────┬──────────────┬────────────────────────────────────┐
  │ Length       │ Protocol     │ Parameters (null-terminated        │
  │ (4 bytes,   │ Version      │ key-value pairs)                   │
  │ big-endian) │ (4 bytes:    │ "user\0postgres\0database\0mydb\0" │
  │             │ 0x00030000   │                                    │
  │             │ = v3.0)      │                                    │
  └──────────────┴──────────────┴────────────────────────────────────┘
```

After startup, all messages follow the pattern: **1 byte message type + 4 bytes
length (big-endian) + payload**.

### Common Message Types

| Byte | Name             | Direction      | Meaning                     |
|------|------------------|----------------|-----------------------------|
| `R`  | Authentication   | Server → Client| Auth challenge/OK           |
| `Z`  | ReadyForQuery    | Server → Client| Server is ready for a query |
| `Q`  | Query            | Client → Server| SQL query string            |
| `T`  | RowDescription   | Server → Client| Column names and types      |
| `D`  | DataRow          | Server → Client| One row of results          |
| `C`  | CommandComplete  | Server → Client| Query finished              |

### Authentication Flow

```
  Client                          Server
    │                                │
    │── Startup Message ───────────→│
    │   (protocol v3, user, db)     │
    │                                │
    │←── AuthenticationOk ('R') ────│
    │                                │
    │←── ReadyForQuery ('Z') ──────│
    │                                │
    │── Query ("SELECT 1") ────────→│
    │                                │
    │←── RowDescription ('T') ─────│
    │←── DataRow ('D') ────────────│
    │←── CommandComplete ('C') ────│
    │←── ReadyForQuery ('Z') ──────│
```

This shows why understanding byte order (the `htons()`/`htonl()` functions
from earlier) matters in practice — real protocols use big-endian framing
everywhere. The 4-byte length field in every PostgreSQL message is sent in
network byte order (big-endian), and you need `ntohl()` to read it correctly
on a little-endian machine.

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

> **What are `errno` and `EAGAIN`?**
>
> `errno` is a global variable (from `#include <errno.h>`) that system calls
> set when they fail. It tells you **why** the call failed — not just that it
> failed, but the specific reason.
>
> `EAGAIN` (also called `EWOULDBLOCK`) means "there's nothing to read RIGHT
> NOW, but try again later." This is **not an error** — it's expected behavior
> with non-blocking sockets. It's the OS saying "the mailbox is empty, come
> back later."
>
> Without this check, you'd have no way to tell the difference between "no
> data yet" and "the connection broke."

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

---

## Debug Challenge

| File | Description | Bugs |
|------|-------------|------|
| `debug_network.c` | Find and fix 4 network programming bugs (byte order, parsing) | 4 |

These exercises contain **intentionally broken code**. Your job is to find and
fix each bug. Each function has a comment explaining what it SHOULD do and a
HINT about the bug class. Run the program — failing tests tell you which
functions are still broken.

```bash
make debug    # compile the buggy version
./exercises/debug_network   # see which tests fail
# ... fix bugs ...
# recompile and rerun until all tests pass
```

---

[← Previous: Module 25: Parsing & the SQL Engine](../25-build-parsing-sql-engine/README.md) | [Next: Module 27: Encryption & Security →](../32-encryption-security/README.md)
