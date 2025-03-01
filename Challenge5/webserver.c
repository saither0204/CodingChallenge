#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define RESPONSE_SIZE 4096

void handle_client(int client_socket);
void send_response(int client_socket, const char* status, const char* content_type, const char* body);
void handle_get_request(int client_socket, char* path);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }
    
    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error setting socket options");
        exit(1);
    }
    
    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }
    
    // Listen for connections
    if (listen(server_socket, 10) < 0) {
        perror("Error listening");
        exit(1);
    }
    
    printf("Server started on port %d\n", PORT);
    
    // Main server loop
    while (1) {
        // Accept connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            continue;
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("Connection accepted from %s:%d\n", client_ip, ntohs(client_addr.sin_port));
        
        // Handle client request
        handle_client(client_socket);
        
        // Close client socket
        close(client_socket);
    }
    
    // Close server socket
    close(server_socket);
    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    
    // Read client request
    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read <= 0) {
        perror("Error reading from socket");
        return;
    }
    
    // Parse HTTP request line
    char method[10], path[255], protocol[10];
    sscanf(buffer, "%s %s %s", method, path, protocol);
    printf("Received request: %s %s %s\n", method, path, protocol);
    
    // Handle GET requests
    if (strcmp(method, "GET") == 0) {
        handle_get_request(client_socket, path);
    } else {
        // Method not supported
        const char* body = "<html><body><h1>501 Not Implemented</h1><p>Method not supported.</p></body></html>";
        send_response(client_socket, "501 Not Implemented", "text/html", body);
    }
}

void handle_get_request(int client_socket, char* path) {
    // Default to index.html for root path
    if (strcmp(path, "/") == 0) {
        strcpy(path, "/index.html");
    }
    
    // Serve content based on requested path
    if (strcmp(path, "/index.html") == 0) {
        const char* body = "<html><body><h1>Welcome to the C Web Server</h1>"
                          "<p>This is a simple web server implemented in C.</p>"
                          "<p><a href='/about'>About</a></p></body></html>";
        send_response(client_socket, "200 OK", "text/html", body);
    } else if (strcmp(path, "/about") == 0) {
        const char* body = "<html><body><h1>About This Server</h1>"
                          "<p>This is a basic HTTP server written in C.</p>"
                          "<p><a href='/'>Home</a></p></body></html>";
        send_response(client_socket, "200 OK", "text/html", body);
    } else {
        // 404 Not Found
        const char* body = "<html><body><h1>404 Not Found</h1>"
                          "<p>The requested resource was not found on this server.</p>"
                          "<p><a href='/'>Home</a></p></body></html>";
        send_response(client_socket, "404 Not Found", "text/html", body);
    }
}

void send_response(int client_socket, const char* status, const char* content_type, const char* body) {
    char response[RESPONSE_SIZE];
    int body_length = strlen(body);
    
    // Format HTTP response
    sprintf(response, "HTTP/1.1 %s\r\n"
                      "Content-Type: %s\r\n"
                      "Content-Length: %d\r\n"
                      "Connection: close\r\n"
                      "\r\n"
                      "%s", 
                      status, content_type, body_length, body);
    
    // Send response
    write(client_socket, response, strlen(response));
}