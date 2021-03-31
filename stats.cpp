#include "stats.h"

#include "stats.h"

stats::stats(PNG &im) {
    int width = im.width();
    int height = im.height();
    sumRed.resize(height);
    sumGreen.resize(height);
    sumBlue.resize(height);
    sumsqRed.resize(height);
    sumsqGreen.resize(height);
    sumsqBlue.resize(height);

    // build sumTable[columns][rows]
    for (unsigned int j = 0; j < im.height(); j++) {
        sumRed[j].resize(width);
        sumGreen[j].resize(width);
        sumBlue[j].resize(width);
        sumsqRed[j].resize(width);
        sumsqGreen[j].resize(width);
        sumsqBlue[j].resize(width);

        for (unsigned int i = 0; i < im.width(); i++) {
            sumRed[j][i] = buildSumTable(&sumRed, im.getPixel(i, j)->r, i, j);
            sumGreen[j][i] = buildSumTable(&sumGreen, im.getPixel(i, j)->g, i, j);
            sumBlue[j][i] = buildSumTable(&sumBlue, im.getPixel(i, j)->b, i, j);
            sumsqRed[j][i] = buildSumTable(&sumsqRed, pow(im.getPixel(i, j)->r, 2), i, j);
            sumsqGreen[j][i] = buildSumTable(&sumsqGreen, pow(im.getPixel(i, j)->g, 2), i, j);
            sumsqBlue[j][i] = buildSumTable(&sumsqBlue, pow(im.getPixel(i, j)->b, 2), i, j);
        }
    }
}

// sums of all pixel values in one colour channel
long stats::getSum(char channel, pair<int, int> ul, int w, int h) {
    long retVal;
    if (channel == *"r") retVal = findSum(ul, w, h, &sumRed);
    if (channel == *"g") retVal = findSum(ul, w, h, &sumGreen);
    if (channel == *"b") retVal = findSum(ul, w, h, &sumBlue);
    return retVal;
}

// sums of squares of all pixel values in one colour channel
long stats::getSumSq(char channel, pair<int, int> ul, int w, int h) {
    long retVal;
    if (channel == *"r") retVal = findSum(ul, w, h, &sumsqRed);
    if (channel == *"g") retVal = findSum(ul, w, h, &sumsqGreen);
    if (channel == *"b") retVal = findSum(ul, w, h, &sumsqBlue);

    return retVal;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int w, int h) {
    if(w == 0 || h == 0) return 0;

	long r = getSum('r', ul, w, h);
	long g = getSum('g', ul, w, h);
	long b = getSum('b', ul, w, h);
	long rSq = getSumSq('r', ul, w, h);
	long gSq = getSumSq('g', ul, w, h);
	long bSq = getSumSq('b', ul, w, h);
	double area = w * h;

    double rv = rSq - (pow(r,2)/area);
    double gv = gSq - (pow(g,2)/area);
    double bv = bSq - (pow(b,2)/area);

    return rv + gv + bv;

    // cout << "rSq = " << rSq << endl;
    // cout << "gSq = " << gSq << endl;
    // cout << "bSq = " << bSq << endl;
    // cout << "rSq + gSq + bSq = " << rSq + gSq + bSq << endl;
    // cout << "pow(r,2) + pow(g,2) + pow(b,2) = " << pow(r,2) + pow(g,2) + pow(b,2) << endl;
    // cout << "w = " << w << endl;
    // cout << "h = " << h << endl;
	
	// return  (rSq + gSq + bSq) - ((pow(r, 2) + pow(g, 2) + pow(b, 2)) / area);
}

RGBAPixel stats::getAvg(pair<int, int> ul, int w, int h) {
    if(w == 0 || h == 0) return RGBAPixel(); // default black pixel
    double area = (double) w * (double) h;
	int rAvg = findSum(ul, w, h, &sumRed) / area;
	int gAvg = findSum(ul, w, h, &sumGreen) / area;
	int bAvg = findSum(ul, w, h, &sumBlue) / area;
	return RGBAPixel(rAvg, gAvg, bAvg);
}

long stats::buildSumTable(vector< vector<long> > *sumTable, long currVal, int i, int j) {
    if (j > 0) currVal += (*sumTable)[j - 1][i];
    if (i > 0) currVal += (*sumTable)[j][i - 1];
    if (i > 0 && j > 0) currVal -= (*sumTable)[j - 1][i - 1];
    return currVal;
}

long stats::findSum(pair<int,int> ul, int w, int h, vector< vector<long> > *sumTable) {
    // if top left corner, return sumTable top left corner
    if (ul.first == 0 && ul.second == 0) return (*sumTable)[h - 1][w - 1];
    // if top edge, 
    if (ul.second == 0) return (*sumTable)[h - 1][ul.first + w - 1] - (*sumTable)[h - 1][ul.first - 1];
    // if bottom edge
    if (ul.first == 0) return (*sumTable)[ul.second + h - 1][w - 1] - (*sumTable)[ul.second - 1][w - 1];

	return (*sumTable)[ul.second + h - 1][ul.first + w - 1] - 
        (*sumTable)[ul.second + h - 1][ul.first - 1] - 
        (*sumTable)[ul.second - 1][ul.first + w - 1] + 
        (*sumTable)[ul.second - 1][ul.first - 1];
}