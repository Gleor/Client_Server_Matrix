// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>

#define N 6
#define HISTORY_SIZE 5

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void subMatrix(double mat[N][N], double temp[N][N], double p, double q, double n) {
   int i = 0, j = 0;
   // filling the sub matrix
   for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
         // skipping if the current row or column is not equal to the current
         // element row and column
         if (row != p && col != q) {
            temp[i][j++] = mat[row][col];
            if (j == n - 1) {
               j = 0;
               i++;
            }
         }
      }
   }
}

double determinantOfMatrix(double matrix[N][N], double n) {
   double determinant = 0.0;
   if (n == 1) {
      return matrix[0][0];
   }
   if (n == 2) {
      return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
   }
   double temp[N][N], sign = 1;
   for (int i = 0; i < n; i++) {
      subMatrix(matrix, temp, 0, i, n);
      determinant += sign * matrix[0][i] * determinantOfMatrix(temp, n - 1);
      sign = -sign;
   }
   return determinant;
}

double calculateAverage(double history[HISTORY_SIZE]) {
    double sum = 0;
    for (int i = 0; i < HISTORY_SIZE; i++) {
        sum += history[i];
    }
    return sum / (double)HISTORY_SIZE;
}

int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portNum;
     socklen_t clilen;
     double matrix[N][N];
     double history[HISTORY_SIZE] = { 0 };
     int historyIndex = 0;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portNum = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portNum);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     while (1) {
         bzero(matrix, N * N * sizeof(double));
         n = read(newsockfd, matrix, N * N * sizeof(double));

         if (n < 0) {
            error("ERROR reading from socket");
            break;
         }
         if (n == 0) {
            printf("Closing server\n");
            break;
         }

         double det = determinantOfMatrix(matrix, N);
         printf("Det. = %f\n", det);

         history[historyIndex % HISTORY_SIZE] = det;
         historyIndex++;

         if (historyIndex >= HISTORY_SIZE) {
             double avgDet = calculateAverage(history);
             printf("Avg. det = %f\n", avgDet);
             printf("Del. det = %f\n", history[(historyIndex - HISTORY_SIZE) % HISTORY_SIZE]);
         }
         else {
             printf("Avg. det = N/A\n");
             printf("Del. det = N/A\n");
         }
         printf("\n");
     }

     close(newsockfd);
     close(sockfd);
     return 0;
}
