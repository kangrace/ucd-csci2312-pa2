#include <sstream>
#include <iomanip>
#include <algorithm>
#include "Cluster.h"

using namespace std;

namespace Clustering
{
    // LNode constructor
    LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n)
    {

    }

    Cluster::Cluster()
    {
        __size = 0;
        __points = nullptr;
    }

    // the big three:
    // 1: copy constructor
    Cluster::Cluster(const Cluster &source) : Cluster()
    {
        for (int i = 0; i < source.__size; i++)
            add(source[i]);
    }

    // 2: overloaded operator =
    Cluster &Cluster::operator=(const Cluster &source)
    {
        LNodePtr current;

        if(this == &source)
            return *this;
        else
        {
            while (__points != nullptr)
            {
                current = __points;
                __points = __points -> next;
                delete current;
                --__size;
            }
            for(int i = 0; i < source.__size; ++i)
                add(source[i]);
        }
        return *this;
    }

    // 3: destructor
    Cluster::~Cluster()
    {
        if(__points != nullptr)
        {
            LNodePtr prev;
            LNodePtr current;

            prev = __points;
            current = nullptr;
            while (prev != nullptr)
            {
                current = prev -> next;
                delete prev;
                prev = current;
            }
        }
    }

    // getters
    int Cluster::getSize() const
    {
        return __size;
    }

    // setter functions
    void Cluster::add(const Point &point)
    {
        //empty list
        if (__size == 0)
        {
            ++__size;
            __points = new LNode(point, nullptr);
        }

        else
        {
            // already contains the point
            if (contains(point))
                return;

            LNodePtr next;
            LNodePtr prev;

            next = __points;
            prev = nullptr;

            // add node in positions
            while (next != nullptr)
            {
                if (point < next -> point)
                {
                    if (prev == nullptr)
                    {
                        __points = new LNode(point, next);
                        ++__size;
                        return;
                    }
                    else
                    {
                        prev -> next = new LNode(point, next);
                        ++__size;
                        return;
                    }
                }
                prev = next;
                next = next -> next;
            }
            // no position, add to end
            prev->next = new LNode(point, nullptr);
            ++__size;
        }
    }

    const Point &Cluster::remove(const Point &point)
    {
        if (contains(point))
        {
            LNodePtr prev;
            LNodePtr next;

            prev = nullptr;
            next = __points;

            while (next != nullptr)
            {
                if (next->point == point)
                {
                    // first element
                    if (prev == nullptr)
                    {
                        __points = next -> next;
                        delete next;
                        --__size;
                        break;
                    }
                    else
                    {
                        prev -> next = next -> next;
                        delete next;
                        --__size;
                        break;
                    }

                }
                prev = next;
                next = next -> next;
            }

        }
        return point;
    }

    bool Cluster::contains(const Point &source)
    {
        LNodePtr next = __points;
        while (next != nullptr)
        {
            // use id to see if point is int he list
            if (next->point.getId() == source.getId())
                return true;
                // search the next ids
            else next = next->next;

        }
        //not in list
        return false;

    }

    // overloaded operators

    // Subscript
    const Point &Cluster::operator[](unsigned int index) const
    {
        if(__size > 0)
        {
            LNodePtr cursor = __points;

            for (int i = 0; i < index && cursor -> next != nullptr; ++i)
                cursor = cursor->next;
            return cursor->point;

        }

    }

    // compound assignment (point arguments)
    Cluster &Cluster::operator+=(const Point &point)
    {
        add(point);
        return *this;
    }

    Cluster &Cluster::operator-=(const Point &point)
    {
        remove(point);
        return *this;
    }

    // compound assignment (cluster argument)
    // union
    Cluster &Cluster::operator+=(const Cluster &right)
    {
        for (int i = 0; i < right.getSize(); ++i)
        {
            // if it doesn't contain the point, add it
            if(!contains(right[i]))
                add(right[i]);
        }
        return *this;
    }

    // difference
    Cluster &Cluster::operator-=(const Cluster &right)
    {
        for (int i = 0; i < right.__size; ++i)
            // if it does contain the point, remove it
            if (contains(right[i]))
                remove(right[i]);
        return *this;


    }

    // friends : IO
    ostream &operator<<(ostream &output, const Cluster &cluster)
    {
        for (int i = 0; i < cluster.__size; i++)
            output << cluster[i] << endl;
        return output;
    }


    istream &operator>>(istream &input, Cluster &cluster)
    {
        string line;
        while (getline(input,line))
        {
            if (line.empty())
                return input;

            int size = count(line.begin(), line.end(), ',') + 1;
            Point in(size);

            stringstream lineStream(line);
            lineStream >> in;

            cluster.add(in);
        }
        return input;
    }


    // Friends: Comparison
    bool operator==(const Cluster &left, const Cluster &right)
    {
        // compare size
        if (left.__size != right.__size)
            return false;

        // compare values
        for (int i = 0; i < left.__size; ++i)
        {
            if (left[i] != right[i])
                return false;
        }
        // values are the same
        return true;
    }



    bool operator!=(const Cluster &left, const Cluster &right)
    {
        return !(left == right);
    }

    // Friends: Arithmetic (Cluster and Point)
    const Cluster operator+(const Cluster &left, const Point &right)
    {
        Cluster temp(left);
        temp += right;
        return temp;
    }

    const Cluster operator-(const Cluster &left, const Point &right)
    {
        Cluster temp(left);
        temp -= right;
        return temp;
    }

    // Friends: Arithmetic (two Clusters)
    // union
    const Cluster operator+(const Cluster &left, const Cluster &right)
    {
        Cluster temp(left);
        temp += right;
        return temp;
    }

    // (asymmetric) difference
    const Cluster operator-(const Cluster &left, const Cluster &right)
    {
        Cluster temp(left);
        temp -= right;
        return temp;
    }

}

