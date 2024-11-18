#include <WiFi.h>

int sum_add = 0;
int sum_mul = 1;
int total = 0;
String outputString;
int8_t process_data[20];
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
  
  if (client) {
    Serial.println("Client connected");
    
    // Read data from the client
    while (client.available()) {
      String message = client.readStringUntil('\n');
      Serial.println("Received from Python: " + message); //message
      
      // Send a response back to the Python client
      client.println(outputString);

      processcmd(message);
    }

    delay(100);  // Small delay to ensure data is sent/received correctly
    client.stop();  // Close the connection
    Serial.println("Client disconnected");
  }
}

String getValue(String data, char separator, int index) 
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String createOutputString(int sumAdd, int sumMul, int scaledSumAdd, int scaledSumMul) 
{
  return String(sumAdd) + "," + String(sumMul) + "," + String(scaledSumAdd) + "," + String(scaledSumMul);
}

void processcmd(String st1) 
{
  int numDataPoints = 0;

  // Calculate the number of data points in the input string
  for (int i = 0; i < st1.length(); i++) {
    if (st1.charAt(i) == ',') {
      numDataPoints++;
    }
  }
  numDataPoints++; // Account for the last data point after the final comma

  // Process each data point
  for (int i = 0; i < numDataPoints; i++) {
    int value = getValue(st1, ',', i).toInt();
    process_data[i] = value;
    Serial.print("process ");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(value);
    sum_add = sum_add + value;
    sum_mul = sum_mul * value;
  }
  Serial.print("Total sum_add =");Serial.println(sum_add);
  Serial.print("Total sum_mul =");Serial.println(sum_mul);

  int scaledSumAdd = sum_add * 10;
  int scaledSumMul = sum_mul * 100;

  Serial.print("Total sum_add =");Serial.println(scaledSumAdd);
  Serial.print("Total sum_mul =");Serial.println(scaledSumMul);

  outputString = createOutputString(sum_add, sum_mul, scaledSumAdd, scaledSumMul);
  Serial.println("Combined Output: " + outputString);

  sum_add = 0;
  sum_mul = 1;
}