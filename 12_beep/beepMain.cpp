
#include <iomanip>

int main()
{
	const char BeepChar = '\a'; /* the "alarm" special char */
    	cout << "Maybe you can hear a beep here---" << BeepChar << endl;
	cout.put(0x07); 
	return 0;
}

