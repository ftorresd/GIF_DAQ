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


// #include "crate.h"
// #include "be_board.h"

#include "utils.h"
#include "IniFile.h"

#include "uhal/uhal.hpp"

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
const Uint MINNBOARDS = 1;

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
        RAWData  BoardData;
        uhal::ConnectionManager *ConnectionManager;
        int nBoards;
        vector<uhal::HwInterface> *BEBoards;
        

    public:
        DataReader();
        virtual ~DataReader();
        void     SetIniFile(string inifilename);
        void     SetMaxTriggers();
        Data32   GetMaxTriggers();
        void     SetCrate(string ConnectionFilePath);
        void     SetBEBoard();
        int      GetQFlag(Uint it);
        void     Init(string inifilename);
        void     FlushBuffer();
        void     Update();
        string   GetFileName();
        void     WriteRunRegistry(string filename);
        void     Run();
};

#endif // DATAREADER_H
