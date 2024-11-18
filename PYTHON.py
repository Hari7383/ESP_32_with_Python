import socket

def send_data():
    # Define the ESP32's IP address and port
    ESP32_IP = "192.168.29.76"  # Replace with the ESP32's IP address
    PORT = 8080

    # Create a socket object for TCP communication
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        # Connect to the ESP32
        client_socket.connect((ESP32_IP, PORT))
        print(f"Connected to ESP32 at {ESP32_IP}:{PORT}")

        # Send data to the ESP32
        message = "22,43,4,5,6,7\n"
        client_socket.sendall(message.encode('utf-8'))
        print("Sent to ESP32:", message.strip())

        # Receive a response from the ESP32
        response = client_socket.recv(1024).decode('utf-8')
        response = response.replace("," , " ").split()
        response = [int(x) for x in response]
        print("Received from ESP32:", response)
        for i in range(len(response)):
            print("process ",i,response[i])

    except Exception as e:
        print(f"Error: {e}")
    
    finally:
        # Close the connection
        client_socket.close()


if __name__ == "__main__":
    send_data()