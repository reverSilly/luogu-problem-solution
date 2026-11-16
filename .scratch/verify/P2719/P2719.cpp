// 题目：P2719 搞笑世界杯
// 最后两人同票 ⇔ 前 2n-2 人售完后某类票已售罄 ⇔ 状态为 (n,n-2) 或 (n-2,n)。
// 反面：状态恰为 (n-1,n-1)。到达该状态的路径上两类票始终未售罄（a,b≤n-1<n），
// 故每步都是公平掷硬币，P(反面)=C(2n-2,n-1)/2^{2n-2}=∏_{i=1}^{n-1}(2i-1)/(2i)。
// 答案=1-该乘积。O(n) 递推乘积，double 精度远优于 1e-4 输出要求。
#include<bits/stdc++.h>
using namespace std;

int n;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;n/=2;	// 输入是 2n，转成每类票数 n
	double prod=1;	// prod=C(2n-2,n-1)/2^{2n-2}，n=1 时循环不执行，prod=1，答案 0
	for(int i=1;i<n;++i)
		prod*=double(2*i-1)/(2*i);
	cout<<fixed<<setprecision(4)<<1-prod<<'\n';
	return 0;
}