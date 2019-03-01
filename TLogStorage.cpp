/*
 * TLogStorage.cpp
 *
 *  Created on: Feb 28, 2019
 *      Author: mss
 */

#include "TLogStorage.h"

namespace std {

TLogStorage::TLogStorage(vector<TSingleLogLine> &vRawLog) {
	vector<string> vUniqueParamName;
	vector<string> vUniqueBoardName;
	vector<int> vUniqueChannelNumber;

	GenerateListOfUniques(vRawLog, vUniqueParamName, vUniqueBoardName, vUniqueChannelNumber);

	// Fill vLog structure
	for(int i=0; i<vUniqueBoardName.size(); i++){
		string *ubn = &(vUniqueBoardName.at(i));
		int *ucn = &(vUniqueChannelNumber.at(i));
		string *upn = &(vUniqueParamName.at(i));
		if(i==0){
			LogChannel *lch = new LogChannel;
			lch->boadName = *ubn;
			lch->channel = *ucn;
			LogParam *lp = new LogParam;
			lp->parName = *upn;
			lch->vParam.push_back(*lp);
			vLog.push_back(*lch);
		}
		else{
			while(true){
				bool allBoardChannelPresent = false;
				bool allParamPresent = false;
				vector<LogParam>* vParam = NULL;
				for(uint16_t ilog=0; ilog<vLog.size(); ilog++){
					//cout << i << "/" << vUniqueBoardName.size() << " - " << ilog << " " << vLog.at(ilog).boadName << " -- " << (*ubn) << "  \t| ";
					//cout << vLog.at(ilog).channel << " -- " << (*ucn) << endl;
					if((vLog.at(ilog).boadName.compare(*ubn) != 0) || (vLog.at(ilog).channel != (*ucn))){
						allBoardChannelPresent = true;
					}
					else{
						vParam = &(vLog.at(ilog).vParam);
						for(uint16_t ipar=0; ipar<vParam->size(); ipar++){
							if(vParam->at(ipar).parName.compare(*upn) != 0){
								allParamPresent = true;
							}
						}
					}
				}

				cout << i << " " << true << allBoardChannelPresent << allParamPresent << endl;
				if(!allBoardChannelPresent){
					LogChannel *lch = new LogChannel;
					lch->boadName = *ubn;
					lch->channel = *ucn;
					LogParam *lp = new LogParam;
					lp->parName = *upn;
					lch->vParam.push_back(*lp);
					vLog.push_back(*lch);
				}
				else if(!allParamPresent){
					if(vParam == NULL){
						cerr << "<E> TLogStorage::TLogStorage(): vParam pointer was not assigned an object!" << endl;
						break;
					}
					LogParam *lp = new LogParam;
					lp->parName = *upn;
					vParam->push_back(*lp);
				}
				else{ // all info was added in the structure, end infinite loop
					break;
				}






			}
		}
	}

/*	// Print vLog structure
	for(int ilog=0; ilog<vLog.size(); ilog++){
		cout << vLog.at(ilog).boadName << " " << vLog.at(ilog).channel << endl;
		for(int ipar=0; ipar<vLog.at(ilog).vParam.size(); ipar++){
			cout << "  " << vLog.at(ilog).vParam.at(ipar).parName << endl;
		}
	}
	*/
}

TLogStorage::~TLogStorage() {
	// TODO Auto-generated destructor stub
}

void TLogStorage::GenerateListOfUniques(vector<TSingleLogLine> &vRawLog, vector<string> &vUniqueParamName, vector<string> &vUniqueBoardName, vector<int> &vUniqueChannelNumber){
	if(vRawLog.size()>0){
		vUniqueBoardName.push_back(vRawLog.at(0).GetBoardName());
		vUniqueChannelNumber.push_back(vRawLog.at(0).GetChannel());
		vUniqueParamName.push_back(vRawLog.at(0).GetParName());
	}
	else{
		cerr << "<E> TLogStorage::GenerateListOfUniques: No entries in raw log." << endl;
		return;
	}
	for(uint32_t iraw=1; iraw<vRawLog.size(); iraw++){
		string rawboardname = vRawLog.at(iraw).GetBoardName();
		int rawchannel = vRawLog.at(iraw).GetChannel();
		string rawparname = vRawLog.at(iraw).GetParName();
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


} /* namespace std */
