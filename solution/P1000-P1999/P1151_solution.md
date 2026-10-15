# P1151 子数整数

## 1. 重述问题

对一个五位数 abcde，取三数子：sub1=abc, sub2=bcd, sub3=cde。给定 K（K<1000），求 10000 到 30000 中所有满足 sub1、sub2、sub3 都能被 K 整除的五位数。从小到大输出，无解输出 No。

## 2. 推理链

### 2.1 直接枚举

五位数范围 10000..30000，总共 20001 个。每次 O(1) 截取三位，检查整除。完全可行。

### 2.2 计算方法

对五位数 x：
- sub1 = x / 100
- sub2 = (x / 10) % 1000
- sub3 = x % 1000

检查 (sub1%K==0 && sub2%K==0 && sub3%K==0)。

## 3. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int K,ok=0;
	cin>>K;
	for(int x=10000;x<=30000;++x)
	{
		int s1=x/100;
		int s2=(x/10)%1000;
		int s3=x%1000;
		if(s1%K==0&&s2%K==0&&s3%K==0)
		{
			cout<<x<<'\n';
			ok=1;
		}
	}
	if(!ok)cout<<"No\n";
	return 0;
}
```
