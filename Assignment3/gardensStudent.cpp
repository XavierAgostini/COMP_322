#include "gardens.h"
#include <stack>
#include <iostream>
#include <math.h>

 /* 0-credit */
Fraction ContinuedFraction::getApproximation(unsigned int k) const {

	Fraction toRet;

	    toRet.numerator = 1;

	    toRet.denominator = 0;



	    stack<cf_int> ints;

	    resetIterator();

	    while (!hasNoMore() && k-- > 0) {

	        ints.push(next());

	    }



	    while (!ints.empty()) {

	        cf_int x = ints.top();

	        cf_int n = toRet.numerator;

	        cf_int d = toRet.denominator;

	        toRet.numerator = x * n + d;

	        toRet.denominator = n;

	        ints.pop();

	    }

	    resetIterator();

	    return toRet;

}


RationalCF::RationalCF(Fraction f) : PeriodicCF({},{}) {

    if (f.numerator == 0) {
        fixedPart.push_back(0);
        return;
    }

    while (f.denominator != 0) {
        if (f.numerator != 0) {
            fixedPart.push_back(f.numerator / f.denominator);
        }
        auto tmp = f.numerator % f.denominator;
        f.numerator = f.denominator;
        f.denominator = tmp;
    }

}


RationalCF::~RationalCF() {
    //I didn't use new key word anywhere so I don't think I need to do anything
}

/* QUESTION 3*/

cf_int PeriodicCF::next() const {

	//take the sizes of the fixed and periodic part
	unsigned int fixedSize = fixedPart.size();
	unsigned int periodicSize = periodicPart.size();


		if(*iteratorPosition < fixedPart.size()) {
			return fixedPart[(*iteratorPosition)++];
			//return cf_int in fixed part then iterate position
		}

		else {
			//std::cout << "count: "<<count << std::endl;
			//return cf_int in periodic part
			//since its periodic we want to rap around the cf_int to return
			return periodicPart[((*iteratorPosition)++ - fixedSize) % periodicSize];
		}

}

bool PeriodicCF::hasNoMore() const {

	//hasNoMore : returns true iff the iterator reached the end
	unsigned fixedSize = fixedPart.size();
	unsigned periodicSize = periodicPart.size();

	return (*iteratorPosition < fixedSize) ? false : ((periodicSize > 0) ? false : true);
}

void PeriodicCF::resetIterator() const {
	// resetIterator: resets the iteration over the cf to the beginning; hence 0
	*iteratorPosition = 0;
}

PeriodicCF::~PeriodicCF() {
	//want to delete iteratorPostion as we now no longer need it
   delete iteratorPosition;

}



/* QUESTION 4*/

MagicBoxCF::MagicBoxCF(ContinuedFraction const* f, unsigned long aParam, unsigned long bParam) : boxedFraction(f), a(aParam), b(bParam) {
	//	cout<<"inside the box"<<endl;
	//create a magic box
	//from last assignment:  int i = 0, j = 1, k = 1, l = 0;

	mbnums = new cf_int[4];	//pointer to arrray were we are storing the magic box
	resetIterator();		//initialize/reset the magic box

}

cf_int MagicBoxCF::next() const {
	//solution from assignment #2
	while( ((mbnums[2] == 0 || mbnums[3] == 0) && !(mbnums[2] == 0 && mbnums[3] == 0)) ||
	            (mbnums[2] != 0 && mbnums[3] != 0 && mbnums[0] / mbnums[2] != mbnums[1] / mbnums[3]) ) {
			// while the indeces are not yet ready to spit q
			if(boxedFraction->hasNoMore()) {	//check to make sure we are still inside the continued fraction
	            // no more integers to spit from cf
				mbnums[0] = mbnums[1];
				mbnums[2] = mbnums[3];
	            continue;
			}
			//read p

	        int p = boxedFraction->next();	//get p by using next() we created before

			//change box accordingly
			int i = mbnums[1],
	            j = mbnums[0] + mbnums[1] * p,
				k = mbnums[3],
				l = mbnums[2] + mbnums[3] * p;
			mbnums[0] = i;
			mbnums[1] = j;
			mbnums[2] = k;
			mbnums[3] = l;
	    }
	    // if we reached the end of the cf - return -1
		if(mbnums[2] == 0 && mbnums[3] == 0) return -1;

	    // return q and change box accordingly
		int q = mbnums[0] / mbnums[2];
		int i = mbnums[2],
	        j = mbnums[3],
			k = mbnums[0] - mbnums[2] * q,
			l = mbnums[1] - mbnums[3] * q;
		mbnums[0] = i;
		mbnums[1] = j;
		mbnums[2] = k;
		mbnums[3] = l;
		return q;


}

