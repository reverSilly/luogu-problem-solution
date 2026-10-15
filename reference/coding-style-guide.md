# 编码风格规范（Competitive Programming C++ Style Guide）

> 本规范基于对 606 个 `.cpp` 文件的全量代码分析提炼而成。
> 目标读者：需要按此风格续写代码的协作者/AI 助手。
> 优先级：**速度（编写 + 运行）> 简洁 > 可读性**。本规范描述"实际怎么写"，而非"理论上应该怎么写"。

***

## 目录

1. [文件结构总览](#1-文件结构总览)
2. [头文件与包含](#2-头文件与包含)
3. [命名空间](#3-命名空间)
4. [类型别名](#4-类型别名)
5. [常量与魔法数字](#5-常量与魔法数字)
6. [命名规范](#6-命名规范)
7. [结构体设计](#7-结构体设计)
8. [函数设计](#8-函数设计)
9. [Lambda 惯用法（sth 模式）](#9-lambda-惯用法sth-模式)
10. [输入 / 输出](#10-输入--输出)
11. [内存管理](#11-内存管理)
12. [现代 C++ 特性使用准则](#12-现代-c-特性使用准则)
13. [注释规范](#13-注释规范)
14. [算法与数据结构实现模式](#14-算法与数据结构实现模式)
15. [代码格式化](#15-代码格式化)
16. [版本与提交习惯](#16-版本与提交习惯)

***

## 1. 文件结构总览

每个 `.cpp` 文件是一个**独立完整**的题解，无头文件/源文件分离，无共享模板库。典型结构：

```cpp
// （可选）CLion 自动生成的文件头
//
// Created by <author> on YYYY/MM/DD.
//

// （可选）题目信息 / 判题结果（已脱敏）
// 题目：P3376

#include<bits/stdc++.h>
using namespace std;
using i64=long long;              // （按需）

constexpr int mxn{200005};         // （按需）数组尺寸
constexpr int p{998244353};        // （按需）模数

// 全局数组 / 容器
int a[mxn],dep[mxn];
vector<int>graph[mxn];

// 辅助函数（dfs、qpow、lowbit 等）
int ls(int i){return i*2+1;}

// 主算法函数
void dfs(int x,int f){...}

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	// 读入 → 处理 → 输出
	return 0;
}

// （可选）以注释形式保留的样例输入
```

**要点：**

- 全局状态优先：数组、容器、`ans` 尽量放全局作用域。

- 一题一文件，文件名 = 题目 ID（如 `P3376.cpp`、`CF600E.cpp`、`ABC201A.cpp`）。

- 不要建项目、不要写 Makefile 之外的构建脚本（存在一个 `CMakeLists.txt` 属例外）。

***

## 2. 头文件与包含

### 规则

- **只使用** **`#include<bits/stdc++.h>`**，不单独包含任何标准头。

- 需要 PBDS（平衡树、优先队列增强）时追加 `#include<bits/extc++.h>`。

```cpp
#include<bits/stdc++.h>            // 标准写法（约占 80%，手写）
#include <bits/stdc++.h>           // CLion 自动补全产生（约占 20%）
```

两种写法均可，不做强制。除此之外**禁止**出现 `<iostream>`、`<vector>` 等单独包含。

### 特殊守卫

未完成或有已知错误的文件，用编译期错误阻止误提交：

```cpp
#error undone            // 未完成
#error false solution    // 已知错误思路
```

***

## 3. 命名空间

```cpp
using namespace std;     // 无条件、全局、紧跟 include 之后
```

- 永远写 `using namespace std;`，不做选择性引入。

- **不定义自定义命名空间**（全库 606 个文件中仅 2 处例外）。

- 局部变量与全局同名时，用一元 `::` 访问全局而非改名：

```cpp
void bfs(int s,int t)
{
	::s=s; ::t=t;        // 参数 s/t 遮蔽了全局源点/汇点
}
```

- 极少数场合可局部引入字面量命名空间：`using namespace std::string_literals;`（函数内部）。

***

## 4. 类型别名

### 两种并存风格

| 风格              | 写法                      | 使用场景          |
| --------------- | ----------------------- | ------------- |
| **别名式（首选，新代码）** | `using i64=long long;`  | 需要区分 32/64 位时 |
| **宏式（遗留，仍接受）**  | `#define int long long` | 全题都用 64 位时    |

```cpp
using i64=long long;        // 标准 64 位别名，绝不写 typedef，绝不缩写为 ll
using u64=unsigned long long;
using u32=unsigned int;
using f64=double;           // 或 f80 = long double
```

### `#define int long long` 的配套规则

使用该宏时，`main` **必须写成无返回类型**（否则展开为 `long long main()`）：

```cpp
#define int long long
...
main()// 无 int 前缀
{
	...
}
```

若同文件混合使用，可局部定义后取消：

```cpp
#define int long long
	...                     // 需要 64 位的代码段
#undef int
int main(){...}             // 恢复 32 位
```

### 其他

- `__int128` 直接裸用（中间量防溢出），不定义别名。

- 不定义 `vi`、`pii` 等竞赛模板常见缩写。

***

## 5. 常量与魔法数字

### 数组尺寸

```cpp
constexpr int mxn{200005};              // 单一尺寸，braces 初始化
constexpr int mxn{100005},mxv{2000005}; // 多个逗号并列
```

- 命名：`mxn` / `mxm` / `mxv` / `mxk`（max n/m/value/k）。

- **"+5 松弛约定"**：尺寸 = 约束上界 + 5（100005、200005、5005、3005……），宁可虚高不可越界：约束 2×10⁵ 时可直接写 `mxn{2000005}`。

- 允许在简单题里省略命名常量，直接写裸尺寸：`bool win[10000];`。

### 模数

```cpp
constexpr int p{998244353};       // 小写 p（多数文件）
constexpr int P{1'000'000'007};  // 大写 P（少数）；新代码用 C++14 数字分隔符
constexpr i64 p{1'000'000'007};  // i64 场景
```

### 哨兵值（Sentinel）

按优先级依次使用：

1. **114514 家族**（个人签名，优先用于"显著不可能是答案"的场景）：

```cpp
int ans=-114514;                   // 普通负无穷
fill_n(dis,n,1145141919);          // 图论距离无穷
i64 ans=1145141919810;             // 更大范围的哨兵
dfs(0,1145141919);                 // 假父节点 ID
```

1. **科学计数法**（数值型 INF）：

```cpp
constexpr int inf{1e9};
i64 ans=1e18;
Q.push({i,1e9});                   // Dinic 流量上限
```

1. 浮点比较用 `constexpr double eps{1e-6};`（或 1e-5 / 1e-15 视精度需求）。

**允许** `1e9` 等字面量隐式转换为 int 上下文使用（`while(int cur=dfs(s,1e9))`）。

***

## 6. 命名规范

### 变量

| 类别       | 风格                                                                                                 | 示例                      |
| -------- | -------------------------------------------------------------------------------------------------- | ----------------------- |
| 维度       | 单字母，输入用大写或小写均可                                                                                     | `n,m,k,q,t,T,N,K`       |
| 答案       | 固定用 `ans`（多查询用 `Ans[q.i]` 数组）                                                                      | `ans`, `res`            |
| 循环变量     | `i,j,k,l,r,mid,x,y,u,v,w`                                                                          | <br />                  |
| DP 表     | `dp`                                                                                               | `dp[mxn][20]`           |
| 计数       | `cnt`, `scnt`                                                                                      | <br />                  |
| 图论       | `dis`, `vis`, `dep`, `sz`, `dfn`, `low`, `f`（DSU 父）, `fa`, `gs`/`gson`（重儿子）, `gf`（倍增祖先）, `top`（链头） | <br />                  |
| 图论：点     u,v  图论：边权    w    队列/边集    | 大写 `Q`, `E`, `H`（链式前向星 head）, `nx`                                                                 | <br />                  |
| 邻接表      | `graph` 或 `tree`                                                                                   | `vector<int>graph[mxn]` |
| 线段树/树状数组 | `tr`, `BIT`                                                                                        | <br />                  |
| 稀疏表      | `st`                                                                                               | `st[20][mxn*2]`         |
| 最大/最小    | `mx`, `mn`, `mxcnt`, `mxa`, `mxb`                                                                  | <br />                  |
| 临时占位     | `sth`, `aaa`, `mmm`, `o`, `_1`, `_n`                                                               | 允许随手命名                  |

<br />

<br />

### 函数（全小写、电报式）

```
dfs / dfs1 / dfs2 / dfs3     多遍树遍历（重编号而非重命名）
bfs / dij                    图遍历 / Dijkstra
solve / calc / check         主逻辑 / 计算 / 二分判定
ini                          初始化
qpow / inv / exgcd           快速幂 / 逆元 / 扩展欧几里得
lowbit / ls / rs             位运算 / 线段树子节点（i*2+1 / i*2+2）
getrt / rt                   DSU 查找
let / rlet                   线段树单点赋值
sigma                        树状数组前缀查询（"部分和"）
upd                          松弛更新 min/max
yes / no / e                 输出并 exit(0)
push_up / push_down          线段树上传/下传
sub1 / sub2 / sub123         子任务分发
ceildiv / floordiv           整除辅助
```

### 结构体

- 小写：`node`, `edge`, `seg`, `query`, `range`, `mat`, `E`

- 大写仅用于"较大的抽象"：`Machine`, `Interpreter`, `Trie`, `Chtholly`

- 成员为最小字段对：`{v,w}`（边）、`{l,r}`（区间）、`{sum,lmx,rmx,mx}`（线段树节点）

### 常量

`mxn`, `mxm`, `p`/`P`, `inf`, `eps`（小写为主，不强制全大写）。

### 模板参数

小写：`template<int n,int m> struct mat`、`template<bool up>`、`template<auto& arr>`。

***

## 7. 结构体设计

### 基本原则

- **纯聚合（aggregate）优先**：公有数据成员、无不变量、无 `private:`。

- 全库无 class，构造函数仅在确有价值时编写。

### 初始化

**全部使用花括号**（统一初始化）：

```cpp
E[cnt]={v,w};                       // 数组槽位赋值
return {max(a.mx,b.mx),min(a.mn,b.mn)};
Q.push({v,min(w,ww)});
constexpr int mxn{100005};          // 常量
int cnt[mxv]{};                     // 值初始化清零
bool vis[mxn]{};                     // 同上
```

C++20 指定初始化器（新代码鼓励）：

```cpp
new query{.a{x1-1,y1-1},.b{x2-1,y2-1}};
Instruction x{.a=now,.x=1,.b=now,.y=0,.ty=Type::POP};
```

### 允许的结构体形态

```cpp
// 1. 纯数据
struct edge{int v,w;};

// 2. 匿名结构体 + 数组一步声明（离线查询惯用）
struct{int l,r,i;}Q[mxn*3];

// 3. 嵌套
struct query
{
	struct P{int x,y;}a,b; 
	int ans;
};

// 4. NSDMI 成员默认值
struct seg{int sum{},lmx{},rmx{},mx{};};

// 5. 带极少量成员函数 / 静态方法
struct BIT
{
	int sum[114514];
	static constexpr int lowbit(int x){return x&-x;}
};

// 6. 运算符重载仅限排序与矩阵
friend bool operator<(edge a,edge b){return w[a.i]>w[b.i];}
template<int n,int m,int p> friend mat<n,p> operator*(mat<n,m>a,mat<m,p>b);
template<int n> mat<n,n> operator^(mat<n,n>a,u64 b);   // 矩阵快速幂
```

### 不做

- 不写 getter/setter，不写 `private`。

- 不做 RAII 封装（`unique_ptr` 除外，见 §11）。

- 状态不放结构体成员里，放全局或 `static` 局部。

***

## 8. 函数设计

### 形态选择优先级

1. **全局自由函数**（默认）
2. **局部 lambda**（递归、比较器、一次性的树操作）
3. 结构体静态方法（仅当确实属于该类型）
4. `std::function` 包装的自递归 lambda（`sth` 模式，见 §9）

### 常用骨架

```cpp
// DFS：父节点参数 + 邻接遍历
void dfs(int x,int f)
{
	for(int v:graph[x]) 
		if(v!=f) 
			dfs(v,x);
}

// DSU：路径压缩

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

// Dijkstra：惰性删除 + greater<>
priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
Q.emplace(0,s);
while(!Q.empty())
{
	auto[d,u]=Q.top();Q.pop();
	if(vis[u])
		continue;
	...
}

// 二分答案（整数，区间闭）
for(int l=0,r=n+1;l<r;)
{
	int mid=(l+r)>>1;
	if(check(mid))
		r=mid; 
	else
		l=mid+1;
}
// 注意：偏上界取 mid=(l+r+1)>>1

// 二分答案（浮点）
for(double l=0,r=1e20;r-l>1e-15;)
{
	double mid=(l+r)/2;
	if(check(mid)) 
		r=mid; 
	else
		l=mid;
}
```

### 辅助小函数

一行的辅助函数写成单行；`qpow`、`lowbit`、`ls/rs`、`exgcd` 等按需在每个文件**重新手写**，不建公共库：

```cpp
i64 qpow(i64 a,i64 b,i64 c=1'000'000'007)
{
	i64 res=1;
	for(;b;b>>=1,a=a*a%c)
		if(b&1) 
			res=res*a%c;
	return res;
}
void upd(int&x,int v){x=min(x,v);}   // 或 max / 带模加
```

### 函数内 `static` 局部变量

用于替代成员状态的隐藏全局：

```cpp
void dfs(...)
{
	static int cnt=0;        // DFS 计数器
	static vector<int>stk;   // Tarjan 栈
	static mt19937 rn(random_device{}());   // 局部随机引擎
}
```

***

## 9. Lambda 惯用法（sth 模式）

**本风格最具辨识度的模式**：线段树等递归结构一律不用成员函数，而是立即调用的自递归 lambda，统一命名为 `sth`：

```cpp
// 线段树标准模板
int ls(int i){return i*2+1;}
int rs(int i){return i*2+2;}

// build
function<void(int,int,int)>sth{[&sth](int l,int r,int i){
	if(r-l==1)
		/* 叶子：r-l==1 判定 */;
	else
	{
		int mid=(l+r)>>1;
		sth(l,mid,ls(i));
		sth(mid,r,rs(i));
		// push_up
	}
}};
sth(0,n,0);
```

**约定：**

- 区间语义：**半开** **`[l,r)`**，根节点下标 `0`，叶子条件 `r-l==1`。

- 自捕获写法 `[&sth]`（显式捕获自身）或 `[&]`。

- 查询/修改同样写成独立的 `sth` lambda，捕获边界 `[L,R,v,&sth]`。

- 递归 lambda 需要穿透作用域时才用 `std::function`；纯比较器直接写 `[](...){...}`。

### 其他 lambda 用法

```cpp
// 排序比较器（auto 参数允许）
sort(Q,Q+m,[](auto a,auto b){return a.r<b.r;});
sort(idx.begin(),idx.end(),[](int x,int y){return dis[x]<dis[y];});

// lambda 数组（方向切换）
function<bool()> trans[2]={[&](){...},[&](){...}};

// 模板参数化方向（配合 if constexpr）
template<bool ismx> void ini();
calc<1>(...); calc<-1>(...);        // 小-to-大：加重 / 删除共用
```

***

## 10. 输入 / 输出

### 必备模板

```cpp
int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);          // 可选
	ios::sync_with_stdio(false);
	...
	return 0;
}
```

顺序固定（`cin.tie` 在前），不使用 `cin>>...;ios::sync_with_stdio(false);` 的后置写法。

### 输出

- 换行一律用 `'\n'` 字符字面量，**禁止** **`endl`**。

- 单答案程序可不带尾随换行：`cout<<ans;`

- 三目输出字符串：`cout<<(ok?"Yes\n":"No\n");`（Yes/No 大小写按题目要求，不统一）。

- 浮点：`cout<<fixed<<setprecision(15)<<ans<<'\n';`

### 下标约定

- **内部一律 0-based**，读入时立即转换：

```cpp
cin>>u>>v>>w;--u;--v;
cin>>p;--p;
```

- 输出时再加回：`cout<<i+1<<' ';`

### 读入风格

```cpp
// 循环头声明临时变量（允许 6 个变量堆在 for-init）
for(int i=0,op,l,r,x,ii,y;i<q;++i)
{
	...
}
for(int i=1,u,v;i<n;++i)
{
	cin>>u>>v;--u;--v;
	graph[u].push_back(v);
	...
}

// 多测
int t;
cin>>t;
while(t--)
{
	...
}
// EOF 驱动
while(cin>>A>>B>>C)
{
	...
}
```

### 离线处理主形态

```
读入全部 → 按端点排序查询 → 扫描线求解 → 按原序号输出 Ans[q.i]
```

### 文件 I/O（OI 赛制）

```cpp
freopen("dfs.in","r",stdin);
freopen("dfs.out","w",stdout);
```

### 快速输出（仅在大输出量时）

```cpp
inline void wr(int x)
{
	if(x>9)
		wr(x/10);
	putchar(x%10+'0');
}
```

手写 `getchar()` 快读仅在极端卡常时使用（库中极少）。

***

## 11. 内存管理

### 唯一默认模型：全局静态数组

```cpp
constexpr int mxn{200005};
int a[mxn],dp[mxn],f[mxn];
vector<int>graph[mxn];
list<edge>E[mxm*2];            // 允许超大：bool dp[1005][1005][1005]; ≈ 1GB
struct{int l,r,i;}Q[mxn*3];
```

- 尽量在同一处定义

- 不 `new`/`delete`，不 `reserve`，不写分配器。

- 多测/多阶段需显式重置：

```cpp
for(int i=0;i<n;++i) 
	graph[i].clear();
fill_n(cnt,n,0);
iota(f,f+n,0);
```

### 链式前向星（网络流）

```cpp
struct{int v,w,nx;}E[mxm*2];
int H[mxn],cnt=0;
void addedge(int u,int v,int w)
{
	E[cnt]={v,w,H[u]};H[u]=cnt++;
	E[cnt]={u,0,H[v]};H[v]=cnt++;
}
// 反向边 XOR 定址：E[i^1].w+=res;
```

### 智能指针（少数场景）

指针型数据结构（Splay、LCT、动态开点线段树、可克隆 Trie）使用 `unique_ptr<node>` + `make_unique` + `move`：

```cpp
struct node
{
	int v,sz;
	node*f;
	unique_ptr<node>sons[2]{};
};
auto rt=make_unique<node>(v);
```

### 内存池（持久化结构）

```cpp
struct node{...}pool[mxn*30];
int cnt=0;
node*getnew()
{
	return pool+cnt++;
}          // 环形复用：cnt=(cnt+1)%(mxn*50)
vector<int>rubbish;                          // 空闲节点回收站
```

### 允许的泄漏

一次性的 `new node{}`（如 Trie 插入）允许不释放，可将 delete 逻辑以注释形式保留。

***

## 12. 现代 C++ 特性使用准则

**准则：省键击、免出错就用；增加架构复杂度就不用。**

### 大力使用

| 特性                                               | 写法示例                                              |
| ------------------------------------------------ | ------------------------------------------------- |
| 结构化绑定                                            | `auto[u,v,w]=E[i];` / `for(auto&[k,_]:M)`（允许 `_`） |
| `auto`                                           | 迭代器、lambda、返回类型推导                                 |
| 花括号初始化                                           | 标量、数组、NSDMI 全面使用                                  |
| `constexpr`                                      | 常量及编译期函数（`constexpr i64 qpow(...)`）               |
| 数字分隔符                                            | `1'000'000'007`                                   |
| `if constexpr`                                   | 模板方向分支（`template<bool up>`）                       |
| 透明比较器                                            | `greater<>`                                       |
| `iota` / `fill_n` / `partial_sum` / `accumulate` | STL 算法全家桶                                         |
| `__builtin_clzll` / `__lg` / `popcount`          | 位运算                                               |
| `__int128`                                       | 溢出中间量、大数取模                                        |
| `std::exchange`                                  | `sz[b]+=exchange(sz[a],0);`                       |
| `string_view`                                    | 解析函数参数                                            |
| 指定初始化器（C++20）                                    | `{.a=now,.x=1,...}`                               |
| `enum class` + 底层类型                              | `enum class Type:uint8_t{POP,HALT};`              |

### 谨慎/实验性使用

- `std::optional` / `std::variant` / Ranges / Concepts：**不用**（唯一实验文件 `P2838.cpp` 除外）。

- `consteval` + 立即调用 lambda 初始化 `constexpr static std::array`（brainfuck.cpp 模式）：高级技巧，允许。

- C++23 `starts_with`：可用（依赖 GCC 版本）。

- 异常、线程、自定义迭代器：不用。错误处理只有 `assert` + `exit(0)`。

### C 习惯允许并存

`putchar` / `scanf` / 裸 `main()` 可以与最现代的特性同文件共存，不做风格统一。

***

## 13. 注释规范

### 文件头注释（可选，按需组合）

```cpp
//
// Created by <author> on YYYY/MM/DD.
//

// 题目：AT_arc076_b                                 题目编号
// https://www.luogu.com.cn/problem/AT_arc076_b     题目链接
// AC / WA / TLE                                    判题结果标记
// also CF920E                                      相关题目交叉引用
// for review purpose                               复习标记
```

### 调试注释（本风格的标志性习惯）

**注释掉的** **`cerr`** **调试语句一律保留在源码中**，作为可随时启用的日志系统：

```cpp
// cerr<<u<<' '<<v<<' '<<w<<'\n';
// cerr<<"donate "<<mn[0]+1<<'\n';
```

调试宏（通常以注释禁用，用 `dbg;` 语句形式调用）：

```cpp
#define dbg cerr<<__LINE__<<' '<<__PRETTY_FUNCTION__<<'\n'   // 启用版
#define dbg //cerr<<__LINE__<<'\n'                            // 禁用版
```

极端场合的调试总开关：

```cpp
#define cerr (0)       // 干掉所有 cerr
```

### 行内注释

- 稀疏，只标注关键不变量与坑点：

```cpp
//be careful here!
//notice that we cannot use pre.back to replace a and b
//[l,r)                      区间语义标注
//0 does not donate to its father
//merge 2 / merge 3          DP 转移分类
```

- `assert` 作为文档化不变量使用：

```cpp
assert(dep[u]==dep[v]);
assert(lf%2==0);
```

### 允许保留的"考古层"

- 粗暴暴力版与正解并存的注释块（`calc` vs `calc2` 互验）。

- 废弃思路的完整注释块。

- 文件尾以注释粘贴的样例输入 / 输出数据表。

- 注释掉的 `freopen` 对、`return 0;` 早期退出开关。

语气允许口语化（`//what the fuck???`、`//so magical`、`//must review`）。

***

## 14. 算法与数据结构实现模式

### 逐文件手写，不引库

以下组件均按本规范模板在每个需要的文件中重写：

| 组件       | 要点                                                         |
| -------- | ---------------------------------------------------------- |
| 线段树      | `sth` lambda + `ls/rs`，半开区间，根下标 0                          |
| 树状数组     | `lowbit` + `sigma`（前缀和）/ `update`；支持区间加                    |
| DSU      | `getrt` 路径压缩一行式；可带按秩/带权/可撤销                                |
| Dijkstra | 惰性删除 + `greater<>` 小根堆                                     |
| Dinic    | BFS 分层 + DFS 增广 + `cur[]` 当前弧 + `E[i^1]` 反边                |
| LCA 倍增   | `gf[mxn][20]`，两段式"先抬差后同步降"                                 |
| 重链剖分     | `dfs2`（sz/gs）+ `dfs3`（dfn/top）+ 链上跳                        |
| 小-to-大   | `template<int shift> calc<±1>` 添加/删除复用                     |
| Mo's     | `blen=sqrt(n)`，`(block(l),r)` 排序，`push/pop_front/back` 四原语 |
| SOS DP   | `if(j&(1<<i)) dp[j]+=dp[j^(1<<i)];`                        |
| Chtholly | `map<int,int>` 区间赋值，`erase(it++)` 惯用法                      |
| 稀疏表      | `st[20][mxn*2]` + `__lg` 查询，方向模板化                          |
| 二进制 Trie | 31 位，`for(int i=30;i>=0;--i)`                              |
| 矩阵快速幂    | 模板 `mat<n,m>` + `operator*` / `operator^`                  |
| 数论       | `qpow`/`inv`/`exgcd`/线性筛/`fac`,`ifac` 预处理                  |

### 启发式与随机化

```cpp
static mt19937 rn(random_device{}());
while(clock()-beg<3*CLOCKS_PER_SEC) 
	ans=max(ans,solve());   // 时限内反复
```

### 子任务分发

```cpp
switch(o)
{
	case 5:...
	case 4:sub4();  // 允许刻意 fallthrough
	...
}
```
```cpp
if(m<500)
	sub2();
else
	sub1();   // 按规模启发分发
```

### 控制流

- 嵌套循环跳出允许 `goto label;` … `label:;`。

- 提前结束用 `exit(0)`（配合 `yes()/no()` 输出助手）。

- 条件声明进循环：`while(int cur=dfs(s,1e9))`。

- 多测间状态用"纪元翻转"避免清空：`vis[u]!=o` 配 `o=!o;`。

***

## 15. 代码格式化

- **缩进**：Tab 为主；从 OJ 粘贴或 CLion 生成的文件为 4 空格；同一文件内允许混用。

- **运算符**：普遍不加空格：`a[x]+b[y]`、`cnt[i]=(cnt[i]+1)%p`。`for` 头内逗号紧排：`for(int i=0,op,x;i<q;++i)`。

- **花括号**：函数体换行开括号；一行小函数整个写成单行。

- **include**：`#include<bits/stdc++.h>` 无空格为手写主流。

- `}` 与 `break;` 可同行：`}break;`

- 不跑 formatter，以手写自然格式为准。

***

## 16. 版本与提交习惯

- 同题多版本文件命名：`P3792.cpp` / `P3792'.cpp`（撇号）、`P10100_err.cpp`、`CF1918C (2).cpp`、`未命名22.cpp`。

- 画图草稿（`.sai2`、`.penin.mpk`）与题解笔记（`.md`）与代码同目录存放。

- 对拍配套：`gen.cpp`（造数据）、`sim.cpp` / `checker.cpp`（校验器）、`check.cpp`（`system()` 循环对拍）、`*.in/.out` 样例文件。

- 本地测试与正式提交同文件，用注释开关切换（`freopen` 对、`return 0;`）。

***

## 附：速查模板（新文件起手式）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{200005};
constexpr int p{998244353};

int n,m,a[mxn],dep[mxn],f[mxn];
vector<int>graph[mxn];

int ls(int i){return i*2+1;}
int rs(int i){return i*2+2;}
int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}
i64 qpow(i64 a,i64 b)
{
	i64 r=1;
	for(;b;b>>=1,a=a*a%p)
		if(b&1)
			r=r*a%p;
	return r;
}

void dfs(int x,int fa)
{
	for(int v:graph[x])
		if(v!=fa)
			dfs(v,x);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	// ... solve ...

	cout<<ans<<'\n';
	return 0;
}
```

***

*本规范为描述性规范（descriptive style guide），忠实反映既有 606 个文件的实际风格；与个人偏好冲突处，以"已在库中大量出现"的写法为准。*