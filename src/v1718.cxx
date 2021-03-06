// *************************************************************************************************************
// *   v1718
// *   Alexis Fagot
// *   20/01/2015
// *   Based on v1718 file from :
// *   Y. Benhammou
// *   27/07/09
// *
// *   Documentation about this VME TDC module can be found at the followin url
// *   http://www.caen.it/servlet/checkCaenManualFile?Id=5385
// *
// *   Comments will often refer to this user manual
// *
// *   This file declares defines the public methods of the v1718 class. A lot
// *   of methods are available but not used.
// *************************************************************************************************************

#ifndef __LINUX
#define __LINUX
#endif

#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>

#include "CAENVMElib.h"
#include "CAENVMEoslib.h"

#include "../include/v1718.h"
#include "../include/MsgSvc.h"
#include "../include/utils.h"

using namespace std;

// *************************************************************************************************************

v1718::v1718(IniFile *inifile){
   //Get the base address from the configuration file
   Data32 baseaddress = inifile->addressType("VMEInterface","BaseAddress",BASEV1718);
   Uint RDM_Frequency = inifile->intType("VMEInterface","int_trig_freq",RDM_PULSER_FRQ);

   //Initialisation of the module. See CAENVMElib.h & CAENVMEtypes.h
   CheckStatus(CAENVME_Init(cvV1718, baseaddress, 0, &Handle));

   SetLevel(cvIRQ1);
   SetAM(cvA24_U_DATA);
   SetDatasize(cvD16);
   SetBaseAddress(baseaddress);
   SetPulsers(RDM_Frequency);
}

// *************************************************************************************************************

v1718::~v1718(){
    CAENVME_End(Handle);
}

// *************************************************************************************************************
// Return the handle that identifies the device.

long v1718::GetHandle(void) const{
    return Handle;
}

// *************************************************************************************************************
// Data represents what you want to write or read from your VME bus (depending
// on which CAEN method you call

int v1718::SetData(Data16 data){
    Data=data;
    return 0;
}

// *************************************************************************************************************
// Return the value of Data. To use in case you read data from the VME bus
// using CAEN's methods.

Data16 v1718::GetData(void){
    return Data;
}

// *************************************************************************************************************
// See the list of address modifiers in the CVAddressModifier enum of
// CAENVMEtypes.h . More explanation on Page 34 (4.4.1) and Page 35 (4.4.3).

int v1718::SetAM(CVAddressModifier am){
    AM=am;
    return 0;
}

// *************************************************************************************************************
// Return the value of the address modifier

CVAddressModifier v1718::GetAM(void){
    return AM;
}

// *************************************************************************************************************
// As you can understand it, this specifies the data width you are using. A
// list is available in CVDataWidth enum of CAENVMEtypes.h .

int v1718::SetDatasize(CVDataWidth datasize){
    DataSize=datasize;
    return 0;
}

// *************************************************************************************************************
// Return the data width used.

CVDataWidth v1718::GetDataSize(void){
    return DataSize;
}

// *************************************************************************************************************
// Allows to set the base address of the VME interface. This is available at
// Page 32 (Fig.3.3) or on Page 35 of the V1190A users manual. A little
// explanation is also given in the header file v1718.h

int v1718::SetBaseAddress(Data16 baseaddress){
    BaseAddress=baseaddress;
    return 0;
}

// *************************************************************************************************************
// Return the base address of the vme interface.

Data16 v1718::GetBaseAddress(void){
    return BaseAddress;
}

// *************************************************************************************************************
// Set the Interrupt Request level. The IRQ Status register is a word of 7 bits
// each bit representing a level from IRQ1 to IRQ7.

int v1718::SetLevel(CVIRQLevels level){
    Level=level;
    return 0;
}

// *************************************************************************************************************
// Return the level of the eventual IRQs.

CVIRQLevels v1718::GetLevel(void){
    return Level;
}

// *************************************************************************************************************
// As the name says, it decodes the error code previously returned and saved
// into Status. Returns a string describing the error code. See CAEMVMElib.h .

