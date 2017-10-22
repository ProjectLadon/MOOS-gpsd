/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: GPSd.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef GPSd_HEADER
#define GPSd_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "libgpsmm.h"
#include <string>
#include <chrono>
#include <sstream>

class GPSd : public AppCastingMOOSApp
{
 public:
   GPSd();
   ~GPSd();

 protected: // Standard MOOSApp functions to overload  
	bool OnNewMail(MOOSMSG_LIST &NewMail);
	bool Iterate();
	bool OnConnectToServer();
	bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
	bool buildReport();

 protected:
	void registerVariables();
	double getDeclination(double lat, double lon);

 private: // Configuration variables
    std::string m_gpsd_host;
    std::string m_gpsd_port;
    std::string m_mag_model;
    gpsmm*      p_gpsd_receiver;
    double      m_dec_duration;
    static const int c_buf_len = 10000;

 private: // State variables
 	  double      m_mag_declination;
    gps_data_t* p_gpsdata;
    //std::stringstream m_buf;
    std::string m_json_output;
    int         m_gps_mode;
    double      m_gps_lat;
    double      m_gps_lon;
    double      m_gps_alt;
    double      m_gps_spd;
    double      m_gps_trk;
    double      m_last_declination_time;
};

#endif 
