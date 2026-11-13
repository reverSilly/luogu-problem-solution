#include<bits/stdc++.h>
using namespace std;

int main()
{
	mt19937 rn(chrono::steady_clock::now().time_since_epoch().count());
	for(int i=0;i<6;++i)
		cout<<rn()%4<<(i==5?'\n':' ');
	return 0;
}
