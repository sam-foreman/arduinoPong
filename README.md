CS410 A3                                                                                             
                                                                                                     
Sam Foreman, Ned Geeslin, Xavier Bohorquez                                                           
                                                                                                     
/* -----------------------------------------  R E A D M E ------------------------------------------\
- */                                                                                                 
SERVER:                                                                                              
        The server supports the following features:                                                  
                - request directory listing - "/dir"                                                 
                - search for image in directory - "image/IMAGE_NAME" (returns a 404 error if IMAGE_N\
AME                                                                                                  
                        is not in the directory)                                                     
                - run perl script - "/request.cgi"                                                   
                - 501 & 404 errors                                                                   
                                                                                                     
        NOTE: the web browser cannot handle input redirection. All information is displayed through \
terminal.                                                                                            
                We get the following error message: "localhost sent an invalid response."            
                                                                                                     
                                                                                                     
HARDWARE:                                                                                            
        For the physical computing part of the assignment, we created a version of Pong using an Ard\
uino                                                                                                 
        Uno, and an external 16X24 LED matrix.  The Pong logic is coded through the Arduino IDE. The\
 game                                                                                                
        is set up through the server created in part A.  The webserv.c program sets up a serial conn\
ection                                                                                               
        to the Arduino after the user enters "pong" as the command in any web browser.               
                                                                                                     
        Pong supports 2 players, and ends after one player scores 5 points.                          
                                                                                                     
        NOTE: The filename of the Arduino tends to change, and must be hardcoded before connection.  
        (Ex: sometimes the Arduino is called /dev/cu.usbmodem1411 other times it's /dev/cu.usbmoden1\
422)                                                                                                 
                                                                                                     
                                                                                                     
/* -----------------------------------------  R E A D M E ------------------------------------------- */                                                                                                 
               
