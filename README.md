# MOOS-pGPSd
This is a gpsd client for MOOS. It is intended to re-transmit every type of gpsd packet, including AIS. It is intended to work with both sockets and the local shared memory interface. It also delivers the 

## Dependencies
* MOOS-IvP
* https://github.com/Tencent/rapidjson/ -- provides JSON parse/deparse
* https://geographiclib.sourceforge.io/ -- provides magnetic deviation
* http://www.catb.org/gpsd/ -- provides client interface to GPSd

## Configuration Parameters
* host -- The host of the desired gpsd server, as a text field. Defaults to "localhost". If the value is "sharedmem", MOOS-gpsd will use the shared memory interface rather than the sockets interface.
* port -- The port the socket interface is listening on. Defaults to DEFAULT_GPSD_PORT.
* deviation_update -- the number of seconds to wait between updates to GPSD_deviation

## Outgoing Variables
* GPSD_json -- A STRING that contains each GPSd packet in turn.
* GPSD_latitude -- A DOUBLE containing the latitude in decimal degrees.
* GPSD_longitude -- A DOUBLE containing the longitude in decimal degrees.
* GPSD_elevation -- A DOUBLE containing the elevation in meters above MSL.
* GPSD_speed -- A DOUBLE containing the speed over ground in meters per second.
* GPSD_track -- A DOUBLE containing the course over ground in degrees clockwise from true north. 
* GPSD_deviation -- A DOUBLE containing the magnetic deviation, in degrees from true.
