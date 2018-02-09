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

class gpsdtest; // forward declaration so we can declare the test harness as a friend

class GPSd : public AppCastingMOOSApp {
    friend gpsdtest;
    public:
        GPSd();
        ~GPSd();

    protected: // Standard MOOSApp functions to overload
        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();
        // Standard AppCastingMOOSApp function to overload
    	bool buildReport();
        void registerVariables();
        double getDeclination(double lat, double lon);

        // Configuration variables
        std::string m_gpsd_host;
        std::string m_gpsd_port;
        std::string m_mag_model;
        gpsmm*      p_gpsd_receiver;
        double      m_dec_duration;
        static const int c_buf_len = 10000;

        // State variables
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
