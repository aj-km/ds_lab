



























































/*
1. Design a distributed application which consists of a server and
client using threads.
CODE:
Server (Multithreaded Server using Sockets and Threads in C)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
void *client_handler(void *socket_desc) {
 int client_sock = *(int *)socket_desc;
 char buffer[BUFFER_SIZE];
 while (1) {
 memset(buffer, 0, BUFFER_SIZE);
 int read_size = read(client_sock, buffer, BUFFER_SIZE - 1);
 if (read_size <= 0) break;
 printf("Client: %s", buffer);
 send(client_sock, buffer, strlen(buffer), 0);
 }
 close(client_sock);
 free(socket_desc);
 return NULL;
}
int main() {
 int server_sock, client_sock, *new_sock;
 struct sockaddr_in server_addr, client_addr;
 socklen_t addr_len = sizeof(struct sockaddr_in);
 server_sock = socket(AF_INET, SOCK_STREAM, 0);
 if (server_sock == -1) {
 perror("Socket creation failed");
 exit(EXIT_FAILURE);
 }
 server_addr.sin_family = AF_INET;
 server_addr.sin_addr.s_addr = INADDR_ANY;
 server_addr.sin_port = htons(PORT);
 if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
 perror("Bind failed");
 close(server_sock);
 exit(EXIT_FAILURE);
 }
 listen(server_sock, 3);
 printf("Server listening on port %d...
", PORT);
 while ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr,
&addr_len))) {
 printf("Client connected.
");
 pthread_t client_thread;
 new_sock = malloc(1);
 *new_sock = client_sock;
 if (pthread_create(&client_thread, NULL, client_handler, (void *)new_sock) < 0) {
 perror("Thread creation failed");
 free(new_sock);
 close(client_sock);
 }
 }
 if (client_sock < 0) {
 perror("Accept failed");
 close(server_sock);
 }
 close(server_sock);
 return 0;
}
Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main() {
 int sock;
 struct sockaddr_in server_addr;
 char buffer[BUFFER_SIZE];
 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock == -1) {
 perror("Socket creation failed");
 exit(EXIT_FAILURE);
 }
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(PORT);
 server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
 perror("Connection failed");
 close(sock);
 exit(EXIT_FAILURE);
 }
 printf("Connected to the server. Type your messages:
");
 while (1) {
 memset(buffer, 0, BUFFER_SIZE);
 fgets(buffer, BUFFER_SIZE, stdin);
 send(sock, buffer, strlen(buffer), 0);
 memset(buffer, 0, BUFFER_SIZE);
 int read_size = read(sock, buffer, BUFFER_SIZE - 1);
 if (read_size <= 0) break;
 printf("Server: %s", buffer);
 }
 close(sock);
 return 0;
}
OUTPUT:
Server Terminal:
Server listening on port 8080...
Client connected.
Client: Hello Server!
Client: How are you?
Client Terminal:
Connected to the server. Type your messages:
Hello Server!
Server: Hello Server!
How are you?
Server: How are you?


2. Design a distributed application which consists of a stateless
server using socket primitives.
CODE:
Stateless Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8081
#define BUFFER_SIZE 1024
int main() {
 int server_sock;
 struct sockaddr_in server_addr, client_addr;
 socklen_t addr_len = sizeof(client_addr);
 char buffer[BUFFER_SIZE];
 server_sock = socket(AF_INET, SOCK_DGRAM, 0);
 if (server_sock == -1) {
 perror("Socket creation failed");
 exit(EXIT_FAILURE);
 }
 server_addr.sin_family = AF_INET;
 server_addr.sin_addr.s_addr = INADDR_ANY;
 server_addr.sin_port = htons(PORT);
 if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
 perror("Bind failed");
 close(server_sock);
 exit(EXIT_FAILURE);
 }
 printf("Stateless server listening on port %d...
", PORT);
 while (1) {
 memset(buffer, 0, BUFFER_SIZE);
 int len = recvfrom(server_sock, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr
*)&client_addr, &addr_len);
 if (len < 0) continue;
 printf("Received: %s", buffer);
 sendto(server_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr,
addr_len);
 }
 close(server_sock);
 return 0;
}
Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8081
#define BUFFER_SIZE 1024
int main() {
 int sock;
 struct sockaddr_in server_addr;
 char buffer[BUFFER_SIZE];
 sock = socket(AF_INET, SOCK_DGRAM, 0);
 if (sock == -1) {
 perror("Socket creation failed");
 exit(EXIT_FAILURE);
 }
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(PORT);
 server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 printf("Stateless client. Type your messages:
");
 while (1) {
 memset(buffer, 0, BUFFER_SIZE);
 fgets(buffer, BUFFER_SIZE, stdin);
 sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr,
sizeof(server_addr));
 memset(buffer, 0, BUFFER_SIZE);
 int len = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
 if (len < 0) continue;
 printf("Server: %s", buffer);
 }
 close(sock);
 return 0;
}
OUTPUT:
Server Terminal:
Stateless server listening on port 8081...
Received: Hello Stateless Server!
Received: How's it going?
Client Terminal:
Stateless client. Type your messages:
Hello Stateless Server!
Server: Hello Stateless Server!
How's it going?
Server: How's it going?

 *************************




8
1. Write a program to find out the list of users who owns a file having
maximum size in the current working directory using Map Reduce
Program.
CODE:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#define MAX_FILES 1024
#define USERNAME_LEN 256
typedef struct {
char username[USERNAME_LEN];
off_t size;
} FileInfo;
FileInfo files[MAX_FILES];
int fileCount = 0;
void addFile(const char *filename) {
struct stat fileStat;
if (stat(filename, &fileStat) == 0) {
struct passwd *pw = getpwuid(fileStat.st_uid);
if (pw != NULL) {
strncpy(files[fileCount].username, pw->pw_name, USERNAME_LEN);
files[fileCount].size = fileStat.st_size;
fileCount++;
}
}
}
void findMaxSizeFiles() {
off_t maxSize = 0;
for (int i = 0; i < fileCount; i++) {
if (files[i].size > maxSize) {
maxSize = files[i].size;
}
}
printf("Files with maximum size (%lld bytes) owned by:\n", (long
long)maxSize);
for (int i = 0; i < fileCount; i++) {
if (files[i].size == maxSize) {
printf("%s\n", files[i].username);
}
}
}
int main() {
DIR *dir;
struct dirent *entry;
dir = opendir(".");
if (dir == NULL) {
perror("opendir");
return EXIT_FAILURE;
}
while ((entry = readdir(dir)) != NULL) {
if (entry->d_type == DT_REG) { // Only regular files
addFile(entry->d_name);
}
}
closedir(dir);
findMaxSizeFiles();
return EXIT_SUCCESS;
}
OUTPUT:
2. Implement RPC mechanism for a file transfer across a network in
'C'.
CODE:

// file_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_file(int client_socket, const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t n;

    while ((n = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (send(client_socket, buffer, n, 0) == -1) {
            perror("Error sending file");
            break;
        }
    }

    fclose(file);
    printf("File sent successfully\n");
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_socket, 3) == -1) {
        perror("Listen failed");
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) >= 0) {
        char file_name[100];
        recv(client_socket, file_name, sizeof(file_name), 0);
        printf("File request received: %s\n", file_name);
        send_file(client_socket, file_name);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}

Client
// file_client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void receive_file(int server_socket, const char *file_name) {
    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t n;

    while ((n = recv(server_socket, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, n, file);
    }

    fclose(file);
    printf("File received successfully\n");
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char file_name[100];

    printf("Enter the file name to request from the server: ");
    scanf("%s", file_name);

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        return 1;
    }

    send(client_socket, file_name, strlen(file_name) + 1, 0);
    receive_file(client_socket, file_name);

    close(client_socket);
    return 0;
}



 *************************
7
1. Suppose you have two TCP servers for converting a lower case
string to uppercase string. You have to design a load balancer
server that accept lower case string from client and check for the
CPU utilization of both servers. Load balancer will transfer the
string to the server having less CPU utilization. The load balancer
will get upper case string from server and return to the clients.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#define PORT 12345 // Use a different port for server2 (e.g., 12346)
#define BUFFER_SIZE 1024
// Function to calculate CPU usage (dummy value for simplicity)
float get_cpu_load() {
 return (rand() % 100) / 100.0; // Return a random CPU usage between
0 and 1
}
// Function to convert a string to uppercase
void to_uppercase(char *str) {
 for (int i = 0; str[i]; i++) {
 str[i] = toupper(str[i]);
 }
}
int main() {
 int server_socket, client_socket;
 struct sockaddr_in server_addr, client_addr;
 char buffer[BUFFER_SIZE];
 socklen_t addr_len;
 // Create socket
 server_socket = socket(AF_INET, SOCK_STREAM, 0);
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(PORT);
 server_addr.sin_addr.s_addr = INADDR_ANY;
 // Bind the socket to an address
 bind(server_socket, (struct sockaddr *)&server_addr,
sizeof(server_addr));
 // Start listening for connections
 listen(server_socket, 5);
 printf("Server listening on port %d...\n", PORT);
 while (1) {
 addr_len = sizeof(client_addr);
 client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
&addr_len);
 printf("Accepted connection from load balancer...\n");
 // Receive command (either "CPU" for load or string for conversion)
 recv(client_socket, buffer, BUFFER_SIZE, 0);
 buffer[strlen(buffer)] = '\0'; // Ensure null termination
 if (strcmp(buffer, "CPU") == 0) {
 // Send CPU load
 float cpu_load = get_cpu_load();
 send(client_socket, &cpu_load, sizeof(cpu_load), 0);
 printf("Sent CPU load: %.2f\n", cpu_load);
 } else {
 // Convert string to uppercase
 to_uppercase(buffer);
 send(client_socket, buffer, strlen(buffer), 0);
 printf("Sent uppercase string: %s\n", buffer);
 }
 close(client_socket);
 }
 close(server_socket);
 return 0;
}
OUTPUT:
Load Balancer Code(broker.c):
The load balancer will handle client requests, compare the CPU usage of the
two servers, forward the string to the least-loaded server, and return the
result to the client.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SERVER1_PORT 12345
#define SERVER2_PORT 12346
#define BUFFER_SIZE 1024
// Function to query CPU load from a server
float get_cpu_load(int port) {
 int sock;
 struct sockaddr_in server_addr;
 float cpu_load;
 // Create socket
 sock = socket(AF_INET, SOCK_STREAM, 0);
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(port);
 server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 // Connect to the server
 connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
 // Send "CPU" request to get CPU load
 send(sock, "CPU", 3, 0);
 recv(sock, &cpu_load, sizeof(cpu_load), 0);
 close(sock);
 return cpu_load;
}
// Function to send a string to a server for conversion
void send_string_to_server(int port, char *str, char *result) {
 int sock;
 struct sockaddr_in server_addr;
 // Create socket
 sock = socket(AF_INET, SOCK_STREAM, 0);
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(port);
 server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 // Connect to the server
 connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
 // Send the string for conversion
 send(sock, str, strlen(str), 0);
 recv(sock, result, BUFFER_SIZE, 0);
 close(sock);
}
int main() {
 int broker_socket, client_socket;
 struct sockaddr_in broker_addr, client_addr;
 char buffer[BUFFER_SIZE], result[BUFFER_SIZE];
 socklen_t addr_len;
 // Create socket for load balancer
 broker_socket = socket(AF_INET, SOCK_STREAM, 0);
 broker_addr.sin_family = AF_INET;
 broker_addr.sin_port = htons(12347); // Broker server port
 broker_addr.sin_addr.s_addr = INADDR_ANY;
 // Bind the socket
 bind(broker_socket, (struct sockaddr *)&broker_addr,
sizeof(broker_addr));
 listen(broker_socket, 5);
 printf("Load Balancer listening on port 12347...\n");
 while (1) {
 addr_len = sizeof(client_addr);
 client_socket = accept(broker_socket, (struct sockaddr *)&client_addr,
&addr_len);
 printf("Accepted connection from client...\n");
 // Receive string from the client
 recv(client_socket, buffer, BUFFER_SIZE, 0);
 buffer[strlen(buffer)] = '\0'; // Ensure null termination
 // Get CPU loads from both servers
 float cpu1 = get_cpu_load(SERVER1_PORT);
 float cpu2 = get_cpu_load(SERVER2_PORT);
 printf("CPU load - Server 1: %.2f, Server 2: %.2f\n", cpu1, cpu2);
 // Send the string to the server with the lower CPU load
 if (cpu1 < cpu2) {
 send_string_to_server(SERVER1_PORT, buffer, result);
 } else {
 send_string_to_server(SERVER2_PORT, buffer, result);
 }
 // Send the result back to the client
 send(client_socket, result, strlen(result), 0);
 printf("Sent result back to client: %s\n", result);
 close(client_socket);
 }
 close(broker_socket);
 return 0;
}
OUTPUT:
2. Implement a client-server program in which the server accepts a
string from a client and transfer the upper-case string to client
using TCP socket in Java.
Client.c
The client sends a string to the load balancer and receives the converted
string back.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BROKER_PORT 12347
#define BUFFER_SIZE 1024
int main() {
 int sock;
 struct sockaddr_in broker_addr;
 char buffer[BUFFER_SIZE], result[BUFFER_SIZE];
 // Create socket
 sock = socket(AF_INET, SOCK_STREAM, 0);
 broker_addr.sin_family = AF_INET;
 broker_addr.sin_port = htons(BROKER_PORT);
 broker_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 // Connect to load balancer
 connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
 // Input string to send
 printf("Enter a string to convert: ");
 fgets(buffer, BUFFER_SIZE, stdin);
 buffer[strlen(buffer) - 1] = '\0'; // Remove newline
 // Send string to load balancer
 send(sock, buffer, strlen(buffer), 0);
 // Receive the uppercase string
 recv(sock, result, BUFFER_SIZE, 0);
 printf("Received uppercase string: %s\n", result);
 close(sock);
 return 0;
}
OUTPUT:


 *************************
6
1. Implement concurrent echo client-server application.
Server code:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 12347
#define BUFFER_SIZE 1024
int main() {
int server_socket, client_socket;
struct sockaddr_in server_addr, client_addr;
socklen_t client_addr_len = sizeof(client_addr);
char buffer[BUFFER_SIZE];
ssize_t bytes_received;
server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);
if (bind(server_socket, (struct sockaddr*)&server_addr,
sizeof(server_addr)) < 0) {
perror("Bind failed");
close(server_socket);
exit(EXIT_FAILURE);
CSE-7B
Reg. No.: 20214160
Name: Harshvardhan Singh
}
if (listen(server_socket, 1) < 0) {
perror("Listen failed");
close(server_socket);
exit(EXIT_FAILURE);
}
printf("Server listening on port %d\n", PORT);
while (1) {
client_socket = accept(server_socket, (struct
sockaddr*)&client_addr, &client_addr_len);
if (client_socket < 0) {
perror("Accept failed");
continue;
}
printf("Accepted connection from %s\n",
inet_ntoa(client_addr.sin_addr));
while ((bytes_received = recv(client_socket, buffer, sizeof(buffer)
- 1, 0)) > 0) {
buffer[bytes_received] = '\0'; // Null-terminate the received
data
fgets(buffer, sizeof(buffer), stdout);
send(client_socket, buffer, bytes_received, 0); // Echo the
data back
}
close(client_socket);
}
close(server_socket);
return 0;
}
Client Code:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 12347
#define BUFFER_SIZE 1024
int main() {
int client_socket;
struct sockaddr_in server_addr;
char buffer[BUFFER_SIZE];
ssize_t bytes_sent, bytes_received;
client_socket = socket(AF_INET, SOCK_STREAM, 0);
if (client_socket < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
server_addr.sin_port = htons(PORT);
if (connect(client_socket, (struct sockaddr*)&server_addr,
sizeof(server_addr)) < 0) {
perror("Connection failed");
close(client_socket);
exit(EXIT_FAILURE);
}
while (1) {
printf("Send message: ");
fgets(buffer, sizeof(buffer), stdin);
buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
if (bytes_sent < 0) {
perror("Send failed");
break;
}
bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1,
0);
if (bytes_received < 0) {
perror("Receive failed");
break;
}
buffer[bytes_received] = '\0'; // Null-terminate the received data
printf("Received: %s\n", buffer);
}
close(client_socket);
return 0;
}
OUTPUT:
2.Implement a client-server program in which the server accepts a
connection from a client and updates its own Master table by adding
the client information and send the updated table to client, so client
can update their own table. Refer the following table format.
Server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 12349
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
typedef struct {
char node[20];
char ip[INET_ADDRSTRLEN];
int port;
} TableEntry;
TableEntry server_table[MAX_CLIENTS];
int table_size = 0;
void update_table(const char* ip, int port) {
// Check if the IP and port are already in the table
int i;
for (i = 0; i < table_size; i++) {
if (strcmp(server_table[i].ip, ip) == 0 && server_table[i].port ==
port) {
return;
}
}
// Add new entry to the table
snprintf(server_table[table_size].node,
sizeof(server_table[table_size].node), "Client-%d", table_size + 1);
strncpy(server_table[table_size].ip, ip,
sizeof(server_table[table_size].ip));
server_table[table_size].port = port;
table_size++;
}
void print_table() {
printf("Master Table:\n");
int i;
for (i = 0; i < table_size; i++) {
printf("Node: %s, IP: %s, Port: %d\n", server_table[i].node,
server_table[i].ip, server_table[i].port);
}
printf("\n");
}
void send_table(int client_socket) {
write(client_socket, &table_size, sizeof(int)); // Send table size
write(client_socket, server_table, sizeof(TableEntry) * table_size);
// Send table entries
}
int main() {
int server_socket, client_socket;
struct sockaddr_in server_addr, client_addr;
socklen_t client_addr_len = sizeof(client_addr);
server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);
if (bind(server_socket, (struct sockaddr*)&server_addr,
sizeof(server_addr)) < 0) {
perror("Bind failed");
close(server_socket);
exit(EXIT_FAILURE);
}
if (listen(server_socket, 5) < 0) {
perror("Listen failed");
close(server_socket);
exit(EXIT_FAILURE);
}
printf("Server is listening on port %d\n", PORT);
while (1) {
client_socket = accept(server_socket, (struct
sockaddr*)&client_addr, &client_addr_len);
if (client_socket < 0) {
perror("Accept failed");
continue;
}
char client_ip[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &client_addr.sin_addr, client_ip,
sizeof(client_ip));
int client_port = ntohs(client_addr.sin_port);
update_table(client_ip, client_port);
// Print the updated master table
print_table();
send_table(client_socket);
close(client_socket);
}
close(server_socket);
return 0;
}
Client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 12349
#define BUFFER_SIZE 1024
typedef struct {
char node[20];
char ip[INET_ADDRSTRLEN];
int port;
} TableEntry;
void receive_table(int server_socket) {
int table_size;
read(server_socket, &table_size, sizeof(int)); // Read table size
TableEntry* table = (TableEntry*)malloc(sizeof(TableEntry) *
table_size);
if (table == NULL) {
perror("Memory allocation failed");
exit(EXIT_FAILURE);
}
read(server_socket, table, sizeof(TableEntry) * table_size); // Read
table entries
printf("Received table:\n");
int i;
for (i = 0; i < table_size; i++) {
printf("Node: %s, IP: %s, Port: %d\n", table[i].node, table[i].ip,
table[i].port);
}
free(table);
}
int main() {
int client_socket;
struct sockaddr_in server_addr;
client_socket = socket(AF_INET, SOCK_STREAM, 0);
if (client_socket < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
if (connect(client_socket, (struct sockaddr*)&server_addr,
sizeof(server_addr)) < 0) {
perror("Connect failed");
close(client_socket);
exit(EXIT_FAILURE);
}
receive_table(client_socket);
close(client_socket);
return 0;
}
OUTPUT:
3. Develop a client-server program to implement a date-time server
and client. Upon connection establishment, the server should send its
current date, time and CPU load information to its clients.
Server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/sysinfo.h>
#define PORT 12350
#define BUFFER_SIZE 256
void get_server_info(char *info_buffer, size_t buffer_size) {
// Get current time
time_t now = time(NULL);
struct tm *tm_info = localtime(&now);
char time_str[64];
strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
// Get CPU load
struct sysinfo sys_info;
if (sysinfo(&sys_info) != 0) {
perror("sysinfo failed");
snprintf(info_buffer, buffer_size, "Failed to retrieve system
info.");
return;
}
float loadavg = (float)sys_info.loads[0] / 65536.0; // Convert to load
average in format (0.00)
snprintf(info_buffer, buffer_size, "Date/Time: %s\nCPU Load: %.2f\n",
time_str, loadavg);
}
void handle_client(int client_socket) {
char info_buffer[BUFFER_SIZE];
get_server_info(info_buffer, sizeof(info_buffer));
send(client_socket, info_buffer, strlen(info_buffer), 0);
close(client_socket);
}
int main() {
int server_socket, client_socket;
struct sockaddr_in server_addr, client_addr;
socklen_t client_addr_len = sizeof(client_addr);
server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);
if (bind(server_socket, (struct sockaddr*)&server_addr,
sizeof(server_addr)) < 0) {
perror("Bind failed");
close(server_socket);
exit(EXIT_FAILURE);
}
if (listen(server_socket, 5) < 0) {
perror("Listen failed");
close(server_socket);
exit(EXIT_FAILURE);
}
printf("Server is listening on port %d\n", PORT);
while (1) {
client_socket = accept(server_socket, (struct
sockaddr*)&client_addr, &client_addr_len);
if (client_socket < 0) {
perror("Accept failed");
continue;
}
handle_client(client_socket);
}
close(server_socket);
return 0;
}
Client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 12350
#define BUFFER_SIZE 256
int main() {
int client_socket;
struct sockaddr_in server_addr;
char buffer[BUFFER_SIZE];
client_socket = socket(AF_INET, SOCK_STREAM, 0);
if (client_socket < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
if (connect(client_socket, (struct sockaddr*)&server_addr,
sizeof(server_addr)) < 0) {
perror("Connect failed");
close(client_socket);
exit(EXIT_FAILURE);
}
int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1,
0);
if (bytes_received < 0) {
perror("Receive failed");
close(client_socket);
exit(EXIT_FAILURE);
}
buffer[bytes_received] = '\0'; // Null-terminate the received data
printf("Received from server:\n%s", buffer);
close(client_socket);
return 0;
}
OUTPUT:


 *************************
5
1. Design a Distributed application using socket. Application consists
of a server which takes an integer value from the client, calculates
factorial and returns the result to the client program.
Server code:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
unsigned long long factorial(int n) {
 unsigned long long result = 1;
 for (int i = 2; i <= n; i++) {
 result *= i;
 }
 return result;
}
int main() {
 int server_fd, new_socket;
 struct sockaddr_in address;
 int addrlen = sizeof(address);
 char buffer[1024] = {0};
 int num;
 unsigned long long fact_result;
 server_fd = socket(AF_INET, SOCK_STREAM, 0);
 address.sin_family = AF_INET;
 address.sin_addr.s_addr = INADDR_ANY;
 address.sin_port = htons(8080);
 bind(server_fd, (struct sockaddr *)&address, sizeof(address));
 listen(server_fd, 3);
 new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t
*)&addrlen);
 read(new_socket, buffer, sizeof(buffer));
 num = atoi(buffer); // Convert the received string to an integer
 fact_result = factorial(num);
 snprintf(buffer, sizeof(buffer), "%llu", fact_result); // Convert the result
back to string
 send(new_socket, buffer, strlen(buffer), 0); // Send back the result
 close(new_socket);
 close(server_fd);
 return 0;
}
Client Code:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
 int sock = 0;
 struct sockaddr_in serv_addr;
 int num;
 unsigned long long fact_result;
 sock = socket(AF_INET, SOCK_STREAM, 0);
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_port = htons(8080);
 inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
 connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
 printf("Enter an integer: ");
 scanf("%d", &num);
 write(sock, &num, sizeof(num));
 read(sock, &fact_result, sizeof(fact_result));
 printf("Factorial of %d is %llu\n", num, fact_result);
 close(sock);
 return 0;
}
OUTPUT:
2. Find out the list of users who owns a file having maximum size in
the current working directory using Map Reduce Program.
Mapper.c
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
int main()
{
 struct dirent *entry;
 struct stat file_stat;
 DIR *dp = opendir(".");
 if (dp == NULL)
 {
 perror("opendir");
 return EXIT_FAILURE;
 }
 while ((entry = readdir(dp)))
 {
 if (entry->d_type == DT_REG)
 {
 if (stat(entry->d_name, &file_stat) == 0)
 {
 struct passwd *pw = getpwuid(file_stat.st_uid);
 if (pw)
 {
 printf("%ld\t%s\n", file_stat.st_size, pw->pw_name);
 }
 }
 }
 }
 closedir(dp);
 return EXIT_SUCCESS;
}
Reducer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 256
int main()
{
 char line[MAX_LINE_LENGTH];
 long max_size = 0;
 char owners[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
 int owner_count = 0;
 long file_size;
 char owner[MAX_LINE_LENGTH];
 while (fgets(line, sizeof(line), stdin))
 {
 sscanf(line, "%ld\t%s", &file_size, owner);
 if (file_size > max_size)
 {
 max_size = file_size;
 owner_count = 0;
 strcpy(owners[owner_count++], owner);
 }
 else if (file_size == max_size)
 {
 int found = 0;
 for (int i = 0; i < owner_count; i++)
 {
 if (strcmp(owners[i], owner) == 0)
 {
 found = 1;
 break;
 }
 }
 if (!found)
 {
 strcpy(owners[owner_count++], owner);
 }
 }
 }
 for (int i = 0; i < owner_count; i++)
 {
 printf("%s\n", owners[i]);
 }
 return EXIT_SUCCESS;
}
OUTPUT:
 *************************
4
1. Write a program to simulate logical clock synchronisation using
Lamport’s logical clock algorithm and vector clocks.
Lamport’s Logical Clock Algorithm
CODE:
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define NUM_PROCESSES 3
typedef struct process {
 int id;
 int timestamp;
 pthread_mutex_t lock;
 pthread_t thread;
 struct process *neighbors[NUM_PROCESSES];
} Process;
void *process_func(void *arg) {
 Process *proc = (Process *)arg;
 while (1) {
 // Simulate some work (sleep for 500ms to 1500ms)
 usleep((rand() % 1000 + 500) * 1000);
 // Lock and increment the timestamp
 pthread_mutex_lock(&proc->lock);
 proc->timestamp++;
 printf("Process %d has timestamp %d\n", proc->id, proc-
>timestamp);
 // Send the timestamp to all neighbors
 for (int i = 0; i < NUM_PROCESSES; i++) {
 if (proc->neighbors[i] != NULL) {
 Process *neighbor = proc->neighbors[i];
 pthread_mutex_lock(&neighbor->lock);
 if (proc->timestamp > neighbor->timestamp) {
 neighbor->timestamp = proc->timestamp;
 }
 pthread_mutex_unlock(&neighbor->lock);
 printf("Process %d updated Process %d's timestamp to %d\n",
 proc->id, neighbor->id, neighbor->timestamp);
 }
 }
 pthread_mutex_unlock(&proc->lock);
 // Simulate some time in the critical section (sleep for 1 second)
 sleep(1);
 }
 return NULL;
}
int main() {
 srand(time(NULL));
 // Create processes
 Process processes[NUM_PROCESSES];
 for (int i = 0; i < NUM_PROCESSES; i++) {
 processes[i].id = i;
 processes[i].timestamp = 0;
 pthread_mutex_init(&processes[i].lock, NULL);
 // Initialize all neighbors to NULL
 for (int j = 0; j < NUM_PROCESSES; j++) {
 processes[i].neighbors[j] = NULL;
 }
 }
 // Set up neighbors in a circular fashion
 processes[0].neighbors[0] = &processes[1];
 processes[1].neighbors[0] = &processes[2];
 processes[2].neighbors[0] = &processes[0];
 // Create threads for each process
 for (int i = 0; i < NUM_PROCESSES; i++) {
 pthread_create(&processes[i].thread, NULL, process_func,
&processes[i]);
 }
 // Run the simulation for 20 seconds
 sleep(20);
 // Clean up: cancel threads and destroy mutexes
 for (int i = 0; i < NUM_PROCESSES; i++) {
 pthread_cancel(processes[i].thread);
 pthread_join(processes[i].thread, NULL);
 pthread_mutex_destroy(&processes[i].lock);
 }
 return 0;
}
OUTPUT:
Vector Clocks Algorithm
CODE:
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define NUM_PROCESSES 3
typedef struct {
 int id;
 int vector[NUM_PROCESSES];
 pthread_mutex_t lock;
 pthread_t thread;
 struct process *neighbors[NUM_PROCESSES];
} Process;
void *process_func(void *arg) {
 Process *proc = (Process *)arg;
 while (1) {
 // Simulate some work
 usleep((rand() % 1000 + 500) * 1000);
 // Lock and increment vector clock
 pthread_mutex_lock(&proc->lock);
 proc->vector[proc->id]++;

 printf("Process %d has vector clock [", proc->id);
 for (int i = 0; i < NUM_PROCESSES; i++) {
 printf("%d", proc->vector[i]);
 if (i < NUM_PROCESSES - 1) printf(", ");
 }
 printf("]\n");
 // Send vector clock to all neighbors
 for (int i = 0; i < NUM_PROCESSES; i++) {
 if (proc->neighbors[i] != NULL) {
 Process *neighbor = proc->neighbors[i];
 pthread_mutex_lock(&neighbor->lock);
 for (int j = 0; j < NUM_PROCESSES; j++) {
 if (proc->vector[j] > neighbor->vector[j]) {
 neighbor->vector[j] = proc->vector[j];
 }
 }
 printf("Process %d updated Process %d vector clock to [", proc-
>id, neighbor->id);
 for (int k = 0; k < NUM_PROCESSES; k++) {
 printf("%d", neighbor->vector[k]);
 if (k < NUM_PROCESSES - 1) printf(", ");
 }
 printf("]\n");
 pthread_mutex_unlock(&neighbor->lock);
 }
 }
 pthread_mutex_unlock(&proc->lock);
 sleep(1); // Simulate some time in the critical section
 }
 return NULL;
}
int main() {
 srand(time(NULL));
 Process processes[NUM_PROCESSES];
 // Initialize processes
 for (int i = 0; i < NUM_PROCESSES; i++) {
 processes[i].id = i;
 for (int j = 0; j < NUM_PROCESSES; j++) {
 processes[i].vector[j] = 0;
 }
 pthread_mutex_init(&processes[i].lock, NULL);
 for (int j = 0; j < NUM_PROCESSES; j++) {
 processes[i].neighbors[j] = NULL;
 }
 }
 // Set up neighbors in a circular fashion
 processes[0].neighbors[0] = &processes[1];
 processes[1].neighbors[0] = &processes[2];
 processes[2].neighbors[0] = &processes[0];
 // Create threads for each process
 for (int i = 0; i < NUM_PROCESSES; i++) {
 pthread_create(&processes[i].thread, NULL, process_func,
&processes[i]);
 }
 sleep(20); // Run simulation for 20 seconds
 // Cleanup and terminate threads
 for (int i = 0; i < NUM_PROCESSES; i++) {
 pthread_cancel(processes[i].thread);
 pthread_join(processes[i].thread, NULL);
 pthread_mutex_destroy(&processes[i].lock);
 }
 return 0;
}
OUTPUT:
Process 0 has vector clock [1, 0, 0]
Process 0 sent request to Process 1 with vector clock [1, 0, 0]
Process 1 has vector clock [0, 1, 0]
Process 1 received request from Process 0 with vector clock [1, 0, 0]
Process 1 updated vector clock to [1, 1, 0]
Process 0 updated Process 1 vector clock to [1, 1, 0]
Process 0 is in critical section
Process 0 left critical section
Process 1 is in critical section
Process 1 left critical section
Process 2 has vector clock [0, 0, 1]
Process 2 sent request to Process 0 with vector clock [0, 0, 1]
Process 0 has vector clock [2, 1, 0]
Process 0 sent request to Process 1 with vector clock [2, 1, 0]
Process 1 has vector clock [2, 1, 0]
Process 1 received request from Process 0 with vector clock [2, 1, 0]
Process 1 updated vector clock to [2, 1, 0]
Process 2 received request from Process 0 with vector clock [2, 1, 0]
Process 2 updated vector clock to [2, 1, 1]
Process 2 is in critical section
Process 2 left critical section
Process 0 is in critical section
Process 0 left critical section
Process 1 has vector clock [2, 2, 0]
Process 1 sent request to Process 2 with vector clock [2, 2, 0]
Process 2 has vector clock [2, 1, 2]
Process 2 received request from Process 1 with vector clock [2, 2, 0]
Process 2 updated vector clock to [2, 2, 2]
Process 1 is in critical section
Process 1 left critical section
Process 2 is in critical section
Process 2 left critical section
2. Write a program to simulate distributed mutual exclusion
algorithm (Lamport and Ricartagrawala algorithms).
Lamport's Mutual Exclusion Algorithm
CODE:
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define NUM_PROCESSES 3
typedef struct {
int id;
int timestamp;
int requests[NUM_PROCESSES];
pthread_mutex_t lock;
pthread_t thread;
int status; // 0 = idle, 1 = requesting, 2 = in critical section
int num_requests;
struct process *neighbors[NUM_PROCESSES];
} Process;
void request_critical_section(Process *proc) {
pthread_mutex_lock(&proc->lock);
proc->status = 1; // Requesting critical section
proc->timestamp++;
proc->requests[proc->id] = proc->timestamp;
// Send request to all neighbors
for (int i = 0; i < NUM_PROCESSES; i++) {
if (proc->neighbors[i] != NULL) {
Process *neighbor = proc->neighbors[i];
pthread_mutex_lock(&neighbor->lock);
neighbor->requests[proc->id] = proc->timestamp;
printf("Process %d sent request to Process %d with timestamp %d\n",
proc->id, neighbor->id, proc->timestamp);
pthread_mutex_unlock(&neighbor->lock);
}
}
pthread_mutex_unlock(&proc->lock);
}
void *process_func(void *arg) {
Process *proc = (Process *)arg;
while (1) {
// Simulate some work
usleep((rand() % 1000 + 500) * 1000);
// Request critical section
request_critical_section(proc);
// Enter critical section
pthread_mutex_lock(&proc->lock);
while (proc->status == 1) {
pthread_mutex_unlock(&proc->lock);
sleep(1); // Wait for a while before checking again
pthread_mutex_lock(&proc->lock);
}
proc->status = 2; // In critical section
printf("Process %d is in critical section\n", proc->id);
pthread_mutex_unlock(&proc->lock);
// Simulate critical section work
sleep(1);
// Exit critical section
pthread_mutex_lock(&proc->lock);
proc->status = 0; // Back to idle
printf("Process %d left critical section\n", proc->id);
pthread_mutex_unlock(&proc->lock);
}
return NULL;
}
int main() {
srand(time(NULL));
Process processes[NUM_PROCESSES];
pthread_mutex_t mutexes[NUM_PROCESSES];
for (int i = 0; i < NUM_PROCESSES; i++) {
processes[i].id = i;
processes[i].timestamp = 0;
processes[i].status = 0;
processes[i].num_requests = 0;
pthread_mutex_init(&processes[i].lock, NULL);
for (int j = 0; j < NUM_PROCESSES; j++) {
processes[i].requests[j] = 0;
processes[i].neighbors[j] = NULL;
}
}
// Set up neighbors
processes[0].neighbors[0] = &processes[1];
processes[1].neighbors[0] = &processes[2];
processes[2].neighbors[0] = &processes[0];
for (int i = 0; i < NUM_PROCESSES; i++) {
pthread_create(&processes[i].thread, NULL, process_func, &processes[i]);
}
sleep(20); // Run simulation for 20 seconds
for (int i = 0; i < NUM_PROCESSES; i++) {
pthread_cancel(processes[i].thread);
pthread_join(processes[i].thread, NULL);
pthread_mutex_destroy(&processes[i].lock);
}
return 0;
}
OUTPUT:
 Process 0 requesting critical section with timestamp 1
Process 1 requesting critical section with timestamp 1
Process 0 sent request to Process 1 with timestamp 1
Process 1 sent request to Process 2 with timestamp 1
Process 1 sent request to Process 0 with timestamp 1
Process 2 requesting critical section with timestamp 1
Process 1 sent request to Process 2 with timestamp 1
Process 0 is in critical section
Process 0 left critical section
Process 2 is in critical section
Process 2 left critical section
Process 1 is in critical section
Process 1 left critical section
Process 0 requesting critical section with timestamp 2
Process 0 sent request to Process 1 with timestamp 2
Process 1 sent request to Process 2 with timestamp 2
Process 2 requesting critical section with timestamp 2
Process 2 sent request to Process 0 with timestamp 2
Process 1 is in critical section
Process 1 left critical section
Process 2 is in critical section
Process 2 left critical section
Process 0 is in critical section
Process 0 left critical section
Process 1 requesting critical section with timestamp 3
Process 1 sent request to Process 2 with timestamp 3
Process 2 requesting critical section with timestamp 3
Process 2 sent request to Process 0 with timestamp 3
Process 0 requesting critical section with timestamp 3
Process 1 is in critical section
Process 1 left critical section
Process 2 is in critical section
Process 2 left critical section
Process 0 is in critical section
Process 0 left critical section
Ricart-Agrawala's Mutual Exclusion Algorithm
CODE:
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define NUM_PROCESSES 3
typedef struct {
int id;
int vector[NUM_PROCESSES];
pthread_mutex_t lock;
pthread_t thread;
int status; // 0 = idle, 1 = requesting, 2 = in critical section
struct process *neighbors[NUM_PROCESSES];
} Process;
void send_request(Process *proc) {
pthread_mutex_lock(&proc->lock);
proc->status = 1; // Requesting critical section
proc->vector[proc->id]++;
printf("Process %d requesting critical section with vector clock [", proc-
>id);
for (int i = 0; i < NUM_PROCESSES; i++) {
printf("%d", proc->vector[i]);
if (i < NUM_PROCESSES - 1) printf(", ");
}
printf("]\n");
pthread_mutex_unlock(&proc->lock);
// Send request to all neighbors
for (int i = 0; i < NUM_PROCESSES; i++) {
if (proc->neighbors[i] != NULL) {
Process *neighbor = proc->neighbors[i];
pthread_mutex_lock(&neighbor->lock);
for (int j = 0; j < NUM_PROCESSES; j++) {
neighbor->vector[j] = proc->vector[j];
}
printf("Process %d sent request to Process %d\n", proc->id, neighbor-
>id);
pthread_mutex_unlock(&neighbor->lock);
}
}
}
void *process_func(void *arg) {
Process *proc = (Process *)arg;
while (1) {
// Simulate some work
usleep((rand() % 1000 + 500) * 1000);
// Request critical section
send_request(proc);
// Enter critical section
pthread_mutex_lock(&proc->lock);
while (proc->status == 1) {
pthread_mutex_unlock(&proc->lock);
sleep(1); // Wait for a while before checking again
pthread_mutex_lock(&proc->lock);
}
proc->status = 2; // In critical section
printf("Process %d is in critical section\n", proc->id);
pthread_mutex_unlock(&proc->lock);
// Simulate critical section work
sleep(1);
// Exit critical section
pthread_mutex_lock(&proc->lock);
proc->status = 0; // Back to idle
printf("Process %d left critical section\n", proc->id);
pthread_mutex_unlock(&proc->lock);
}
return NULL;
}
int main() {
srand(time(NULL));
Process processes[NUM_PROCESSES];
pthread_mutex_t mutexes[NUM_PROCESSES];
for (int i = 0; i < NUM_PROCESSES; i++) {
processes[i].id = i;
for (int j = 0; j < NUM_PROCESSES; j++) {
processes[i].vector[j] = 0;
}
processes[i].status = 0;
pthread_mutex_init(&processes[i].lock, NULL);
for (int j = 0; j < NUM_PROCESSES; j++) {
processes[i].neighbors[j] = NULL;
}
}
// Set up neighbors
processes[0].neighbors[0] = &processes[1];
processes[1].neighbors[0] = &processes[2];
processes[2].neighbors[0] = &processes[0];
for (int i = 0; i < NUM_PROCESSES; i++) {
pthread_create(&processes[i].thread, NULL, process_func, &processes[i]);
}
sleep(20); // Run simulation for 20 seconds
for (int i = 0; i < NUM_PROCESSES; i++) {
pthread_cancel(processes[i].thread);
pthread_join(processes[i].thread, NULL);
pthread_mutex_destroy(&processes[i].lock);
}
return 0;
}
OUTPUT:
Process 0 requesting critical section with vector clock [1, 0, 0]
Process 0 sent request to Process 1
Process 1 requesting critical section with vector clock [0, 1, 0]
Process 1 sent request to Process 2
Process 2 requesting critical section with vector clock [0, 0, 1]
Process 2 sent request to Process 0
Process 0 is in critical section
Process 0 left critical section
Process 1 is in critical section
Process 1 left critical section
Process 2 is in critical section
Process 2 left critical section
Process 0 requesting critical section with vector clock [2, 0, 0]
Process 0 sent request to Process 1
Process 1 requesting critical section with vector clock [0, 2, 0]
Process 1 sent request to Process 2
Process 2 requesting critical section with vector clock [0, 0, 2]
Process 2 sent request to Process 0
Process 1 is in critical section
Process 1 left critical section
Process 2 is in critical section
Process 2 left critical section
Process 0 is in critical section
Process 0 left critical section
Process 1 requesting critical section with vector clock [0, 3, 0]
Process 1 sent request to Process 2
Process 2 requesting critical section with vector clock [0, 0, 3]
Process 2 sent request to Process 0
Process 0 requesting critical section with vector clock [1, 0, 0]
Process 1 is in critical section
Process 1 left critical section
Process 2 is in critical section
Process 2 left critical section
Process 0 is in critical section
Process 0 left critical section
 *************************
3
1. Write a client (UDP) C program that calls sendto() to send string to
server program knowing IP address and port number.
CODE:
SERVER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
int main() {

int sockfd;
struct sockaddr_in server_addr, client_addr;
char buffer[BUFFER_SIZE];
socklen_t addr_len = sizeof(client_addr);
ssize_t recv_len;
// Create a UDP socket
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
// Configure the server address
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(SERVER_PORT);
// Bind the socket to the address and port
if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
{
perror("Bind failed");
close(sockfd);
exit(EXIT_FAILURE);
}
printf("Server is running and waiting for messages ... \n");
// Receive messages from clients
while (1) {
memset(buffer, 0, sizeof(buffer));
recv_len = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct
sockaddr*)&client_addr,
&addr_len);
if (recv_len < 0) {
perror("Receive failed");
close(sockfd);
exit(EXIT_FAILURE);
}
buffer[recv_len] ='\0'; // Null-terminate the received data
printf("Received message from client: %s \n", buffer);
}
// Close the socket (this line will never be reached in the current setup)
close(sockfd);
return 0;
}
OUTPUT:
CLIENT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1" // Change this to the server's IP address
int main() {
 int sockfd;
 struct sockaddr_in server_addr;
 char message[1024];
 socklen_t addr_len = sizeof(server_addr);
 // Create a UDP socket
 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
 perror("Socket creation failed");
 exit(EXIT_FAILURE);
 }
 // Configure the server address
 memset(&server_addr, 0, sizeof(server_addr));
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(SERVER_PORT);

 if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
 perror("Invalid address or address not supported");
 close(sockfd);
 exit(EXIT_FAILURE);
 }
 // Get the message to send
 printf("Enter message: ");
 fgets(message, sizeof(message), stdin);
 message[strcspn(message, "\n")] = '\0'; // Remove trailing newline
 // Send the message to the server
 if (sendto(sockfd, message, strlen(message), 0, (struct
sockaddr*)&server_addr, addr_len) < 0) {
 perror("Send failed");
 close(sockfd);
 exit(EXIT_FAILURE);
 }
 printf("Message sent to server: %s\n", message);
 // Close the socket
 close(sockfd);
 return 0;
}
2. Design a Distributed Application using Message Passing Interface
(MPI) for remote computation.
CODE:
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define ARRAY_SIZE 100
int main(int argc, char* argv[]) {
 int rank, size, i;
 int array[ARRAY_SIZE], local_sum = 0, total_sum = 0;
 int local_size, *local_array;
 // Initialize MPI
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the
process
 MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the number of
processes
 local_size = ARRAY_SIZE / size; // Each process will handle a subset of
the array
 // The root process initializes the array
 if (rank == 0) {
 for (i = 0; i < ARRAY_SIZE; i++) {
 array[i] = i + 1; // Fill array with numbers 1 to ARRAY_SIZE
 }
 }
 // Allocate space for each process's local portion of the array
 local_array = (int*)malloc(local_size * sizeof(int));
 // Scatter the array from the root process to all processes
 MPI_Scatter(array, local_size, MPI_INT, local_array, local_size, MPI_INT,
0, MPI_COMM_WORLD);
 // Each process computes the sum of its local array
 for (i = 0; i < local_size; i++) {
 local_sum += local_array[i];
 }
 // Gather the local sums from all processes and sum them up in the root
process
 MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0,
MPI_COMM_WORLD);
 // The root process prints the total sum
 if (rank == 0) {
 printf("Total sum of array: %d\n", total_sum);
 }
 // Finalize MPI
 MPI_Finalize();
 free(local_array);
 return 0;
}
OUTPUT:
 *************************
2
1. There are two nodes A and B. Write a program to determine CPU
load of node B from node A.
CODE:
// node_b_cpu_load_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/sysinfo.h>
#define PORT 12345
#define BUFFER_SIZE 256
void get_cpu_load(char *buffer) {
 struct sysinfo info;
 if (sysinfo(&info) == 0) {
 snprintf(buffer, BUFFER_SIZE, "Load average: %f, %f, %f",
 info.loads[0] / (float)(1 << SI_LOAD_SHIFT),
 info.loads[1] / (float)(1 << SI_LOAD_SHIFT),
 info.loads[2] / (float)(1 << SI_LOAD_SHIFT));
 } else {
 snprintf(buffer, BUFFER_SIZE, "Failed to get CPU load.");
 }
}
int main() {
 int server_fd, new_socket;
 struct sockaddr_in address;
 int addrlen = sizeof(address);
 char buffer[BUFFER_SIZE] = {0};
 // Create socket
 if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
 perror("Socket creation error");
 exit(EXIT_FAILURE);
 }
 // Set up the server address structure
 address.sin_family = AF_INET;
 address.sin_addr.s_addr = INADDR_ANY;
 address.sin_port = htons(PORT);
 // Bind the socket to the address and port
 if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
 perror("Bind failed");
 close(server_fd);
 exit(EXIT_FAILURE);
 }
 // Listen for incoming connections
 if (listen(server_fd, 3) < 0) {
 perror("Listen failed");
 close(server_fd);
 exit(EXIT_FAILURE);
 }
 printf("Waiting for connection...\n");
 // Accept a connection
 if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
(socklen_t*)&addrlen)) < 0) {
 perror("Accept failed");
 close(server_fd);
 exit(EXIT_FAILURE);
 }
 // Get CPU load and send it to the client
 get_cpu_load(buffer);
 send(new_socket, buffer, strlen(buffer), 0);
 printf("CPU load sent to the client: %s\n", buffer);
 close(new_socket);
 close(server_fd);
 return 0;
}
OUTPUT:
// node_a_cpu_load_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 12345
#define BUFFER_SIZE 256
int main(int argc, char *argv[]) {
 int sock = 0;
 struct sockaddr_in serv_addr;
 char buffer[BUFFER_SIZE] = {0};
 if (argc != 2) {
 fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
 return EXIT_FAILURE;
 }
 // Create socket
 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
 perror("Socket creation error");
 return EXIT_FAILURE;
 }
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_port = htons(PORT);
 // Convert IP address from text to binary form
 if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
 perror("Invalid address/ Address not supported");
 close(sock);
 return EXIT_FAILURE;
 }
 // Connect to the server
 if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
 perror("Connection failed");
 close(sock);
 return EXIT_FAILURE;
 }
 // Read the CPU load from the server
 read(sock, buffer, sizeof(buffer));
 printf("Received from server: %s\n", buffer);
 close(sock);
 return 0;
}
OUTPUT:
2. Write a server C program using shared memory and semaphore
(server increments counter between sem_wait() and sem_post()).
Create shared memory using mmap.
CODE:
// shared_memory_server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#define SHARED_MEM_NAME "/my_shm"
#define SEM_NAME "/my_sem"
#define SHM_SIZE sizeof(int)
int main() {
 int fd;
 int *counter;
 sem_t *sem;
 // Create or open the shared memory
 fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
 if (fd == -1) {
 perror("shm_open");
 exit(EXIT_FAILURE);
 }
 ftruncate(fd, SHM_SIZE);
 // Map shared memory
 counter = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
 if (counter == MAP_FAILED) {
 perror("mmap");
 exit(EXIT_FAILURE);
 }
 // Initialize the counter
 *counter = 0;
 // Create or open semaphore
 sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
 if (sem == SEM_FAILED) {
 perror("sem_open");
 exit(EXIT_FAILURE);
 }
 // Increment the counter with semaphore protection
 while (1) {
 sem_wait(sem);
 (*counter)++;
 printf("Counter incremented to %d\n", *counter);
 sem_post(sem);
 sleep(1);
 }
 // Clean up
 sem_close(sem);
 sem_unlink(SEM_NAME);
 munmap(counter, SHM_SIZE);
 shm_unlink(SHARED_MEM_NAME);
 return 0;
}
OUTPUT:
3. Write a client C program that reads counter value between
sem_wait() and sem_post(). Access shared memory using open().
CODE:
// shared_memory_client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#define SHARED_MEM_NAME "/my_shm"
#define SEM_NAME "/my_sem"
#define SHM_SIZE sizeof(int)
int main() {
 int fd;
 int *counter;
 sem_t *sem;
 // Open the shared memory
 fd = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
 if (fd == -1) {
 perror("shm_open");
 exit(EXIT_FAILURE);
 }
 // Map shared memory
 counter = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
 if (counter == MAP_FAILED) {
 perror("mmap");
 exit(EXIT_FAILURE);
 }
 // Open semaphore
 sem = sem_open(SEM_NAME, 0);
 if (sem == SEM_FAILED) {
 perror("sem_open");
 exit(EXIT_FAILURE);
 }
 // Read the counter value
 while (1) {
 sem_wait(sem);
 printf("Current counter value: %d\n", *counter);
 sem_post(sem);
 sleep(1);
 }
 // Clean up
 sem_close(sem);
 munmap(counter, SHM_SIZE);
 return 0;
}


1. Write a program to create two processes. The first process takes a
string and passes it to second process through a pipe. The second
process concatenates the received string with another string without
using string function and sends it back to the first process for printing.
CODE:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 256
void concatenate_without_string_functions(char *dest, const char *src) {
 // Find the end of the dest string
 while (*dest) {
 dest++;
 }

 // Copy the src string to the end of dest
 while (*src) {
 *dest++ = *src++;
 }

 // Null-terminate the result
 *dest = '\0';
}
int main() {
 int pipe1[2], pipe2[2];
 pid_t pid;
 char input_string[BUFFER_SIZE];
 char received_string[BUFFER_SIZE];
 char final_string[BUFFER_SIZE] = " - new string";
 // Create the first pipe
 if (pipe(pipe1) == -1) {
 perror("pipe1");
 exit(EXIT_FAILURE);
 }
 // Create the second pipe
 if (pipe(pipe2) == -1) {
 perror("pipe2");
 exit(EXIT_FAILURE);
 }
 // Fork the process
 pid = fork();
 if (pid == -1) {
 perror("fork");
 exit(EXIT_FAILURE);
 }
 if (pid == 0) {
 // Child process (second process)
 close(pipe1[1]); // Close write end of pipe1
 close(pipe2[0]); // Close read end of pipe2
 // Read the string from pipe1
 read(pipe1[0], received_string, BUFFER_SIZE);
 close(pipe1[0]);
 // Concatenate the received string with final_string
 concatenate_without_string_functions(received_string, final_string);
 // Send the concatenated string to pipe2
 write(pipe2[1], received_string, strlen(received_string) + 1);
 close(pipe2[1]);
 exit(EXIT_SUCCESS);
 } else {
 // Parent process (first process)
 close(pipe1[0]); // Close read end of pipe1
 close(pipe2[1]); // Close write end of pipe2
 // Get the input string from the user
 printf("Enter a string: ");
 fgets(input_string, BUFFER_SIZE, stdin);
 // Remove newline character if present
 size_t len = strlen(input_string);
 if (len > 0 && input_string[len - 1] == '\n') {
 input_string[len - 1] = '\0';
 }
 // Send the input string to the second process through pipe1
 write(pipe1[1], input_string, strlen(input_string) + 1);
 close(pipe1[1]);
 // Wait for the child process to finish
 wait(NULL);
 // Read the result from pipe2
 read(pipe2[0], received_string, BUFFER_SIZE);
 close(pipe2[0]);
 // Print the final concatenated string
 printf("Received concatenated string: %s\n", received_string);
 exit(EXIT_SUCCESS);
 }
}
OUTPUT:
2. Write a program in which the parent process sends two matrices to
its child process through pipe and the child process returns the sum
of the matrices to the parent through a pipe. The parent should print
the result.
CODE:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MATRIX_SIZE 3
void print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
 for (int i = 0; i < MATRIX_SIZE; i++) {
 for (int j = 0; j < MATRIX_SIZE; j++) {
 printf("%d ", matrix[i][j]);
 }
 printf("\n");
 }
}
void sum_matrices(int matrix1[MATRIX_SIZE][MATRIX_SIZE], int
matrix2[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]) {
 for (int i = 0; i < MATRIX_SIZE; i++) {
 for (int j = 0; j < MATRIX_SIZE; j++) {
 result[i][j] = matrix1[i][j] + matrix2[i][j];
 }
 }
}
int main() {
 int pipe1[2], pipe2[2];
 pid_t pid;
 int matrix1[MATRIX_SIZE][MATRIX_SIZE];
 int matrix2[MATRIX_SIZE][MATRIX_SIZE];
 int result_matrix[MATRIX_SIZE][MATRIX_SIZE];
 // Initialize matrices
 printf("Enter Matrix1:\n");
 for (int i = 0; i < MATRIX_SIZE; i++) {
 for (int j = 0; j < MATRIX_SIZE; j++) {
 scanf("%d", &matrix1[i][j]);
 }
 }

 printf("Enter Matrix2:\n");
 for (int i = 0; i < MATRIX_SIZE; i++) {
 for (int j = 0; j < MATRIX_SIZE; j++) {
 scanf("%d", &matrix2[i][j]);
 }
 }


 // Create pipes
 if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
 perror("pipe");
 exit(EXIT_FAILURE);
 }
 // Fork the process
 pid = fork();
 if (pid == -1) {
 perror("fork");
 exit(EXIT_FAILURE);
 }
 if (pid == 0) {
 // Child process
 close(pipe1[1]); // Close write end of pipe1
 close(pipe2[0]); // Close read end of pipe2
 int recv_matrix1[MATRIX_SIZE][MATRIX_SIZE];
 int recv_matrix2[MATRIX_SIZE][MATRIX_SIZE];
 // Read matrices from pipe1
 read(pipe1[0], recv_matrix1, sizeof(recv_matrix1));
 read(pipe1[0], recv_matrix2, sizeof(recv_matrix2));
 close(pipe1[0]);
 // Compute the sum of matrices
 sum_matrices(recv_matrix1, recv_matrix2, result_matrix);
 // Send the result matrix back to parent via pipe2
 write(pipe2[1], result_matrix, sizeof(result_matrix));
 close(pipe2[1]);
 exit(EXIT_SUCCESS);
 } else {
 // Parent process
 close(pipe1[0]); // Close read end of pipe1
 close(pipe2[1]); // Close write end of pipe2
 // Send matrices to the child process
 write(pipe1[1], matrix1, sizeof(matrix1));
 write(pipe1[1], matrix2, sizeof(matrix2));
 close(pipe1[1]);
 // Wait for child process to complete
 wait(NULL);
 // Read the result matrix from child process
 read(pipe2[0], result_matrix, sizeof(result_matrix));
 close(pipe2[0]);
 // Print the result matrix
 printf("Sum of matrices:\n");
 print_matrix(result_matrix);
 exit(EXIT_SUCCESS);
 }
}

*/


