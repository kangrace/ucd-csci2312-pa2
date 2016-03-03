#include <iostream>
#include <cmath>     // sqrt, pow
#include <algorithm> //count
#include <cassert>
#include <sstream>
#include "point.h"

using namespace std;
using namespace Clustering;

namespace Clustering
{
    // static id generator
    unsigned int Point::__idGen = 0;

//constructors
    Point::Point(int dimension)
    {
        //assign id and increase for next point
        __id = __idGen;
        ++__idGen;

        //assign dimension and create array
        __dim = dimension;
        __values = new double[__dim];

        for (int i = 0; i < __dim; ++i)
        {
            __values[i] = 0;
        }
    }

    Point::Point(int dimensions, double *array)
    {
        // assign id and increase for next point
        __id = __idGen;
        ++__idGen;

        __dim = dimensions;
        __values = new double[__dim];
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] = array[i];
        }
    }

// the big three: copy constructor
    Point::Point(const Point &source)
    {
        __dim = source.__dim;
        __id = source.__id;
        __values = new double[__dim];
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] = source.__values[i];
        }

    }
// 2: overloaded operator =
    Point &Point::operator=(const Point &right)
    {
            __dim = right.__dim;
            __id = right.__id;

            if (__values != nullptr)
                delete[] __values;
            __values = new double[__dim];
            for (int i = 0; i < __dim; ++i)
            {
                __values[i] = right.__values[i];
            }

        return *this;
    }
// 3: destructor
    Point::~Point()
    {
        delete [] __values;
    }

//accessors and mutators
    int Point::getId() const
    {
        return __id;
    }

    int Point::getDims() const
    {
        return __dim;
    }


    void Point::setValue(int dim, double val)
    {
        __values[dim] = val;
    }

//get value of that dim
    double Point::getValue(int dim) const
    {
        return __values[dim];
    }


    double Point::distanceTo(const Point &point) const
    {
        // make sure the dimension are the same to calculate distance
        if(__dim != point.__dim)
            return false;

        double answer = 0.0;
        for (int i = 0; i < __dim; ++i)
        {
            answer += pow(point.getValue(i) - getValue(i), 2);
        }
        return sqrt(answer);
    }

//overloaded operators

//members
    Point &Point::operator*=(double value)// p *= 6; p.operator*=(6);
    {
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] *= value;
        }
        return *this;
    }
    Point &Point::operator/=(double value)
    {
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] /= value;
        }
        return *this;
    }

    const Point Point::operator*(double value) const // prevent (p1 * 2) = p2;
    {
        Point temp(*this);
        temp *= value;
        return temp;
    }
    const Point Point::operator/(double value) const // p3 = p2 / 2;
    {
        // can't divide by 0
        assert(value != 0);

        Point temp(*this);
        temp /= value;
        return temp;

    }

    double &Point::operator[](int index)
    {
        return __values[index];
    }

// friends

    Point &operator+=(Point &left, const Point &right)
    {
        //lecture
        if(&left == &right)
        {
            return left *= 2;
        }
        else if (left.__dim == right.__dim)
        {
            for (int i = 0; i < left.__dim; ++i)
                left.__values[i] += right.__values[i];
        }
        return left;

    }

    Point &operator-=(Point &left, const Point &right)
    {
        if(&left == &right)
        {
            return left -= left;
        }
        else if (left.__dim == right.__dim)
        {
            for (int i = 0; i < left.__dim; ++i)
                left.__values[i] -= right.__values[i];
        }
        return left;
    }

// +
    const Point operator+(const Point &left, const Point &right)
    {
        //lecture
        Point temp(left);
        return temp += right;
    }

// -
    const Point operator-(const Point &left, const Point &right)
    {
        // set temp to left and subtract right
        Point temp(left);
        return temp -= right;
    }

// ==
    bool operator==(const Point &left, const Point &right)
    {
        // check to see if ids are equal
        if(left.getId() != right.getId())
            return false; // id are not equal cannot be true

        // check to see if values are equal
        for (int i = 0; i < max(left.__dim, right.__dim); ++i)
        {
            if (left.getValue(i) != right.getValue(i))
                return false;
        }
        return true;
    }

// !=
    bool operator!=(const Point &right, const Point &left)
    {
        return !(left == right);
    }

// <
    bool operator<(const Point &left, const Point &right)
    {
        // check for each if left < right
        for (int i = 0; i < max(left.__dim, right.__dim); ++i)
        {
            // check to see if the values are different
            // return left < right of the values
            if (left.getValue(i) != right.getValue(i))
                return (left.getValue(i) < right.getValue(i));
        }
    }

// >
    bool operator>(const Point &left, const Point &right)
    {
        return (right < left);
    }

// <=
    bool operator<=(const Point &left, const Point &right)
    {
        return !(left > right);
    }

// >=
    bool operator>=(const Point &left, const Point &right)
    {
        return !(left < right);
    }

// <<
    ostream &operator<<(ostream &output, const Point &point)
    {
        int i = 0;
        for( ; i < point.__dim - 1; ++i)
            output << point.__values[i] << ", ";

        output << point.__values[i];
    }

// >>
    istream &operator>>(istream &input, Point &point)
    {
        string line;
        getline(input, line);
        int size = count(line.begin(), line.end(), ',') + 1;

        stringstream lineStream(line);

        if (point.__dim != size)
        {
            delete [] point.__values;

            point.__dim = size;
            point.__values = new double [point.__dim];
        }

        int i = 0;
        while (!lineStream.eof())
        {
            string s;
            getline(lineStream, s, ',');
            stringstream sValue(s);
            sValue >> point.__values[i];
            ++i;
        }

        return input;
    }

}
