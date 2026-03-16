#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

const char* ssid = "";
const char* password = "";

#define LED_PIN D6

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80);

// ---------- HTML Page ----------
String getHTMLPage(float temp, float hum, String lampState) {
  String html = "<!DOCTYPE html><html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>Smart Lamp Control</title>";

  html += "<style>";
  html += "*{margin:0;padding:0;box-sizing:border-box;}";
  html += "body{";
  html += "font-family:Arial,sans-serif;";
  html += "background:linear-gradient(135deg,#0f172a,#1e293b);";
  html += "color:white;";
  html += "min-height:100vh;";
  html += "display:flex;";
  html += "justify-content:center;";
  html += "align-items:center;";
  html += "padding:20px;";
  html += "}";

  html += ".container{";
  html += "width:100%;";
  html += "max-width:900px;";
  html += "min-height:80vh;";
  html += "background:rgba(30,30,30,0.95);";
  html += "border-radius:24px;";
  html += "padding:30px 20px;";
  html += "box-shadow:0 8px 30px rgba(0,0,0,0.35);";
  html += "display:flex;";
  html += "flex-direction:column;";
  html += "justify-content:center;";
  html += "align-items:center;";
  html += "}";

  html += "h1{";
  html += "color:#38bdf8;";
  html += "font-size:clamp(2rem,5vw,3.5rem);";
  html += "margin-bottom:25px;";
  html += "}";

  html += ".status{";
  html += "font-size:clamp(1.2rem,3vw,2rem);";
  html += "margin-bottom:25px;";
  html += "text-align:center;";
  html += "}";

  html += ".buttons{";
  html += "display:flex;";
  html += "flex-wrap:wrap;";
  html += "justify-content:center;";
  html += "gap:15px;";
  html += "width:100%;";
  html += "margin-bottom:30px;";
  html += "}";

  html += ".btn{";
  html += "flex:1 1 220px;";
  html += "max-width:300px;";
  html += "padding:18px 20px;";
  html += "font-size:clamp(1rem,2.5vw,1.4rem);";
  html += "font-weight:bold;";
  html += "text-decoration:none;";
  html += "border-radius:16px;";
  html += "color:white;";
  html += "text-align:center;";
  html += "transition:0.2s;";
  html += "}";

  html += ".btn:active{transform:scale(0.98);}";

  html += ".on{background:#22c55e;}";
  html += ".off{background:#ef4444;}";

  html += ".data-grid{";
  html += "display:grid;";
  html += "grid-template-columns:repeat(auto-fit,minmax(220px,1fr));";
  html += "gap:20px;";
  html += "width:100%;";
  html += "margin-top:10px;";
  html += "}";

  html += ".info-card{";
  html += "background:#0f172a;";
  html += "padding:25px 20px;";
  html += "border-radius:18px;";
  html += "text-align:center;";
  html += "box-shadow:0 4px 12px rgba(0,0,0,0.25);";
  html += "}";

  html += ".label{";
  html += "font-size:clamp(1rem,2vw,1.3rem);";
  html += "color:#cbd5e1;";
  html += "margin-bottom:10px;";
  html += "}";

  html += ".value{";
  html += "font-size:clamp(1.6rem,4vw,2.5rem);";
  html += "font-weight:bold;";
  html += "color:#f8fafc;";
  html += "}";

  html += "@media (max-width:600px){";
  html += ".container{min-height:100vh;border-radius:0;padding:25px 15px;}";
  html += "body{padding:0;}";
  html += "}";
  html += "</style>";

  html += "</head><body>";

  html += "<div class='container'>";
  html += "<h1>Smart Lamp</h1>";

  html += "<div class='status'>Lamp State: <b>" + lampState + "</b></div>";

  html += "<div class='buttons'>";
  html += "<a class='btn on' href='/on'>Turn ON</a>";
  html += "<a class='btn off' href='/off'>Turn OFF</a>";
  html += "</div>";

  html += "<div class='data-grid'>";
  html += "<div class='info-card'>";
  html += "<div class='label'>Temperature</div>";
  html += "<div class='value'>" + String(temp, 1) + " &deg;C</div>";
  html += "</div>";

  html += "<div class='info-card'>";
  html += "<div class='label'>Humidity</div>";
  html += "<div class='value'>" + String(hum, 1) + " %</div>";
  html += "</div>";
  html += "</div>";

  html += "</div></body></html>";

  return html;
}

void handleRoot() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    server.send(200, "text/html", "<h2>Failed to read from DHT11 sensor</h2>");
    return;
  }

  String lampState = digitalRead(LED_PIN) ? "ON" : "OFF";
  server.send(200, "text/html", getHTMLPage(t, h, lampState));
}


void handleOn() {
  digitalWrite(LED_PIN, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}


void handleOff() {
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("NodeMCU IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}