/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: GPSd.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "GPSd.h"
#include <iostream>
#include "libgpsmm.h"
#include <thread>
#include <GeographicLib/MagneticModel.hpp>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Constants.hpp>
#include <ctime>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

//---------------------------------------------------------
// Constructor

GPSd::GPSd() {
    p_gpsd_receiver   = NULL;
    m_gpsd_host       = "localhost";
    m_gpsd_port       = DEFAULT_GPSD_PORT;
    p_gpsdata         = NULL;
    m_mag_declination   = NAN;
    m_dec_duration    = 600;
    m_last_declination_time = MOOSTime();;
    m_buf             = "";
}

//---------------------------------------------------------
// Destructor

GPSd::~GPSd()
{
    if (p_gpsd_receiver) delete p_gpsd_receiver;
    if (p_gpsdata) delete p_gpsdata;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GPSd::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GPSd::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GPSd::Iterate()
{
  AppCastingMOOSApp::Iterate();

  p_gpsdata = p_gpsd_receiver->read();  // read in the data
  m_buf += p_gpsd_receiver->data();     // grab the data buffer
  if ((p_gpsdata != NULL) && (p_gpsdata->set)) {
    Notify("GPSD_mode", p_gpsdata->fix.mode);
    Notify("GPSD_latitude", p_gpsdata->fix.latitude);
    Notify("GPSD_longitude", p_gpsdata->fix.longitude);
    Notify("GPSD_elevation", p_gpsdata->fix.elevation);
    Notify("GPSD_speed", p_gpsdata->fix.speed);
    Notify("GPSD_track", p_gpsdata->fix.track); 
  }
  string json_output = "[";
  size_t index = 0;
  while ((index = m_buf.find("\n")) != string::npos) {
    json_output += m_buf.substr(0, index);
    m_buf = m_buf.substr(index + 1);
    json_output += ", ";
  }
  string json_output = "]";

  Notify("GPSD_json", json_output);

  if (((MOOSTime() - m_last_declination_time) > m_dec_duration) &&  // check if it's time to do another declination check
      (p_gpsdata != NULL) && (p_gpsdata->set) &&                  // check that we have position data
      (p_gpsdata->fix.mode > 1)) {                                // and that it's valid
    m_last_declination_time = MOOSTime();
    m_mag_declination = getDeclination(p_gpsdata->fix.latitude, p_gpsdata->fix.longitude);
  }

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GPSd::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if (param == "HOST") {
      this->m_gpsd_host = value;
      handled = true;
    } else if (param == "PORT") {
      this->m_gpsd_port = atoi(value);
      handled = true;
    } else if (param == "DECLINATION_UPDATE") {
      this->m_dec_duration = atof(value);
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }

  p_gpsd_receiver = new gpsmm(m_gpsd_host, m_gpsd_port);
  p_gpsd_receiver->stream(WATCH_ENABLE|WATCH_JSON);
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void GPSd::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool GPSd::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File: GPSd.cpp                               \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}

double GPSd::getDeclination(double lat, double lon) {
  time_t tt = system_clock::to_time_t(system_clock::now());
  tm utc_tm = *gmtime(&tt);
  MagneticModel mag("emm2015");
  // intermediate values
  double Bx, By, Bz, H;
  // output values
  double strength, inclination, declination;
  
  // Grab the magnetic components
  mag(utc_tm.tm_year + 1900, lat, lon, 0, Bx, By, Bz);
  
  // convert intermediate representation to the output
  MagneticModel::FieldComponents(Bx, By, Bz, H, strength, declination, inclination);

  return declination;
}



