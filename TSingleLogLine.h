/*
 * TSingleLogLine.h
 *
 *  Created on: Feb 28, 2019
 *      Author: mss
 */

#ifndef TSINGLELOGLINE_H_
#define TSINGLELOGLINE_H_

#include <string>
#include <iostream>
#include <iomanip>

namespace std {

class TSingleLogLine {
private:
	string timeStamp;
	string boardName;
	int channel;
	string parName;
	float value;

public:
	TSingleLogLine();
	virtual ~TSingleLogLine();

	// Setters and getters
	const string& GetBoardName() const;
	void SetBoardName(const string& boardName);
	int GetChannel() const;
	void SetChannel(int channel);
	const string& GetTimeStamp() const;
	void SetTimeStamp(const string& timeStamp);
	const string& GetParName() const;
	void SetParName(const string& parName);
	float GetValue() const;
	void SetValue(float value);

	void Print(ostream &out);
};

} /* namespace std */

#endif /* TSINGLELOGLINE_H_ */
