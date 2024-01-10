# TCP_UDP_RPS
Implemented TCP and UDP program for the game rock, paper, scissor between two clients

### 1. BASIC

#### 1.1 TCP - Basic Client Server Communication:

- tcp_client.c and tcp_server.c contain the code for sending and receiving basic text.
- To run them, open TWO terminals - one for client and one for server.
- cd into ```/basic```
- In server terminal run:
  ```
  gcc tcp_server.c
  ./a.out
  ```
  
- In client terminal run:
  ```
  gcc tcp_client.c
  ./a.out
  ```

#### 1.2 UDP - Basic Client Server Communication:
- udp_client.c and udp_server.c contain the code for sending and receiving basic text.
- To run them, open TWO terminals - one for client and one for server.
- cd into ```/basic```
- In server terminal run:
  ```
  gcc udp_server.c
  ./a.out
  ```
  
- In client terminal run:
  ```
  gcc udp_client.c
  ./a.out
  ```



<br>
<br>

### 2. RPS (Rock Paper Scissors)

#### 2.1 TCP - Multi-Client(2) Server Communication

- AtcpClientRPS.c, BtcpClientRPS and tcpServerRPS.c contain the code for rps.
- To run them, open THREE terminals - two for client and one for server.
- cd into ```/rps```
- In server terminal run:
  ```
  gcc tcpServerRPS.c
  ./a.out
  ```
  
- In client A terminal run:
  ```
  gcc AtcpClientRPS.c
  ./a.out
  ```

- In client B terminal run:
  ```
  gcc BtcpClientRPS.c
  ./a.out
  ```
- Here you have to enter: Rock (r) or Paper (p) or Scissors (s)

#### 2.2 UDP - Multi-Client(2) Server Communication

- udpServerRPS.c, AudpClientRPS and BudpClientRPS.c contain the code for rps.
- To run them, open THREE terminals - two for client and one for server.
- cd into ```/rps```
- In server terminal run:
  ```
  gcc udpServerRPS.c
  ./a.out
  ```
  
- In client A terminal run:
  ```
  gcc AudpClientRPS.c
  ./a.out
  ```

- In client B terminal run:
  ```
  gcc  BudpClientRPS.c
  ./a.out
  ```

- Here you have to enter: Rock (0) or Paper (1) or Scissors (2)
