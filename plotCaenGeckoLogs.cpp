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

#include <TApplication.h>
#include <TFile.h>
#include <TH1D.h>

using namespace std;

int main(int argc, char* argv[]){
	cliParser cli;
	cli.AddPar("m", "Mode of operation", true);
	cli.GetModeList()->AddPar("default", "Default mode");
	cli.AddPar("i", "Input directory or file", true);
	cli.AddPar("o", "Output directory or file", true);
	//cli.AddPar("v", "Verbosity level");
	if(cli.Parse(argc, argv)!=0){
		cli.PrintUsage(cout, "aliFitSimAnalysis");
		return -1;
	}

	string inputPath = cli.GetParString("i");
	string outputPath = cli.GetParString("o");
	//int verbose = cli.GetParInt("v");
	if(cli.GetParString("m").compare("default")==0){
		TApplication *theApp = new TApplication("App", &argc, argv);	// needed to print interactive canvases and gui
		theApp->SetIdleTimer(100000,"exit()"); 							// exit automatically after 100000s of being idle
		theApp->SetReturnFromRun(true);									// when the program exits there will be no error message
			TLogFileReader *lfr = new TLogFileReader();
			lfr->ReadFile(inputPath);
			//lfr->Print(cout);
			TLogStorage *ls = new TLogStorage(lfr->GetStorage());
			delete lfr;
			// Do sth with ls ...

		cout << "Finished..." << endl;
		theApp->Run();
	}

	return 0;
}
