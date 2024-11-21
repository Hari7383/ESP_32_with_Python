#include <WiFi.h>

String message;
const char* ssid = "ntrb0001";          // Replace with your Wi-Fi SSID
const char* password = "Nila@021";  // Replace with your Wi-Fi password

WiFiServer server(8080);  // Create a server listening on port 8080

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Display the IP address
  Serial.println("Connected to WiFi");
  Serial.println("IP address: " + WiFi.localIP().toString());

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();  // Check if there's any incoming client 
  if (client) 
  {
    Serial.println("Client connected");
    // Read data from the client
    while (client.available()) 
    {
      int i = 1;
      int j = 1;
      int a = 2;
      String clt = "";
      while(i <= 2)
      {
        message = client.readStringUntil('\n');
        Serial.println("Received from Python: " + message); //message
        
        // Send a response back to the Python client
        j = i * a;
        clt += String(j);
        if (i <= 1) 
        { 
          clt += ",";
        }
        i++;
      }
        client.println(clt);
        Serial.println("Send to PYTHON: " + clt);
       // processcmd(message);
      
    }

    delay(100);  // Small delay to ensure data is sent/received correctly
    client.stop();  // Close the connection
    Serial.println("Client disconnected");
  }
}
