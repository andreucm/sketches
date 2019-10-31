/**stringTest.cpp*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>


// bool String2Int(const std::string & str, int & result)
// {
//     try
//     {
//         std::size_t lastChar;
//         result = stoi(str, &lastChar, 10);
//         return lastChar == str.size();
//     }
//     catch (std::invalid_argument&)
//     {
//         return false;
//     }
//     catch (std::out_of_range&)
//     {
//         return false;
//     }
// }

int main()
{
	//playing with strings
	std::string aa, bb, cc;
	char buf[50];
      int aaInt;

	aa="Hello";
	bb="World!";
	aa.append(" ");
	aa.append(bb);
	aa.append(" (string version)");
	std::cout << aa << std::endl;
	cc = "Hello World! (string version)";
	aa.append("\0"); //this has no effect when compare
	std::cout << "compare result = " << aa.compare(cc) << std::endl;
	aa.clear();
	buf[0] = 'a';buf[1] = 'b';buf[2] = '\0';
	aa.append(buf);
	std::cout << aa << std::endl;

	std::string myStr = "hola";

	if (myStr.compare("") != 0) std::cout << "myStr is not void" << std::endl;
	else std::cout << "myStr is void" << std::endl;

	//string exportCmnd = "export DISPLAY=':0.0'";
	//string exportCmnd = "export ZZZ=':0.0'";
	//system(exportCmnd.c_str());

      //string to integer
      aa = "123000";
      //bool rtV = String2Int(aa,aaInt);
      //std::cout << rtV << "; " << aaInt << std::endl;


      std::string s1("hola");
      std::string s2("hola");

      std::cout << "s1.compare(s2): " << s1.compare(s2) << std::endl;

	  std::cout << "s2.compare(s1): " << s2.compare(s1) << std::endl;
      std::string s3("jpñs");
      std::cout << "s1.compare(s3): " << s1.compare(s3) << std::endl;
      std::cout << "s3.compare(s1): " << s3.compare(s1) << std::endl;

      s1 = "<orderItem>";
      s2 = "<orderItem>";
      std::cout << "s1.compare(s2): " << s1.compare("<orderItem>") << std::endl;
      std::cout << "s2.compare(s1): " << s2.compare(s1) << std::endl;

	  std::string str_1("param1");
	  str_1.insert(0,"namespace/");
	  std::cout << "str_1: " << str_1 << std::endl;

	  std::string str_2("namespace");
	  std::cout << "str_2+std::string(\"/param1\"): " << str_2+std::string("/param1") << std::endl;
	  std::cout << "str_2.append(\"/param1\"): " << str_2.append("/param1") << std::endl;

	return 0;
}
