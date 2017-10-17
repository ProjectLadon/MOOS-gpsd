/*****************************************************************/
/*    NAME: Pierce Nichols                                       */
/*    ORGN: Ladon Project                                        */
/*    FILE: main.cpp                                             */
/*    DATE: Oct 16 2017                                          */
/*                                                               */
/* This file is part of the Ladon Project stack                  */
/*                                                               */
/*****************************************************************/

#include "MBUtils.h"
#include "ReleaseInfo.h"
#include "ColorParse.h"
#include "PGPSD_MOOSApp.h"
#include "PGPSD_Info.h"
#include "libgpsmm.h"

using namespace std;

int main(int argc, char *argv[])
{
  string mission_file     = "";
  string run_command      = argv[0];
  string host             = "localhost";
  string port_str         = to_string(DEFAULT_GPSD_PORT);
  string dev_update_str   = to_string(600);
  string json_var         = "GPSD_json";
  string lat_var          = "GPSD_latitude";
  string lon_var          = "GPSD_longitude";
  string ele_var          = "GPSD_elevation";
  string spd_var          = "GPSD_speed";  
  string track_var        = "GPSD_track";
  string dev_var          = "GPSD_deviation";

  // TODO: Replace this with proper input parser 
  // shotgun parsers like this are of the devil (but easy and quick to write)
  for(int i=1; i<argc; i++) {
    string argi = argv[i];
    if((argi=="-v") || (argi=="--version") || (argi=="-version"))
      showReleaseInfoAndExit();
    else if((argi=="-e") || (argi=="--example") || (argi=="-example"))
      showExampleConfigAndExit();
    else if((argi == "-h") || (argi == "--help") || (argi=="-help"))
      showHelpAndExit();
    else if((argi == "-i") || (argi == "--interface"))
      showInterfaceAndExit();
    else if(strEnds(argi, ".moos") || strEnds(argi, ".moos++"))
      mission_file = argv[i];
    else if(strBegins(argi, "--alias="))
      run_command = argi.substr(8);
    else if(strBegins(argi, "--GPSD_json="))
      json_var = argi.substr(12);
    else if(strBegins(argi, "--GPSD_latitude="))
      lat_var = argi.substr(16);
    else if(strBegins(argi, "--GPSD_longitude="))
      lon_var = argi.substr(17);
    else if(strBegins(argi, "--GPSD_elevation="))
      ele_var = argi.substr(17);
    else if(strBegins(argi, "--GPSD_speed="))
      spd_var = argi.substr(13);
    else if(strBegins(argi, "--GPSD_track="))
      track_var = argi.substr(13);
    else if(strBegins(argi, "--GPSD_deviation="))
      dev_var = argi.substr(17);
    else if(i==2)
      run_command = argi;
  }
  
  if(mission_file == "")
    showHelpAndExit();

  cout << termColor("green");
  cout << "pGPSd launching as " << run_command << endl;
  cout << termColor() << endl;

  PGPSD_MOOSApp gps(host, port_str);	
  gps.setVars(json_var, lat_var, lon_var, ele_var,
              spd_var, track_var, dev_var);

  gps.Run(run_command.c_str(), mission_file.c_str());

  return(0);
}










