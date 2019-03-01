/*
 * TSingleLogLine.cpp
 *
 *  Created on: Feb 28, 2019
 *      Author: mss
 */

#include "TSingleLogLine.h"

namespace std {

TSingleLogLine::TSingleLogLine() {
	timeStamp = "X";
	boardName = "X";
	channel = -1;
	parName = "X";
	value = -1;
}

TSingleLogLine::~TSingleLogLine() {
	// Nothing needed here
}

const string& TSingleLogLine::GetBoardName() const {
	return boardName;
}

void TSingleLogLine::SetBoardName(const string& boardName) {
	this->boardName = boardName;
}

int TSingleLogLine::GetChannel() const {
	return channel;
}

void TSingleLogLine::SetChannel(int channel) {
	this->channel = channel;
}

const string& TSingleLogLine::GetTimeStamp() const {
	return timeStamp;
}

void TSingleLogLine::SetTimeStamp(const string& timeStamp) {
	this->timeStamp = timeStamp;
}

const string& TSingleLogLine::GetParName() const {
	return parName;
}

void TSingleLogLine::SetParName(const string& parName) {
	this->parName = parName;
}

float TSingleLogLine::GetValue() const {
	return value;
}

void TSingleLogLine::SetValue(float value) {
	this->value = value;
}

void TSingleLogLine::Print(ostream &out){
	out << left << setw(22) << TSingleLogLine::timeStamp;
	out << left << setw(9) << TSingleLogLine::boardName;
	out << right << setw(2) << TSingleLogLine::channel << "  ";
	out << left << setw(7) << TSingleLogLine::parName;
	out << right << setw(8) << TSingleLogLine::value;
	out << endl;
}

} /* namespace std */
