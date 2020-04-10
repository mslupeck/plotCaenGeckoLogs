//============================================================================
// Name        : plotCaenGeckoLogs.cpp
// Author      : mss
// Version     :
// Copyright   : Your copyright notice
// Description : Plots trends from CaenGecko txt logs
//============================================================================

#include <iostream>
#include <string>

#include "cliParser.h"
#include "TLogFileReader.h"
#include "TLogStorage.h"
#include "TLogPlotter.h"

#include <TApplication.h>
#include <TFile.h>
#include <TH1D.h>

using namespace std;

void SplitInputPaths(const string& inputPathList, vector<string> &vInPath, char delim = ' '){
    std::stringstream ss(inputPathList);
    std::string token;
    while (std::getline(ss, token, delim)) {
        vInPath.push_back(token);
    }
}

int main(int argc, char* argv[]){
	cliParser cli;
	cli.AddPar("m", "Mode of operation", true);
	cli.GetModeList()->AddPar("default", "Default mode");
	cli.AddPar("i", "Input directory or file", true);
	cli.AddPar("p", "Plot only parameter with this name [VMon, IMonH, ChStatus]", false);
	cli.AddPar("t0", "Force the start time of the x-axis", false);
	if(cli.Parse(argc, argv)!=0){
		cli.PrintUsage(cout, "aliFitSimAnalysis");
		return -1;
	}

	string inputPathList = cli.GetParString("i");
	vector<string> vInPath;
	SplitInputPaths(inputPathList, vInPath, ',');
	for(uint16_t iin=0; iin<vInPath.size(); iin++){
		cout << vInPath.at(iin) << endl;
	}

	string selParam = cli.GetParString("p");
	int t0 = cli.GetParInt("t0");
	if(cli.GetParString("m").compare("default")==0){
		TApplication *theApp = new TApplication("App", &argc, argv);	// needed to print interactive canvases and gui
		theApp->SetIdleTimer(100000,"exit()"); 							// exit automatically after 100000s of being idle
		theApp->SetReturnFromRun(true);									// when the program exits there will be no error message
			vector<TLogFileReader*> vlfr;
			for(uint16_t iInPath=0; iInPath<vInPath.size(); iInPath++){
				TLogFileReader *lfr = new TLogFileReader();
				lfr->ReadFile(vInPath.at(iInPath));
				//lfr->Print(cout);
				vlfr.push_back(lfr);
			}
			// TODO: change input to logStorage to vector of lfr from single lfr:
			TLogStorage *ls = new TLogStorage(&(vlfr.at(0)->GetStorage()));
			ls->Fill();
			for(uint16_t i=0; i<vlfr.size(); i++){
				delete vlfr.at(i);
			}
			//ls->PrintContents();
			TLogPlotter *lp = new TLogPlotter(ls);
			lp->Plot(selParam, t0);
		cout << "Finished..." << endl;
		theApp->Run();
	}

	return 0;
}
