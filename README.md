# Diablo
This is a basic implementation of a C2 (Command-and-Control) connecting to a AM0N-Eye client that connects to a remote server specified by the IP address and port number provided as command-line arguments. Once connected, the client receives a payload of executable code from the server and executes it.

The payload is received into a memory-mapped buffer that has read, write, and execute permissions. This buffer is allocated using the mmap system call. Once the payload is received, the socket is closed, and the buffer is executed as a function using a function pointer cast.

This script can be used as a starting point to build more complex C2 clients that can perform various tasks on a compromised system, such as privilege escalation or data exfiltration.

Build: gcc Diablo.c -o Diablo

Use : ./command_control ip_address port

![236619099-93f0310c-504b-4117-b0e9-206fb72baf4c](https://github.com/S3N4T0R-0X0/Diablo/assets/121706460/1e128a11-c670-41a1-81ff-0b3fa97da742)


