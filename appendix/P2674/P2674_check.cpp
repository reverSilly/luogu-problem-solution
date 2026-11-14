// P2674 轻量验证：公式反解 K  vs  正向枚举 (n,K) 计算 P(K,n)
// 自包含，无外部数据。固定种子随机 + 全量小范围 + 手工边界。
// 仅验证「最小的两个 K / 单个 K / Poor」的判定，与输出格式无关。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 方法一：公式反解（与正解一致）。返回排序后的 K 列表（N=1 时返回 {3,4}）。
vector<i64>formula(i64 N)
{
	vector<i64>ks;
	if(N==1)
		return {3,4};	// P(K,1)=1 对所有 K≥3
	for(i64 n=2;n*(n+1)/2<=N;++n)
	{
		i64 num=2*N-2*n,den=n*(n-1);
		if(num%den==0)
		{
			i64 K=2+num/den;
			if(K>=3)
				ks.push_back(K);
		}
	}
	sort(ks.begin(),ks.end());
	return ks;
}

// 方法二：正向暴力。枚举 n，再枚举 K，直接算 P(K,n)=n+(K-2)*n*(n-1)/2 判等。
// 对 n=1：P=1 恒成立，仅当 N=1 时有效，K 枚举到 10 足以取到最小两个。
vector<i64>brute(i64 N)
{
	vector<i64>ks;
	for(i64 n=1;n*(n+1)/2<=N;++n)
	{
		i64 kmax;
		if(n==1)
			kmax=(N==1?10:2);	// n=1 时 P 恒为 1；N≠1 则无解，kmax<3 使循环不执行
		else
			kmax=2+(2*N-2*n)/(n*(n-1));	// P(K,n)≤N 的 K 上界
		for(i64 K=3;K<=kmax;++K)
		{
			i64 P=n+(K-2)*n*(n-1)/2;
			if(P==N)
				ks.push_back(K);
		}
	}
	sort(ks.begin(),ks.end());
	ks.erase(unique(ks.begin(),ks.end()),ks.end());
	return ks;
}

// 把 K 列表转成题面输出串（无空格拼接；单个 K 只输出一个；空则 Poor）
string out(i64 N,const vector<i64>&ks)
{
	if(ks.empty())
		return "Poor"+to_string(N);
	string s=to_string(ks[0]);
	if(ks.size()>=2)
		s+=to_string(ks[1]);
	return s;
}

int main()
{
	mt19937 rn(20250674);
	i64 tested=0,bad=0;
	auto run=[&](i64 N)
	{
		++tested;
		auto a=formula(N),b=brute(N);
		// 只比较前两个 K（答案只关心最小两个）
		vector<i64>ta(a.begin(),a.begin()+min<size_t>(2,a.size()));
		vector<i64>tb(b.begin(),b.begin()+min<size_t>(2,b.size()));
		if(ta!=tb||out(N,a)!=out(N,b))
		{
			++bad;
			if(bad<=20)
				cout<<"MISMATCH N="<<N<<" formula="<<out(N,a)<<" brute="<<out(N,b)<<'\n';
		}
	};
	for(i64 N=1;N<=3000;++N)
		run(N);
	for(i64 N:{1,2,3,4,5,6,10,15,36,998991,1000405,999999,1000000,500000,123456,999983})
		run(N);
	for(int t=0;t<300;++t)
		run(1+rn()%1000000);
	cout<<"tested="<<tested<<" mismatch="<<bad<<'\n';
	cout<<(bad?"FAIL\n":"ALL OK\n");
	return 0;
}