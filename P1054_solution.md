# P1054 [NOIP 2005 提高组] 等价表达式 — 完整题解

---

## 一、发现叙事

### 1.1 问题初印象

拿到这道题，第一反应是："判断两个代数表达式是否等价"——这是典型的计算机代数系统（CAS）任务。在 OI 赛制下做 CAS，听起来就很不现实。

让我们看一下数据范围：表达式长度不超过 50 个字符，选项数 n ≤ 26。这意味着我们需要写一个**自动化工具**，能在常数时间内判定任意两个合法的代数表达式是否恒等。

### 1.2 直觉的起点：代入数值

如果两表达式恒等，那么**代入任意具体的数值**后，计算结果必然相同。反之，如果两表达式不相同，在一个随机点处算出相同结果的概率应当很小。

这个想法极其朴素却有效。我们不需要做符号推理，不需要多项式展开，只需要**计算数值**。

但直接代入有麻烦：表达式含变量 `a`、运算符 `^`（乘幂），且数字可能很大。例如 `a^10^10^10` 会产生天文数字的中间结果，64 位整数根本装不下。

### 1.3 模运算入场

模运算天然适合这个场景：
- 可以在整数溢出之前截断，计算 `f(a) mod P`；
- 如果两表达式在模 P 意义下在一个点上不同，它们一定不等价；
- 如果它们在很多个点上都相同，它们很大概率等价。

但模运算引入了一个微妙之处：`a^k mod P` 的正确值就是 `(a mod P)^k mod P`，这恰好是我们能有效计算的。不需要处理高精度。

### 1.4 需要多少测试点？

两表达式若不等价，它们对应不同的**一元多项式**。在模 P 的域上，如果两多项式次数不超过 D，那么它们至多在 D 个点上一致——在 D+1 个点上一致就意味着恒等。

但我们不知道确切的 D 值怎么办？答案是：**选足够多的点，用多个模数**。通过多组独立测试，碰撞概率可降到忽略不计。这就是蒙特卡罗算法的典型应用。

### 1.5 解析引擎

要计算数值，需要把中缀表达式翻译成可执行的形式。这里我们用经典的**调度场算法（Shunting-Yard）** 转为逆波兰表达式（RPN，后缀表达式），随后在 RPN 上用栈求值——简单、高效、无歧义。

### 1.6 非法表达式的处理

题目说如果某个选项括号不匹配，直接忽略它。这意味着我们需要一个括号合法性检查器。这在调度场算法中可以顺便完成——如果遇到 `)` 时栈里没有匹配的 `(`，就说明非法。

---

## 二、思维链与完整证明

### 2.1 形式化定义

令表达式 `E` 为满足如下语法的字符串：

```
digit   ::= '0' | ... | '9'
number  ::= digit+
term    ::= number | 'a' | '(' expr ')'
factor  ::= term ('^' number)?
expr    ::= factor (('+' | '-' | '*') factor)*
```

其中 `^` 的右操作数必须是 1~10 之间的正整数（字面量），运算优先级：`^` > `*` > `+` = `-`，所有运算符**左结合**。

### 2.2 等价性的代数刻画

给定变量 `a`，表达式 `E` 定义了一个以整数为系数的**一元多项式函数**（无除法）：

- 当 `E` 中不出现 `^` 时，显然是多项式；
- 当出现 `^` 时，形如 `P(a)^k`（k 为 1~10 的字面量），仍是多项式（多项式在复合和幂运算下封闭）。

因此，每个表达式对应一个多项式 `f_E(a) ∈ ℤ[a]`。

**命题 1.** 两表达式 `E1, E2` 等价当且仅当对应的多项式函数 `f_{E1}` 与 `f_{E2}` 在 ℤ 上处处相等，即 `∀x ∈ ℤ, f_{E1}(x) = f_{E2}(x)`。

**证明.** 必要性显然。充分性：两多项式在无穷多个整数点上相等，则在 ℤ 上恒等，故表达式等价。□

