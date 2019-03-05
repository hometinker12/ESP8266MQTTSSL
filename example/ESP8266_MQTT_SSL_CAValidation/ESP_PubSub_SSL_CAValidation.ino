// Example of X509 certificate validation using issuing
// certificate authority and NTP time sync to create
// a secure connection to a MQTT broker!
//
// Mar 2019 by Jesse Bedard
// Released to the public domain

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>

// Define Your Settings
const char* ssid = "#######";
const char* password = "#######";
const char* mqtt_server = "#######";
const char* mqtt_clientname = "TESTClient";
const int mqtt_port = 8883;
const char* mqtt_username = "#######";
const char* mqtt_password = "#######";
char* mqtt_topic = "test/ssltest/state";

//Replace with you issuing certificate authority Base64 format
//This is also known as the "intermediate" authority that issued
//your certificate
static const char digicert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEsTCCA5mgAwIBAgIQCKWiRs1LXIyD1wK0u6tTSTANBgkqhkiG9w0BAQsFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0xNzExMDYxMjIzMzNaFw0yNzExMDYxMjIzMzNaMF4xCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xHTAbBgNVBAMTFFJhcGlkU1NMIFJTQSBDQSAyMDE4MIIBIjANBgkqhkiG9w0B
AQEFAAOCAQ8AMIIBCgKCAQEA5S2oihEo9nnpezoziDtx4WWLLCll/e0t1EYemE5n
+MgP5viaHLy+VpHP+ndX5D18INIuuAV8wFq26KF5U0WNIZiQp6mLtIWjUeWDPA28
OeyhTlj9TLk2beytbtFU6ypbpWUltmvY5V8ngspC7nFRNCjpfnDED2kRyJzO8yoK
MFz4J4JE8N7NA1uJwUEFMUvHLs0scLoPZkKcewIRm1RV2AxmFQxJkdf7YN9Pckki
f2Xgm3b48BZn0zf0qXsSeGu84ua9gwzjzI7tbTBjayTpT+/XpWuBVv6fvarI6bik
KB859OSGQuw73XXgeuFwEPHTIRoUtkzu3/EQ+LtwznkkdQIDAQABo4IBZjCCAWIw
HQYDVR0OBBYEFFPKF1n8a8ADIS8aruSqqByCVtp1MB8GA1UdIwQYMBaAFAPeUDVW
0Uy7ZvCj4hsbw5eyPdFVMA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEF
BQcDAQYIKwYBBQUHAwIwEgYDVR0TAQH/BAgwBgEB/wIBADA0BggrBgEFBQcBAQQo
MCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBCBgNVHR8E
OzA5MDegNaAzhjFodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vRGlnaUNlcnRHbG9i
YWxSb290Q0EuY3JsMGMGA1UdIARcMFowNwYJYIZIAYb9bAECMCowKAYIKwYBBQUH
AgEWHGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwCwYJYIZIAYb9bAEBMAgG
BmeBDAECATAIBgZngQwBAgIwDQYJKoZIhvcNAQELBQADggEBAH4jx/LKNW5ZklFc
YWs8Ejbm0nyzKeZC2KOVYR7P8gevKyslWm4Xo4BSzKr235FsJ4aFt6yAiv1eY0tZ
/ZN18bOGSGStoEc/JE4ocIzr8P5Mg11kRYHbmgYnr1Rxeki5mSeb39DGxTpJD4kG
hs5lXNoo4conUiiJwKaqH7vh2baryd8pMISag83JUqyVGc2tWPpO0329/CWq2kry
qv66OSMjwulUz0dXf4OHQasR7CNfIr+4KScc6ABlQ5RDF86PGeE6kdwSQkFiB/cQ
ysNyq0jEDQTkfa2pjmuWtMCNbBnhFXBYejfubIhaUbEv2FOQB3dCav+FPg5eEveX
TVyMnGo=
-----END CERTIFICATE-----
)EOF";


//Simple boolean to indicate first startup loop
bool startup = false;

//Define WifiClientSecure/pubsubclient/set CA Cert list
WiFiClientSecure client; 
PubSubClient mqttclient(client);
X509List cert(digicert);


void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}


void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
    char receivedChar = (char)payload[i];
    Serial.println(receivedChar);
    //Do some action based on message recieved
 }
}
 
 
void reconnect() {
 // Loop until we're reconnected to the MQTT server
 while (!mqttclient.connected()) {
  Serial.print("Attempting MQTT connection...");
  //Attempt to connect
  if (mqttclient.connect(mqtt_clientname, mqtt_username, mqtt_password)) {
    Serial.println("connected");
    //Subscribe to topic
    mqttclient.subscribe(mqtt_topic);
  } else {
    Serial.print("failed, rc=");
    Serial.print(mqttclient.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  } //if
 }//while
}
 
void setup()
{
  Serial.begin(9600);  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi.");
  int _try = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    _try++;  
  }
  Serial.println("Connected to the WiFi network");  
  //****
  //Important to set setTrustAnchors to verify certificates
  //setInsecure() will allow the ssl connection without verification
  //****
  //client.setInsecure(); //WARNING Do NOT verify server
  
  client.setTrustAnchors(&cert);
  //NTP is required for CA Cert Validation
  setClock();
  
  //Connect to your MQTT Server and set callback
  mqttclient.setServer(mqtt_server, mqtt_port);
  mqttclient.setCallback(callback);

}
 
void loop()
{
  if (!mqttclient.connected()) {
  reconnect();
  }

  //Publish a startup message
  if (startup == false){
    Serial.println("Publish startup message 1");
    mqttclient.publish(mqtt_topic, "1",true);
    startup = true;
  };
 
  mqttclient.loop();
}
