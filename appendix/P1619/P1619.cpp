//
// 题目：P1619 解一元二次方程的烦恼
// https://www.luogu.com.cn/problem/P1619
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int lim{40000000};       // 可分解的上界
constexpr int sqr{6330};           // sqrt(4e7) 的上取整 + 松弛

int pr[1000],cntp;
bool comp[sqr];

void sieve()
{
	for(int i=2;i<sqr;++i)
	{
		if(comp[i])
			continue;
		pr[cntp++]=i;
		for(i64 j=(i64)i*i;j<sqr;j+=i)
			comp[j]=true;
	}
}

bool isprime(int x)
{
	if(x<2)
		return false;
	for(int k=0;k<cntp&&(i64)pr[k]*pr[k]<=x;++k)
		if(x%pr[k]==0)
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	sieve();

	vector<int>num;                // 每组提取出的数（超界统一记作 lim+1）
	string ln;
	while(getline(cin,ln))
	{
		bool has=false;
		i64 v=0;
		for(char c:ln)
			if('0'<=c&&c<='9')
			{
				has=true;
				if(v<=lim)
					v=v*10+(c-'0');
				if(v>lim)
					v=lim+1;       // 饱和，之后只关心"超界"这件事
			}
		if(!has)
			break;
		num.push_back((int)v);
	}

	for(int i=0;i<(int)num.size();++i)
	{
		int n=num[i];
		if(i)
			cout<<'\n';            // 每组数据之间空一行
		cout<<"Enter the number=\n";
		if(n<2)
		{
			cout<<"Prime? No!\n";
			continue;              // halt
		}
		if(n>lim)                  // 超界数按题面"不判素性"处理：直接判合数并报超界
		{
			cout<<"Prime? No!\n";
			cout<<"The number is too large!\n";
			continue;
		}
		if(isprime(n))
		{
			cout<<"Prime? Yes!\n";
			continue;              // 质数直接 halt
		}
		cout<<"Prime? No!\n";
		cout<<n<<'=';
		int x=n;
		bool first=true;
		for(int k=0;k<cntp&&(i64)pr[k]*pr[k]<=x;++k)
		{
			if(x%pr[k])
				continue;
			int e=0;
			while(x%pr[k]==0)
			{
				x/=pr[k];
				++e;
			}
			if(!first)
				cout<<'*';
			first=false;
			cout<<pr[k];
			if(e>1)
				cout<<'^'<<e;
		}
		if(x>1)                    // 剩下一个大质因子
		{
			if(!first)
				cout<<'*';
			cout<<x;
		}
		cout<<'\n';
	}
	return 0;
}
