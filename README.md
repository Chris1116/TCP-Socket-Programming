# TCP-Socket-Programming

## Project Specifications

### Server:
1. **DNS**: Convert the URLs sent by the client into IP addresses and return them to the client.
2. **QUERY**: Use file reading to query the `query.txt` file with the student ID sent by the client. Return the email associated with this student ID to the client. If the student ID is not found, return "No such student ID".
3. **QUIT**: After the client disconnects, continue waiting for the next service without disconnecting.

### Client:
The client should be able to connect to the server, send requests to the server, and receive data returned from the server.
