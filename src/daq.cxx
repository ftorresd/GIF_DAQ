// **********************************************************************
// *   DAQ for the GIF
// *   Y.Benhammou, Alexis Fagot
// *   14/1/2015
// *********************************************************************

#include "../include/DataReader.h"
#include "../include/utils.h"
#include "../include/MsgSvc.h"
#include <limits>

using namespace std;

int main (int argc ,char *argv[])
{
    MSG_INFO("****************************************************");
    MSG_INFO("******    DAQ Program XX/0X/16 Version 3.0    ******");
    MSG_INFO("****************************************************");

    DataReader *DR = new DataReader();

    /* Initialisation of the setup */

    MSG_INFO("[DAQ] Initialisation of the TDCs. Please wait, this may take a few minutes...");

    DR->Init(__configpath);

    MSG_INFO("[DAQ] Initialisation done");

    /* Run */

    //The DAQ is communicating with the WEB DCS. It reads a file containing
    //run commands from the DCS (START and STOP) and gives feedback on the
    //run status (RUNNING, NEXT).
    //When the DAQ gets START, the run starts and a busy is sent via NEXT.
    //When the run ends, the DAQ sends NEXT and waits for the next START.
    //In case the DAQ gets STOP from the WEB DCS, the DAQ exits safely.
    //In case of an error, the DAQ sends STATUS_ERR or WRITE_ERR and exits.
    string runStatus = GetRunStatus();

    //Enter the DAQ Loop only if we got a first START
    if(CtrlRunStatus(runStatus) == START){

        //Stay in the run loop while you don't have STOP
        //or an error
        while(CtrlRunStatus(runStatus) != STOP){
            MSG_INFO("[DAQ] Run about to start...");

            runStatus = "RUNNING";
            SetRunStatus(runStatus);

            //If you got a writing error, exit
            if(CtrlRunStatus(runStatus) != RUNNING) break;

            DR->Update();
            DR->Run();

            MSG_INFO("[DAQ] Run finished. Waiting for the next signal...");

            runStatus = "NEXT";
            SetRunStatus(runStatus);
            if(CtrlRunStatus(runStatus) != NEXT) break;

            while(CtrlRunStatus(runStatus) == NEXT){
                sleep(20);
                runStatus = GetRunStatus();
            }

            if(CtrlRunStatus(runStatus) != START) break;
        }
    }

    MSG_INFO("[DAQ] DAQ will shut down");
    return CtrlRunStatus(runStatus);
}
