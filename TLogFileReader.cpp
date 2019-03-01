/*
 * TLogFileReader.cpp
 *
 *  Created on: Feb 28, 2019
 *      Author: mss
 */

#include "TLogFileReader.h"


namespace std {

TLogFileReader::TLogFileReader() {
	// Nothing here
}

TLogFileReader::~TLogFileReader() {
	// Nothing here
}

int16_t TLogFileReader::ReadLine(const string& sLine, const int32_t iLine){
	stringstream ssLine(sLine);
	int16_t iCol=0;
	if(sLine.find_first_of("#")!=0){ //if not a comment
		string sWord;
		while(!ssLine.eof()){ // first parsing to count the number of columns
			ssLine >> sWord;
			iCol++;
		}
		if(iCol != nCols+1){
			cerr << "<E> TLogFileReader::ReadLine(): Wrong number of columns when parsing line " << iLine << ": " << iCol << " != " << nCols << endl;
			cerr << "                                -> " << ssLine.str() << endl;
			return -1;
		}
		else{
			ssLine.clear();
			ssLine.seekg(ios_base::beg);	// go back to the beginning of the line
			TSingleLogLine singleLogLine;	// temp object to store read line in
			string sTimeStamp, sBoardName, sParName, tmp, sCh, sVal;
			int iCh;
			float fVal;
			ssLine >> sTimeStamp;
			singleLogLine.SetTimeStamp(sTimeStamp.substr(1, sTimeStamp.size()-3)); // remove the brackets and colon at the end
			ssLine >> sBoardName;
			singleLogLine.SetBoardName(sBoardName.substr(1, sBoardName.size()-2)); // remove the surrounding brackets
			ssLine >> tmp;
			ssLine >> tmp;
			ssLine >> tmp;
			if(tmp != "ch"){
				cerr << "<E> TLogFileReader::ReadLine(): Consistency problems (column 'ch' missing) at line " << iLine << endl;
				cerr << sLine << endl;
				return -3;
			}

			// Extraction of channel int from string with brackets
			ssLine >> sCh;
			sCh = sCh.substr(1, sCh.size()-2);
			stringstream ssCh(sCh);
			ssCh >> iCh;
			if(!(ssCh.good() || ssCh.eof())){	// check if there were stringstream conversion errors
				vStorage.clear();
				cerr << "<E> TLogFileReader::ReadLine(): Channel conversion problems at line " << iLine;
				cerr << " (eof,fail,bad)=(" << ssCh.eof() << "," << ssCh.fail() << "," << ssCh.bad() << ")" << endl;
				cerr << sCh << endl;
				return -4;
			}
			singleLogLine.SetChannel(iCh);

			ssLine >> tmp; // should be 'par' but no more checks at this point
			ssLine >> sParName;
			singleLogLine.SetParName(sParName.substr(1, sParName.size()-2)); // remove the surrounding brackets
			ssLine >> tmp; // should be 'val' but no more checks at this point

			// Extraction of value float from string with brackets and semicolon at the end
			ssLine >> sVal;
			sVal = sVal.substr(1, sVal.size()-3);
			stringstream ssVal(sVal);
			ssVal >> fVal;
			if(!(ssVal.good() || ssVal.eof())){	// check if there were stringstream conversion errors
				vStorage.clear();
				cerr << "<E> TLogFileReader::ReadLine(): Value conversion problems at line " << iLine;
				cerr << " (eof,fail,bad)=(" << ssVal.eof() << "," << ssVal.fail() << "," << ssVal.bad() << ")" << endl;
				cerr << sVal << endl;
				return -4;
			}
			singleLogLine.SetValue(fVal);

			vStorage.push_back(singleLogLine);
		}
	}
	return 0;
}

int16_t TLogFileReader::Print(ostream &out){
	cout << "Printing file contents: " << GetFilePath() << endl;
	for(uint32_t iline=0; iline<vStorage.size(); iline++){
		vStorage.at(iline).Print(out);
	}

	return 0;
}

void TLogFileReader::ResetData(){
	// Nothing to delete additionally to what base class handles
}
void TLogFileReader::InitLutVectors(){
	// Nothing to init
}
void TLogFileReader::FillLutVectors(){
	// Nothing to fill
}

} /* namespace std */
