#include <chrono>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#define h_addr h_addr_list[0]

void write_on_csv_file(const std::string &file_name, int channel, char *voltage,
                       char *current) {
    std::ofstream file(file_name, std::ios_base::app);
    if (file.is_open()) {
        file << channel << ',' << voltage << ',' << current << '\n';
    }
    file.close();
}

int main(int argc, char *argv[]) {
    auto a = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(a);
    std::string time_str = std::ctime(&t);
    time_str = time_str.substr(0, time_str.size() - 1);

    std::string file_name = time_str + ".csv";

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    portno = atoi("5025");

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname("192.168.10.10");

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    char buffer2[256];

    // TODO:
    for (;;) {
        bzero(buffer, 256);
        strcpy(buffer, "INST OUT1\n");
        n = write(sockfd, buffer, strlen(buffer));

        int channel = 1;

        bzero(buffer, 256);
        strcpy(buffer, "MEAS:CURR?\n");
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
        // sleep(1);

        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        // sleep(1);

        bzero(buffer2, 256);
        strcpy(buffer2, "MEAS:VOLT?\n");
        n = write(sockfd, buffer2, strlen(buffer2));
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }

        // sleep(1);

        bzero(buffer2, 256);
        n = read(sockfd, buffer2, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        // erasing new line characters present in the power supply response
        for (int i = 0; i < sizeof(buffer); i++) {
            if (buffer[i] == '\n') {
                buffer[i] = '\0';
            }
        }
        for (int i = 0; i < sizeof(buffer); i++) {
            if (buffer2[i] == '\n') {
                buffer2[i] = '\0';
            }
        }

        // std::cout << buffer << ',' << buffer2 << std::endl;
        write_on_csv_file(file_name, channel, buffer2, buffer);

        bzero(buffer, 256);
        strcpy(buffer, "INST OUT2\n");
        n = write(sockfd, buffer, strlen(buffer));

        channel = 2;

        bzero(buffer, 256);
        strcpy(buffer, "MEAS:CURR?\n");
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
        // sleep(1);

        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        // sleep(1);

        bzero(buffer2, 256);
        strcpy(buffer2, "MEAS:VOLT?\n");
        n = write(sockfd, buffer2, strlen(buffer2));
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }

        // sleep(1);

        bzero(buffer2, 256);
        n = read(sockfd, buffer2, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        // erasing new line characters present in the power supply response
        for (int i = 0; i < sizeof(buffer); i++) {
            if (buffer[i] == '\n') {
                buffer[i] = '\0';
            }
        }
        for (int i = 0; i < sizeof(buffer); i++) {
            if (buffer2[i] == '\n') {
                buffer2[i] = '\0';
            }
        }

        // std::cout << buffer << ',' << buffer2 << std::endl;
        write_on_csv_file(file_name, channel, buffer2, buffer);

        usleep(1000);
    }

    return 0;
}