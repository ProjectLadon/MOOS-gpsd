# MOOS-pGPSd
This is a gpsd client for MOOS. It is intended to re-transmit every type of gpsd packet, including AIS. It is intended to work with the socket interface. It delivers both the raw JSON and key  

## Dependencies
* MOOS-IvP
* https://github.com/Tencent/rapidjson/ -- provides JSON parse/deparse
* https://geographiclib.sourceforge.io/ -- provides magnetic declination
* http://www.catb.org/gpsd/ -- provides client interface to GPSd

## Configuration Parameters
* host -- The host of the desired gpsd server, as a text field. Defaults to "localhost". 
* port -- The port the socket interface is listening on. Defaults to DEFAULT_GPSD_PORT.
* declination_update -- the number of seconds to wait between updates to GPSD_declination. Defaults to 600.
* magnetic_model -- the name of the magnetic model to use. Defaults to emm2017

## Outgoing Variables
* GPSD_json -- A STRING that contains a JSON array of all gpsd packets received since last iteration.
* GPSD_mode -- A DOUBLE containing the current GPS fix mode
* GPSD_latitude -- A DOUBLE containing the latitude in decimal degrees.
* GPSD_longitude -- A DOUBLE containing the longitude in decimal degrees.
* GPSD_elevation -- A DOUBLE containing the elevation in meters above MSL.
* GPSD_speed -- A DOUBLE containing the speed over ground in meters per second.
* GPSD_track -- A DOUBLE containing the course over ground in degrees clockwise from true north. 
* GPSD_declination -- A DOUBLE containing the magnetic declination, in degrees from true.
