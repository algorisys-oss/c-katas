# Module 17: Build an HTTP Server

## The Big Idea

You already know how to send and receive bytes over TCP sockets (Module 16).
The internet is just **structured text over sockets**. HTTP is the structure.

When you type `http://localhost:8080/index.html` in a browser, here is
*everything* that happens:

1. Your browser opens a TCP connection to `localhost` port `8080`
2. It sends a **text message** (the HTTP request) over that connection
3. Your server reads that text, figures out what file is wanted
4. It sends back a **text message** (the HTTP response) with the file contents
5. The connection closes

That's it. No magic. Just text over a pipe.

---

## HTTP: The Full Flow

```
Client (Browser/curl)                    Server (Your C program)
       |                                        |
       |  ---- TCP 3-way handshake -----------> |   (SYN, SYN-ACK, ACK)
       |                                        |
       |  ---- HTTP Request ------------------> |   "GET /index.html HTTP/1.1\r\n..."
       |                                        |
       |       [ Server reads request,          |
       |         finds the file on disk,        |
       |         builds response text ]         |
       |                                        |
       |  <--- HTTP Response ------------------- |   "HTTP/1.1 200 OK\r\n..."
       |  <--- Headers ------------------------- |   "Content-Type: text/html\r\n..."
       |  <--- Blank line ---------------------- |   "\r\n"
       |  <--- Body (file contents) ------------ |   "<html>...</html>"
       |                                        |
       |  ---- TCP close ---------------------> |   (FIN, ACK)
       |                                        |
```

Every HTTP exchange follows this pattern. Always.

---

## Anatomy of an HTTP Request

When your browser asks for a page, it sends text that looks exactly like this:

```
GET /index.html HTTP/1.1\r\n
Host: localhost:8080\r\n
User-Agent: curl/7.68.0\r\n
Accept: */*\r\n
\r\n
```

Let's break it apart:

### The Request Line (first line)

```
GET /index.html HTTP/1.1\r\n
^^^  ^^^^^^^^^^^  ^^^^^^^^
 |       |           |
 |       |           +-- Protocol version
 |       +-------------- Path (what resource?)
 +---------------------- Method (what action?)
```

**Methods** — what the client wants to do:
| Method | Purpose                  | Has body? |
|--------|--------------------------|-----------|
| GET    | Retrieve a resource      | No        |
| POST   | Send data to server      | Yes       |
| PUT    | Replace a resource       | Yes       |
| DELETE | Remove a resource        | No        |
| HEAD   | GET but headers only     | No        |

For a static file server, you only need GET.

### Headers (lines after the request line)

```
Host: localhost:8080\r\n
^^^^  ^^^^^^^^^^^^^^
 |         |
 key      value
```

- Each header is `Key: Value\r\n`
- Headers give extra info: who's asking, what formats they accept, etc.
- The `Host` header is required in HTTP/1.1

### The Blank Line

```
\r\n
```

This empty line means "end of headers." Everything after it is the body
(for POST/PUT requests). GET requests have no body, so the message ends here.

### Important: \r\n

HTTP uses `\r\n` (carriage return + line feed) as line endings, not just `\n`.
This is called CRLF. Every line in HTTP ends with `\r\n`. The blank line
separating headers from body is also `\r\n`. This matters when you parse!

---

## Anatomy of an HTTP Response

Your server sends back text like this:

```
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
Content-Length: 45\r\n
\r\n
<html><body>Hello, World!</body></html>
```

### The Status Line (first line)

```
HTTP/1.1 200 OK\r\n
^^^^^^^^ ^^^ ^^
   |      |   |
   |      |   +-- Reason phrase (human-readable)
   |      +------ Status code (machine-readable)
   +------------- Protocol version
```

### Common Status Codes

```
+------+---------------------+----------------------------------------+
| Code | Reason              | When to use                            |
+------+---------------------+----------------------------------------+
| 200  | OK                  | Everything worked, here's the content  |
| 400  | Bad Request         | Client sent garbage we can't parse     |
| 404  | Not Found           | Requested file doesn't exist           |
| 403  | Forbidden           | File exists but access denied          |
| 500  | Internal Server Err | Something broke on our side            |
+------+---------------------+----------------------------------------+
```

### Response Headers

```
Content-Type: text/html\r\n      <-- What kind of data is in the body
Content-Length: 45\r\n            <-- How many bytes in the body
Connection: close\r\n             <-- We'll close after this response
```

**Content-Type** tells the browser how to interpret the body:
- `text/html` — render as a web page
- `text/css` — it's a stylesheet
- `image/png` — display as an image
- `application/octet-stream` — raw binary, offer as download

**Content-Length** tells the browser how many bytes to read for the body.
Without it, the browser doesn't know when the body ends.

### The Blank Line + Body

Same as requests: `\r\n` separates headers from body. The body is the actual
file contents (HTML, CSS, image bytes, whatever).

---

## MIME Types

MIME = Multipurpose Internet Mail Extensions. It's how we tell the browser
"this is HTML" vs "this is a PNG image."

Your server needs to look at the file extension and pick the right MIME type:

