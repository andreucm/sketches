
// INfo References: 
// http://www.cplusplus.com/reference/vector
// http://www.cplusplus.com/reference/list

#include <sys/time.h>
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <fstream>

const unsigned int N_ELEMENTS = 1e6;
const unsigned int LOOP_STEP = 5;

using namespace std;

class kronos
{
      protected:
            timeval t1,t2;
            double dT;
            unsigned char memInvader[10000];
            
      public:
            kronos(){}
            ~kronos(){}
            double getdT(){return dT;}
            void start(){gettimeofday(&t1, NULL);}
            void stop()
            {
                  gettimeofday(&t2, NULL); 
                  dT = (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );
            }
            void print(const string & str) const
            {
                  cout << str << dT << endl;    
            }
            void stopAndPrint(const string & str)
            {
                  stop();
                  print(str);
            }
};

class classA
{
      protected: 
            double num;
            string label;
            
      public:
            classA()
            {
                  num=0;
                  label="hello world IRI talks, just to fill the memory a little bit, thus allocation will be harder";
//                   cout << "End of default constructor" << endl;
            }
                
            classA(const double value)
            {
                  num = value;
                  label = "hello world";
//                   cout << "End of constructor: num=" << num << endl;
            }
            
            classA(const string & str)
            {
                  num = 0;
                  label = str;
//                   cout << "End of constructor: label=" << label << endl;
            }
            
            ~classA()
            {
//                   cout << "End of destructor: num=" << num << endl;
            }
            
            void setValue(double value)
            {
                  num=value;
            }
            
            double getValue() const
            {
                  return num;
            }
            
            void by2()
            {
                  num = num*2;
            }
            
            bool operator<(classA & ca) const
            {
                  if ( this->num > ca.num ) return true;
                  else return false;
            }
};

