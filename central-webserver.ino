#include "ESP8266WiFi.h" // #include "ESP8266WebServer.h"
#include "ESPAsyncWebServer.h"
#include "ESPAsyncTCP.h"
#include "FS.h"

byte SECURITY_SYSTEM_STATUS = 0;
String SECURITY_SYSTEM_PASSWORD = "guga@123";

const String ssid = "Deposito do Taquinho";
const String password = "guga@123";

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 80);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// REST API
AsyncWebServer server(30120);
AsyncWebSocket ws("/ws");

String __HTML__ = "<p>SECURITY_SYSTEM_LOAD_LODADED_YET<p/>";
String __CSS__ = "";
String __JS__ = "";

// Prototypes
void readEssentialFiles();
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
  server.serveStatic("/static", SPIFFS, "/view");
  server.serveStatic("/static/css", SPIFFS, "/view");
  server.serveStatic("/static/js", SPIFFS, "/view");

  File testFile1 = SPIFFS.open("/view/main.982c5d44.js", "r");
  if (!testFile1) {

    Serial.println("Falha ao abrir o arquivo (/view/main.982c5d44.js)");
    return;
  }

  File testFile2 = SPIFFS.open("/view/main.90ecee4d.css", "r");
  if (!testFile2) {

    Serial.println("Falha ao abrir o arquivo (/view/main.90ecee4d.css)");
    return;
  }
  
  server.begin();
}

String parseSecSystemStatus() {
  
  if (SECURITY_SYSTEM_STATUS == 1) {

    return "Enabled";
  } else {
    return "Disabled";
  }
}

void handleRoot(AsyncWebServerRequest *request) {
  
  request->send(200, "text/html", __HTML__);
}
void handleCssFile(AsyncWebServerRequest *request) {

  request->send(200, "text/css", __CSS__);
}
void handleJsFile(AsyncWebServerRequest *request) {

  request->send(200, "text/js", __JS__);
}
void handleGetSecSystemStatus(AsyncWebServerRequest *request) {

  request->send(200, "text/plain;charset=UTF-8", parseSecSystemStatus());
}
void handleEnableSecSystem(AsyncWebServerRequest *request) {

  SECURITY_SYSTEM_STATUS = 1;
  ws.textAll("Enabled");
  request->send(200, "text/plain;charset=UTF-8", "OK");
}
void handleDisableSecSystem(AsyncWebServerRequest *request) {

  SECURITY_SYSTEM_STATUS = 0;
  ws.textAll("Disabled");
  request->send(200, "text/plain;charset=UTF-8", "OK");
}

void createAPIRoutes() {
  
   // server.on("/", HTTP_GET, handleRoot);
   // server.on("/data/css/index.min.css", HTTP_GET, handleCssFile);
   // server.on("/data/js/index.min.js", HTTP_GET, handleJsFile);

   server.on("/api/get-sec-system-status", HTTP_GET, handleGetSecSystemStatus);
   server.on("/api/enable-security-system", HTTP_POST, handleEnableSecSystem);
   server.on("/api/disable-security-system", HTTP_POST, handleDisableSecSystem);
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

void readEssentialFiles() {
  
  // Read Server Files
  SPIFFS.begin();
  
  File htmlFile = SPIFFS.open("/html/index.min.html", "r");
  File cssFile = SPIFFS.open("/css/index.min.css", "r");
  File jsFile = SPIFFS.open("/js/index.min.js", "r");
  
  if (!htmlFile) {
    
    Serial.println("Falha ao abrir o arquivo (/html/index.min.html)");
    return;
  }
  if (!cssFile) {

    Serial.println("Falha ao abrir o arquivo (/css/index.min.css)");
    return;
  }
  if (!jsFile) {

    Serial.println("Falha ao abrir o arquivo (/js/index.min.js)");
    return;
  }
  
  __HTML__ = htmlFile.readString();
  __CSS__ = cssFile.readString();
  __JS__ = jsFile.readString();

  htmlFile.close();
  cssFile.close();
  jsFile.close();
}

void loop() {
    
  digitalWrite(D4, HIGH);
  delay(1000);
  digitalWrite(D4, LOW);
  delay(1000);
}
