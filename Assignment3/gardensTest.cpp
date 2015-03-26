#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include "gardens.h"


ostream &operator<<(ostream &out, const Flower &flower) {
    flower.writeMVGPicture(out, 300, 1600, 1600);
    return out;
}


ostream &operator<<(ostream &out, const Fraction &f) {
    return out << f.numerator << "/" << f.denominator;
}



int main() {

    map<string,const ContinuedFraction*> cfs;

    const Fraction seventyFiveOverThirtyThree = {75,33};

    cfs["-Fraction 75/33-"] = new RationalCF(seventyFiveOverThirtyThree);

    const Fraction thirtyOverThirteen = {30,13};

    const RationalCF *rcf2 = new RationalCF(thirtyOverThirteen);
    cfs["-Fraction 30/13-"] = rcf2;
    cfs["-Golden Ratio-"] = new PeriodicCF({1});
    const PeriodicCF *sqrt2 = new PeriodicCF({1},{2});
    cfs["-Square Root 2-"] = sqrt2;
    cfs["-Square Root 8-"] = new PeriodicCF({2},{1,4});
    cfs["-Square Root 18-"] = new MagicBoxCF(sqrt2, 0, 3);
    cfs["-Fraction 125/13-"] = new MagicBoxCF(rcf2, 5, 2);
    cfs["-Square Root 72-"] = new MagicBoxCF(cfs["-Square Root 18-"], 0, 2);
    
    cout << "Back and forth" << endl;
    cout << "=============" << endl;

    cout << thirtyOverThirteen << " has the cf " << *rcf2 << endl;
    cout << *rcf2 << " is the cf of " << rcf2->getValue() << endl;


    cout << "Print all cfs" << endl;
    cout << "=============" << endl;
    for (auto& x : cfs) {
        string z = x.first;
        cout << "The CF of " << x.first << " is " << *(x.second) << endl;
    }

    Flower f(sqrt2, 7);
    cout << endl << "The first few seeds of Square Root 2" << endl;

    cout << "Angle after 3 cycles of sqrt{2} rotations: ";


    auto z = f.getSeeds(10);
    for (auto& s : z) {
        cout << setprecision(3) <<"(" << s.x << ", " << s.y << ")";
    }cout << endl;

    cout << endl << "Test MVG for Square Root 2" << endl;
    cout << "=============" << endl;
    f.writeMVGPicture(cout, 10, 1600, 1600);


    for (auto& x : cfs) {
        delete x.second;
    }
    return 0;
}
