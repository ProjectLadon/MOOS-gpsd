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

    gpsmm*      p_gpsd_receiver;

    double      m_dec_duration;

 private: // State variables
 	  double      m_mag_declination;
    gps_data_t* p_gpsdata;
    std::string m_buf;
    double      m_last_declination_time;
};

#endif 