void v1718::CheckStatus(CVErrorCodes status) const{
    // These exceptions will get passed up the call chain
    // This provides more flexible error handling, as the return value method is more of a C-ism
    switch (status){
        case cvBusError:
            MSG_ERROR("[v1718-ERROR] VME bus error");
        case cvCommError:
            MSG_ERROR("[v1718-ERROR] Communication error");
        case cvGenericError:
            MSG_ERROR("[v1718-ERROR] General VME library error");
        case cvInvalidParam:
            MSG_ERROR("[v1718-ERROR] Invalid parameter passed to VME library");
        case cvTimeoutError:
            MSG_ERROR("[v1718-ERROR] Request timed out");
        default:
            return;
    }
}

// *************************************************************************************************************
// Get the Interrupt Request Status and print whether they are active or not.

bool v1718::CheckIRQ(){
    Data8 data;
    CheckStatus(CAENVME_IRQCheck(Handle, &data));

    // Pick the requested IRQ line from the data and return its status
    return (((data>>(Level-1)) & 1) > 0);
}

// *************************************************************************************************************
//Set the width of the

void v1718::SetPulsers(Uint RDM_Frequency) {
    //Clear all outputs
    CheckStatus(CAENVME_WriteRegister(Handle, cvOutMuxRegClear, CLEARALL));

    //Set outputs 0 and 1 to Pulser A and 2 and 3 to Pulser B
    CheckStatus(CAENVME_WriteRegister(Handle, cvOutMuxRegSet, SET0PULS+SET1PULS+SET2PULS+SET3PULS));

    //Pulser A is used to send BUSY signals and VETO the trigger coincidence
    //Period in step units -> P
    //Width in step units  -> W
    //Number of pulses to be generated -> Np
    Uchar P = 1;
    Uchar W = (Uchar)BUSY_WIDTH/StepUnitMap.at(cvUnit104ms);
    Uchar Np = 1;

    CheckStatus(CAENVME_SetPulserConf(Handle, cvPulserA, P, W, cvUnit104ms, Np, cvManualSW, cvManualSW));

    //Pulser B is used to send RANDOM trigger pulses in case of non efficiency scans
    //An infinite amount of pulses is generated (Np = 0)
    if(RDM_Frequency < 10){
        MSG_WARNING("[v1718-WARNING] RANDOM Trigger pulse frequency is lower than 10Hz -> using 10Hz");
        P = 255;
    } else if(RDM_Frequency > 2439){
        MSG_WARNING("[v1718-WARNING] RANDOM Trigger pulse frequency is greater than 2439Hz -> using 2439Hz");
        P = 1;
    } else {
        MSG_INFO("[v1718] RANDOM Trigger pulse frequency is set to" + intTostring(RDM_Frequency) + "Hz");
        P = (Uchar)1/(RDM_Frequency*StepUnitMap.at(cvUnit410us));
    }
    W = 1;
    Np = 0;

    CheckStatus(CAENVME_SetPulserConf(Handle, cvPulserB, P, W, cvUnit410us, Np, cvManualSW, cvManualSW));
}

// *************************************************************************************************************
//Turn ON-OFF output pulser A - used as BUSY signal for the global DAQ

void v1718::SendBUSY(PulserLevel level) {
    if(level == ON)
        CheckStatus(CAENVME_StartPulser(Handle, cvPulserA));
    else if(level == OFF){
        usleep(1e6);
        CheckStatus(CAENVME_StopPulser(Handle, cvPulserA));
    }
}

// *************************************************************************************************************
//Turn ON-OFF output pulser B - used as RANDOM TRIGGER for non efficiency scans

void v1718::RDMTriggerPulse(PulserLevel level) {
    if(level == ON)
        CheckStatus(CAENVME_StartPulser(Handle, cvPulserB));
    else if(level == OFF)
        CheckStatus(CAENVME_StopPulser(Handle, cvPulserB));
}
