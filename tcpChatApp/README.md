# Introduction
This project presents a Centralized Anonymous Chat Application built using TCP (Transmission Control Protocol), designed to facilitate anonymous exchange of messages between users through a central server. 
The core objective of this system is to enable real-time communication between clients, ensuring anonymity and reliability by using the TCP protocol.
 <img src="https://github.com/mohit1018102/SocketProgramming/blob/main/tcpChatApp/img/design.JPG"/>
# Key Feature
In this application, clients connect to a central server, which acts as the intermediary for all message exchanges. 
Each user is assigned a random alias or ID, ensuring that no personal identifying information (like IP addresses or names) is shared between users. 
The server manages all active connections and relays messages, making it possible for users to communicate while remaining completely anonymous.

1. **Anonymity:** Users communicate using unique aliases without revealing their identity.
2. **Centralized Communication:** A central server relays all messages between clients.
3. **TCP-based:** Reliable and ordered message delivery using the TCP protocol.

<img src="https://github.com/mohit1018102/SocketProgramming/blob/main/tcpChatApp/img/tcp-socket.png"/>

4. **Threaded Architecture:** The server handles multiple clients simultaneously via threading.
5. **Real-Time Messaging:** Users can send and receive messages in real-time.




