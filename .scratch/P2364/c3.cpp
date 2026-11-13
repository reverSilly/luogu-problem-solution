#include<bits/stdc++.h>
using namespace std;
int main()
{
	puts("hello");
	ifstream fin("in.txt");
	if(!fin){puts("cannot open");return 0;}
	string a;fin>>a;
	printf("a=%s\n",a.c_str());
	return 0;
}
