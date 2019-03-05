/*
 * TLogStorage.cpp
 *
 *  Created on: Feb 28, 2019
 *      Author: mss
 */

#include "TLogStorage.h"

namespace std {

TLogStorage::TLogStorage(vector<TSingleLogLine> *vRawLog) {
	globalMinTime = 0;
	globalMaxTime = 0;
	this->vRawLog = vRawLog;
	if(GenerateStructure() < 0){
		return;
	}
	SortStructure();
}

TLogStorage::~TLogStorage() {
	// TODO Auto-generated destructor stub
}

void TLogStorage::GenerateListOfUniques(){
	if(vRawLog->size()>0){
		vUniqueBoardName.push_back(vRawLog->at(0).GetBoardName());
		vUniqueChannelNumber.push_back(vRawLog->at(0).GetChannel());
		vUniqueParamName.push_back(vRawLog->at(0).GetParName());
	}
	else{
		cerr << "<E> TLogStorage::GenerateListOfUniques: No entries in raw log." << endl;
		return;
	}
	for(uint32_t iraw=1; iraw<vRawLog->size(); iraw++){
		string rawboardname = vRawLog->at(iraw).GetBoardName();
		int rawchannel = vRawLog->at(iraw).GetChannel();
		string rawparname = vRawLog->at(iraw).GetParName();
		bool isTripletSame = false;
		for(uint32_t i=0; i<vUniqueBoardName.size(); i++){
			string *uboardname = &(vUniqueBoardName.at(i));
			int uchannel = vUniqueChannelNumber.at(i);
			string *uparname = &(vUniqueParamName.at(i));
			if((rawboardname.compare(*uboardname) == 0) && (rawchannel == uchannel) && (rawparname.compare(*uparname) == 0)){
				isTripletSame = true;
			}
		}
		if(!isTripletSame){
			vUniqueBoardName.push_back(rawboardname);
			vUniqueChannelNumber.push_back(rawchannel);
			vUniqueParamName.push_back(rawparname);
		}
	}
}

// Sort the structure
void TLogStorage::SortStructure(){
	sort(vLog.begin(), vLog.end(), LogChannel::compare);
	for(uint32_t ilog=0; ilog<vLog.size(); ilog++){
		vector<LogParam>* vParam = &(vLog.at(ilog).vParam);
		sort(vParam->begin(), vParam->end(), LogParam::compare);
	}
}

int TLogStorage::GenerateStructure() {
	GenerateListOfUniques();

	// Fill vLog structure
	for(uint32_t i=0; i<vUniqueBoardName.size(); i++){
		string *ubn = &(vUniqueBoardName.at(i));
		int *ucn = &(vUniqueChannelNumber.at(i));
		string *upn = &(vUniqueParamName.at(i));
		if(i==0){
			LogChannel *lch = new LogChannel;
			lch->boardName = *ubn;
			lch->channel = *ucn;
			LogParam *lp = new LogParam;
			lp->parName = *upn;
			lch->vParam.push_back(*lp);
			vLog.push_back(*lch);
		}
		else{
			int loopSafetyCnt=0;
			while(true){
				loopSafetyCnt++;
				bool boardChannelPresent = false;
				bool paramPresent = false;
				vector<LogParam>* vParam = NULL;
				for(uint16_t ilog=0; ilog<vLog.size(); ilog++){
					//cout << i << "/" << vUniqueBoardName.size() << " - " << ilog << " " << vLog.at(ilog).boardName << " -- " << (*ubn) << "  \t| ";
					//cout << vLog.at(ilog).channel << " -- " << (*ucn) << endl;

					if((vLog.at(ilog).boardName.compare(*ubn) == 0) && (vLog.at(ilog).channel == (*ucn))){
						boardChannelPresent = true;
						// If board channel is found then try to find a corresponding parameter within this board-channel
						vParam = &(vLog.at(ilog).vParam);
						for(uint16_t ipar=0; ipar<vParam->size(); ipar++){
							if(vParam->at(ipar).parName.compare(*upn) == 0){
								paramPresent = true;
								break;
							}
						}
						break;
					}
				}

				// For debugging uncomment to see how adding proceeds
				/*cout << i << "-" << loopSafetyCnt << "  " << true << boardChannelPresent << endl;
				cout << "  Unique: " << *ubn << "  " << *ucn << "  " << *upn << endl;
				for(int ilog=0; ilog<vLog.size(); ilog++){
					cout << "  " << vLog.at(ilog).boardName << " " << vLog.at(ilog).channel;
					for(int ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
						cout << "  " << vLog.at(ilog).vParam.at(ipar).parName << "  ";
					}
					cout << endl;
				}
				cout << endl;
				*/

				if(!boardChannelPresent){
					LogChannel *lch = new LogChannel;
					lch->boardName = *ubn;
					lch->channel = *ucn;
					LogParam *lp = new LogParam;
					lp->parName = *upn;
					lch->vParam.push_back(*lp);
					vLog.push_back(*lch);
				}
				else if(!paramPresent){
					if(vParam == NULL){
						cerr << "<E> TLogStorage::TLogStorage(): vParam pointer was not assigned an object!" << endl;
						return -1;
					}
					LogParam *lp = new LogParam;
					lp->parName = *upn;
					vParam->push_back(*lp);
				}
				else{ // all info was added in the structure, end infinite loop
					break;
				}

				if(loopSafetyCnt > 1e3){
					cerr << "<E> TLogStorage::TLogStorage(): Something went wrong and probably infinite loop started..." << endl;
					return -2;
				}
			}
		}
	}
	return 0;
}

time_t TLogStorage::GetTime(string& rawTimeStamp){
	// year
	int iyear0 = 0;
	int iyear1 = rawTimeStamp.find_first_of('-');
	int imonth0 = iyear1+1;
	int imonth1 = rawTimeStamp.find_first_of('-', imonth0);
	int iday0 = imonth1+1;
	int iday1 = rawTimeStamp.find_first_of('T', iday0);
	int ihour0 = iday1+1;
	int ihour1 = rawTimeStamp.find_first_of(':', ihour0);
	int imin0 = ihour1+1;
	int imin1 = rawTimeStamp.find_first_of(':', imin0);
	int isec0 = imin1+1;

	struct tm timestamp;
	stringstream ss;

	ss << rawTimeStamp.substr(iyear0, iyear1-iyear0);
	ss >> timestamp.tm_year;
	timestamp.tm_year -= 1900;
	ss.clear();

	ss << rawTimeStamp.substr(imonth0, imonth1-imonth0);
	ss >> timestamp.tm_mon;
	timestamp.tm_mon -= 1;
	ss.clear();

	ss << rawTimeStamp.substr(iday0, iday1-iday0);
	ss >> timestamp.tm_mday;
	ss.clear();

	ss << rawTimeStamp.substr(ihour0, ihour1-ihour0);
	ss >> timestamp.tm_hour;
	ss.clear();

	ss << rawTimeStamp.substr(imin0, imin1-imin0);
	ss >> timestamp.tm_min;
	ss.clear();

	ss << rawTimeStamp.substr(isec0);
	ss >> timestamp.tm_sec;
	ss.clear();

	time_t t = mktime(&timestamp);
	return t;
}

void TLogStorage::FillStructure(){
	for(uint32_t iraw=0; iraw<vRawLog->size(); iraw++){
		string rawBoardName = vRawLog->at(iraw).GetBoardName();
		int    rawChannel   = vRawLog->at(iraw).GetChannel();
		string rawParName   = vRawLog->at(iraw).GetParName();
		string rawTimeStamp = vRawLog->at(iraw).GetTimeStamp();
		float  rawValue     = vRawLog->at(iraw).GetValue();
		for(uint32_t ilog=0; ilog<vLog.size(); ilog++){
			LogChannel* lc = &(vLog.at(ilog));
			if((rawBoardName.compare(lc->boardName) == 0) && (rawChannel == lc->channel)){
				for(uint32_t ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
					LogParam* lp = &(lc->vParam.at(ipar));
					if(rawParName.compare(lp->parName) == 0){
						LogInfo* li = new LogInfo();
						li->timeStamp = GetTime(rawTimeStamp);
						li->value = rawValue;
						lp->vVal.push_back(li);
						break;
					}
				}
				break;
			}
		}
	}
}

void TLogStorage::AnalyseMinMax(){
	for(uint32_t ilog=0; ilog<vLog.size(); ilog++){
		LogChannel* lc = &(vLog.at(ilog));
		for(uint32_t ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
			LogParam* lp = &(lc->vParam.at(ipar));
			for(uint32_t ival=0; ival<lp->vVal.size(); ival++){
				LogInfo* li = lp->vVal.at(ival);
				if((lp->minTime > li->timeStamp) || (lp->minTime == -999)){
					lp->minTime = li->timeStamp;
				}
				if((lp->maxTime < li->timeStamp) || (lp->maxTime == -999)){
					lp->maxTime = li->timeStamp;
				}
				if((lp->minValue > li->value) || (lp->minValue == -999)){
					lp->minValue = li->value;
				}
				if((lp->maxValue < li->value) || (lp->maxValue == -999)){
					lp->maxValue = li->value;
				}
			}
		}
	}

	// Initialize time global min and global max
	if(vLog.size()>0){
		if(vLog.at(0).vParam.size()>0){
			globalMaxTime = vLog.at(0).vParam.at(0).maxTime;
			globalMinTime = vLog.at(0).vParam.at(0).minTime;
		}
	}
	for(uint32_t ilog=0; ilog<vLog.size(); ilog++){
		LogChannel* lc = &(vLog.at(ilog));
		for(uint32_t ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
			LogParam* lp = &(lc->vParam.at(ipar));
			if(globalMaxTime < lp->maxTime){
				globalMaxTime = lp->maxTime;
			}
			if(globalMinTime > lp->minTime){
				globalMinTime = lp->minTime;
			}
		}
	}
}

void TLogStorage::CreateGraphs(){
	for(uint32_t ilog=0; ilog<vLog.size(); ilog++){
		LogChannel* lc = &(vLog.at(ilog));
		for(uint32_t ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
			LogParam* lp = &(lc->vParam.at(ipar));
			stringstream ssname;
			ssname << lc->boardName << "_" << lc->channel << "_" << lp->parName;
			lp->gr = new TGraph(lp->vVal.size());
			lp->gr->SetName(ssname.str().c_str());
			lp->gr->SetTitle(ssname.str().c_str());
		}
	}
}

int TLogStorage::FillHistos(){
	FillStructure();
	AnalyseMinMax();
	CreateGraphs();

	//Fill histos
	for(uint32_t ilog=0; ilog<vLog.size(); ilog++){
		LogChannel* lc = &(vLog.at(ilog));
		for(uint32_t ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
			LogParam* lp = &(lc->vParam.at(ipar));
			for(uint32_t ival=0; ival<lp->vVal.size(); ival++){
				LogInfo* li = lp->vVal.at(ival);
				lp->gr->SetPoint(ival, li->timeStamp, li->value);
			}
		}
	}

	return 0;
}

void TLogStorage::PrintStructure(){
	cout << "  <I> TLogStorage::PrintStructure(): vLog structure" << endl;
	for(uint32_t ilog=0; ilog<vLog.size(); ilog++){
		LogChannel* lc = &(vLog.at(ilog));
		cout << "      " << lc->boardName << " " << lc->channel << "    ";
		for(uint32_t ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
			cout << lc->vParam.at(ipar).parName << "  ";
		}
		cout << endl;
	}
}

void TLogStorage::PrintContents(){
	cout << "  <I> TLogStorage::PrintContents(): vLog content" << endl;
	for(uint32_t ilog=0; ilog<vLog.size(); ilog++){
		LogChannel* lc = &(vLog.at(ilog));
		cout << "      " << lc->boardName << " " << lc->channel << "    " << endl;
		for(uint32_t ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
			LogParam* lp = &(lc->vParam.at(ipar));
			cout << "        " << lp->parName << endl;
			for(uint32_t ival=0; ival<lp->vVal.size(); ival++){
				cout << "          " << lp->vVal.at(ival)->timeStamp << ": " << lp->vVal.at(ival)->value << endl;
			}
		}
	}
}

vector<LogChannel>* TLogStorage::GetLog(){
	return &vLog;
}

void TLogStorage::GetUniques(vector<string> &vUniqueBoardName, vector<int> &vUniqueChannelNumber, vector<string> &vUniqueParamName){
	vUniqueBoardName = this->vUniqueBoardName;
	vUniqueChannelNumber = this->vUniqueChannelNumber;
	vUniqueParamName = this->vUniqueParamName;
}

time_t TLogStorage::GetGlobalMaxTime(){
	return globalMaxTime;
}

time_t TLogStorage::GetGlobalMinTime(){
	return globalMinTime;
}

} /* namespace std */
