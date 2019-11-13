#include <algorithm>
#include <iostream>
#include <vector>

bool isFive(int x)
{
	if (x==5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
	std::vector<int> v={0,1,5,3,4,5,6,5,8,9};
	v.erase(std::remove_if(v.begin(), v.end(), isFive), v.end());
	for(auto i : v)
		std::cout << i << std::endl;
}
