# TELEMETRY RECEIVER

This projects is intended to build a telemetry receiver for OpenPilot Flight Controllers based on ESP8266 boards.
Its main function is to setup a realtime waypoint navigation path for the UAV, in order to follow the transmitter, for aerial photography/video.
 
 ** Operation **

This receiver has a basic operation:	

1. Wait for transmitter message (which includes its GPS location and UAV desired position)
2. Read UAV GPS location and heading
3. With target location, UAV location and UAV desired relative position (to target), builds a waypoint path and sends it to the flight controller (UAVTalk protocol)

On the other hand it also:

1. Receive heartbeat messages from the flight controller and will resend to the transmitter to enable automatic Return To Base operation.
 
## ** Changelog **

> 23.03.2015-001  

- Wifi_ap and tcp_server running. Accepts connections on port 80 and prints received data.  