int main()
{
      //containers
      vector<classA> vectA;
      list<classA> list1, list2;
      deque<classA> deqA;
       
      //counters, iterators
      unsigned int ii,jj;
      list<classA>::iterator itr, itr2;
      
      //timing
      kronos crono;

      //variables
      classA *newAobj;
      timeval t1,t2;
      double dT;
      ofstream logFile; 
      
      //open log file
      logFile.open("/home/andreu/Desktop/stl_log.txt", ofstream::out);
      
      //start tests
cout << endl << "1. size, capacity, resize and reserve" << endl;
      cout << "  INIT: " << endl;
      cout << "    vector.size(): " << vectA.size() << endl;
      cout << "    vector.capacity(): " << vectA.capacity() << endl;
      
      cout << "  vectA.resize(11)" << endl;
      vectA.resize(11);
      cout << "    vector.size(): " << vectA.size() << endl;
      cout << "    vector.capacity(): " << vectA.capacity() << endl;
      
      cout << "  vectA.clear()" << endl;
      vectA.clear();
      cout << "    vector.size(): " << vectA.size() << endl;
      cout << "    vector.capacity(): " << vectA.capacity() << endl;
      
      cout << "  vectA.erase(vectA.begin(),vectA.end())" << endl;
      vectA.erase(vectA.begin(),vectA.end());
      cout << "    vector.size(): " << vectA.size() << endl;
      cout << "    vector.capacity(): " << vectA.capacity() << endl;
      
      cout << "  vectA.reserve(15)" << endl;
      vectA.reserve(15);
      cout << "    vector.size(): " << vectA.size() << endl;
      cout << "    vector.capacity(): " << vectA.capacity() << endl;
      
      cout << "  vectA.resize(1)" << endl;
      vectA.resize(1);
      cout << "    vector.size(): " << vectA.size() << endl;
      cout << "    vector.capacity(): " << vectA.capacity() << endl;      
      
      //in C++98
      cout << "  vector<classA>().swap(vectA)" << endl;
      vector<classA>().swap(vectA);
      cout << "    vector.size(): " << vectA.size() << endl;
      cout << "    vector.capacity(): " << vectA.capacity() << endl;      

      //IN C++11:
      //vectA.shrink_to_fit(); // will reduce its capacity to fit current size.       

      
cout << endl << "2. Filling a vector with/without prior resizing/reserving" << endl;      
      cout << "  Fill vector, without prior resizing/reserving. ";
      crono.start();
      for (ii=0; ii<N_ELEMENTS; ii++)
      {
            newAobj = new classA( (double) (ii+1) );
            vectA.push_back(*newAobj);
      }
      crono.stopAndPrint("dT(2.1) = ");
      vector<classA>().swap(vectA);//capacity to 0

      cout << "  Fill vector, without prior resizing/reserving. Logging to file. " << endl;
      for (jj=1; jj<N_ELEMENTS; jj=jj*LOOP_STEP)
      {
            crono.start();
            for (ii=0; ii<jj; ii++)
            {
                  newAobj = new classA( (double) (ii+1) );
                  vectA.push_back(*newAobj);
            }
            crono.stop();
            logFile << jj << " " << crono.getdT() << endl;
            vector<classA>().swap(vectA);//reset capacity to 0
      }
      logFile << endl;
            
      cout << "  Fill vector, with prior reserve(). ";
      crono.start();
      vectA.reserve(N_ELEMENTS);
      for (ii=0; ii<N_ELEMENTS; ii++)
      {
            newAobj = new classA( (double) (ii+1) );
            vectA.push_back(*newAobj);
      }
      crono.stopAndPrint("dT(2.2) = ");      
      vector<classA>().swap(vectA);//capacity to 0
      
      cout << "  Fill vector, with prior reserve(). . Logging to file. " << endl;
      for (jj=1; jj<N_ELEMENTS; jj=jj*LOOP_STEP)
      {      
            crono.start();
            vectA.reserve(N_ELEMENTS);
            for (ii=0; ii<jj; ii++)
            {
                  newAobj = new classA( (double) (ii+1) );
                  vectA.push_back(*newAobj);
            }
            crono.stop();
            logFile << jj << " " << crono.getdT() << endl;
            vector<classA>().swap(vectA);//reset capacity to 0
      }
      logFile << endl;
      
      cout << "  Fill vector, with prior resize(). ";
      crono.start();
      vectA.resize(N_ELEMENTS);
      for (ii=0; ii<N_ELEMENTS; ii++)
      {
            newAobj = new classA( (double) (ii+1) );
            vectA[ii] = *newAobj;
      }
      crono.stopAndPrint("dT(2.3) = ");      
      
      
cout << endl << "3. Filling a list" << endl;      
      crono.start();
      for (ii=0; ii<N_ELEMENTS; ii++)
      {
            newAobj = new classA( (double) (ii+1) );
            list1.push_back(*newAobj);
      }
      crono.stopAndPrint("  Fill a list. dT(3.1) = ");
      
cout << endl << "4. Run over the container" << endl;      
      cout << "  Run over the vector. ";
      crono.start();
      for (ii=0; ii<N_ELEMENTS; ii++)
      {
            vectA[ii].by2();
      }
      crono.stopAndPrint("dT(4.1) = ");
      
      cout << "  Run over the list. ";
      crono.start();
      for (itr = list1.begin(); itr != list1.end(); itr++)
      {
            itr->by2();
      }
      crono.stopAndPrint("dT(4.2) = ");      
      
cout << endl << "5. Remove one element at the middle of the container" << endl;            
      cout << "  Remove a vector element. ";
      crono.start();
      vectA.erase( vectA.begin()+N_ELEMENTS/2,vectA.begin()+N_ELEMENTS/2+1 );
      crono.stopAndPrint("dT(5.1) = ");
      
      cout << "  Remove a list element. ";
      crono.start();
      itr = list1.begin();
      for (ii=0; ii<N_ELEMENTS/2; ii++) itr++;
      itr2 = itr;
      itr2++;
      //crono.start();
      list1.erase(itr,itr2);
      crono.stopAndPrint("dT(5.2) = ");  
      
cout << endl << "6. Insert one element at the middle of the container" << endl;            
      cout << "  Insert a vector element.";
      crono.start();
      vectA.erase( vectA.begin()+N_ELEMENTS/2,vectA.begin()+N_ELEMENTS/2+1 );
      crono.stopAndPrint("dT(6.1) = ");

      cout << "  Insert a list element.";
      //crono.start();
      itr = list1.begin();
      for (ii=0; ii<N_ELEMENTS/2; ii++) itr++;
      newAobj = new classA( (double) (ii+1) );
      crono.start();
      //list1.insert(itr,*newAobj);
      crono.stopAndPrint("dT(6.2) = ");  
      
cout << endl << "7. Things We can do with a list" << endl;                  
      cout << "  Sort the list by overloaded < operator. ";
      crono.start();
      list1.sort();
      crono.stopAndPrint("dT(7.1) = ");
      cout << "    First element is : " << list1.front().getValue() << endl;
      
      cout << "  Reverse the list. ";
      crono.start();
      list1.reverse();
      crono.stopAndPrint("dT(7.2) = ");
      cout << "    First element is : " << list1.front().getValue() << endl;
      
      cout << "  Splice listB at a given position of list1. ";
      for (ii=0; ii<10; ii++)
      {
            newAobj = new classA( - (double) (ii+1) );
            list2.push_back(*newAobj);
      }
      crono.start();
      list1.splice(list1.begin(),list2);
      crono.stopAndPrint("dT(7.2) = ");
      cout << "    First element is : " << list1.front().getValue() << endl;
      
      
cout << endl << "The end " << endl;
      cout << "  clear all" << endl << endl;
      vectA.clear();
      list1.clear();
      logFile.close();
      return 0;
}
