/*                                                                                                                        
 * CS410 PA3 webserv.c                                                                                                      
 * Sam Foreman Xavier Bohorquez Ned Geeslin                                                                                  
 *                                                                                                                        
 */
 
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <termios.h>

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno;
  char oldbuf[1000];
  //set port to the inputted value
    portno = atoi(argv[1]);
  //  portno = 8080;
  socklen_t newaddresslen;
  struct sockaddr_in address, newaddress;

  //create socket 
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *) &address, sizeof(address));

  //   address.sin_family = AF_INET;                                                                                      
  //    address.sin_addr.s_add = INADDR_ANY;                                                                           
  
  address.sin_port = htons(portno);
  //bind socket to the address
  if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0)
    printf("socket bind error.\n");
  //open socket up for communication
  listen(sockfd, 1);
  while (1)
    {
      newaddresslen = sizeof(newaddress);
      
      //take request from the client
      newsockfd = accept(sockfd, (struct sockaddr *) &newaddress, &newaddresslen);
      int oldout, dupcheck;
      // oldout = dup(1);
      //dupcheck = dup2(newsockfd, 1);
      // if (dupcheck < 0)
       //		perror("what??\n");
      // close(newsockfd);
      // printf("HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n<h1>testing</h1>");
      //fork in order to get successive requests
      if (fork() == 0)
	{
	  close(sockfd);
	  bzero(oldbuf, 1000);

	  //read the message sent from the client
	  read(newsockfd, oldbuf, 1000);
	  char *buf = oldbuf + 5;
	  printf("message: %s\n", buf);
	  char message[] = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n<h1>Connected to web server...</h1>";
	  int length = strlen(message);
	  int send_re = send(newsockfd, message, length, 0);
	  if (send_re == -1)
	    perror("send error\n");
	  // parse the message

	  //if mesage == 'dir', run the ls command (show directory contents)
	  if (strstr(buf, "dir") != NULL)
	    {
	      char *arg[2] = {"ls", NULL};
	      
	      execvp(arg[0], arg);
	      exit(0);
	    }

	  //if message == 'pwd', run the pwd command (used for testing)
	  else if (strstr(buf, "pwd") != NULL)
	    {
	      char *arg[2] = {"pwd", NULL};
	      execvp(arg[0], arg);
	      exit(0);
	    }
	  else if (strstr(buf, "request") != NULL)
            {
              //chmod +x request.cgi;                                                                  
	      char *arg[2] = {"./request.cgi", NULL};
              execvp(arg[0], arg);
              exit(0);
            }
	  else if (strstr(buf, "images") != NULL)
	    {
	     char *newbuf = buf + 7;
	     char imagebuf [10000];
	     char tok[2] = " ";
	     char *file;
	     int fileCheck;
	     file = strtok(newbuf, tok);
	     
	     fileCheck = open(file, O_RDWR);
	     if (fileCheck == -1)
	       {
		 char message[] = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n<h1>404 Error. File not found...</h1>";
		 int length = strlen(message);
		 int send_re = send(newsockfd, message, length, 0);
		 if (send_re == -1)
		   perror("send error\n");
		 exit(0);
	       }
	     char *arg[3] = {"cat", file, NULL};   
	     execvp(arg[0], arg);
	     exit(0);
	     }
	  else if (strstr(buf, "pong") != NULL)
	    {
	      
	      int c;
	      FILE *file;
	      file = fopen("/dev/cu.usbmodem1421","w");  //Opening device file
	      if (file == NULL)
		perror("error.\n");
	      else
	      {
			  	char message[] = "\r\n\r\n<h1>Success! Welcome to Pong!</h1>";
				int length = strlen(message);
				int send_re = send(newsockfd, message, length, 0);
				if (send_re == -1)
					perror("send error\n");
	}
	      system("/bin/stty raw");
	      while ((c = getchar()) != 'q')
	     /* K E Y   G U I D E
	      * inputted char -> num sent to arduino -> num read by arduino
	      * w -> 1 -> 49 //move paddleL up
	      * s -> 2 -> 50 //move paddleL down
	      * o -> 3 -> 51 //move paddleR up
	      *  l -> 4 -> 52 //move paddleR down
	      * q -> quit
	      */
		{
		  if (c == 'w')
		    fprintf(file, "%d", 1);
		  else if (c == 's')
		    fprintf(file, "%d", 2);
		  else if (c == 'o')
		    fprintf(file, "%d", 3);
		  else if (c == 'l')
		    fprintf(file, "%d", 4);
		  // sleep(1);
		}
	      system("/bin/stty cooked");
	      fclose(file);
	      exit(0);
	    }	
	  else if (strstr(buf, "my-histogram") != NULL)
	    {
	      char *newbuf = buf+15;
	      char tok[2] = " ";
	      char *file;
	      int fileCheck;
	      int dupfile;
	      dupfile = open("a.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
	      int oldhistout;
	      oldhistout = dup(1);
	      
	      file = strtok(newbuf, tok);
	      printf("directory to search: %s\n", file);
	      
	      dup2(dupfile, 1);
	      close(dupfile);
	      char *arg[3] = {"find", file, NULL};
	      printf("execing\n");
	      execvp(arg[0], arg);
	      dup2(oldhistout, 1);
	      printf("test\n");
	      char *arg2[4] = {"file", "-f", "a.txt", NULL};
	      execvp(arg2[0], arg2);
	      
	      
	      exit(0);
	    }
	  
	  //request doesn't exist
	  else
	    {
	      	char message[] = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n<h1>501 error. Cannot implement request...</h1>";
	  		int length = strlen(message);
	 		int send_re = send(newsockfd, message, length, 0);
	 		if (send_re == -1)
	   			perror("send error\n");
	        exit(0);
	    }
	  
	}
      // fflush(stdout);
      // dup2(oldout, 1);
       close(newsockfd);
	 // dup2(oldout, 1);
    }
}
