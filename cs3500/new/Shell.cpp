// CPSC 3500: Shell
// Implements a basic shell (command line interface) for the file system

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <string.h> 

using namespace std;

#include "Shell.h"
#define MAXDATASIZE 100
#define PORTNUM "11001"


static const string PROMPT_STRING = "NFS> ";	// shell prompt

// get sockaddr, IPv4 or IPv6:
void *get_in_addr2(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Mount the network file system with server name and port number in the format of server:port
void Shell::mountNFS(string fs_loc) {
	//create the socket cs_sock and connect it to the server and port specified in fs_loc
	//if all the above operations are completed successfully, set is_mounted to true  \
	
	int sockfd, rv, numbytes,new_fd;
	struct addrinfo hints, *servinfo, *p;
	//TODO: no hardcode
	char ip[12];
	char port[5];
	char s[INET6_ADDRSTRLEN];  
    char buf[MAXDATASIZE];

  sockfd = cs_sock;
	
	/*
	for(int i = 0; i < 12; i++) {
		ip[i] = fs_loc[i];
		cout << ip[i];
	}
	cout << endl;
	
	for(int i = 13; i < 18; i++) {
		port[i] = fs_loc[i];
		cout << port[i];
	}
	
	cout << endl;
	*/

  if(is_mounted){
    cerr << "Files System Mounted" << endl;
    return;
  }
	
	memset(&hints, 0, sizeof(hints));
  //memset(&servinfo, 0, sizeof(servinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_port = htonl(PORT);
	
	if((rv = getaddrinfo(fs_loc.c_str(), PORTNUM, &hints, &servinfo)) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}
	
	//cout << hints.ai_addrlen << endl;
	//cout << hints.ai_canonname << endl;
	cout<< "i didn;t exit\n";
	for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            //close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return;
    }
	//inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    inet_ntop(p->ai_family, get_in_addr2((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

	if(send(new_fd, "Hello, world!", 13, 0) == -1)
		perror("send");
  buf[numbytes] = '\0';

    printf("client: received '%s'\n", buf);

    close(new_fd);

    is_mounted = true;

    return;
}
	
	


// Unmount the network file system if it was mounted
void Shell::unmountNFS() {
	// close the socket if it was mounted

  if(is_mounted){
    if(close(cs_sock)){
      perror("Couldn't close socket \n");
      exit(1);
    }
  }
  is_mounted = false;
}

// Remote procedure call on mkdir
void Shell::mkdir_rpc(string dname) {
  // to implement
  int num = 0;
  string name = "mkdir" + dname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) == -1){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procedure call on cd
void Shell::cd_rpc(string dname) {
  // to implement
  int num = 0;
  string name = "cd" + dname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) < 0){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;


}

