# P1150 Peter 的烟

## 1. 重述问题

Peter 有 n 根烟，抽完后得到烟蒂。每 k 个烟蒂（k > 1）可以换 1 根新烟。问最终总共能吸多少根烟。不能借烟蒂。

## 2. 推理链

### 2.1 直接模拟

用变量 `ans` 记录已吸总数，`stub` 记录手上烟蒂数。

每次循环：
- 吸掉手上的全部烟 n：`ans += n`，`stub += n`
- 用 stub/k 兑换新烟，留下 stub%k 个余数烟蒂

循环直到 n == 0。

### 2.2 正确性

这直接翻译了"手上有烟就吸，有烟蒂就换"的描述。n 迅速缩小（每次至少除以 k），对数轮即结束。

## 3. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 n,k;
	cin>>n>>k;
	i64 ans=0,stub=0;
	while(n)
	{
		ans+=n;
		stub+=n;
		n=stub/k;
		stub%=k;
	}
	cout<<ans<<'\n';
	return 0;
}
```
