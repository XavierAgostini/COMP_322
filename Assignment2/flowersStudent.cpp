#include "flowers.h"
#include <math.h>


ContinuedFraction fractionalPart(ContinuedFraction &fr) {
	//just replace the first index with 0
	if(fr.fixedPart.size()>0) {
		fr.fixedPart[0] = 0;
	}
	else {
		fr.fixedPart = fr.periodicPart;
		fr.fixedPart[0] =0;
		//if fraction has no fixed part then assign the periodic part to it
		//and then delete the first index
	}
	return fr;
}
unsigned int spitSqrt8(unsigned int index){
	//sqrt 8 = [2;1,4,1,4,1,4...]
	if(index==0) return 2;
	if(index%2!=0) return 1;
	return 4;
}

unsigned int spitEulerSquare(unsigned int index) {
	//e^2 = [7;2, 1,1,3,18,5  ,1,1,6,30,8  , 1,1,9,42,11    ,1, 1, 12,54,14...]
	//index	[0;1, 2,3,4,5,6   ,7,8,9,10,11 ,12,13,14,15,16  ,17,18,19,20,21

	//need test case for null
//	if(index== NULL) return -1;
	//the first 2 out of 5 are 1
	if(index==0) return 7;
	if(index==1) return 2;
	//first two elements are fixed, the rest follows a periodic pattern of 5 values
	if((index-2)%5==0) return 1;
	if((index-3)%5==0) return 1;
	//the third starts at 3, then increments by 3
	if((index-4)%5==0) return ((index-4)/5)*3 +3;
	//the fourth starts at 18 and increments by 12
	if((index-5)%5==0) return ((index-5)/5)*12+18;
	//the fifth starts at 5 and increments by 3
	return ((index-6)/5)*3+5;
}

Fraction getApproximation(ContinuedFraction &fr, unsigned int n){
	ContinuedFraction dummyCF =fr;//  = new ContinuedFraction;
	Fraction *toRet = new Fraction; //fraction we want to return

//	// b/a : b = a*q +r
	for(int i=n-1; i >=0; --i) {

		//take the temporary denominator to be the spit out integer
		int tempD=spit(dummyCF,i);
	//	std::cout << "tempD: " << tempD << " ";
	//	std::cout << "int i: " << i << " ";
		int tempN = 1;
		if(toRet -> numerator ==0) {
			toRet -> numerator = tempN;
			toRet -> denominator = tempD;
		}
		else {
			tempD = tempD*(*toRet).denominator +(*toRet).numerator;
			tempN = (*toRet).denominator;
			toRet -> numerator = tempN;
			toRet -> denominator =tempD;

		}
//		std::cout << "fraction: " << tempN << "/" <<tempD <<" ";
	}
	int dum1 = toRet -> numerator;
	int dum2 = toRet -> denominator;

	toRet -> numerator = dum2;
	toRet -> denominator = dum1;
	return *toRet;
}


double getAngle(ContinuedFraction &theta, int k) {
	//need to copy theta, in case we need to utilize later on in program
	ContinuedFraction phi;
	phi.fixedPart= theta.fixedPart;
	phi.periodicPart =theta.periodicPart;

	//create a dummy continued fraction that is just the fractional part of phi
    ContinuedFraction dummyCF = fractionalPart(phi);
    //use approximation as defined in assignment problem
    Fraction dummyFR = getApproximation(dummyCF,7);
    //calculate the angle
    double toRet = (((dummyFR).numerator)*k*2*M_PI)/((dummyFR).denominator);
    //if angle is over 2*pi then need to rotate to equivalent value between 0-2*pi
    if(toRet >2*M_PI) {
    	int numTurns = toRet/(2*M_PI);
    	toRet = toRet - numTurns*2*M_PI;
    }
//    else if(toRet < 0) {
//    	int g = toRet/(-2*M_PI);
//    	if(g==0) toRet = toRet +2*M_PI; //ex) if angle is -1 degree
//    	else toRet = toRet +g*2*M_PI;
//    }

    return toRet;
}
Seed getSeed(ContinuedFraction &theta, int k) {
	double angle = getAngle(theta,k);
//	std::cout << " "<<"angle: " << angle << " ";
	//create a seed to return
	Seed *toRet = new Seed;
	//set the seed coordinate equal to provides equations
	(*toRet).x = sqrt(k/M_PI) * cos(angle);
	(*toRet).y = sqrt(k/M_PI) * sin(angle);

	return *toRet;
}

void pushSeed(std::list<Seed> &flower, ContinuedFraction &theta) {
	int k = flower.size(); //get size of flower to determine number of current seeds
	Seed newSeed = getSeed(theta, k); //make a new seed and pass in theta and location k
	flower.push_back(newSeed); //push the seed into the flower
}

int spitNextMagicBox(MagicBox &box) {
	while(true){
//		std::cout<<"im in"<<std::endl;
//		need to copy the box values for later
		int i = (box).i;
		int j = (box).j;
		int k = (box).k;
		int l = (box).l;

		// case 1
		if (k==0 && l==0) {
			//no more integers to spit
			//reached end
			return 0;
		}
		else if (k==0 || l==0 || (i/k != j/l)) {
			// case 3
			//std::cout << "index: " << index;
			//use spit method in flowerExamples to determine if we are at end of continuedFraction
			//which will return -1 is at end
			int p = spit((box).boxedFraction, (box).indexInBoxedFraction++);

			if(p!=-1) {
				(box).i =j;
				(box).j =i+j*p;
				(box).k =l;
				(box).l =k+l*p;
				//dont return p
			} else {
				(box).i =j;
				(box).j =j;
				(box).k =l;
				(box).l =l;
			}
		} else {
			// case 2

			int q = i/k;
			(box).i =k;
			(box).j =l;
			(box).k =i-k*q;
			(box).l =j-l*q;
			return q;
		}

	}
}

ContinuedFraction getCFUsingMB(ContinuedFraction &f, int a, int b, int length) {
	ContinuedFraction toRet;
	//create a magix box
	MagicBox box;
	//initailize the box with the desired parameters
	(box).boxedFraction = f;
	(box).i=a;
	(box).j=b;
	//variables for the loop
	int i =0;
	int k;

	while(i<length){				//repeat for length-1 times
		k = spitNextMagicBox(box); //set k to be the spat out integer
		if(k <= 0) break;			//break out of loop early if we start returning 0
		toRet.fixedPart.push_back(k);	//push k into the continued fractions
		i++;
	}

	return toRet;		//return the continued fraction
}