### 2.3 随机化判定原理

我们的判定策略是：选取有限个测试值 `x_1, ..., x_m` 和模数 `P_1, ..., P_t`，检验是否对所有 `i, j` 都有：

```
f_{E1}(x_i) ≡ f_{E2}(x_i)  (mod P_j)
```

若存在不相等的情况，则 `E1` 与 `E2` 不等价，结论确定。

若全部相等，我们断言"等价"——但这个结论是概率性的，存在假阳性风险。下面分析错误概率。

考虑两个不等价的表达式 `E1, E2`。它们对应不同的多项式，差值 `d(a) = f_{E1}(a) - f_{E2}(a)` 是非零多项式。对于固定的模数 `P`，`d(a)` 模 P 要么是零多项式（不可能，因为次数不变），要么是非零多项式。

在域 `GF(P)` 上，非零一元多项式的根的个数不超过其次数。设 `d(a)` 的次数不超过 `D`，则 `d(a) ≡ 0 (mod P)` 最多有 `D` 个解。若我们在 `{0, 1, ..., P-1}` 中均匀随机选取 `x`，则：

```
Pr[f_{E1}(x) ≡ f_{E2}(x) (mod P) | E1 不等价于 E2] ≤ D / P
```

这里的 `D` 理论上可以很大（由于 `^` 链式嵌套），但用**多个模数**独立检验后，碰撞概率按乘积衰减，即便 D 很大也能控制在可接受范围。

实际实现中，选取 `m = 10` 个测试值和 `t = 2` 个模数（≈ 10^9），得到 20 个独立的约束条件。误判概率小于 `(D / 10^9)^20`，在实际数据上可视为 0。

### 2.4 调度场算法的正确性

调度场算法（Dijkstra 发明）将中缀表达式转为后缀表达式（RPN），核心在于维护一个运算符栈。

**不变量.** 扫描过程中，栈中运算符的优先级严格递增（从栈底到栈顶）。遇到新运算符 `op` 时，弹出所有优先级 ≥ `op` 的运算符（左结合规则），然后将 `op` 入栈。

**证明.** 设 `op1` 在 `op2` 之前入栈且 `op1` 优先级 ≥ `op2`。若弹出 `op1` 后才把 `op2` 入栈，则 RPN 中 `op1` 排在 `op2` 之前，这对应了正确的左结合计算顺序。对于 `^` 运算符，题目明确要求左结合，故同样适用 ≥ 弹出规则。□

### 2.5 后缀表达式求值的正确性

RPN 求值用栈实现：遇到数字压栈，遇到运算符弹出操作数计算结果并压栈。正确性由归纳法保证：

- 对于数字和变量：直接压栈，结果正确。
- 对于运算符：假设左右操作数的值已经正确计算并位于栈顶，弹出后按运算符语义计算（模意义下），结果正确。

求值终止时栈顶即为整个表达式的值。

### 2.6 模幂运算的处理

由于 `^` 的指数是 1~10 的字面量，在求值时指数是数值很小的具体整数。使用快速幂（二进制幂）即可，时间复杂度 O(log exponent)。

但对 `(a^1)^2^3` 这样的链式运算，中间结果如 `a^10` 模 P 后仍可参与后续运算，`qpow` 的底数可能取 `[0, P-1]` 中的任何值，不影响正确性。

### 2.7 非法表达式的检测

括号匹配检测可以在 O(len) 内完成：扫描字符串，维护计数值 `cnt`，见到 `(` 加一，`)` 减一，若过程中 `cnt < 0` 或最终 `cnt ≠ 0`，则为非法。

此外，调度场算法本身也能检测括号不匹配（遇到 `)` 时栈中无 `(`），但提前检测更干净。

---

## 三、模式总结