```
+------------+---------------------------+
| Extension  | MIME Type                 |
+------------+---------------------------+
| .html      | text/html                 |
| .css       | text/css                  |
| .js        | application/javascript    |
| .png       | image/png                 |
| .jpg .jpeg | image/jpeg                |
| .gif       | image/gif                 |
| .txt       | text/plain                |
| .json      | application/json          |
| (other)    | application/octet-stream  |
+------------+---------------------------+
```

Implementation is just a chain of `strcmp` calls on the file extension.

---

## Serving Static Files

Here's the algorithm for serving a file:

```
1. Parse the request  -->  extract the path (e.g., "/style.css")

2. Map URL to filesystem:
       URL path:        /style.css
       www_root:        ./www
       Filesystem path: ./www/style.css

3. Security check:  reject if path contains ".."
                    (prevents reading /etc/passwd via "/../../../etc/passwd")

4. Open the file:   fopen(filesystem_path, "rb")
       If fails  --> send 404 Not Found
       If succeeds --> continue

5. Get file size:   fseek to end, ftell, fseek back

6. Read contents:   fread into buffer

7. Determine MIME type from extension

8. Build response:  status line + headers + blank line + body

9. Send response:   write() to client socket

10. Cleanup:        fclose, free buffer, close socket
```

### The Path Mapping

```
Browser requests:    http://localhost:8080/images/cat.png
                                          ^^^^^^^^^^^^^^^^
                                          URL path: /images/cat.png

Server www_root:     ./www/

Filesystem path:     ./www/images/cat.png
                     ^^^^^^^^^^^^^^^^^^^^^
                     www_root + URL path
```

### Path Traversal Attack (and why we block "..")

```
Malicious request:   GET /../../../etc/passwd HTTP/1.1

Without protection:  ./www/../../../etc/passwd
                     = /etc/passwd              <-- DANGER! Reads system files!

With our check:      if (strstr(path, "..")) --> 403 Forbidden
```

Always sanitize paths. Never trust the client.

---

## Concurrency: Handling Multiple Clients

A basic server handles one client at a time. While it's serving a file to
client A, client B has to wait. For a real server, we need concurrency.

### Strategy 1: Fork Per Connection

```
main loop:
    client_fd = accept()
    pid = fork()
    if (pid == 0) {
        // Child process: handle this client
        handle_client(client_fd)
        exit(0)
    }
    // Parent: close client_fd, go back to accept()
```

```
           accept()
              |
         fork()----+
         |         |
       Parent    Child
       (loops)   (handles client)
         |         |
       accept()  read request
         |       serve file
        ...      close & exit
```

- Simple to implement
- Each client gets its own process (isolated)
- Downside: forking is expensive (copies the whole process)

### Strategy 2: Thread Per Connection

```
    client_fd = accept()
    pthread_create(&tid, NULL, handle_client, client_fd)
    // Main thread: go back to accept()
```

- Lighter than forking (threads share memory)
- Still one-thread-per-client overhead

### Strategy 3: Event Loop (select/epoll)

```
    while (1) {
        ready_fds = select(all_fds)    // Wait for ANY socket to have data
        for each ready fd:
            if (fd == server_fd) accept new client
            else handle_client_data(fd)
    }
```

- Single thread handles thousands of connections
- Most scalable (nginx, Node.js use this model)
- Most complex to implement

**In this module, we use fork-per-connection.** It's the simplest and teaches
process management. We'll explore the others in later modules.

---

## Exercises

1. **http_parser.c** — Parse HTTP requests and build responses.
   You'll write functions to break apart the text of HTTP messages.

2. **http_server.c** — A complete static file server.
   Combines sockets (Module 16) + parsing (exercise 1) into a real server.

### Testing Your Server

Once `http_server` compiles and runs:

```bash
# Terminal 1: Start the server
./http_server 8080

# Terminal 2: Test with curl
curl -v http://localhost:8080/index.html
curl -v http://localhost:8080/style.css
curl -v http://localhost:8080/nonexistent.txt    # Should get 404

# Or open http://localhost:8080/index.html in a browser!
```

### What You'll Build

```
+--------------------------------------------------+
|  Your HTTP Server                                |
|                                                  |
|  1. Create TCP socket (Module 16 stuff)          |
|  2. Bind to port, listen                         |
|  3. Loop:                                        |
|     a. Accept connection                         |
|     b. Fork child process                        |
|     c. Child: read request text from socket      |
|     d. Child: parse request (http_parser logic)  |
|     e. Child: find file in www/ directory        |
|     f. Child: send response with file contents   |
|     g. Child: close connection, exit             |
|     h. Parent: continue accepting                |
+--------------------------------------------------+
```

---

## Key Takeaways

- HTTP is just structured text. Request = "give me this." Response = "here it is."
- `\r\n` line endings everywhere. Blank line = end of headers.
- Content-Type tells the client what the data is. Content-Length says how big.
- Always sanitize URL paths — never let clients read files outside www/.
- Fork-per-connection is the simplest concurrency model.
- Everything you need is: `socket`, `bind`, `listen`, `accept`, `read`, `write`, `close`, `fork`.
