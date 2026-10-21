//
// 题目：P1601 高精度加法
// https://www.luogu.com.cn/problem/P1601
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

string a,b,sum_;                 // sum_ 低位在前地存放结果

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b;

	int i=a.size()-1,j=b.size()-1,carry=0;
	while(i>=0||j>=0||carry)
	{
		int x=i>=0?a[i--]-'0':0;
		int y=j>=0?b[j--]-'0':0;
		int t=x+y+carry;
		sum_+=char('0'+t%10);
		carry=t/10;
	}
	reverse(sum_.begin(),sum_.end());

	int p=0;                     // 去掉结果的前导零, 至少保留一位
	while(p+1<(int)sum_.size()&&sum_[p]=='0')
		++p;
	cout<<sum_.substr(p)<<'\n';
	return 0;
}
