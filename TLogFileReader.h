/*
 * TLogFileReader.h
 *
 *  Created on: Feb 28, 2019
 *      Author: mss
 */

#ifndef TLOGFILEREADER_H_
#define TLOGFILEREADER_H_

#include "TSingleLogLine.h"
#include "TAsciiFileReader.cpp" // cpp needed because TAsciiFileReader is a template

namespace std {

class TLogFileReader : public TAsciiFileReader<TSingleLogLine> {
private:
	const int nCols = 10;
	void InitLutVectors();										// VIIH = virtual inherited, implemented here
	void FillLutVectors();										// VIIH

public:
	TLogFileReader();
	virtual ~TLogFileReader();

	// Declarations of virtual methods declared in parent class, which are defined here
	int16_t ReadLine(const string& sLine, const int32_t iLine);	// VIIH
	int16_t Print(ostream&);									// VIIH
	void ResetData();											// VIIH
};

} /* namespace std */

#endif /* TLOGFILEREADER_H_ */
