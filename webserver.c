//starting code was taken from https://gist.github.com/nir9/2db83a89c18a8add1e705710e9303208
// modified to keep server running, accepts address line http://localhost:8080/c=% , where % is a char
// responses with: 
// Current value: %
#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>

int main()
{
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		printf("socket failed: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = 0;
	addr.sin_port = htons(8080);
	bind(s, (struct sockaddr*)&addr, sizeof(addr));

	listen(s, 10);

	char counter = '0'; // Initialize the counter

	while(1) { // Keep the server running
		SOCKET client = accept(s, 0, 0);

		char request[256] = {0};
		recv(client, request, 256, 0);

		// GET /[file] ...
		if (memcmp(request, "GET /c=", 7) == 0) {
			counter = request[7]; // set the counter
		}

		char buffer[256] = {0};
		sprintf(buffer, "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\nCurrent value: %c", counter); // Prepare the response
		send(client, buffer, strlen(buffer), 0); // Send the response

		closesocket(client); // Close the client socket after handling the request
	}
}

