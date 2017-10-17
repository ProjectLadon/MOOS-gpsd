/*****************************************************************/
/*    NAME: Michael Benjamin                                     */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: Relayer.h                                            */
/*    DATE: Jun 26th 2008                                        */
/*                                                               */
/* This file is part of MOOS-IvP                                 */
/*                                                               */
/* MOOS-IvP is free software: you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation, either version  */
/* 3 of the License, or (at your option) any later version.      */
/*                                                               */
/* MOOS-IvP is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty   */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See  */
/* the GNU General Public License for more details.              */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with MOOS-IvP.  If not, see                     */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/

#ifndef PGPSD_MOOSAPP_HEADER
#define PGPSD_MOOSAPP_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class pGPSd_MOOSApp : public CMOOSApp
{
    public:
        pGPSd_MOOSApp();
        pGPSd_MOOSApp(std::string host, std::string port_str);
        virtual ~pGPSd_MOOSApp() {}

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

    protected:
        void RegisterVariables();

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

        gpsmm       m_gps_receiver;

        double      m_start_time_postings;
        double      m_start_time_iterations;
};

#endif 