bool MagicBoxCF::hasNoMore() const {
	//we want to stop when k==0 and l==0
	if(mbnums[2]==0 && mbnums[3]==0) {
		return true;
	}
	else {
		return false;
	}
}

void MagicBoxCF::resetIterator() const {
	boxedFraction->resetIterator();
	mbnums[0] = a;
	mbnums[1] = b;
	mbnums[2] = 1;
	mbnums[3] = 0;


}

MagicBoxCF::~MagicBoxCF() {
	//mbnums was declared with new operator so we need to delete it
	delete mbnums;
}



/* QUESTION 5*/

ostream &operator<<(ostream& outs, const ContinuedFraction &cf) {
		//want to print 10 integers of a cf
		int count = 9;
		//start from beginning of cf
		cf.resetIterator();
		outs << "[" << cf.next() << "; ";
		bool state = true;
		while(!cf.hasNoMore() && count>0){
			if(state){
				outs << cf.next();
				state = false;
			}else{
				outs << ", " << cf.next() ;
			}
			count --;
		}
		//if periodic and size is greater than 10
		//signify with use of elipse ...
		if(count == 0 && !cf.hasNoMore()) {
			outs <<" ...]";
		}
		else {
			outs << "]" ;

		}
		//now we need to return the output stream to be printed

		return outs;
}


/* QUESTION 6 */

float Flower::getAngle(unsigned int k) const {
	// copy cf so that the original is not modified
	// get approximation using 7 digits
	Fraction fr = theta->getApproximation(7);
	// compute fractional part of the rotations
	double fractpart = ((k*fr.numerator)%fr.denominator/(double)fr.denominator);
	//return
	return (2 * pie * fractpart);
}
//don' need anything inside this constructore
Flower::Flower(const ContinuedFraction *f, unsigned int apxLengthParam) : theta(f), apx_length(apxLengthParam) {}

Seed Flower::getSeed(unsigned int k) const {
	Seed s;
	// we follow the formulas in the assignment's document
//	float angle = getAngle(theta, k);
	float angle = getAngle(k);
	s.x = sqrt(k / (pie * 1.0)) * cos(angle);
	s.y = sqrt(k / (pie * 1.0)) * sin(angle);
	return s;
}

vector<Seed> Flower::getSeeds(unsigned int k) const {
	//create a vector of type seeds
	vector<Seed> flowerSeeds;
	for(unsigned int i = 0; i < k; i++){
		flowerSeeds.push_back(getSeed(i));
	}
	return flowerSeeds;
}


Flower::~Flower() {
    //don't need anything in here
}

/* QUESTION 7*/

void Flower::writeMVGPicture(ostream &out, unsigned int k, unsigned int scale_x, unsigned int scale_y) const {
	//height is scale_y
	//width is scale _x
	unsigned int C_x, C_y, B_x, B_y, min;

	//need to determine min(H,W)
	if(scale_x < scale_y) {
		min = scale_x;
	}
	else if(scale_y < scale_x) {
		min = scale_y;
	}
	else {
		min = scale_x;
	}

	for(int i=0;i<k;i++){
		//centre pixel equation
		C_x = 0.5*(scale_y) + 0.5*(scale_y-200)*(getSeed(i).x)*sqrt(pie/float(k));
		C_y = 0.5*(scale_x) + 0.5*(scale_x-200)*(getSeed(i).y)*sqrt(pie/float(k));

		//border pixel equation

		//special case for i=0, cant have a radius of 0
		if(i==0){
			B_x = C_x +  1;
		}
		else {
			//need to case k as a float to return a decimal otherwise you will get a 0
			B_x = C_x +  sqrt(i/float(k))*min/100;
		}

		B_y = C_y;

//		fill blue circle C_x,C_y B_x,B_y;

		std::cout << "fill blue circle "<< C_x <<", " << C_y <<", " << B_x <<", " <<  B_y << std::endl;
	}

}





