// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main () 
{
      string line;
      ifstream myfile ("../dataFile.txt"); //creates and opens file stream
      ofstream outfile;  //creates file stream
      vector<string> dataVector;
      unsigned int ii;
      
      //opens output file
      outfile.open("/home/andreu/Desktop/outfile.txt", ofstream::out);
      
      //gets file data
      if (myfile.is_open())
      {
            while ( myfile.good() )
            {
                  getline (myfile,line);
                  //cout << line << endl;
                  dataVector.push_back(line);
            }
            myfile.close();
      }
      else cout << "Unable to open input file";
      
      //print data vector to std out and to outfile
      for (ii=0; ii<dataVector.size(); ii++)
      {
            cout << dataVector.at(ii) << endl;
            outfile << dataVector.at(ii) << endl;
      }
      
      outfile.close();
      
      return 0;
}
