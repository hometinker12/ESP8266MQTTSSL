ESP8266 Secure MQTT Examples
===========================================

# Quick links
- [BearSSL Validation Examples](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/examples/BearSSL_Validation)

# Secure MQTT Broker Connection
This project has different examples using the PubSubClient with WiFiClientSecure to connect securely to a MQTT broker. Each example will connect to a MQTT broker, subscribe to a topic, and publish a message to a topic.

# Contents
- [Warnings](#warnings)
- Examples:
  - [Certificate Thumbprint Verification](#certificate-thumbprint-verification)
  - [Issuing CA Verification-Requires NTP](#issuing-ca-verification-requires-ntp)
- [Issues and Support](#issues-and-support)

### Warnings
  - Use caution when disabling SSL verification! You should on use it for testing, make sure you add your certificate thumbprint or add a CA certificate for long term use.

### Certificate Thumbprint Verification
This method will verify the SHA1 certificate thumbprint of your MQTT server. While less ideal, this method will also work with self-signed certificates, however when the certificate is changed or updated you will need to update your code with the new thumbprint.

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
This method will verify your MQTT server's certificate was issued from an authorized certificate authority. After some testing on my part it appears that WiFiClientSecure will not traverse a certificate chain, meaning you need to provide the intermediate (issuing) certificate authority certificate. This method is more flexible as most issuing certificate authorities have a lengthy expiration time so your project will persist through certificate changes and updates, provided you use the same authority.

IMPORTANT: This method REQUIRES the use of the NTP client and either a local NTP server or internet access to allow you device to talk to an internet NTP server. It will NOT work if you do not set a valid time.

To get the issuing certificate: 
  - Navigate to the MQTT webserver (if accessible) in chrome
  - Press F12
  - Click on the "Security" tab
  - Click "View Certificate"
  - Click the "Details" tab
  - On the upper part of the window select the certificate authority DIRECTLY above the site name
  - Click the "Export" button
  - Save the certificate in the "BASE64" format
  - Open the certificate with your favorite text editor
  - Copy the information in the file and paste it into the code (replace what is there)
    The file should start with "-----BEGIN CERTIFICATE-----" and end with "-----END CERTIFICATE-----"
    
![Alt text](pictures/certauth1.png?raw=true "Certificate Export")
  
### Issues and Support
This project is published as is for testing your projects. If you have questions or issues I will respond at a best effort basis (when I have time).

