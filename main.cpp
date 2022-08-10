#ifndef MVECTOR_H // the 'include guard'
#define MVECTOR_H 

#include "timing.h"
#include <vector>
#include <iostream>
#include <fstream>

// Class that represents a mathematical vector
class MVector
{
public:
    // constructors
    MVector() {}
    explicit MVector(int n) : v(n) {}
    MVector(int n, double x) : v(n, x) {}
    MVector(std::initializer_list<double> l) : v(l) {}


    // access element and change its value
    double &operator[](int index)
    {
        //size_t s = v.size();
        //if (index >= s || index < 0)
        //{
        //    std::cout << "Index out of bounds!";
        //    exit(1); // exit with an error
        //}
        return v[index];
    }

    // access element without changing value 
    double operator[](int index) const 
    {
        //size_t s = v.size();
        //if (index >= s || index < 0)
        //{
        //    std::cout << "Index out of bounds!";
        //    return 0; // exit with an error
        //}
        return v[index];
    }

    // number of elements
    int size() const 
    { 
        return v.size(); 
    } 

    // swaps elements 
    void swap(int i, int j)
    {
        double temp;
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }

    // allows use of << on MVector types
    friend std::ostream& operator<<(std::ostream &os, const MVector v)
    {
        size_t s = v.size();
        os << "["; // brackets for visual aesthetic
        for (size_t i = 0; i < s-1; i++) // loops up to and including penultimate entry
        {
            os << v[i] << ", ";
        }
        os << v[s - 1]; // final entry doesnt have a comma after it
        os << "]";
        return os;
    }

    // assigns an MVector's entries to random numbers between xmin and xmax
    void initialise_random(double xmin, double xmax)
    {
        size_t s = v.size();
        for (size_t i = 0; i < s; i++)
        {
            v[i] = xmin + (xmax - xmin) * rand() / static_cast<double>(RAND_MAX); // need static_cast to get a decimal fraction
        }
            
    }

    // paritions an MVector object into 3 sections [S1,S2,S3] based  
    // on the last element x. S1 where all elements are less than x, 
    // S2 where all elements are equal to x and S3 where all elements
    // are greater than x.
    int partition(int index1,int index2)
    {
        // index2 denotes where x (or the 'pivot') is i.e. the element
        // we are comparing everything else in v to. 
        // This has to be the last element in the vector.

        // get value at end point
        double x = v[index2];

        // want to loop through elements of v checking if they are
        // greater than, equal to or less than x.
        for (int i = index1; i < index2 ; i++)
        {
            if (v[i] <= x)
            {
                swap(i, index1);
                index1++;
            }
        }
        // when we reach x, swap x with the element at point index1.
        swap(index1, index2);

        // returns where x is located in the vector after the partition
        // so we can then partition to the left and right of it.
        return index1;
    }

    // chooses a random x, sends it to the last position
    // in the vector and then calls partition()
    int random_partition(int index1, int index2)
    {
      int indexr = index1 + rand() % (index2-index1);
      swap(indexr, index2);
      return partition(index1, index2);
    }

