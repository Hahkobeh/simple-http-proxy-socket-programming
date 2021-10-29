//
// CODE WRITTEN BY JACOB ARTUSO, SOME PARTS HAVE BEEN COPIED AND MODIFIED FROM CODE GIVEN BY SINA KESHVADI
//

#include <algorithm>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <netdb.h>
#include <vector>

#include <csignal>
#define PORT "1235"
#define HTTPPORT "80"
#define BUFFERSIZE 2048



using namespace std;

void handle_client(vector<string>&);
bool handle_client_main_only(vector<string>&);
bool url_contains_bad_words(string,vector<string>&);
void close_sockets(int);
//all sockets
int server_socket, new_socket, server_client_socket, bad_word_socket, config_socket;


int main(int argc, char *argv []) {

    //create vector<string> of bad words from args
    vector<string> bad_words;

    bad_words.resize(argc - 1);

    for(int j = 0; j < bad_words.size(); j++){
        string temp = argv[ j + 1 ];
        bad_words.at(j) = temp;
    }



    //create server and initialize server info, bind, then listen
    addrinfo server, *server_info; // server
    int opt = 1;
    char buffer[BUFFERSIZE] = {0};

    signal(SIGINT,close_sockets);
    signal(SIGTERM,close_sockets);


    memset(&server, 0, sizeof(addrinfo));

    server.ai_family = AF_INET;
    server.ai_socktype = SOCK_STREAM;
    server.ai_flags = AI_PASSIVE;
    int addrinfo = getaddrinfo(NULL, PORT, &server, &server_info);
    if(addrinfo < 0){
        perror("getaddrinfo failed");
        exit(EXIT_FAILURE);
    }


    server_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (server_socket < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    int setsock_server = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if (setsock_server < 0) {
        perror("setsock failed");
        exit(EXIT_FAILURE);
    }


    int bind_server = bind(server_socket, server_info->ai_addr, server_info->ai_addrlen);
    if (bind_server < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int listen_server = listen(server_socket, 5);
    if (listen_server < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    //Configure blocked words using telnet
    if(bad_words.size() == 0) {
            config_socket = accept(server_socket, server_info->ai_addr, &server_info->ai_addrlen);
            if (new_socket < 0) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            if (handle_client_main_only(bad_words)) {
                close(config_socket);
            }

    }else{
        if (bad_words.at(bad_words.size() - 1) != "CONTINUE") {
            config_socket = accept(server_socket, server_info->ai_addr, &server_info->ai_addrlen);
            if (new_socket < 0) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            if (handle_client_main_only(bad_words)) {
                close(config_socket);
            }

        }
    }



    //listen and accept any connections
    while(true){


        new_socket = accept(server_socket, server_info->ai_addr, &server_info->ai_addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }


        //fork so multiple processes can occur, children deal with connecting

        pid_t pid = fork();
        if(pid < 0 ){
            perror("failed to create child");
        }
        if(pid == 0) {
            //child handles reading
            handle_client(bad_words);


        }else {
            //this is parent, it acts as the listener
            close(new_socket);

        }

    }


    close(server_socket);
    return 0;
}


//read from client, test for bad words, send to internet, receive from internet, send to client
void handle_client(vector<string> &bad_words){
    char buffer[BUFFERSIZE];
    char buffer2[BUFFERSIZE * 8];
    memset(buffer,0,sizeof(buffer));
    memset(buffer2,0,sizeof(buffer2));
    while((recv(new_socket,buffer,sizeof(buffer),0)) > 0) {
        char request[BUFFERSIZE];
        memcpy(request,buffer,sizeof(request));


        addrinfo server_client, *server_client_info; // server
        //int opt = 1;
        char host[BUFFERSIZE], url[BUFFERSIZE];

        memset(host,0,sizeof(host));
        memset(url,0,sizeof(url));


        memset(&server_client, 0, sizeof(addrinfo));


        char *read_url = strtok(buffer, "\r\n");
        char *read_host = strtok(NULL, "\r\n");

        sscanf(read_url, "GET http://%s", url);
        sscanf(read_host, "Host: %s", host);


        //cout << host<< endl<< url<<endl;
        server_client.ai_family = AF_INET;
        server_client.ai_socktype = SOCK_STREAM;
        int addrinfo = getaddrinfo(host, HTTPPORT, &server_client, &server_client_info);
        if (addrinfo < 0) {
            perror("getaddrinfo failed");
            exit(EXIT_FAILURE);
        }

        //COPIED FROM 01_host_to_ip.cpp to show ip for debugging

        if (server_client_info->ai_family == AF_INET){
            //cout << "IPv4" << endl;
        }

        sockaddr_in *address = (sockaddr_in *)server_client_info->ai_addr;
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(server_client_info->ai_family, &(address->sin_addr), ip_str, INET_ADDRSTRLEN);
        //cout << "IP Address: " << ip_str << endl;


        server_client_socket = socket(server_client_info->ai_family, server_client_info->ai_socktype, server_client_info->ai_protocol);
        if(server_client_socket < 0){
            perror("Client socket creation failed");
            exit(EXIT_FAILURE);
        }
        int server_client_connect = connect(server_client_socket,server_client_info->ai_addr,server_client_info->ai_addrlen);
        if(server_client_connect < 0){
            perror("Client connect failed");
            exit(EXIT_FAILURE);
        }



        string request_string = url;

        //check for bad words here
        if(url_contains_bad_words(request_string,bad_words)){
            cout << "bad words";
            char redirect1[] = "GET http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html HTTP/1.1\r\nHost: pages.cpsc.ucalgary.ca\r\nConnection: close\r\n\r\n";

            int server_client_send = send(server_client_socket,redirect1,sizeof(redirect1),0);
            if(server_client_send < 0){
                perror("Client send failed");
                exit(EXIT_FAILURE);
            }

            int bytes_received;
            while ((bytes_received = recv(server_client_socket, buffer2, BUFFERSIZE, 0)) > 0) {
                if(bytes_received < 0){
                    perror("Client send failed");
                    exit(EXIT_FAILURE);
                }




                send(new_socket, buffer2, bytes_received, 0);
                memset(buffer2, 0, sizeof(buffer2));


            }


            //if none found go here
        }else {
            int server_client_send = send(server_client_socket,request,sizeof(request),0);
            if(server_client_send < 0){
                perror("Client send failed");
                exit(EXIT_FAILURE);
            }

            int bytes_received;
            while ((bytes_received = recv(server_client_socket, buffer2, BUFFERSIZE, 0)) > 0) {
                if(bytes_received < 0){
                    perror("Client send failed");
                    exit(EXIT_FAILURE);
                }

                send(new_socket, buffer2, bytes_received, 0);
                memset(buffer2, 0, sizeof(buffer2));


            }

        }
        close(addrinfo);
    }

}
//set all blocked words
bool handle_client_main_only(vector<string> &bad_words){
    char buffer[BUFFERSIZE];
    memset(buffer,0,sizeof(buffer));
    while((recv(config_socket,buffer,sizeof(buffer),0))> 0) {
        if (strncmp(buffer, "BLOCK", 5) == 0) {
            char block_word[BUFFERSIZE];
            sscanf(buffer, "BLOCK %s\n", block_word);
            string temp = block_word;
            bad_words.push_back(temp);
            send(config_socket, bad_words.at(bad_words.size() - 1).c_str(), temp.size(), 0);
            send(config_socket, " has been added!\n", 17, 0);
            memset(buffer,0,sizeof(buffer));
            continue;
        }
        if (strncmp(buffer, "REMOVE", 6) == 0) {
            char block_word[BUFFERSIZE];
            sscanf(buffer, "REMOVE %s\n", block_word);
            string temp = block_word;
            bad_words.erase(remove(bad_words.begin(),bad_words.end(), temp),bad_words.end());
            send(config_socket, temp.c_str(), temp.size(), 0);
            send(config_socket, " has been removed from the list!\n", 33, 0);
            memset(buffer,0,sizeof(buffer));
            continue;
        }

        if (strncmp(buffer, "CONTINUE", 8) == 0) {
            send(config_socket, "Continuing to proxy\n", 20, 0);
            return true;
        }
    }
    return false;
}
//test if url contains blocked words
bool url_contains_bad_words(string request_string,vector<string> &bad_words){
    /*for(string temp : bad_words) {
        cout << temp.c_str() << " "<< endl;
    }*/

    for(int i = 0; i < bad_words.size(); i++) {

        if (request_string.find(bad_words.at(i)) != string::npos) {
            return true;
        }
    }
    return false;
}
//close sockets
void close_sockets(int signal){
    cout << "CLOSED";
    close(bad_word_socket);
    close(server_socket);
    close(new_socket);
    close(server_client_socket);
    exit(signal);
}