// Remote procedure call on home
void Shell::home_rpc() {
  // to implement

  int num = 0;
  string name = "home\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) == -1){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procedure call on rmdir
void Shell::rmdir_rpc(string dname) {
  // to implement
  int num = 0;
  string name = "rmdir" + dname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) == -1){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procedure call on ls
void Shell::ls_rpc() {
  // to implement

  int num = 0;
  string name = "ls\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) == -1){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procedure call on create
void Shell::create_rpc(string fname) {
  // to implement

  int num = 0;
  string name = string("create") + fname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) == -1){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procedure call on append
void Shell::append_rpc(string fname, string data) {
  // to implement

  int num = 0;
  string name = "append" + fname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) < 0){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procesure call on cat
void Shell::cat_rpc(string fname) {
  // to implement
  int num = 0;
  string name = "cat" + fname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) == -1){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procedure call on head
void Shell::head_rpc(string fname, int n) {
  // to implement

  int num = 0;
  string name = "head" + fname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) < 0){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procedure call on rm
void Shell::rm_rpc(string fname) {
  // to implement

  int num = 0;
  string name = "rm" + fname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) == -1){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Remote procedure call on stat
void Shell::stat_rpc(string fname) {
  // to implement

  int num = 0;
  string name = "stat" + fname + "\r\n";

  name.resize(MAXDATASIZE, '0');
  int size = strlen(name.c_str());

  if(cs_sock != -1){
    if((num = send(cs_sock, name.c_str(), size, 0)) < 0){
      cout << "Unabe to send " << name << endl;
      exit(1);
    }
  }else
    exit(1);

    cout << "Success" << endl;
    return;
}

// Executes the shell until the user quits.
void Shell::run()
{
  // make sure that the file system is mounted
  if (!is_mounted)
 	return; 
  
  // continue until the user quits
  bool user_quit = false;
  while (!user_quit) {

    // print prompt and get command line
    string command_str;
    cout << PROMPT_STRING;
    getline(cin, command_str);

    // execute the command
    user_quit = execute_command(command_str);
  }

  // unmount the file system
  unmountNFS();
}

// Execute a script.
void Shell::run_script(char *file_name)
{
  // make sure that the file system is mounted
  if (!is_mounted)
  	return;
  // open script file
  ifstream infile;
  infile.open(file_name);
  if (infile.fail()) {
    cerr << "Could not open script file" << endl;
    return;
  }


  // execute each line in the script
  bool user_quit = false;
  string command_str;
  getline(infile, command_str, '\n');
  while (!infile.eof() && !user_quit) {
    cout << PROMPT_STRING << command_str << endl;
    user_quit = execute_command(command_str);
    getline(infile, command_str);
  }

  // clean up
  unmountNFS();
  infile.close();
}


// Executes the command. Returns true for quit and false otherwise.
bool Shell::execute_command(string command_str)
{
  // parse the command line
  struct Command command = parse_command(command_str);

  // look for the matching command
  if (command.name == "") {
    return false;
  }
  else if (command.name == "mkdir") {
    mkdir_rpc(command.file_name);
  }
  else if (command.name == "cd") {
    cd_rpc(command.file_name);
  }
  else if (command.name == "home") {
    home_rpc();
  }
  else if (command.name == "rmdir") {
    rmdir_rpc(command.file_name);
  }
  else if (command.name == "ls") {
    ls_rpc();
  }
  else if (command.name == "create") {
    create_rpc(command.file_name);
  }
  else if (command.name == "append") {
    append_rpc(command.file_name, command.append_data);
  }
  else if (command.name == "cat") {
    cat_rpc(command.file_name);
  }
  else if (command.name == "head") {
    errno = 0;
    unsigned long n = strtoul(command.append_data.c_str(), NULL, 0);
    if (0 == errno) {
      head_rpc(command.file_name, n);
    } else {
      cerr << "Invalid command line: " << command.append_data;
      cerr << " is not a valid number of bytes" << endl;
      return false;
    }
  }
  else if (command.name == "rm") {
    rm_rpc(command.file_name);
  }
  else if (command.name == "stat") {
    stat_rpc(command.file_name);
  }
  else if (command.name == "quit") {
    return true;
  }

  return false;
}

// Parses a command line into a command struct. Returned name is blank
// for invalid command lines.
Shell::Command Shell::parse_command(string command_str)
{
  // empty command struct returned for errors
  struct Command empty = {"", "", ""};

  // grab each of the tokens (if they exist)
  struct Command command;
  istringstream ss(command_str);
  int num_tokens = 0;
  if (ss >> command.name) {
    num_tokens++;
    if (ss >> command.file_name) {
      num_tokens++;
      if (ss >> command.append_data) {
        num_tokens++;
        string junk;
        if (ss >> junk) {
          num_tokens++;
        }
      }
    }
  }

  // Check for empty command line
  if (num_tokens == 0) {
    return empty;
  }
    
  // Check for invalid command lines
  if (command.name == "ls" ||
      command.name == "home" ||
      command.name == "quit")
  {
    if (num_tokens != 1) {
      cerr << "Invalid command line: " << command.name;
      cerr << " has improper number of arguments" << endl;
      return empty;
    }
  }
  else if (command.name == "mkdir" ||
      command.name == "cd"    ||
      command.name == "rmdir" ||
      command.name == "create"||
      command.name == "cat"   ||
      command.name == "rm"    ||
      command.name == "stat")
  {
    if (num_tokens != 2) {
      cerr << "Invalid command line: " << command.name;
      cerr << " has improper number of arguments" << endl;
      return empty;
    }
  }
  else if (command.name == "append" || command.name == "head")
  {
    if (num_tokens != 3) {
      cerr << "Invalid command line: " << command.name;
      cerr << " has improper number of arguments" << endl;
      return empty;
    }
  }
  else {
    cerr << "Invalid command line: " << command.name;
    cerr << " is not a command" << endl; 
    return empty;
  } 

  return command;
}

