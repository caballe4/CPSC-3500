#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <string.h>
#include "FileSys.h"


using namespace std;

#define PORT "11001"
#define BACKLOG 10
#define MAXDATASIZE 100

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char* argv[]) {
	
	/*
	int numbytes;
	if (argc < 2) {
		cout << "Usage: ./nfsserver port#\n";
        return -1;
    }
    int port = atoi(argv[1]);

    //networking part: create the socket and accept the client connection
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;// connector's address information
	struct sockaddr_in serv_address;
    socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];

    // struct sigaction sa;
    int yes=1;
	int rv;
	char buf[MAXDATASIZE];
	
	memset(&hints, 0, sizeof hints);
	//memset(&serv_address, '0', sizeof(serv_address));
	
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
	//serv_address.sin_port = htons(PORT);
	
	
	char quit = 'y';
	char response = 'n';
	
	
	
	
	//while(tolower(response) != quit){
		if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			return 1;
		}
		else {
			cout << "PASSED 1\n";
		}
		
		// loop through all the results and bind to the first we can
		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
				perror("server: socket");
				continue;
			} 
			else {
				cout << "PASSED 2\n";
			}

			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
					sizeof(int)) == -1) {
				perror("setsockopt");
				exit(1);
			}
			else {
				cout << "PASSED 3\n";
			}
			break;
		}
		
		freeaddrinfo(servinfo); // all done with this structure

		if (p == NULL)  {
			fprintf(stderr, "server: failed to bind\n");
			exit(1);
		}

		if (listen(sockfd, BACKLOG) == -1) {
			perror("listen");
			exit(1);
		}
		
		printf("server: waiting for connections...\n");
		
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			//continue;
		}
		
		if (bind(new_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			//continue;
		}
		else 
			cout << "PASSED 4\n";

		while(tolower(response) != quit) {  // main accept() loop
			sin_size = sizeof their_addr;

			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
			printf("server: got connection from %s\n", s);

			if (!fork()) { // this is the child process
				close(sockfd); // child doesn't need the listener
				if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
					perror("recv");
					exit(1);
				}
			}
			int count = sin_size;
			char *ptr = (char*)buf;
			int num = 0;

			while(count > 0){
				num = read(new_fd, ptr + count, sizeof(buf) - count);

				if(num < 0)
					perror("read");
				if( num == 0)
					close(sockfd);
				ptr += count;
				num += count;
			}
			if(count == sizeof(buf))
				sin_size= ntohl(sin_size);
			cout << "Do you want to quit:(y/n): ";
			cin >> response;
			close(new_fd);  // parent doesn't need this
		}

	//}
	*/
	
    //int sock; //change this line when necessary!
	
	
	// TODO: 
	//		 * rmdir only truly works if the directory removed is last in the curent directory
	//		 * Test and debug the cd'ing a nonexisting directory issue
	//		 * Write function that displays current directory name after each command.
	//			or is this something user should know?
	//		 * free memoryused in some functions
	//		 * try and find a way to clear memory without just deleting DISK
	// 		 * append doesnt' like edge cases; fix
	
	//IMPORTANT TODO: * ONCE DONE: MAKE SURE TO DO EXTENSIVE TESTING: THIS WILL BE A BITCH 
	FileSys fs;
    fs.mount(5);
	
    // mount the file system
     //assume that sock is the new socket created 
                    //for a TCP connection between the client and the server.   
	
	/*
	cout << "--------------------- TEST ONE -----------------------\n" << endl;
	// create a directory
	fs.mkdir("hello_dir");
	fs.mkdir("num_two");
	fs.mkdir("remove_me");
	
	// test ls
	fs.ls();
	cout << endl;
	
	// test rmdir
	fs.rmdir("remove_me");
	fs.ls();
	cout << endl;
	
	cout << "\n--------------------- TEST TWO -----------------------\n" << endl;
	// test cd -- one layer
	fs.cd("booger");
	cout << endl;
	fs.cd("hello_dir");
	cout << endl;
	
	cout << "\n--------------------- TEST THREE -----------------------\n" << endl;
	// mkdir and cd an ls in multiple layers
	fs.cd("level_two");
	fs.mkdir("level_two");
	fs.cd("level_two");
	cout << endl;
	
	fs.ls();
	cout << endl;
	
	fs.home();
	cout << endl;
	
	fs.mkdir("im_back");
	fs.ls();
	cout << endl;
	
	fs.cd("hello_dir");
	fs.cd("nuts");
	fs.mkdir("nuts");
	fs.cd("nuts");
	cout << endl;
	
	fs.ls();
	cout << endl;
	fs.cd("level_two");
	fs.ls();
	cout << endl;
	
	cout << "\n--------------------- TEST FOUR -----------------------\n" << endl;
	
	// for simplicity; go to home directory
	fs.home();
	fs.ls();
	cout << endl;
	
	// create file
	fs.create("im_a_file");
	fs.ls();
	cout << endl;
	
	// simple tests
	fs.cd("im_a_file");
	cout << endl;
	fs.ls();
	cout << "\n--------------------- TEST FIVE -----------------------\n" << endl;	
	
	// append data to im_a_file
	fs.append("im_a_file", "0123456789aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	fs.cat("im_a_file");
	cout << endl;

	cout << "\n--------------------- TEST SIX -----------------------\n" << endl;	
	
	fs.head("im_a_file", 100);
	cout << endl;
	fs.head("im_a_file", 200);
	cout << endl;	
	
	cout << "\n--------------------- TEST SEVEN -----------------------\n" << endl;	
	
	fs.ls();
	cout << endl;
	fs.stat("num_two");
	fs.stat("im_a_file");
	fs.create("new_file");
	fs.stat("new_file");

	for (int i = 0; i < 25; i++)
		fs.append("new_file", "0123456789");
	
	fs.cat("new_file");
	fs.stat("new_file");
	
	cout << "\n--------------------- TEST SEVEN -----------------------\n" << endl;	
	cout << endl;
	
	fs.ls();
	cout << endl;
	fs.rm("im_a_file");
	cout << endl;
	fs.rm("new_file");
	

	fs.mkdir("HJKDS");
	fs.ls();
	cout << endl;
	fs.rmdir("num_two");
	fs.ls();
	*/
	
	// his testing
    //loop: get the command from the client and invoke the file
    //system operation which returns the results or error messages back to the clinet
    //until the client closes the TCP connection.
	/*
	fs.ls();
	fs.mkdir("dir1");
	fs.mkdir("dir2");
	fs.ls();
	fs.cd("dir1");
	fs.create("file1");
	fs.append("file1", "hellowworld!");
	fs.stat("file1");
	fs.ls();
	fs.cat("file1");
	fs.head("file1", 5);
	fs.rm("file2");
	fs.cat("file2");
	fs.create("file1");
	fs.create("file2");
	fs.rm("file1");
	fs.ls();
	fs.home();
	fs.ls();
	cout << "STAT" << endl;
	fs.stat("dir1");
	*/
    //close the listening socket
	
	fs.create("file");
	fs.ls();
	fs.append("file", "please");
	fs.append("file", " NUTS");
	fs.cat("file");
	cout << endl;
	cout << "HEAD" << endl;
	fs.head("file", 4);
	fs.stat("file");
    //unmout the file system
	system("rm DISK");
    fs.unmount();

    return 0;
}
