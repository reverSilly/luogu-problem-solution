//
// 题目：P1532 卡布列克圆舞曲
// https://www.luogu.com.cn/problem/P1532
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 step(i64 v,int k)             // 一次卡布列克变换,固定 k 位宽(前导补零)
{
	int c[12]{};
	for(int i=0;i<k;++i)
	{
		c[i]=int(v%10);
		v/=10;
	}
	sort(c,c+k);
	i64 mx=0,mn=0;
	for(int i=0;i<k;++i)
	{
		mn=mn*10+c[i];            // 升序拼成的较小数
		mx=mx*10+c[k-1-i];        // 降序拼成的较大数
	}
	return mx-mn;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n;
	while(cin>>n)                 // 多行直到 EOF
	{
		int k=0;
		for(i64 t=n;t;t/=10)
			++k;
		if(k==0)
			k=1;                  // 0 视作 1 位数
		unordered_map<i64,int>pos; // 值 -> 首次出现位置
		vector<i64>seq;
		i64 v=n;
		while(!pos.count(v))      // 走到重复值即进入循环
		{
			pos[v]=int(seq.size());
			seq.push_back(v);
			v=step(v,k);
		}
		int st=pos[v];            // 循环节从该值首次出现处开始
		for(int i=st;i<(int)seq.size();++i)
			cout<<seq[i]<<(i+1<(int)seq.size()?' ':'\n');
	}
	return 0;
}
