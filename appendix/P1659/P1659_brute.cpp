//
// 题目：P1659 [国家集训队] 拉拉队排练
// 暴力基准：枚举所有中心向两侧扩展统计全部奇数长回文子串长度，排序后取前 K 大乘积
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 p{19930726};

int n;
i64 k;
string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>s;
	vector<int>lens;
	for(int i=0;i<n;++i)
		for(int r=0;i-r>=0&&i+r<n&&s[i-r]==s[i+r];++r)
			lens.push_back(2*r+1);

	if((i64)lens.size()<k)
	{
		cout<<-1<<'\n';
		return 0;
	}
	sort(lens.begin(),lens.end(),greater<>());
	i64 ans=1;
	for(int i=0;i<k;++i)
		ans=ans*lens[i]%p;
	cout<<ans<<'\n';
	return 0;
}
