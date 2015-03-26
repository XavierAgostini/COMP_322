#include <vector>
#include <iostream>

#ifndef hw3_gardens_h
#define hw3_gardens_h

using namespace std;



/** Continued Fractions **/

//--- shorthand notation for unsigned long
typedef unsigned long cf_int;

//--- struct for Fraction
struct Fraction {
    cf_int numerator;
    cf_int denominator;
};



//--- Defines an interface/virtual class for a simple continued fraction
class ContinuedFraction {
friend class MagicBoxCF; //magic box will need to use the iterator
protected:
    //--- methods necessary for iterating through the CF
    
    /* next: returns the next integer in the cf representation
     **/
    virtual cf_int next() const = 0;
    
    /* hasNoMore : returns true iff the iterator reached the end
     **/
    virtual bool hasNoMore() const = 0;
    
    /* resetIterator: resets the iteraton over the cf to the beginning
    **/
    virtual void resetIterator() const = 0;
    
public:
    /* TODO getApproximation: return the approximation - much like the previous homework assignments
     * -> 'k'  is the maximum number of integers to use for the approximation
     **/
    Fraction getApproximation(unsigned int k) const;
    
    virtual ~ContinuedFraction() {} // since this class is abstract, we need to define a virtual descructor
    
    // friend methods and classes
    friend ostream &operator<<(ostream &out, const ContinuedFraction &cf);
};



//--- Defines a class for periodic continued fractions
class PeriodicCF : public ContinuedFraction {
protected:
    // data
    vector<cf_int> fixedPart;
    vector<cf_int> periodicPart;
    
    //-- necessary for iterating: this will point to the item in the continued fraction that the iterator has reached
    unsigned int *iteratorPosition;
    
    // TODO - see ContinuedFraction on what these should do - use iteratorPosition to keep track of the iterator
    virtual cf_int next() const;
    virtual bool hasNoMore() const;
    virtual void resetIterator() const;
public:
    // constructors
    PeriodicCF(const vector<cf_int> &fixedParam, const vector<cf_int> &periodicParam) : fixedPart{fixedParam}, periodicPart{periodicParam} {
        iteratorPosition = new unsigned int(0);
//        cout<<"FUCK";
    }
    PeriodicCF(const vector<cf_int> &periodicParam) : PeriodicCF({}, periodicParam) {}
    
    // TODO destructor : make sure it deallocates all memory allocated using 'new'
    ~PeriodicCF();
};


//--- Defines a class for continued fractions on rational numbers (i.e. finite representation)
class RationalCF : public PeriodicCF {
public:

    // constructors
    RationalCF (const vector<cf_int> &fixedParam) : PeriodicCF(fixedParam,{}) {
//    	cout<<"inside RationalCF"<<endl;
    }
    // TODO : constructor takes a fraction as parameter and using code from previous assignments, computes the fixed part vector of integers for this cf
    RationalCF (Fraction frac);
    
    // TODO destructor : make sure it deallocates all memory allocated using 'new'
    ~RationalCF();
    
    //----additional methods
    unsigned int length() const {return (unsigned int) fixedPart.size(); }
    Fraction getValue() const { return PeriodicCF::getApproximation(length()); }
};


//--- Defines a class for continued fractions generated using a magic box
class MagicBoxCF : public ContinuedFraction {
private:
    //-- the fraction boxed - same as in HW2
    const ContinuedFraction *boxedFraction;
    //-- this MagicBoxCF will represent a+b*x, where x is the boxedFraction
    const cf_int a,b;
    
    //--  this iterator will point to an array of 4 ints, which are the 4 integers used to keep track of the magic box
    cf_int *mbnums;
    
    // TODO : see ContinuedFraction for details on what you should do. Make sure you use mbnums, which will have to be allocated in the constructor
    virtual cf_int next() const;
    virtual bool hasNoMore() const;
    virtual void resetIterator() const;
public:
    // TODO: constructor : initialize the data, AND make sure to allocate memory for the 4 integers used by the magic box - use these in next(), hasNoMore() and resetIterator()!
    MagicBoxCF(const ContinuedFraction *f, cf_int a, cf_int b);
    
    // TODO destructor : make sure it deallocates all memory allocated using 'new'
    ~MagicBoxCF();
};

/** Gardens **/

struct Seed { float x, y;};

//--- defines a class that implements a flower
class Flower {
protected:
    // use this as PI in your computations
    const float pie = 3.141592654;
    
    // the cf used to turn seeds
    const ContinuedFraction *theta;
    
    // since we can only use an approixmation of theta, we need to have a predefined approximation length. Use this to call the method getApproximation
    const unsigned int apx_length;
    
    /* TODO : getAngle - same method as in HW2
     **/
    float getAngle(unsigned int k) const;
public:
    // TODO : constructor - initialize data
    Flower(const ContinuedFraction *f, unsigned int apxLengthParam);
    
    // TODO : make sure it deallocates all memory allocated using 'new'
    ~Flower();
    
    //---additional methods
    
    /* TODO : getSeed - same as HW2
     **/
    Seed getSeed(unsigned int k) const;
    
    /* TODO : getSeeds - same as HW2
     **/
    vector<Seed> getSeeds(unsigned int k) const;
    
    
    /* TODO : writeMVGPicture - explained in the write-up that comes with this assignment
     **/
    void writeMVGPicture(ostream &out, unsigned int k, unsigned int scale_x, unsigned int scale_y) const;
    
    
};


// printing methods - explained in the write-up that comes with this assignment
ostream &operator<<(ostream &out, const ContinuedFraction &cf); //TODO

ostream &operator<<(ostream &out, const Flower &flower); //DONE in test
ostream &operator<<(ostream &out, const Fraction &f); //DONE in test


#endif
