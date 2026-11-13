// 题目：P2388 阶乘之乘
// 思路：末尾零 = min(v2,v5) = v5（因为 v2>=v5）
//       ∏ k! = ∏ j^{n-j+1}，v5(j)=Σ_i [5^i|j]
//       交换求和 ⇒ 每个 q=5^i 贡献 Σ_{m=1}^{n/q} (n-mq+1)
//       等差数列求和 ⇒ a*(n+1) - q*a*(a+1)/2，O(log_5 n)
//       答案约 1.25e15，必须 i64；本题不取模
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n,ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(i64 q=5;q<=n;q*=5)
	{
		i64 a=n/q;
		ans+=a*(n+1)-q*a*(a+1)/2;   // 先乘后除：a*(a+1) 必为偶数
	}
	cout<<ans<<'\n';
	return 0;
}
