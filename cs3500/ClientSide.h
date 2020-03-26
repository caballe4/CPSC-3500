#include <iostream>
#include <signal.h>
#include <string>
#include <strings.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <cstring>

#ifndef CLIENTSIDE_H
#define CLIENTSIDE_H

using namespace std;

class ClientSide
{
	private: 
	
	public:
		ClientSide();
		
		struct addrinfo hints, *servinfo, *p;
		char* buf;
		int port_num;
		int sockfd;
		string command;
		
		int search(string name, string num);
		int give_command();
		string read_sock(int length);
		int write_sock(void* p, int length);
		int finish();
};
#endif