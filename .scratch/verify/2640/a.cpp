// 题目：P2640 神秘磁石
// 思路：破坏力最大 ⇔ 两坐标都是素数且差恰好为 k。
//       埃氏筛打出 n 以内素数表，枚举较小素数 p，判 p 与 p+k 是否都素数且 p+k<=n。
//       按 p 升序输出，每对一行；一对都没有输出 empty。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,k;
bool np[mxn];	// np[i]=true 表示 i 不是素数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	np[0]=np[1]=true;
	for(int i=2;i*i<=n;++i)
		if(!np[i])
			for(int j=i*i;j<=n;j+=i)
				np[j]=true;
	bool found=false;
	for(int p=2;p+k<=n;++p)	// p 是较小坐标，p+k<=n 保证两端都不越界
		if(!np[p]&&!np[p+k])
		{
			cout<<p<<' '<<p+k<<'\n';
			found=true;
		}
	if(!found)
		cout<<"empty\n";
	return 0;
}
