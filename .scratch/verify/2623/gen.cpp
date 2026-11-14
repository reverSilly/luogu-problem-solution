#include<bits/stdc++.h>
using namespace std;
int main()
{
	// 最坏规模：100 件甲类（泛化）物品，m=2000，A=200 使内层循环全程有效
	printf("100 2000\n");
	for(int i=0;i<100;++i)printf("1 200 0\n");
	return 0;
}