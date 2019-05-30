# MQTT-Doorbell
The need addressed by this project is to know when a remote bell (situated in a second house) rings. 
Doorbell based on MQTT publish/subscribe. 
E2E projectdetails are available in the repository. Project is based on Raspberry PI 3B+ Lolin D1 as remote slave(s)

Raspberry PI 3B+
================
- Starting from an existing 12V AC wired bell, explained how to add the right passive (diode + resistor) to interface with Raspberry GPIO input.
- configuration of a commercial MQTT server
- Python code reads the GPIO input and publish a "bell" MQTT topic 

Lolin D1 mini
=============
- Arduino IDE code to subscribe to the "bell" MQTT topic and to interface with buzzer to beep.
