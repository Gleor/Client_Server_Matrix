// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>

#define MATRIX_SIZE 6
#define TIMER 1

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void fillMatrix(double matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    srand(time(NULL));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = (double)rand()/RAND_MAX*2.0 - 1.0; // Random double between -1 and 1
        }
    }
}

void printMatrix(double matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int sockfd, portNum, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    double matrix[MATRIX_SIZE][MATRIX_SIZE];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portNum = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portNum);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    char stop[5] = "Stop";
    char buf[10];
    // Main loop
    while(1) {
        // Generate matrix
        fillMatrix(matrix);
        printMatrix(matrix);

        // Send matrix to server
        n = write(sockfd,matrix,sizeof(matrix));
        if (n < 0) 
             error("ERROR writing to socket");
        
        scanf("%s", buf);

        if (strcmp(buf, stop) == 0) {
            printf("aborting\n");
            break;
        }
        sleep(TIMER); // Wait
    }

    close(sockfd);
    return 0;
}