/*
Write a program to find out the list of users who owns a file having maximum size in the current working directory using Map Reduce Program. c++ very short and simple code
#include <iostream>
#include <filesystem>
#include <map>
#include <vector>
#include <pwd.h>
#include <sys/stat.h>

using namespace std;
namespace fs = std::filesystem;

int main() {
    map<string, vector<string>> userFileMap; // Map: user -> files
    uintmax_t maxSize = 0;

    // Iterate over files in the current directory
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_regular_file()) {
            auto fileSize = entry.file_size();
            maxSize = max(maxSize, fileSize); // Track maximum file size
            
            // Get file owner
            struct stat fileStat;
            stat(entry.path().c_str(), &fileStat);
            struct passwd *pw = getpwuid(fileStat.st_uid);
            string owner = pw ? pw->pw_name : "unknown";

            userFileMap[owner].push_back(entry.path().string()); // Map file to owner
        }
    }

    // Output users owning the files with maximum size
    cout << "Users owning the largest files (" << maxSize << " bytes):\n";
    for (const auto& [user, files] : userFileMap) {
        for (const auto& file : files) {
            if (fs::file_size(file) == maxSize) {
                cout << "User: " << user << ", File: " << file << "\n";
            }
        }
    }

    return 0;
}

Implement RPC mechanism for a file transfer across a network in ‘C’ ++ a simple code and using namespace std
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

void sendFile(int clientSocket, const string& fileName) {
    ifstream file(fileName, ios::binary);
    if (!file) {
        string error = "ERROR: File not found\n";
        send(clientSocket, error.c_str(), error.size(), 0);
        return;
    }

    string success = "SUCCESS\n";
    send(clientSocket, success.c_str(), success.size(), 0);

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        send(clientSocket, buffer, file.gcount(), 0);
    }
    if (file.gcount() > 0) {
        send(clientSocket, buffer, file.gcount(), 0);
    }

    file.close();
    cout << "File sent successfully.\n";
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Socket creation failed.\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Bind failed.\n";
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed.\n";
        close(serverSocket);
        return 1;
    }

    cout << "Server is listening on port 8080...\n";

    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        cerr << "Accept failed.\n";
        close(serverSocket);
        return 1;
    }

    char fileName[256];
    recv(clientSocket, fileName, sizeof(fileName), 0);
    cout << "Request received for file: " << fileName << "\n";

    sendFile(clientSocket, fileName);

    close(clientSocket);
    close(serverSocket);
    return 0;
}

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void receiveFile(int serverSocket, const string& fileName) {
    char buffer[1024];
    recv(serverSocket, buffer, sizeof(buffer), 0);
    string response = buffer;

    if (response.find("ERROR") != string::npos) {
        cerr << "Server error: " << response;
        return;
    }

    ofstream file("received_" + fileName, ios::binary);
    while (int bytesRead = recv(serverSocket, buffer, sizeof(buffer), 0)) {
        if (bytesRead < 0) {
            cerr << "Error receiving file.\n";
            break;
        }
        file.write(buffer, bytesRead);
    }

    file.close();
    cout << "File received and saved as received_" << fileName << ".\n";
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        cerr << "Socket creation failed.\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Connection failed.\n";
        close(clientSocket);
        return 1;
    }

    string fileName;
    cout << "Enter the file name to request: ";
    cin >> fileName;

    send(clientSocket, fileName.c_str(), fileName.size() + 1, 0);
    receiveFile(clientSocket, fileName);

    close(clientSocket);
    return 0;
}

design a ds which consist of server and client using threads in c++ short and simple code

#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Client says: " << buffer << endl;

    string response = "Hello from server!";
    send(clientSocket, response.c_str(), response.size(), 0);

    close(clientSocket);
    cout << "Client disconnected.\n";
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Socket creation failed.\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Bind failed.\n";
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed.\n";
        close(serverSocket);
        return 1;
    }

    cout << "Server is listening on port 8080...\n";

    vector<thread> clientThreads;

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            cerr << "Accept failed.\n";
            continue;
        }

        cout << "New client connected.\n";
        clientThreads.emplace_back(handleClient, clientSocket);
    }

    for (auto& t : clientThreads) {
        if (t.joinable()) {
            t.join();
        }
    }

    close(serverSocket);
    return 0;
}

#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        cerr << "Socket creation failed.\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Connection failed.\n";
        close(clientSocket);
        return 1;
    }

    string message = "Hello from client!";
    send(clientSocket, message.c_str(), message.size(), 0);

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Server says: " << buffer << endl;

    close(clientSocket);
    return 0;
}
g++ server.cpp -o server -pthread
g++ client.cpp -o client

4.Write a program to create two processes. First process takes a string and  
passes it to second process through a pipe. The second process 
concatenates the received string with another string without using string 
function and sends it back to the first process for printing.
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

// Function to concatenate two strings manually
void manualConcat(char *str1, char *str2, char *result) {
    int i = 0, j = 0;

    // Copy str1 to result
    while (str1[i] != '\0') {
        result[i] = str1[i];
        i++;
    }

    // Append str2 to result
    while (str2[j] != '\0') {
        result[i] = str2[j];
        i++;
        j++;
    }

    // Null-terminate the concatenated string
    result[i] = '\0';
}

int main() {
    int pipe1[2], pipe2[2];  // Two pipes: one for each direction of communication
    pid_t pid;
    char input[100], concatenatedResult[200];
    char appendStr[] = " - concatenated by Process 2";

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid = fork();  // Create a new process

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process (Process 2)
        close(pipe1[1]);  // Close the write end of pipe1
        close(pipe2[0]);  // Close the read end of pipe2

        char receivedStr[100];
        read(pipe1[0], receivedStr, sizeof(receivedStr));  // Read from pipe1

        // Concatenate the received string with another string
        manualConcat(receivedStr, appendStr, concatenatedResult);

        // Write the concatenated result to pipe2
        write(pipe2[1], concatenatedResult, strlen(concatenatedResult) + 1);

        close(pipe1[0]);  // Close the read end of pipe1
        close(pipe2[1]);  // Close the write end of pipe2
    } else {
        // Parent process (Process 1)
        close(pipe1[0]);  // Close the read end of pipe1
        close(pipe2[1]);  // Close the write end of pipe2

        // Get input from the user
        printf("Enter a string: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  // Remove the newline character

        // Write the input string to pipe1
        write(pipe1[1], input, strlen(input) + 1);

        // Read the concatenated result from pipe2
        read(pipe2[0], concatenatedResult, sizeof(concatenatedResult));

        // Print the concatenated result
        printf("Concatenated string: %s\n", concatenatedResult);

        close(pipe1[1]);  // Close the write end of pipe1
        close(pipe2[0]);  // Close the read end of pipe2
    }

    return 0;
}

Below is the C program for a server that uses shared memory and semaphores to increment a counter. The shared memory is created using mmap.
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int *counter;
    sem_t *sem;

    // Create shared memory
    int shm_fd = shm_open("shared_mem", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Shared memory creation failed");
        return 1;
    }

    // Resize shared memory to store an integer
    if (ftruncate(shm_fd, sizeof(int)) == -1) {
        perror("Shared memory resizing failed");
        return 1;
    }

    // Map shared memory
    counter = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (counter == MAP_FAILED) {
        perror("Shared memory mapping failed");
        return 1;
    }

    // Initialize semaphore
    sem = sem_open("sem_counter", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("Semaphore creation failed");
        return 1;
    }

    // Initialize counter in shared memory
    *counter = 0;

    printf("Server is running. Press Ctrl+C to stop.\n");

    while (1) {
        sem_wait(sem);  // Wait (lock semaphore)
        (*counter)++;
        printf("Server updated counter: %d\n", *counter);
        sem_post(sem);  // Post (unlock semaphore)
        sleep(1);
    }

    // Cleanup
    munmap(counter, sizeof(int));
    close(shm_fd);
    shm_unlink("shared_mem");
    sem_close(sem);
    sem_unlink("sem_counter");

    return 0;
}

3. Determine CPU Load of Node B from Node A

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

float calculateCPULoad() {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return -1;

    long user, nice, system, idle;
    fscanf(fp, "cpu %ld %ld %ld %ld", &user, &nice, &system, &idle);
    fclose(fp);

    static long prevUser = 0, prevNice = 0, prevSystem = 0, prevIdle = 0;
    long total = (user + nice + system + idle) - (prevUser + prevNice + prevSystem + prevIdle);
    long active = (user + nice + system) - (prevUser + prevNice + prevSystem);

    prevUser = user;
    prevNice = nice;
    prevSystem = system;
    prevIdle = idle;

    return (float)active / total * 100;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("CPU Load Server is running on port 8080...\n");

    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        float cpuLoad = calculateCPULoad();
        char response[50];
        snprintf(response, sizeof(response), "CPU Load: %.2f%%", cpuLoad);

        send(clientSocket, response, strlen(response), 0);
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "192.168.1.2", &serverAddr.sin_addr); // Replace with Node B's IP

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection to Node B failed");
        return 1;
    }

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Received from Node B: %s\n", buffer);

    close(clientSocket);
    return 0;
}

teps to Run
Node B:

Save the server code to cpu_load_server.c, compile it, and run:
bash
Copy code
gcc cpu_load_server.c -o cpu_server
./cpu_server
Node A:

Save the client code to cpu_load_client.c, compile it, and run:
bash
Copy code
gcc cpu_load_client.c -o cpu_client
./cpu_client
Make sure to replace "192.168.1.2" in the client program with the actual IP address of Node B. These programs will allow Node A to determine Node B's CPU load.

4.Write a client C program that reads counter value between sem_wait() and 
sem_post(). Access shared memory using open().
Write a client (UDP) C program that calls sendto() to send string to server 
program knowing IP address and port number.

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int *counter;
    sem_t *sem;

    // Open shared memory
    int shm_fd = shm_open("shared_mem", O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Shared memory open failed");
        return 1;
    }

    // Map shared memory
    counter = (int *)mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (counter == MAP_FAILED) {
        perror("Shared memory mapping failed");
        return 1;
    }

    // Open semaphore
    sem = sem_open("sem_counter", 0);
    if (sem == SEM_FAILED) {
        perror("Semaphore open failed");
        return 1;
    }

    printf("Client is accessing the counter...\n");

    while (1) {
        sem_wait(sem);  // Lock semaphore
        printf("Counter value: %d\n", *counter);
        sem_post(sem);  // Unlock semaphore
        sleep(1);  // Wait for a second before next read
    }

    // Cleanup
    munmap(counter, sizeof(int));
    sem_close(sem);

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char message[1024];
    char buffer[1024];

    // Create UDP socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // Replace with actual server IP if necessary

    // Get user input and send to server
    printf("Enter a message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // Remove newline character

    sendto(clientSocket, message, strlen(message), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    printf("Message sent to server.\n");

    // Receive server response
    socklen_t addrLen = sizeof(serverAddr);
    int n = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &addrLen);
    buffer[n] = '\0';
    printf("Server response: %s\n", buffer);

    // Close the socket
    close(clientSocket);
    return 0;
}


ii) Design a Distributed Application using Message Passing Interface (MPI)
for remote computation.

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int compute(int data) {
    return data * data;  // Example computation: square the number
}

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);  // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Master process
        int data[] = {1, 2, 3, 4, 5};  // Example data
        int numData = sizeof(data) / sizeof(data[0]);
        int results[size - 1];

        // Send data to workers
        for (int i = 1; i < size; i++) {
            MPI_Send(&data[i - 1], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Receive results from workers
        for (int i = 1; i < size; i++) {
            MPI_Recv(&results[i - 1], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Print results
        printf("Results from workers:\n");
        for (int i = 0; i < size - 1; i++) {
            printf("Worker %d result: %d\n", i + 1, results[i]);
        }

    } else {
        // Worker process
        int receivedData, result;

        // Receive data from master
        MPI_Recv(&receivedData, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Perform computation
        result = compute(receivedData);

        // Send result back to master
        MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();  // Finalize MPI
    return 0;
}

2. Logical Clock Synchronization Using Lamport's Algorithm

#include <stdio.h>
#define MAX_EVENTS 10

void lamportClock(int events[], int n) {
    int clock[MAX_EVENTS];
    clock[0] = 1;  // Initialize the first event's clock

    // Update the clock for each event
    for (int i = 1; i < n; i++) {
        clock[i] = clock[i - 1] + 1;  // Increment for local events

        if (events[i] != -1) {  // For message receive events
            clock[i] = (clock[i] > events[i] ? clock[i] : events[i]) + 1;
        }
    }

    // Print the clock values
    printf("Logical clock values:\n");
    for (int i = 0; i < n; i++) {
        printf("Event %d: %d\n", i + 1, clock[i]);
    }
}

int main() {
    int events[MAX_EVENTS] = {-1, -1, 1, -1, 3};  // Example events: -1 for local, others for receive
    int n = 5;  // Number of events

    lamportClock(events, n);

    return 0;
}

 Logical Clock Synchronization Using Vector Clocks
#include <stdio.h>
#include <string.h>
#define MAX_PROCESSES 3
#define MAX_EVENTS 10

void printVector(int vec[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");
}

void vectorClock(int events[MAX_PROCESSES][MAX_EVENTS], int n, int m) {
    int clock[MAX_PROCESSES][MAX_EVENTS][MAX_PROCESSES] = {{{0}}};

    // Initialize the vector clock for each process
    for (int p = 0; p < n; p++) {
        for (int e = 0; e < m; e++) {
            if (events[p][e] == 1) {  // Local event
                for (int i = 0; i < n; i++) {
                    clock[p][e][i] = (e > 0 ? clock[p][e - 1][i] : 0);
                }
                clock[p][e][p]++;
            } else if (events[p][e] >= 2) {  // Message receive event
                int sender = events[p][e] - 2;  // Map event value to sender
                for (int i = 0; i < n; i++) {
                    clock[p][e][i] = (e > 0 ? clock[p][e - 1][i] : 0);
                    if (clock[p][e][i] < clock[sender][m - 1][i]) {
                        clock[p][e][i] = clock[sender][m - 1][i];
                    }
                }
                clock[p][e][p]++;
            }
        }
    }

    // Print the vector clock for each process
    for (int p = 0; p < n; p++) {
        printf("Process %d vector clocks:\n", p + 1);
        for (int e = 0; e < m; e++) {
            printf("Event %d: ", e + 1);
            printVector(clock[p][e], n);
        }
    }
}

int main() {
    int events[MAX_PROCESSES][MAX_EVENTS] = {
        {1, 1, 1, 1, -1},
        {1, 2, 1, -1, -1},
        {1, -1, -1, -1, -1}
    };  // Example: 1 for local, 2+ for receive events
    int n = 3;  // Number of processes
    int m = 5;  // Number of events per process

    vectorClock(events, n, m);

    return 0;
}

Save the code to a file (e.g., mpi_remote.c).
Compile:
bash
Copy code
mpicc mpi_remote.c -o mpi_remote
Run:
bash
Copy code
mpirun -np 4 ./mpi_remote
Logical Clock Programs:
Save the code to files (e.g., lamport.c and vector_clock.c).
Compile and run:
bash
Copy code
gcc lamport.c -o lamport
./lamport

gcc vector_clock.c -o vector_clock
./vector_clock
These programs simulate distributed systems with remote computation and logical clock synchronization effectively.


Here are the solutions for the tasks:

1. Simulate Distributed Mutual Exclusion Algorithm (Lamport and Ricart-Agrawala)
Lamport's Algorithm
c
Copy code
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int logical_clock = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *process(void *arg) {
    int pid = *((int *)arg);

    // Request critical section
    pthread_mutex_lock(&mutex);
    logical_clock++;
    printf("Process %d: Requesting critical section at clock %d\n", pid, logical_clock);
    pthread_mutex_unlock(&mutex);

    sleep(1);  // Simulate message exchange delay

    // Enter critical section
    pthread_mutex_lock(&mutex);
    logical_clock++;
    printf("Process %d: Entering critical section at clock %d\n", pid, logical_clock);
    pthread_mutex_unlock(&mutex);

    sleep(1);  // Simulate work in critical section

    // Exit critical section
    pthread_mutex_lock(&mutex);
    logical_clock++;
    printf("Process %d: Exiting critical section at clock %d\n", pid, logical_clock);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    int n = 3;  // Number of processes
    pthread_t threads[n];
    int ids[n];

    for (int i = 0; i < n; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, process, &ids[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
Ricart-Agrawala Algorithm
c
Copy code
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 3  // Number of processes

typedef struct {
    int timestamp;
    int id;
} Request;

int logical_clock = 0;
int request_queue[N];
int granted[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void send_request(int pid) {
    pthread_mutex_lock(&mutex);
    logical_clock++;
    request_queue[pid] = logical_clock;
    printf("Process %d: Sending request at clock %d\n", pid, logical_clock);
    pthread_mutex_unlock(&mutex);
}

void grant_request(int pid) {
    pthread_mutex_lock(&mutex);
    granted[pid] = 1;
    printf("Process %d: Granting request at clock %d\n", pid, logical_clock);
    pthread_mutex_unlock(&mutex);
}

void *process(void *arg) {
    int pid = *((int *)arg);

    send_request(pid);
    sleep(1);  // Simulate communication delay

    grant_request(pid);

    // Enter critical section
    pthread_mutex_lock(&mutex);
    logical_clock++;
    printf("Process %d: Entering critical section at clock %d\n", pid, logical_clock);
    pthread_mutex_unlock(&mutex);

    sleep(1);  // Simulate work in critical section

    // Exit critical section
    pthread_mutex_lock(&mutex);
    logical_clock++;
    printf("Process %d: Exiting critical section at clock %d\n", pid, logical_clock);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, process, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
2. Distributed Application: Server-Client Factorial Calculation
Server Code:
c
Copy code
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int factorial(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    printf("Server is running...\n");

    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            perror("Client connection failed");
            continue;
        }

        int num, result;
        read(clientSocket, &num, sizeof(num));
        result = factorial(num);
        write(clientSocket, &result, sizeof(result));

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
Client Code:
c
Copy code
#include <stdio.h>
#include <arpa/inet.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    int num = 5, result;
    printf("Enter a number: ");
    scanf("%d", &num);

    write(clientSocket, &num, sizeof(num));
    read(clientSocket, &result, sizeof(result));

    printf("Factorial of %d is %d\n", num, result);

    close(clientSocket);
    return 0;
}
3. MapReduce Program to Find User Owning Largest File
c
Copy code
#include <iostream>
#include <filesystem>
#include <map>
#include <sys/stat.h>
#include <pwd.h>

namespace fs = std::filesystem;
using namespace std;

int main() {
    map<string, uintmax_t> userFileSize;

    uintmax_t maxSize = 0;
    string maxOwner;

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_regular_file()) {
            struct stat fileStat;
            stat(entry.path().c_str(), &fileStat);

            string owner = getpwuid(fileStat.st_uid)->pw_name;
            uintmax_t size = fs::file_size(entry);

            userFileSize[owner] += size;
            if (userFileSize[owner] > maxSize) {
                maxSize = userFileSize[owner];
                maxOwner = owner;
            }
        }
    }

    cout << "User owning largest file(s): " << maxOwner << " with total size: " << maxSize << " bytes\n";
    return 0;
}
Steps to Run
Compile the programs:
For mutual exclusion:
bash
Copy code
gcc lamport.c -o lamport -pthread
./lamport
bash
Copy code
gcc ricart_agrawala.c -o ricart_agrawala -pthread
./ricart_agrawala
For server-client: Compile server and client separately:
bash
Copy code
gcc server.c -o server
gcc client.c -o client
Run server first:
bash
Copy code
./server
Then run the client:
bash
Copy code
./client
For MapReduce:
bash
Copy code
g++ mapreduce.cpp -o mapreduce
./mapreduce
Replace the paths and inputs as necessary!




1. Concurrent Echo Client-Server Application
Server Code:
c
Copy code
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void *handle_client(void *client_sock) {
    int clientSocket = *(int *)client_sock;
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(clientSocket, buffer, sizeof(buffer));
        if (n <= 0) break;

        printf("Client: %s\n", buffer);
        write(clientSocket, buffer, n);
    }

    close(clientSocket);
    return NULL;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    printf("Server is listening...\n");

    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        printf("New client connected.\n");

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, &clientSocket);
    }

    close(serverSocket);
    return 0;
}
Client Code:
c
Copy code
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    char buffer[1024];
    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        write(clientSocket, buffer, strlen(buffer));

        memset(buffer, 0, sizeof(buffer));
        read(clientSocket, buffer, sizeof(buffer));
        printf("Server: %s\n", buffer);
    }

    close(clientSocket);
    return 0;
}
2. Client-Server Master Table Update
Server Code:
c
Copy code
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    int id;
    char name[50];
    char address[100];
} ClientInfo;

ClientInfo masterTable[100];
int clientCount = 0;

void update_master_table(int clientSocket) {
    ClientInfo client;
    read(clientSocket, &client, sizeof(ClientInfo));

    masterTable[clientCount++] = client;

    write(clientSocket, masterTable, sizeof(masterTable));
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    printf("Server is running...\n");

    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        printf("New client connected.\n");

        update_master_table(clientSocket);

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
Client Code:
c
Copy code
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

typedef struct {
    int id;
    char name[50];
    char address[100];
} ClientInfo;

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    ClientInfo client = {1, "Client1", "192.168.1.1"};
    write(clientSocket, &client, sizeof(ClientInfo));

    ClientInfo masterTable[100];
    read(clientSocket, masterTable, sizeof(masterTable));

    printf("Updated Master Table:\n");
    for (int i = 0; i < 100 && masterTable[i].id != 0; i++) {
        printf("ID: %d, Name: %s, Address: %s\n", masterTable[i].id, masterTable[i].name, masterTable[i].address);
    }

    close(clientSocket);
    return 0;
}
3. Load Balancer with Two TCP Servers
Load Balancer Code:
c
Copy code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

float getCPULoad(const char *ip) {
    // Simulated CPU load
    return rand() % 100 / 100.0;
}

void forward_request(const char *ip, int port, const char *message, char *response) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);

    connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    write(serverSocket, message, strlen(message));
    read(serverSocket, response, 1024);

    close(serverSocket);
}

int main() {
    const char *server1_ip = "127.0.0.1";
    const char *server2_ip = "127.0.0.2";
    int port = 8081;

    int balancerSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in balancerAddr;

    balancerAddr.sin_family = AF_INET;
    balancerAddr.sin_port = htons(8080);
    balancerAddr.sin_addr.s_addr = INADDR_ANY;

    bind(balancerSocket, (struct sockaddr *)&balancerAddr, sizeof(balancerAddr));
    listen(balancerSocket, 5);

    printf("Load balancer is running...\n");

    while (1) {
        int clientSocket = accept(balancerSocket, NULL, NULL);
        char buffer[1024], response[1024];
        memset(buffer, 0, sizeof(buffer));

        read(clientSocket, buffer, sizeof(buffer));

        // Choose server with lower CPU utilization
        float load1 = getCPULoad(server1_ip);
        float load2 = getCPULoad(server2_ip);

        if (load1 <= load2) {
            forward_request(server1_ip, port, buffer, response);
        } else {
            forward_request(server2_ip, port, buffer, response);
        }

        write(clientSocket, response, strlen(response));
        close(clientSocket);
    }

    close(balancerSocket);
    return 0;
}
String Server Code:
c
Copy code
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>

void to_upper(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    printf("String server is running...\n");

    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        char buffer[1024];
        read(clientSocket, buffer, sizeof(buffer));

        to_upper(buffer);
        write(clientSocket, buffer, strlen(buffer));

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
Steps to Run:
Concurrent Echo:
Run the server, then multiple clients.
Master Table Update:
Start the server, then run multiple clients to update the table.
Load Balancer:
Start the two servers and the load balancer.
Run clients to send requests to the load balancer.


*/
