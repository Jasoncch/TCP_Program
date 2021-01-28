#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{

	string IP_Address;			
	int port = 12345;			

	cout << "Input the IP address: " << endl;
	cin >> IP_Address;
	cout << "IP Address: " << IP_Address << "        " << "Port Number: " << port << endl;


	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	// Hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, IP_Address.c_str(), &hint.sin_addr);

	// Connect to the server
	cout << "Connect status:";
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cout << "fail"<<endl;

		cout << "Please check your connection" << endl;
		WSACleanup();
		return 1;
	}
	else
		cout << "success" << endl;


	// Do-while loop to send and receive data
	char buf[1024];
	string input="temp";

	while (input != "exit"){
		cout << "Input the file name to be requested from the server:"<<endl;
		cin >> input;

		int sendResult = send(sock, input.c_str(), input.size() + 1, 0);
		cout << "Send status: ";
		if (sendResult == SOCKET_ERROR) {
			cout << "fail" << endl;
		}
		else
		{
			cout << "success" << endl;
			//open file for writing
			
			if (input != "exit") {
				
				ZeroMemory(buf, 1024);
				//get the first line
				int bytesReceived = recv(sock, buf, 1024, 0);
				if (string(buf, 0, bytesReceived) != "File")
				{
					ofstream outfile;
					outfile.open(input);
					cout << "Open file status: ";
					if (outfile.is_open())
						cout << "success" << endl;
					else {
						cout << "fail" << endl;
						cout << "Cannot open the file, please check your storage space" << endl;
						return 1;
					}
					cout << "Received text: " << string(buf, 0, bytesReceived) << endl;
					outfile.close();
					cout << "Save text status: ";
					//check if the file is closed
					if (outfile.is_open()) {
						cout << "fail";
						cout << "Cannot save the text in file, please try again" << endl;
					}
					else
						cout << "success" << endl;
					cout << "Receive File " << input << " From the Server Successful!" << endl;
				}
				else cout << "Cannot find the requested file, please try again" << endl;
			}
		}
	} 
	closesocket(sock);
	WSACleanup();
	return 0;
}




















