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
    m_mag_declination = NAN;
    m_dec_duration    = 600;
    m_last_declination_time = 0;
    //m_buf.str()       = "";
    m_json_output     = "";
    m_mag_model       = "emm2015";
    m_gps_mode        = 0;
    m_gps_alt         = 0;
    m_gps_lon         = 0;
    m_gps_alt         = 0;
    m_gps_spd         = 0;
    m_gps_trk         = 0;
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
    if(key != "APPCAST_REQ") {// handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
     }
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
  //m_buf << p_gpsd_receiver->data();     // grab the data buffer
  //cerr << "**********************************************************" << endl;
  //cerr << "Got buffer: " << endl;
  //cerr << "**********************************************************" << endl;
  //cerr << m_buf.str() << endl;
  //cerr << p_gpsd_receiver->data() << endl;
  //cerr << "**********************************************************" << endl;
  if ((p_gpsdata != NULL) && (p_gpsdata->set)) {
    m_gps_mode                = p_gpsdata->fix.mode;
    m_gps_lat                 = p_gpsdata->fix.latitude;
    m_gps_lon                 = p_gpsdata->fix.longitude;
    m_gps_alt                 = p_gpsdata->fix.altitude ;
    m_gps_spd                 = p_gpsdata->fix.speed;
    m_gps_trk                 = p_gpsdata->fix.track;
    Notify("GPSD_MODE",       m_gps_mode);
    Notify("GPSD_LATITUDE",   m_gps_lat);
    Notify("GPSD_LONGITUDE",  m_gps_lon);
    Notify("GPSD_ELEVATION",  m_gps_alt);
    Notify("GPSD_SPEED",      m_gps_spd);
    Notify("GPSD_TRACK",      m_gps_trk);
    m_json_output = p_gpsd_receiver->data();
    Notify("GPSD_JSON", m_json_output);
  }

  if (((MOOSTime() - m_last_declination_time) > m_dec_duration) &&  // check if it's time to do another declination check
      (p_gpsdata != NULL) && (p_gpsdata->set) &&                  // check that we have position data
      (p_gpsdata->fix.mode > 1)) {                                // and that it's valid
    m_last_declination_time = MOOSTime();
    m_mag_declination = getDeclination(p_gpsdata->fix.latitude, p_gpsdata->fix.longitude);
  }

  p_gpsd_receiver->clear_fix();

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
      this->m_gpsd_port = value;
      handled = true;
    } else if (param == "MAG_MODEL") {
      this->m_mag_model = value;
      handled = true;
    } else if (param == "DECLINATION_UPDATE") {
      this->m_dec_duration = atof(value.c_str());
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }

  p_gpsd_receiver = new gpsmm(m_gpsd_host.c_str(), m_gpsd_port.c_str());
  p_gpsd_receiver->stream(WATCH_ENABLE|WATCH_JSON|WATCH_SCALED);
  p_gpsdata = p_gpsd_receiver->read();
  p_gpsd_receiver->clear_fix();

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

  ACTable actab(7);
  actab << "Mode | Lat | Lon | Alt | Speed | Track | Mag Declination";
  actab.addHeaderLines();
  actab << to_string(m_gps_mode);
  actab << to_string(m_gps_lat);
  actab << to_string(m_gps_lon);
  actab << to_string(m_gps_alt);
  actab << to_string(m_gps_spd);
  actab << to_string(m_gps_trk);
  actab << to_string(m_mag_declination);
  m_msgs << actab.getFormattedString() << endl;
  m_msgs << "============================================ \n";
  m_msgs << "Last JSON: " << m_json_output << endl;;

  return(true);
}


//------------------------------------------------------------
// Procedure: getDeclination()

double GPSd::getDeclination(double lat, double lon) {
  time_t tt = system_clock::to_time_t(system_clock::now());
  tm utc_tm = *gmtime(&tt);
  GeographicLib::MagneticModel mag(m_mag_model.c_str());
  // intermediate values
  double Bx, By, Bz, H;
  // output values
  double strength, inclination, declination;

  // Grab the magnetic components
  mag(utc_tm.tm_year + 1900, lat, lon, 0, Bx, By, Bz);

  // convert intermediate representation to the output
  GeographicLib::MagneticModel::FieldComponents(Bx, By, Bz, H, strength, declination, inclination);

  return declination;
}
