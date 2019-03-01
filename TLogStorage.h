/*
 * TLogStorage.h
 *
 *  Created on: Feb 28, 2019
 *      Author: mss
 */

#ifndef TLOGSTORAGE_H_
#define TLOGSTORAGE_H_

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "TSingleLogLine.h"
#include <ctime>

namespace std {

struct LogInfo{
	struct tm timeStamp;
	float value;
};

struct LogParam{
	string parName;
	vector<LogInfo> vVal;
};

struct LogChannel{
	string boadName;
	int channel;
	vector<LogParam> vParam;
};

class TLogStorage {
private:
	vector<LogChannel> vLog;

	void GenerateListOfUniques(vector<TSingleLogLine> &vRawLog, vector<string> &vUniqueParamName, vector<string> &vUniqueBoardName, vector<int> &vUniqueChannelNumber);

public:
	TLogStorage(vector<TSingleLogLine> &vRawLog);
	virtual ~TLogStorage();
};

} /* namespace std */

#endif /* TLOGSTORAGE_H_ */
