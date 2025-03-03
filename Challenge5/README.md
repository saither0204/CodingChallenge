# Simple C Web Server

This is a **simple HTTP web server written in C** that listens for incoming client connections, processes HTTP requests, and serves basic HTML responses.

---

## **Features**

- Listens on **port 8080**.
- Handles **GET requests**.
- Serves **basic HTML pages**.
- Returns **404 Not Found** for invalid routes.
- Returns **501 Not Implemented** for unsupported HTTP methods.

---

## **How It Works**

### **1. Creating a Server Socket**

```c
server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket < 0) {
    perror("Error creating socket");
    exit(1);
}
```

- Creates a **TCP socket**.

### **2. Binding Socket to Address**

```c
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);
if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("Error binding socket");
    exit(1);
}
```

- Binds the socket to **port 8080**.

### **3. Listening and Accepting Connections**

```c
if (listen(server_socket, 10) < 0) {
    perror("Error listening");
    exit(1);
}
```

- The server listens for incoming connections.

### **4. Handling Client Requests**

```c
ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
if (bytes_read <= 0) {
    perror("Error reading from socket");
    return;
}
```

- Reads the **HTTP request** from the client.

### **5. Parsing and Processing HTTP Requests**

```c
char method[10], path[255], protocol[10];
sscanf(buffer, "%s %s %s", method, path, protocol);
```

- Extracts the HTTP **method**, **path**, and **protocol**.

### **6. Serving Pages**

```c
if (strcmp(path, "/") == 0) {
    strcpy(path, "/index.html");
}
```

- If requesting `/`, the server serves `index.html`.

#### **Supported Pages:**

- `/index.html` - Home Page
- `/about` - About Page

### **7. Sending an HTTP Response**

```c
sprintf(response, "HTTP/1.1 %s\r\n"
                 "Content-Type: %s\r\n"
                 "Content-Length: %d\r\n"
                 "Connection: close\r\n"
                 "\r\n"
                 "%s",
                 status, content_type, body_length, body);
write(client_socket, response, strlen(response));
```

- Constructs and sends an **HTTP response** to the client.

---

## **Installation and Running the Server**

### **1. Compile the Server**

```sh
gcc server.c -o server
```

### **2. Run the Server**

```sh
./server
```

### **3. Open Browser and Visit**

- `http://localhost:8080/` → Serves `index.html`
- `http://localhost:8080/about` → Serves `about.html`
- `http://localhost:8080/unknown` → **404 Not Found**

### **4. Test with**``

```sh
curl -v http://localhost:8080/
curl -v http://localhost:8080/about
curl -v http://localhost:8080/random
```

---

## **Possible Improvements**

- Serve **real files** from disk instead of hardcoded HTML.
- Add **POST request support**.
- Implement **multi-threading** for handling multiple clients.
- Add **HTTPS (TLS/SSL) support**.
- Implement **dynamic content generation**.
