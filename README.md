                                                                       
/* -----------------------------------------  R E A D M E ------------------------------------------\
- */   

Created as the final project for CS410 by: Sam Foreman, Ned Geeslin, and Xavier Bohorquez

The project is a recreation of Atari's 'Pong' using an Arduino Uno and an external 16X24 LED matrix.
The pong logic is programmed in Java with the Arduino IDE. It connects to a server programmed in C.

SERVER:

The server (webserv.c) is set up through the socket interface, and can be interacted through a web
browser once connected. For the assignment, it supports the following features:
  - request directory listing - "/dir"
  - search for image in directory - "image/IMAGE_NAME" (returns a 404 error if IMAGE_NAME
  is not in the directory.
  - run perl script - "/request.cgi"
  - 501 & 404 errors.
  
If the user enters "pong" as the command to the server, webserv.c sets up a serial connection to
the Arduino Uno. At this point, the players can use the arrow keys to control the game.


HARDWARE:

The Pong game created is not an exact replica of Atari's 'Pong', but has virtually identical
gameplay. Two players control a paddle at one side of the matrix and use 
arduinoPong.ino uses the AdaFruit_GFX and Adafruit_HT1632 libraries for toggling lights on the LED
display. The program listens for input from the server at each step, and adjusts the locations of
the two paddles accordingly. 

The ball bounces between the two paddles. It picks up speed and changes direction based on how it 
connects with the paddles. If the ball gets past a paddle and hits the wall of the matrix, the 
other player scores a point.  The game ends with a win screen after one player scores 5 points, 
then restarts.                 
                                                                                                     
        NOTE: The filename of the Arduino tends to change, and must be hardcoded before connection.  
        (Ex: sometimes the Arduino is called /dev/cu.usbmodem1411 other times it's /dev/cu.usbmoden1\
422)                                                                                                 
                                                                                                     
                                                                                                     
/* -----------------------------------------  R E A D M E ------------------------------------------- */                                                                                                 
               
