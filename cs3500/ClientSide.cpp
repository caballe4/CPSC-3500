#include "ClientSide.h"

using namespace std;

ClientSide::ClientSide()
{
	memset(&hints, 0, sizeof(hints));
	memset(&servinfo, 0, sizeof(servinfo));
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	servinfo->ai_family = AF_UNSPEC;
	servinfo->ai_socktype = SOCK_STREAM;
}

int ClientSide::search(string name, string num)
{
	int rv;
	if((rv = getaddrinfo(name.c_str(), num.c_str(), &hints, &servinfo)) == -1){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}
	cout << "Connecting to server" << endl;
	return 0;
}

int ClientSide::give_command()
{
	for(p = servinfo; p != NULL; p= p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("client: socket");
            continue;
		}
		if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			perror("client: connect");
			continue;
		}
		break;
	}
	if(p == NULL){
		fprintf(stderr, "client: failed to connect\n");
        return -1;
	}
	
	freeaddrinfo(servinfo);
	freeaddrinfo(p);
	return 0;
}

int ClientSide::finish()
{
	close(sockfd);
	return 0;
}

int ClientSide::write_sock(void *ptr, int length)
{
	int count = 0;
	int num;
	while(count < length){
		if((num = write(sockfd, p, length-count)) == -1){
			perror("client: write");
			close(sockfd);
		}
		ptr += num;
		count += num;
	}
	return count;
}

string ClientSide::read_sock(int length)
{
	int size = 0;
	buf = new char[length];
	char* ptr = (char*)buf;
	size_t num = length;
	size_t read_length;
	while(num > 0){
		if((read_length = read(sockfd, ptr, num)) == -1){
			perror("client: read");
			return "-1";
		}
		num -= read_length;
		ptr += read_length;
		length += read_length;
	}
	command = buf;
	return command; 
}
	