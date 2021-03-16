// ****************************************************************************************************
// *   DataReader
// *   Alexis Fagot
// *   23/01/2015
// ****************************************************************************************************
#ifndef __LINUX
#define __LINUX
#endif

#ifndef DATAREADER_H
#define DATAREADER_H

#include <string>
#include <TFile.h>
#include <TTree.h>


// #include "v1190a.h"
// #include "v1718.h"

#include "utils.h"
#include "IniFile.h"

using namespace std;

// Structures
struct RAWData{
    vector<int>            *EventList;
    vector<int>            *NHitsList;
    vector<int>            *QFlagList;
    vector<vector<int> >   *ChannelList;
    vector<vector<float> > *TimeStampList;
};

// Definitions
const Uint MAXTRIGGERS = 1000;
const Uint MINNTDC = 1;

typedef enum _QualityFlag {
    GOOD      = 1,
    CORRUPTED = 0
} QualityFlag;
class DataReader
{
    private:
        bool     StopFlag;
        IniFile *iniFile;
        Data32   MaxTriggers;
        // v1718   *VME;
        int      nTDCs;
        // v1190a  *TDCs;
        RAWData  TDCData;

    public:
        DataReader();
        virtual ~DataReader();
        void     SetIniFile(string inifilename);
        void     SetMaxTriggers();
        Data32   GetMaxTriggers();
        void     SetVME();
        void     SetTDC();
        int      GetQFlag(Uint it);
        void     Init(string inifilename);
        void     FlushBuffer();
        void     Update();
        string   GetFileName();
        void     WriteRunRegistry(string filename);
        void     Run();
};

#endif // DATAREADER_H
