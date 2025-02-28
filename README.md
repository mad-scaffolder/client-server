# client-server
Client-Server application for Prometheus

Possible inputs:
 - an array of space separated integers;
 - a text message if any of characters is not a digit.

Tasks chosen to solve:
 - send an array of integers to Server, sort them
    in descending order and return to Client;
 - send a message to Server, found a number of vowels
    in a message, and return it to Client.

Implemented at this point:
 - Server starts and waits in the background;
 - Client starts and establishes connection to Server;
 - Client sends data obtained from user input to Server;
 - Server takes data and performs one out of two possible
    operations with data and forms a response message to Client;
 - Server sends data to Client;
 - Client takes data from Server, prints it and asks for 
    the next input from user;
 - If user message is 'exit', Client sends a message and disconects,
    Server takes message, closes connection to Client and waits;
 - If user message is 'end', Client sends a message and disconects,
    Server takes message, closes connection to Client and shuts down.


Estimated functionality:
 - Server starts and waits in the background;
 - Client starts and establishes connection to Server;
 - Server makes a thread for the further interactions with
    Client and waits for the next Client;
 - Client sends data obtained from user input to Server;
 - Server takes data and performs one out of two possible
    operations with data and forms a response message to Client;
 - Server sends data to Client;
 - Client takes data from Server, prints it and asks for 
    the next input from user;
 - Client disconnects from server if user enters 'exit';
 - Server shuts down if he takes a 'end' message from Client,
 - Server disconnects from the Client and waits for another connection
    if he takes 'exit' message from the Client.
