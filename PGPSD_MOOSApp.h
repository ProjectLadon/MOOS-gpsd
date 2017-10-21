/*****************************************************************/
/*    NAME: Pierce Nichols                                       */
/*    ORGN: Ladon Project                                        */
/*    FILE: PGPSD_MOOSApp.h                                      */
/*    DATE: Oct 16 2017                                          */
/*                                                               */
/* This file is part of the Ladon Project stack                  */
/*                                                               */
/*****************************************************************/

#ifndef PGPSD_MOOSAPP_HEADER
#define PGPSD_MOOSAPP_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <chrono>
#include <ratio>

class pGPSd_MOOSApp : public CMOOSApp
{
    public:
        pGPSd_MOOSApp() = delete;
        pGPSd_MOOSApp(std::string host, std::string port_str);
        virtual ~pGPSd_MOOSApp() {
            if (m_gpsd_receiver) delete m_gpsd_receiver;
        }

        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();
        void setVars (std::string json,
                      std::string lat,
                      std::string lon,
                      std::string ele,
                      std::string spd,
                      std::string track,
                      std::string dev);
        void setDeviationUpdate (std::string period);

    protected: // State variables
        double m_mag_deviation = 0;
        gps_data_t* p_gpsdata = NULL;

    protected: // Configuration variables
        std::string m_json_var; 
        std::string m_lat_var;
        std::string m_lon_var;
        std::string m_ele_var;
        std::string m_spd_var;  
        std::string m_track_var; 
        std::string m_dev_var;

        std:string m_gpsd_host;
        int        m_gpsd_port;

        gpsmm*     m_gpsd_receiver;

        std::chrono::duration<double, std::chrono::seconds> m_dev_duration;

        double      m_start_time_postings;
        double      m_start_time_iterations;
};

#endif 









