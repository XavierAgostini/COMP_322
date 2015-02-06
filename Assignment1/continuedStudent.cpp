#include "continued.h"

unsigned int gcd(unsigned int a, unsigned int b) {
	unsigned int q,r;
	if(a == 0) {
		q = 0;
	}
	else {
		r = b % a;
	}
	while ( r!=0) {
		q = r;
		r = a % r;
	}
	return q;
}

int gcd(int b, int a) {

	if(b == 0) return a;

	if(a == 0) return b;

	if (b & 1) {

		if(a & 1) {

			if (b > a) return gcd((b - a) >> 1, a);

			else return gcd((a - b) >> 1, b);

		}
		else {
			gcd(b,a>>1);
		}

	}
	else {
		if (a & 1) {
			return gcd(b>>1,a);
		}
		else {
			return 2 * gcd(b>>1,a>>1);
		}

	}

	return -1;
}

ContinuedFraction *getCFlargerThanOne(unsigned int b, unsigned int a) {

	ContinuedFraction *point = new ContinuedFraction;
	unsigned int quotient = b/a;
	unsigned int remainder = b%a;

	if(remainder == 0) {
		(*point).head = quotient;
		(*point).tail = nullptr;
	}
	else {
		(*point).head = quotient;
		(*point).tail = getCFlargerThanOne(a, remainder);
	}


	return point;
}

ContinuedFraction *getCF(unsigned int b, unsigned int a) {

	ContinuedFraction *point = new ContinuedFraction;
	if(b>=a) {
		return getCFlargerThanOne(b,a);
	}
	else {
		(*point).head = 0;
		(*point).tail = getCFlargerThanOne(b,a);
		return point;
	}


}


ContinuedFraction *getCF(unsigned int head, ContinuedFraction *fixed, ContinuedFraction *period) {

	ContinuedFraction *dummy1 = new ContinuedFraction;
	(*dummy1).head = (*fixed).head;
	(*dummy1).tail = (*fixed).tail;

	ContinuedFraction *dummy2 = new ContinuedFraction;
	(*dummy2).head = (*period).head;
	(*dummy2).tail = (*period).tail;

	ContinuedFraction *toRet = new ContinuedFraction;
	toRet->head = head;
	toRet->tail = dummy1;
	ContinuedFraction *tmp = toRet;
	// go to the end of fixed
	while(tmp->tail != nullptr) {
		tmp = tmp->tail;
	}
	// attach end of fixed to period
	tmp->tail = dummy2;
	// go to the end of period
	while(tmp->tail != nullptr) {
		tmp = tmp->tail;
	}
	// attach the end of period to the beginning of period
	tmp->tail = dummy2;
	return toRet;



}


Fraction getApproximation(ContinuedFraction *fr, unsigned int n) {
	ContinuedFraction *dummy1 = new ContinuedFraction;
	Fraction *toRet = new Fraction;

	(*dummy1).head = (*fr).head;
	(*dummy1).tail = (*fr).tail;

// b/a : b = a*q +r

	for(int i = 0; i<n; i++){
		ContinuedFraction *tmp = dummy1;
		int j=0;
		while((j<(n-1-i)) && (tmp->tail != nullptr)) {
				tmp = tmp->tail;
				j++;
		}
		//	std::cout <<"temp.head: "<<(*tmp).head;

		int tempD = (*tmp).head;
//		std::cout << "tempD: " << tempD <<std::endl;
		int tempN = 1;
		if(toRet -> numerator == 0) {
			toRet -> numerator = tempN;
			toRet -> denominator = tempD;
		}
		else {
			tempD = tempD*(*toRet).denominator +(*toRet).numerator;
			tempN = (*toRet).denominator;
			toRet -> numerator = tempN;
			toRet -> denominator =tempD;
		}

//	std::cout << "fraction: " << (*toRet).numerator <<" / " << (*toRet).denominator << std::endl;
//		std::cout << "denominator: " << (*toRet).denominator <<std::endl;

	}
	int dum1 = toRet -> numerator;
	int dum2 = toRet -> denominator;

	toRet -> numerator = dum2;
	toRet -> denominator = dum1;

	return *toRet;
}

//ContinuedFraction *test(ContinuedFraction *fr, unsigned int n) {
//
//	ContinuedFraction *dummy1 = new ContinuedFraction;
//	ContinuedFraction *tester = fr;
//
//	dummy1 -> head = (*fr).head;
//	for(int i=1; i<n; i++) { //use 1 because 0 will give extra result
//		tester = tester -> tail;
//		ContinuedFraction *dummy = new ContinuedFraction;
//		dummy -> head = (*tester).head;
//		dummy -> tail = nullptr;
//
//		if((*dummy1).tail == nullptr) {
//			dummy1 ->tail = dummy;
//		}
//		else {
//			auto tmp = dummy1;
//				// go to the end of fixed
//				while(tmp->tail != nullptr) tmp = tmp->tail;
//				// attach end of fixed to period
//				tmp->tail = dummy;
//		}
//
//	}
//
//	return dummy1;
//}

