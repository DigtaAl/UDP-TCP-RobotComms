# Robot Communication System: Client-Server Implementation

## Overview

This project focuses on implementing a robust client-server communication system for a robot, enabling seamless interaction and control. The system is designed to enhance the capabilities of the robot by allowing for real-time data exchange, command execution, and monitoring through a client-server architecture.

## Features

- **Bi-Directional Communication:** Establishes a reliable two-way communication channel between the robot and the server, enabling data transmission in both directions.

- **Command Execution:** The system supports the execution of commands from the server to the robot, providing a flexible and remote control mechanism.

- **Real-Time Data Exchange:** Facilitates real-time exchange of sensor data, status updates, and other relevant information between the robot and the server.

### Server

- The server component is responsible for managing incoming connections, processing client requests, and coordinating communication with connected robots.

- To run the server, navigate to the `build` directory and execute the following command:

  ```make, ./server || ./client
