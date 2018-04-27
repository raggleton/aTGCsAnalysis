#ifndef PDFVARIATIONMAP_H
#define PDFVARIATIONMAP_H

struct range 
{
  double low;
  double high;
  range (){};
  range(double low_, double high_):low(low_),high(high_){};
  
};

//for reference it was confirmed what values should be taken out of the range:
// https://hypernews.cern.ch/HyperNews/CMS/get/generators/3001/1/1/1.html

extern const std::map<int, range> PDFVariationMap = {
	{292200, range(2001,2100)},
	{263000, range(11,110)},
	{-1, range(2001,2100)},
	{292000, range(2001,2100)},
	{260000, range(2001,2100)},
        // We make a new convention about signal range since new signal samples have the same nominal values but different ranges.
        // Use nominal value/100 for signal to distinguish it in the LUT from others.
        {292000/100, range(1010,1109)},
	{292200/100, range(1010,1109)}
};

#endif
