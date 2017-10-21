/*****************************************************************/
/*    NAME: Pierce Nichols                                       */
/*    ORGN: Ladon Project                                        */
/*    FILE: PGPSD_MOOSApp.cpp                                    */
/*    DATE: Oct 16 2017                                          */
/*                                                               */
/* This file is part of the Ladon Project stack                  */
/*                                                               */
/*****************************************************************/

#include <iterator>
#include "PGPSD_MOOSApp.h"
#include "MBUtils.h"

using namespace std;

//---------------------------------------------------------
// Constructor

pGPSd_MOOSApp::pGPSd_MOOSApp(std::string host, std::string port_str) {
    m_gpsd_receiver = NULL;
    m_gpsd_host = host;
    m_gpsd_port = stoi(port_str);
    m_start_time_postings   = 0;
    m_start_time_iterations = 0;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool pGPSd_MOOSApp::OnNewMail(MOOSMSG_LIST &NewMail) {
    return(true);
}


//---------------------------------------------------------
// Procedure: OnConnectToServer

bool pGPSd_MOOSApp::OnConnectToServer()
{
  return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()

bool pGPSd_MOOSApp::Iterate() {

  unsigned int i, amt = (m_tally_recd - m_tally_sent);
  for(i=0; i<amt; i++) {
    m_tally_sent++;
    Notify(m_outgoing_var, m_tally_sent);
  }
  
  // If this is the first iteration just note the start time, otherwise
  // note the currently calculated frequency and post it to the DB.
  if(m_start_time_iterations == 0)
    m_start_time_iterations = MOOSTime();
  else {
    double delta_time = (MOOSTime() - m_start_time_iterations);
    double frequency = 0;
    if(delta_time > 0)
      frequency  = (double)(m_iterations) / delta_time;
    Notify(m_outgoing_var+"_ITER_HZ", frequency);
  }
    

  // If this is the first time a received msg has been noted, just
  // note the start time, otherwise calculate and post the frequency.
  if(amt > 0) {
    if(m_start_time_postings == 0)
      m_start_time_postings = MOOSTime();
    else {
      double delta_time = (MOOSTime() - m_start_time_postings);
      double frequency  = 0;
      if(delta_time > 0)
	frequency = (double)(m_tally_sent) / delta_time;
      Notify(m_outgoing_var+"_POST_HZ", frequency);
    }
  }
    return(true);
}



//---------------------------------------------------------
// Procedure: OnStartUp()
//      Note: happens before connection is open

bool pGPSd_MOOSApp::OnStartUp()
{
  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  m_MissionReader.GetConfiguration(GetAppName(), sParams);
    
  STRING_LIST::iterator p;
  for(p = sParams.begin();p!=sParams.end();p++) {
      string sLine     = *p;
      string sVarName  = MOOSChomp(sLine, "=");
      sLine = stripBlankEnds(sLine);
      
      if(MOOSStrCmp(sVarName, "host")) {
          if(!strContains(sLine, " ")) {
              m_gpsd_host = stripBlankEnds(sLine);
          }
      }
      
      else if(MOOSStrCmp(sVarName, "port")) {
          if(!strContains(sLine, " ")) {
              m_gpsd_port = stoi(stripBlankEnds(sLine));
          }
      }
      
      else if(MOOSStrCmp(sVarName, "deviation_update")) {
          if(!strContains(sLine, " ")) {
              m_dev_duration = stof(stripBlankEnds(sLine));
          }
      }
  }

  m_gpsd_receiver = new gpsmm(m_gps_host, m_gps_port);
  return(true);
}










