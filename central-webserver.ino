#include "ESP8266WiFi.h" // #include "ESP8266WebServer.h"
#include "ESPAsyncWebServer.h"
#include "ESPAsyncTCP.h"
#include "FS.h"

const String ssid = "YOUR_SSID";
const String password = "YOUR_PASSWORD";

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 80);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// REST API
AsyncWebServer server(30120);
AsyncWebSocket ws("/ws");

// Prototypes
void connectToWifiHandle();
void createAPIRoutes();

// Websocket Server Events Binding
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  
  if(type == WS_EVT_CONNECT) {
    
    Serial.println("WebSocket client connection received");
    client->text(parseSecSystemStatus());
  }

  if (type == WS_EVT_DATA) {
    
    // CLIENT SENT DATA PACKAGE
  }
  
}

void setup() {

  pinMode(D4, OUTPUT);

  // Start Serial
  Serial.begin(115200);
  delay(1000);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  Serial.println("1 - Connecting to WiFi...");
  connectToWifiHandle();

  Serial.println("2 - Reading Essential Files...");
  readEssentialFiles();

  Serial.println("3 - Creating API Routes...");
  createAPIRoutes();
  
  Serial.println("4 - Binding websocket event...");
  ws.onEvent(onWsEvent);

  Serial.println("4 - Starting Web Server...");
  // server.onNotFound(onRequest);
  // server.onFileUpload(onUpload);
  // server.onRequestBody(onBody);
  server.addHandler(&ws);
  
  server.serveStatic("/", SPIFFS, "/view/").setDefaultFile("index.html");
  
  server.begin();
}

String parseSecSystemStatus() {
  
  if (SECURITY_SYSTEM_STATUS == 1) {

    return "Enabled";

  } else {

    return "Disabled";
  }
}

void createAPIRoutes() {

  // server.on("/api/test", HTTP_GET, handleTest);
}

void connectToWifiHandle() {

  // Set Wifi to Station Mode
  WiFi.mode(WIFI_STA);

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected. (" + ssid + ")");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
    
  digitalWrite(D4, HIGH);
  delay(1000);
  digitalWrite(D4, LOW);
  delay(1000);
}
