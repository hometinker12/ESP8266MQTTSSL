ESP8266 Secure MQTT Examples
===========================================

# Quick links
- [BearSSL Validation Examples](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/examples/BearSSL_Validation)

# Secure MQTT Broker Connection
This project has different examples using the PubSubClient with WiFiClientSecure to connect securely to a MQTT broker. Each example will connect to a MQTT broker, subscribe to a topic, and publish a message to a topic.

# Contents
- Examples:
  - [Certificate Thumbprint Verification](#certificate-thumbprint-verification)
  - [Issuing CA Verification-Requires NTP](#issuing-ca-verification-requires-ntp)
- [Warnings](#warnings)
- [Issues and Support](#issues-and-support)

### Certificate Thumbprint Verification
This method will verify the SHA1 certificate thumbprint of your MQTT server. While less ideal this method will work with self-signed certificates, however if the certificate is changed you will need to update your code.

To get the SHA1 certificate thumbprint: 
  - Navigate to the MQTT webserver (if accessible) in chrome
  - Press F12
  - Click on the "Security" tab
  - Click "View Certificate"
  - Copy the "SHA-1 Fingerprint"
Alternatively:
  - Open the certificate file (if stored locally)
  - View the details of the cert
  - Copy the "SHA-1 Certificate Fingerprint"

![Alt text](pictures/certthumbprint.png?raw=true "Certificate Thumbprint")

### Issuing CA Verification-Requires NTP

### Warnings
  - Use caution when disabling SSL verification! You should on use it for testing, make sure you add your certificate thumbprint or add a CA certificate for long term use.
  
### Issues and Support
This project is published as is for testing your projects. If you have questions or issues I will respond at a best effort basis (when I have time).