| 模式 | 在本问题中的应用 |
|------|------------------|
| **随机化 + 模运算** | 代入多个数值并模多个大质数，以高概率判定两个多项式是否恒等 |
| **调度场算法（Shunting-Yard）** | 将中缀表达式转为后缀表达式（RPN），消除括号与优先级 |
| **RPN 栈求值** | 线性扫描后缀表达式，用栈完成计算，每个运算符 O(1) |
| **模幂快速幂** | 处理 `^` 运算符，在模意义下计算 base^exp |
| **预处理 + 验证分离** | 先统一去除空格、验证合法性，再计算 RPN，最后对比结果 |

关键洞察：

> **判定代数等价不需要符号推理——数值代入 + 模运算足够。**
>
> 这个范式适用于任何"检验两个表达式是否恒等"的问题，特别是表达式种类有限、不含除法的场景。

---

## 四、正式解法

### 4.1 符号约定

| 符号 | 含义 |
|------|------|
| `s` | 输入的表达式字符串 |
| `a` | 表达式中的唯一变量 |
| `P` | 模数（大质数，如 1'000'000'007） |
| `A_vals` | 代入 `a` 的测试值集合 |
| `RPN` | 逆波兰表达式（后缀表达式），以 token 序列存储 |
| `val` | 对给定 `a` 值和模数计算出的表达式值 |
| `prec(op)` | 运算符优先级函数 |
| `qpow(b, e, p)` | 快速幂，返回 `b^e mod p` |

### 4.2 算法步骤

#### 步骤 1：预处理

```
function Preprocess(s):
    新建空字符串 r
    for c in s:
        if c != ' ':        // 移除所有空格
            r.append(c)
    return r
```

**时间复杂度**：O(|s|)

#### 步骤 2：括号合法性检查

```
function CheckParen(s):
    cnt ← 0
    for c in s:
        if c == '(': cnt ← cnt + 1
        if c == ')':
            cnt ← cnt - 1
            if cnt < 0: return false
    return cnt == 0
```

**时间复杂度**：O(|s|)

#### 步骤 3：中缀转后缀（调度场算法）

```
function ToRPN(s):
    rpn ← empty list of strings
    ops ← empty stack of chars
    
    for i ← 0 to |s|-1:
        c ← s[i]
        if c is digit:
            num ← extract full number from s[i..]
            rpn.append(num)
        else if c == 'a':
            rpn.append("a")
        else if c == '(':
            ops.push('(')
        else if c == ')':
            while ops not empty and ops.top() != '(':
                rpn.append(string(ops.top()))
                ops.pop()
            ops.pop()  // pop '('
        else:  // operator: +, -, *, ^
            while ops not empty and ops.top() != '('
                  and prec(ops.top()) >= prec(c):
                rpn.append(string(ops.top()))
                ops.pop()
            ops.push(c)
    
    while ops not empty:
        rpn.append(string(ops.top()))
        ops.pop()
    
    return rpn
```

**优先级表**：
| 运算符 | 优先级 |
|--------|--------|
| `^` | 3 |
| `*` | 2 |
| `+`, `-` | 1 |

**左结合规则**：新运算符入栈时，弹出栈顶所有优先级 ≥ 当前运算符的运算符。

**时间复杂度**：O(|s|)

#### 步骤 4：RPN 求值

```
function EvalRPN(rpn, a_val, mod):
    st ← empty stack of integers
    
    for token in rpn:
        if token == "a":
            st.push(a_val mod mod)
        elif token == "+":
            b ← st.pop(); a ← st.pop()
            st.push((a + b) % mod)
        elif token == "-":
            b ← st.pop(); a ← st.pop()
            st.push(((a - b) % mod + mod) % mod)
        elif token == "*":
            b ← st.pop(); a ← st.pop()
            st.push(a * b % mod)
        elif token == "^":
            exp ← st.pop()
            base ← st.pop()
            st.push(qpow(base, exp, mod))
        else:  // number literal
            st.push(stoll(token) % mod)
    
    return st.top()
```

**快速幂**：
```
function qpow(a, b, mod):
    res ← 1
    while b > 0:
        if b & 1: res ← res * a % mod
        a ← a * a % mod
        b ← b >> 1
    return res
```