    // checks if an element is a leaf
    bool is_leaf(int index, int n)
    {
        // vertex vec[i] has children at vec[i*2+1]
        // and vec[i*2 +2]. So to check if v[i] is a 
        // leaf i.e. no children we need to check if 
        // its childrens' index's are larger than the 
        // size of the vector (n)

        if ((index * 2 + 1) >= n or (index * 2 + 2) >= n)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    std::vector<double> v;
};

#endif

// Bubblesort
// Initially sends the greatest element to the nth position
// then we look at the sequence of n - 1 elements and send
// the greatest element to the n-1 position , then we look 
// at n-2 etc til we get to n-(n-1) and then we stop.
// Inner loop in total iterates n + (n-1) + (n-2) + ... + 1
// times. So we have O(n(n+1)/2) = O(n^2) using the sum of 
// an arithmetic sequnce formul courtesy of rkid gauss
void bubble(MVector &v)
{
    // initially loop through whole vector
    int k = 1;
    while (k < v.size())
    {
        for (int i = 0; i < v.size() - k; i++)
        {
            // check if left element is greater than right
            if (v[i + 1] < v[i]) v.swap(i, i + 1);
        }
        // increase k to loop through one less element
        k++;
    }
}

// slower version 
void bubble2(MVector &v)
{
    for (int j = 0; j < v.size(); j++)
    {
        for (int i = 0; i < v.size() - 1; i++)
        {
            if (v[i + 1] < v[i]) v.swap(i, i + 1);
        }
    }
    
}

//quicksort
void quick_recursive(MVector& v, int start, int end)
{
    if (start < end)
    {
        // get pivot index after first partition
        int index = v.random_partition(start, end);

        // partitions to the left of the pivot
        quick_recursive(v, start, index - 1);

        //partitions to the right of the pivot
        quick_recursive(v, index + 1, end);
    }
    
}
// wrapper function for quick_recursive
void quick(MVector &v)
{ 
    quick_recursive(v, 0, v.size() - 1);
}

void heap_from_root(MVector& v, int i, int n)
{
    // ignore leaves
    if (!v.is_leaf(i,n))
    {
        // check if left child is greater and swap
        if (v[i] < v[2 * i + 1] && v[2 * i + 1] > v[2 * i + 2])
        {
            v.swap(i, 2 * i + 1);
            // call heap_from_root again to check that we still have a heap 
            // after swapping
            heap_from_root(v, 2 * i + 1, n);
        }
        // do same for right child
        if (v[i] < v[2 * i + 2] && v[2 * i + 2] > v[2 * i + 1])
        {
            v.swap(i, 2 * i + 2);
            heap_from_root(v, 2 * i + 2, n);
        }
    }
}

void heap1(MVector& v)
{
    // want to call heap from root on all nodes
    // then swap out the root for a leaf and repeat

    for (int j = 0; j < v.size(); j++)
    {
        for (int i = v.size() - (j+1); i >=0 ; i--)
        {
            heap_from_root(v, i, v.size()-j);
        }
        // swap out root for a leaf
        v.swap(v.size() - (j+1), 0);
    }
    
}

void heap(MVector& v)
{
    // initially build the heap
    for (int i = v.size(); i >= 0; i--)
    {
        heap_from_root(v, i, v.size());
    }
    // swap greatest value out and call
    // heap_from_root on smaller tree
    for (int i = v.size()-1; i >= 0; i--)
    {
        // swap out root for leaf
        v.swap(i, 0);

        // reapply heap_from_root to
        // the root node
        heap_from_root(v, 0, i);
    }
}


// function to test speed of bubblesort
double testingbubble(int n)
{
    // declare MVector object vec of length n 
    MVector vec(n);

    // give vec random entries from 0 to 10
    vec.initialise_random(0, 10);

    //time bubble1 on vec
    double startTime = Timer();
    bubble(vec);
    double endTime = Timer();
    double duration = endTime - startTime;

    return duration;
}

// function to test speed of quicksort
double testingquick(int n)
{
    // declare MVector object vec of length n 
    MVector vec(n);

    // give vec random entries from 0 to 10
    vec.initialise_random(0, 10);

    //time quick on vec
    double startTime = Timer();
    quick(vec);
    double endTime = Timer();
    double duration = endTime - startTime;

    return duration;
}

double testingheap(int n)
{
    // declare MVector object vec of length n 
    MVector vec(n);

    // give vec random entries from 0 to 10
    vec.initialise_random(0, 10);

    //time heap on vec
    double startTime = Timer();
    heap(vec);
    double endTime = Timer();
    double duration = endTime - startTime;

    return duration;
}

int main()
{   
    std::srand(std::time(NULL)); // new rng seed every time program is run

    // creating object of type ofstream
    std::ofstream Data1, Data2, Data3, Data4, Data5, Data6, Data7;

    //Data1.open("Data1.txt");
    //if (!Data1) return 1; // opening fails

    //Data1.width(20); Data1 << "i";
    //Data1.width(20); Data1 << "Time taken" << std::endl;

    //for (int i = 1; i <= 250; i++)
    //{
    //    double timeSum = 0;
    //    for (int j = 1; j <= 100; j++)
    //    {
    //        double time = testingbubble(i);
    //        timeSum += time; 
    //    }   
    //    Data1.width(20); Data1 << i;
    //    Data1.width(20); Data1 << timeSum / 100 << std::endl;
    //}

    //Data2.open("Data2.txt");
    //if (!Data2) return 1; // opening fails

    //Data2.width(20); Data2 << "i";
    //Data2.width(20); Data2 << "Time taken" << std::endl;

    //
    //for (int i = 1; i <= 250; i++)
    //{
    //    double timeSum = 0;
    //    for (int j = 1; j <= 10000; j++)
    //    {
    //        double time = testingquick(i);
    //        timeSum += time;
    //    }
    //    Data2.width(20); Data2 << i;
    //    Data2.width(20); Data2 << timeSum / 10000 << std::endl;
    //}

   //Data3.open("Data3.txt");
   //if (!Data3) return 1; // opening fails

   //Data3.width(20); Data3 << "i";
   //Data3.width(20); Data3 << "Time taken" << std::endl;

   // for (int i = 1; i <= 250; i++)
   // {
   //     double timeSum = 0;
   //     for (int j = 1; j <= 10000; j++)
   //     {
   //         double time = testingquick(i);
   //         timeSum += time;
   //     }
   //     Data3.width(20); Data3 << i;
   //     Data3.width(20); Data3 << timeSum / 10000 << std::endl;
   // }

    //Data4.open("Data4.txt");
    //if (!Data4) return 1; // opening fails

    //Data4.width(20); Data4 << "i";
    //Data4.width(20); Data4 << "Time taken" << std::endl;

    //for (int i = 1; i <= 1000; i++)
    //{
    //    double timeSum = 0;
    //    for (int j = 1; j <= 100; j++)
    //    {
    //        double time = testingquick(i);
    //        timeSum += time;
    //    }
    //    Data4.width(20); Data4 << i;
    //    Data4.width(20); Data4 << timeSum / 100 << std::endl;
    //}

    //Data5.open("Data5.txt");
    //if (!Data5) return 1; // opening fails

    /*Data5.width(20); Data5 << "i";
    Data5.width(20); Data5 << "time taken" << std::endl;

    for (int i = 1; i <= 250; i++)
    {
        double timeSum = 0;
        for (int j = 1; j <= 1500; j++)
        {
            double time = testingheap(i);
            timeSum += time;
        }
        Data5.width(20); Data5 << i;
        Data5.width(20); Data5 << timeSum / 1500 << std::endl;
    }*/

    //Data6.open("Data6.txt");
    //if (!Data6) return 1; // opening fails

    //Data6.width(20); Data6 << "i";
    //Data6.width(20); Data6 << "time taken" << std::endl;

    //for (int i = 1; i <= 1000; i++)
    //{
    //    double timeSum = 0;
    //    for (int j = 1; j <= 10; j++)
    //    {
    //        double time = testingquick(i);
    //        timeSum += time;
    //    }
    //    Data6.width(20); Data6 << i;
    //    Data6.width(20); Data6 << timeSum / 10 << std::endl;
    //}
    Data7.open("Data7.txt");
    if (!Data7) return 1; // opening fails

    Data7.width(20); Data7 << "i";
    Data7.width(20); Data7 << "time taken" << std::endl;

    for (int i = 1; i <= 1000; i++)
    {
        double timeSum = 0;
        for (int j = 1; j <= 1000; j++)
        {
            double time = testingheap(i);
            timeSum += time;
        }
        Data7.width(20); Data7 << i;
        Data7.width(20); Data7 << timeSum / 1000 << std::endl;
    }

}



