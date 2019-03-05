/*
 * TLogPlotter.h
 *
 *  Created on: Mar 1, 2019
 *      Author: mss
 */

#ifndef TLOGPLOTTER_H_
#define TLOGPLOTTER_H_

#include "TLogStorage.h"
#include <TCanvas.h>
#include <TPad.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TLegend.h>

namespace std {

class TLogPlotter {
private:
	TLogStorage* logStorage;

public:
	TLogPlotter(TLogStorage* ls);
	virtual ~TLogPlotter();

	void Plot();
};

} /* namespace std */

#endif /* TLOGPLOTTER_H_ */