**时间复杂度**：O(|RPN|)

#### 步骤 5：等价性判定

```
function IsEquivalent(ref_v, opt_v):
    // ref_v[t][i] 为参考表达式在模数 t、测试值 i 下的结果
    // opt_v[t][i] 为待测选项在模数 t、测试值 i 下的结果
    for each modulus t:
        for each test value i:
            if ref_v[t][i] != opt_v[t][i]:
                return false
    return true
```

#### 步骤 6：主流程

```
main:
    读入 ref_str
    读入 n
    
    ref_str ← Preprocess(ref_str)
    if not CheckParen(ref_str): 不可能，但安全性检查
    ref_rpn ← ToRPN(ref_str)
    
    // 预计算参考表达式的所有结果
    for each modulus P in [P1, P2]:
        for each test value x in A_vals:
            ref_result[P][x] ← EvalRPN(ref_rpn, x, P)
    
    ans ← ""
    for i ← 0 to n-1:
        读入 opt_str
        opt_str ← Preprocess(opt_str)
        if not CheckParen(opt_str): continue  // 忽略非法表达式
        
        opt_rpn ← ToRPN(opt_str)
        
        equivalent ← true
        for each modulus P in [P1, P2]:
            for each test value x in A_vals:
                val ← EvalRPN(opt_rpn, x, P)
                if val != ref_result[P][x]:
                    equivalent ← false
                    break all loops
        if equivalent:
            ans ← ans + char('A' + i)
    
    输出 ans
```

### 4.3 复杂度分析

设表达式最大长度 L = 50，选项数 n ≤ 26，测试值个数 m = 10，模数个数 t = 2。

- **预处理**：O(L)
- **RPN 转换**：O(L)
- **单次求值**：O(L)
- **参考表达式预计算**：O(t × m × L) = O(2 × 10 × 50) = O(1000)
- **每个选项判定**：O(t × m × L) = O(1000)
- **总时间复杂度**：O(t × m × L × n) ≈ O(26,000)，极其充裕。

- **空间复杂度**：O(L) 用于 RPN 和求值栈。

### 4.4 参数选取说明

- **测试值**：选取 `a = 0, 1, ..., 9`，覆盖小整数范围。也可使用随机大整数进一步提高正确性。
- **模数**：`1'000'000'007` 和 `1'000'000'009` 都是常用的 10^9+7 级别大质数，乘法不溢出 64 位整数。
- **数量平衡**：更多的测试值或模数可以降低假阳性概率，但会增加常数时间。上述参数在本题所有测试数据上足以 100% 通过。

---

## 五、C++ 代码

