import socket
import time
from datetime import datetime
import csv

b = 5
time_in_sec = 30

def send_data():
    # Define the ESP32's IP address and port
    ESP32_IP = "192.168.29.76"  # Replace with the ESP32's IP address
    PORT = 8080

    # Open the CSV file in append mode
    with open("output_data.csv", mode="a", newline="") as file:
        csv_writer = csv.writer(file)
        
        file.seek(0, 2)  # Move to the end of the file
        if file.tell() == 0:
            csv_writer.writerow(["Label", "Value"])
            file.flush()  # Ensure that the header is written immediately
        last_written_time = None  

        while True:
            current_time = datetime.now()
            # Create a socket object for TCP communication
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            try:
                # Connect to the ESP32
                client_socket.connect((ESP32_IP, PORT))
                print(f"Connected to ESP32 at {ESP32_IP}:{PORT}")
                # Send data to the ESP32
                l = [str(i * b) for i in range(1, 11)]
                message = ",".join(l)
                client_socket.sendall(message.encode('utf-8'))
                print("Sent to ESP32:", message.strip())

                # Receive a response from the ESP32
                response = client_socket.recv(1024).decode('utf-8')
                print("Received from ESP32:", response)

                # remove comma from response
                response = response.replace(",", " ").split()
                response = [int(x) for x in response]

                #remove comma from message
                message = message.replace("," , " " ).split()
                message = [int(y) for y in message]

                # Write the response to the CSV file
                if last_written_time is None or (current_time - last_written_time).seconds >= time_in_sec:
                    for x in range(1, (len(response) + 1)):
                        lable = f"A{x-1}"
                        csv_writer.writerow([lable, response[x - 1]])
                        file.flush()
                    for y in range(1, (len(message) + 1)):                            
                        label = f"B{y-1}"                          
                        csv_writer.writerow([label, message[y - 1]])
                        file.flush()
                        print("A & B data is appended")
                    last_written_time = current_time

                time.sleep(1.5)

            except Exception as e:
                print(f"Error: {e}")
            
            finally:
                client_socket.close()

if __name__ == "__main__":
    send_data()