#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};
constexpr int inf{1145141919};    // 114514 家族哨兵:"尚未找到任何方案"

int n;
bool np[mxn];                     // np[i]=true 表示 i 非质数(试除法独立打表)

bool isp(int x)                   // 试除法判质,与标程的埃氏筛互为独立实现
{
	if(x<2)
		return false;
	for(int i=2;i*i<=x;++i)
		if(x%i==0)
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<=n;++i)
		np[i]=!isp(i);
	int ba{inf},bb{inf};          // 当前字典序最小的前两个质数
	for(int a=2;a+4<=n;++a)       // 枚举一切整数 a,b(不只质数),c 由和唯一确定
		for(int b=2;a+b+2<=n;++b)
			if(!np[a]&&!np[b]&&!np[n-a-b]&&(a<ba||(a==ba&&b<bb)))
			{
				ba=a;             // 显式按题面 tie-break 规则比较,不靠"首个命中"
				bb=b;
			}
	cout<<ba<<' '<<bb<<' '<<n-ba-bb<<'\n';
	return 0;
}