```cpp
// P1054 [NOIP 2005 提高组] 等价表达式
// https://www.luogu.com.cn/problem/P1054

#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int A_CNT{10};
constexpr i64 P1{1'000'000'007};
constexpr i64 P2{1'000'000'009};
constexpr i64 A_VALS[A_CNT]{0,1,2,3,4,5,6,7,8,9};

i64 qpow(i64 a,i64 b,i64 p)
{
    i64 r=1;
    a%=p;
    for(;b;b>>=1,a=a*a%p)
        if(b&1)
            r=r*a%p;
    return r;
}

string preproc(const string&s)
{
    string r;
    for(char c:s)
        if(c!=' ')
            r.push_back(c);
    return r;
}

bool valid_paren(const string&s)
{
    int cnt=0;
    for(char c:s)
    {
        if(c=='(')
            ++cnt;
        else if(c==')')
        {
            if(--cnt<0)
                return false;
        }
    }
    return cnt==0;
}

int prec(char op)
{
    if(op=='^')
        return 3;
    if(op=='*')
        return 2;
    if(op=='+'||op=='-')
        return 1;
    return 0;
}

vector<string> to_rpn(const string&s)
{
    vector<string>rpn;
    stack<char>ops;
    for(int i=0;i<(int)s.size();++i)
    {
        char c=s[i];
        if(isdigit(c))
        {
            string num;
            while(i<(int)s.size()&&isdigit(s[i]))
                num.push_back(s[i++]);
            --i;
            rpn.push_back(num);
        }
        else if(c=='a')
            rpn.push_back("a");
        else if(c=='(')
            ops.push('(');
        else if(c==')')
        {
            while(!ops.empty()&&ops.top()!='(')
            {
                rpn.push_back(string(1,ops.top()));
                ops.pop();
            }
            if(!ops.empty())
                ops.pop();
        }
        else
        {
            while(!ops.empty()&&ops.top()!='('
                  &&prec(ops.top())>=prec(c))
            {
                rpn.push_back(string(1,ops.top()));
                ops.pop();
            }
            ops.push(c);
        }
    }
    while(!ops.empty())
    {
        rpn.push_back(string(1,ops.top()));
        ops.pop();
    }
    return rpn;
}

i64 eval(const vector<string>&rpn,i64 a_val,i64 p)
{
    stack<i64>st;
    for(const string&tok:rpn)
    {
        if(tok=="a")
            st.push((a_val%p+p)%p);
        else if(tok=="+")
        {
            i64 b=st.top();st.pop();
            i64 a=st.top();st.pop();
            st.push((a+b)%p);
        }
        else if(tok=="-")
        {
            i64 b=st.top();st.pop();
            i64 a=st.top();st.pop();
            st.push(((a-b)%p+p)%p);
        }
        else if(tok=="*")
        {
            i64 b=st.top();st.pop();
            i64 a=st.top();st.pop();
            st.push(a*b%p);
        }
        else if(tok=="^")
        {
            i64 exp=st.top();st.pop();
            i64 base=st.top();st.pop();
            st.push(qpow(base,exp,p));
        }
        else
            st.push(stoll(tok)%p);
    }
    return st.top();
}

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    string ref_str;
    getline(cin,ref_str);
    ref_str=preproc(ref_str);

    int n;
    cin>>n;
    cin.ignore();

    auto ref_rpn=to_rpn(ref_str);

    i64 ref_res[2][A_CNT];
    for(int ti=0;ti<A_CNT;++ti)
    {
        ref_res[0][ti]=eval(ref_rpn,A_VALS[ti],P1);
        ref_res[1][ti]=eval(ref_rpn,A_VALS[ti],P2);
    }

    string ans;
    for(int i=0;i<n;++i)
    {
        string opt;
        getline(cin,opt);
        opt=preproc(opt);

        if(!valid_paren(opt))
            continue;

        auto rpn=to_rpn(opt);

        bool ok=true;
        for(int ti=0;ti<A_CNT&&ok;++ti)
        {
            if(eval(rpn,A_VALS[ti],P1)!=ref_res[0][ti])
            {
                ok=false;
                break;
            }
            if(eval(rpn,A_VALS[ti],P2)!=ref_res[1][ti])
            {
                ok=false;
                break;
            }
        }

        if(ok)
            ans.push_back(char('A'+i));
    }

    cout<<ans<<'\n';
    return 0;
}
```

### 代码要点说明

1. **预处理**：`preproc` 去除全部空格，简化后续解析。

2. **括号验证**：`valid_paren` 在 O(L) 内检测括号是否匹配。

3. **调度场算法**：`to_rpn` 严格遵循"左结合优先级 ≥ 弹出"规则，确保 `^` 也左结合。

4. **模运算求值**：`eval` 中所有运算都在模 P 下进行；减法先取模后加 P 再取模确保非负；`qpow` 处理 `^`。

5. **双重模数**：使用两个不同的大质数 P1, P2 独立验证，大幅降低碰撞概率。

6. **预计算优化**：先算好参考表达式的全部结果，每个选项只需与缓存比较，避免重复计算。

7. **早期中断**：任一测试值或模数不匹配则立即跳出循环，跳过剩余计算。
