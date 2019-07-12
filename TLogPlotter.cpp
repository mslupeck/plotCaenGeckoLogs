/*
 * TLogPlotter.cpp
 *
 *  Created on: Mar 1, 2019
 *      Author: mss
 */

#include "TLogPlotter.h"

namespace std {

TLogPlotter::TLogPlotter(TLogStorage *ls) {
	logStorage = ls;
}

TLogPlotter::~TLogPlotter() {
	// TODO Auto-generated destructor stub
}

void TLogPlotter::Plot(string &selParam, int t0) {
	// Not really unique one by one, but rather each triplet of them is unique
	vector<string> vUniqueParamName;
	vector<string> vUniqueBoardName;
	vector<int> vUniqueChannelNumber;
	logStorage->GetUniques(vUniqueBoardName, vUniqueChannelNumber, vUniqueParamName);
	// Select only a single parameter to plot
	if(selParam.size() > 0){ // if selection was made in the CLI
		vUniqueParamName.clear();
		vUniqueParamName.push_back(selParam);
	}

	vector<LogChannel>* vLog = logStorage->GetLog();

	// Extract singularly unique parameters
	vector<string> vParName;
	vParName.push_back(vUniqueParamName.at(0));
	for(uint32_t ipar=0; ipar<vUniqueParamName.size(); ipar++){
		bool parExists = false;
		for(uint32_t iparUnique=0; iparUnique<vParName.size(); iparUnique++){
			if(vParName.at(iparUnique).compare(vUniqueParamName.at(ipar)) == 0){
				parExists = true;
				break;
			}
		}
		if(!parExists){
			vParName.push_back(vUniqueParamName.at(ipar));
		}
	}

	// Find max and min values for each of the params for all possible channels
	vector<float> vvalMax;
	vector<float> vvalMin;
	for(uint32_t iparUnique=0; iparUnique<vParName.size(); iparUnique++){
		string* uniquePar = &(vParName.at(iparUnique));
		float valMax=-1, valMin=-1;
		bool first = true;
		for(uint32_t ilog=0; ilog<vLog->size(); ilog++){
			LogChannel* lc = &(vLog->at(ilog));
			for(uint32_t ipar=0; ipar<vLog->at(ilog).vParam.size(); ipar++){
				LogParam* lp = &(lc->vParam.at(ipar));
				if(lp->parName.compare(*uniquePar) == 0){
					if(first){
						valMax = lp->maxValue;
						valMin = lp->minValue;
						first = false;
					}
					else{
						if(valMax < lp->maxValue){
							valMax = lp->maxValue;
						}
						if(valMin > lp->minValue){
							valMin = lp->minValue;
						}
					}
				}
			}
		}
		vvalMax.push_back(valMax);
		vvalMin.push_back(valMin);
	}

	string xAxisTitle = "Time [h] since ";
	time_t globMinTime = logStorage->GetGlobalMinTime();
	if(t0 != -999999){ // if t0 set from CLI
		globMinTime = t0;
	}
	// Lower the min time, so that it starts from the last full hour
	uint32_t globTimeOffset = globMinTime%3600;
	globMinTime = globMinTime - globTimeOffset;
	xAxisTitle += ctime(&globMinTime);
	cout << "  <I> TLogPlotter::Plot(): Global start time set to: " << globMinTime << "   -> " << ctime(&globMinTime) << endl;

	// Offset all data in graphs so that it starts from 0 and is in hours
	for(uint32_t ilog=0; ilog<vLog->size(); ilog++){
		LogChannel* lc = &(vLog->at(ilog));
		for(uint32_t ipar=0; ipar<vLog->at(ilog).vParam.size(); ipar++){
			TGraph* gr = lc->vParam.at(ipar).gr;
			for(uint32_t i=0; i<gr->GetN(); i++){
				double x=0, y=0;
				gr->GetPoint(i, x, y);
				double newTime = x-globMinTime;
				newTime /= 3600; // convert units [s -> h]
				gr->SetPoint(i, newTime, y);
			}
		}
	}

	vector<Color_t> vCol;
	vCol.push_back(kRed);
	vCol.push_back(kPink-9);
	vCol.push_back(kViolet+2);
	vCol.push_back(kBlue);
	vCol.push_back(kAzure+2);
	vCol.push_back(kCyan+1);
	vCol.push_back(kTeal-5);
	vCol.push_back(kGreen+2);
	vCol.push_back(kSpring+3);
	vCol.push_back(kYellow+2);
	vCol.push_back(kOrange+2);
	vCol.push_back(kBlack);
	vCol.push_back(kGray);

	float lmargin = 0.1;
	float rmargin = 0.2;
	float bmargin = 0.1;
	float tmargin = 0.07;
	float fontsize = 0.035;

	for(uint32_t iparUnique=0; iparUnique<vParName.size(); iparUnique++){
		string* uniquePar = &(vParName.at(iparUnique));
		stringstream ss;
		ss << "c" << (*uniquePar);
		TCanvas *c = new TCanvas(ss.str().c_str(), ss.str().c_str(), 950, 680);
		gPad->SetMargin(lmargin, rmargin, bmargin, tmargin);
		gPad->SetGrid(1,1);
		TLegend* leg = new TLegend(1.0-rmargin, bmargin+0.1, 1.0, 1.0-tmargin);
		bool first = true;
		for(uint32_t ilog=0; ilog<vLog->size(); ilog++){
			LogChannel* lc = &(vLog->at(ilog));
			for(uint32_t ipar=0; ipar<vLog->at(ilog).vParam.size(); ipar++){
				LogParam* lp = &(lc->vParam.at(ipar));
				if(lp->parName.compare(*uniquePar) == 0){
					string legEntryName = lp->gr->GetName();
					legEntryName = legEntryName.substr(0,legEntryName.find_last_of('_'));
					leg->AddEntry(lp->gr, legEntryName.c_str(), "lep");
					lp->gr->SetLineWidth(2);
					lp->gr->SetMarkerColor(vCol.at(ilog));
					lp->gr->SetLineColor(vCol.at(ilog));
					lp->gr->SetMarkerStyle(8);
					lp->gr->SetMarkerSize(1);
					if(first){
						TGraph *grdummy = new TGraph(2);
						grdummy->SetPoint(0, 0, vvalMin.at(iparUnique));
						grdummy->SetPoint(1, (logStorage->GetGlobalMaxTime()-globMinTime)/3600+1, vvalMax.at(iparUnique)*1.1);
						grdummy->SetLineWidth(0);
						grdummy->SetMarkerColor(kWhite);
						grdummy->SetMarkerStyle(1);
						grdummy->Draw("AP");

						grdummy->SetTitle((*uniquePar).c_str());
						//grdummy->GetXaxis()->SetRangeUser(0, logStorage->GetGlobalMaxTime()-logStorage->GetGlobalMinTime());
						//grdummy->GetYaxis()->SetRangeUser(vvalMin.at(iparUnique), vvalMax.at(iparUnique)*1.1);
						grdummy->GetXaxis()->SetTitle(xAxisTitle.c_str());
						grdummy->GetYaxis()->SetTitle("Value");
						grdummy->GetXaxis()->SetTitleOffset(0.95);
						grdummy->GetXaxis()->SetTitleSize(fontsize*1.15);
						grdummy->GetXaxis()->SetLabelSize(fontsize);
						grdummy->GetYaxis()->SetTitleOffset(1.0);
						grdummy->GetYaxis()->SetTitleSize(fontsize*1.15);
						grdummy->GetYaxis()->SetLabelSize(fontsize);
						first = false;
					}
					lp->gr->Draw("L sames");
					break;
				}
			}
		}
		leg->Draw();
	}
}

} /* namespace std */
