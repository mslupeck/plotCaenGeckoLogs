/*
 * TLogStorage.h
 *
 *  Created on: Feb 28, 2019
 *      Author: mss
 */

#ifndef TLOGSTORAGE_H_
#define TLOGSTORAGE_H_

#include <bits/stdc++.h> // sort
#include <stdint.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "TSingleLogLine.h"
#include <ctime>
#include <TGraph.h>

namespace std {

struct LogInfo{
	time_t timeStamp;
	float value;
};

struct LogParam{
	string parName;
	vector<LogInfo*> vVal;
	time_t minTime;
	time_t maxTime;
	time_t minValue;
	time_t maxValue;
	TGraph* gr;

	LogParam(){
		minTime = -999;
		maxTime = -999;
		minValue = -999;
		maxValue = -999;
		gr = NULL;
	}

	static bool compare(const LogParam &l1, const LogParam &l2){
	    return (l1.parName < l2.parName);
	}
};

struct LogChannel{
	string boardName;
	int channel;
	vector<LogParam> vParam;

	static bool compare(const LogChannel &l1, const LogChannel &l2){
		if(l1.boardName.compare(l2.boardName) == 0){
			return l1.channel < l2.channel;
		}
		else{
			return l1.boardName < l2.boardName;
		}
	}
};

class TLogStorage {
private:
	vector<TSingleLogLine>* vRawLog;
	vector<LogChannel> vLog;
	time_t globalMinTime;
	time_t globalMaxTime;

	vector<string> vUniqueParamName;
	vector<string> vUniqueBoardName;
	vector<int> vUniqueChannelNumber;

	void GenerateListOfUniques();
	int GenerateStructure();
	void SortStructure();
	time_t GetTime(string& rawTimeStamp);
	void FillStructure();
	void AnalyseMinMax();
	void CreateGraphs();

public:
	TLogStorage(vector<TSingleLogLine> *vRawLog);
	virtual ~TLogStorage();

	int FillHistos();
	void PrintStructure();
	void PrintContents();

	vector<LogChannel>* GetLog();

	void GetUniques(vector<string> &vUniqueBoardName, vector<int> &vUniqueChannelNumber, vector<string> &vUniqueParamName);
	time_t GetGlobalMinTime();
	time_t GetGlobalMaxTime();
};

} /* namespace std */

#endif /* TLOGSTORAGE_H_ */
