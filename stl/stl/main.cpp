#include "alloc\alloc.h"
#include <vector>

int main()
{
	std::vector<int, Alloc<int>> vec = {1,2,3,4,5};
	for (auto x : vec)
	{
		std::cout << x;
	}
		
	system("pause");
	return 0;
}