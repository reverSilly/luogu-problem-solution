const{execFileSync}=require('child_process');
const base='D:/Information/My/OI/luogu_problems/.scratch/verify/';
function run(id,input){
	return execFileSync(base+id+'/a.exe',[],{input,encoding:'utf8',timeout:20000}).replace(/\r\n/g,'\n').trim();
}
let fail=0;
function chk(id,input,exp,label){
	const got=run(id,input);
	const ok=got===String(exp);
	if(!ok)fail++;
	console.log((ok?'OK  ':'FAIL')+' P'+id+' '+label+' => '+got+(ok?'':' (expect '+exp+')'));
}
// ---------- P2818 ----------
chk('2818','7\n9\n',2,'statement example n=7 m=9');
chk('2818','7\n7\n',7,'full circle r=0');
chk('2818','7\n8\n',1,'one past circle');
chk('2818','2\n2\n',2,'min n');
chk('2818','3\n1'+'0'.repeat(1000)+'\n',1,'10^1000 mod 3');
chk('2818','9\n1'+'0'.repeat(1000)+'\n',1,'10^1000 mod 9');
chk('2818','100000000\n1'+'0'.repeat(1000)+'\n',100000000,'max n, 10^1000');
chk('2818','100000000\n100000001\n',1,'r=1');
chk('2818','99999999\n'+'9'.repeat(1001)+'\n',(() => {let r=0n;for(const ch of '9'.repeat(1001))r=(r*10n+BigInt(ch))%99999999n;return r===0n?99999999:Number(r);})(),'1001 digits of 9, BigInt cross-check');
// ---------- P2819 ----------
chk('2819','3 3 3\n1 2\n2 3\n3 1\n',6,'triangle m=3');
chk('2819','3 2 2\n1 2\n2 3\n',2,'path m=2');
chk('2819','2 1 3\n1 2\n',6,'single edge m=3');
chk('2819','4 6 3\n1 2\n1 3\n1 4\n2 3\n2 4\n3 4\n',0,'K4 m=3');
chk('2819','4 3 2\n1 4\n2 4\n3 4\n',2,'star center=4 m=2');
chk('2819','4 4 3\n1 2\n2 3\n3 1\n3 4\n',12,'triangle+pendant m=3');
chk('2819','1 1 5\n1 1\n',0,'self loop');
chk('2819','2 2 2\n1 2\n1 2\n',2,'parallel edges');
chk('2819','4 2 2\n1 2\n3 4\n',4,'disconnected robustness');
{
	let s='10 10 3\n';for(let i=1;i<=10;i++)s+=i+' '+(i%10+1)+'\n';
	chk('2819',s,1026,'cycle C10 m=3: 2^10+2');
}
{
	let s='10 9 3\n';for(let i=1;i<=9;i++)s+=i+' '+(i+1)+'\n';
	chk('2819',s,1536,'path n=10 m=3: 3*2^9');
}
{
	let s='6 9 3\n';for(let u=1;u<=3;u++)for(let v=4;v<=6;v++)s+=u+' '+v+'\n';
	chk('2819',s,42,'K3,3 m=3');
}
{
	let s='100 99 2\n';for(let i=1;i<=99;i++)s+=i+' 100\n';
	const t0=Date.now();chk('2819',s,2,'star n=100 center last, m=2');
	console.log('     star n=100 time '+(Date.now()-t0)+'ms');
}
{
	// 随机稠密图 n=100 k=2500 m=3 -> 答案应为 0（含 K4），检查不超时
	let s='100 2500 3\n';const seen=new Set();let seed=12345;
	const rnd=()=> (seed=(seed*1103515245+12345)&0x7fffffff)/0x7fffffff;
	while(seen.size<2500){const u=1+Math.floor(rnd()*100),v=1+Math.floor(rnd()*100);if(u===v)continue;const key=Math.min(u,v)+'-'+Math.max(u,v);if(seen.has(key))continue;seen.add(key);s+=u+' '+v+'\n';}
	const t0=Date.now();const got=run('2819',s);
	console.log('     dense n=100 k=2500 m=3 => '+got+' in '+(Date.now()-t0)+'ms');
}
// ---------- P2820 ----------
chk('2820','3 3\n1 2 1\n2 3 2\n1 3 3\n',3,'triangle');
chk('2820','2 2\n1 2 5\n1 2 7\n',7,'parallel edges');
chk('2820','4 2\n1 2 3\n3 4 4\n',0,'disconnected acyclic');
chk('2820','1 1\n1 1 10\n',10,'self loop');
chk('2820','4 5\n1 2 1\n2 3 1\n3 4 1\n1 4 1\n1 3 5\n',6,'square+diagonal');
chk('2820','3 3\n1 2 2\n1 2 2\n2 3 3\n',2,'equal parallel');
{
	// k 很大：n=100 完全图每对 100 条平行边，权 1..100 -> tot=100*sum(1..100)*4950? 计算见下
	let s='100 ';const edges=[];
	for(let u=1;u<=100;u++)for(let v=u+1;v<=100;v++)for(let c=1;c<=100;c++)edges.push(u+' '+v+' '+c);
	s+=edges.length+'\n'+edges.join('\n')+'\n';
	const tot=BigInt(edges.length)*0n;let T=0n;for(let u=1;u<=100;u++)for(let v=u+1;v<=100;v++)for(let c=1;c<=100;c++)T+=BigInt(c);
	// MSF: 99 条权 1 的边
	const exp=T-99n;
	const t0=Date.now();const got=run('2820',s);
	const ok=got===exp.toString();if(!ok)fail++;
	console.log((ok?'OK  ':'FAIL')+' P2820 huge k=495000 => '+got+' (expect '+exp+') '+(Date.now()-t0)+'ms');
}
// ---------- P2822 ----------
chk('2822','1 2\n2 2\n',1,'sample1 explanation');
chk('2822','1 2\n4 4\n',4,'n=4 m=4 k=2');
chk('2822','1 2\n4 1\n',2,'n=4 m=1 k=2');
chk('2822','1 2\n1 1\n',0,'ghost cell trap');
chk('2822','1 3\n3 3\n',2,'k=3 n=3');
chk('2822','1 1\n2 2\n',6,'k=1 all pairs');
chk('2822','1 2\n0 0\n',0,'n=m=0');
chk('2822','1 2\n2 2000\n',1,'m>>n');
chk('2822','3 2\n4 4\n4 1\n1 1\n','4\n2\n0','multi query');
chk('2822','1 2\n2000 1\n',1000,'n=2000 m=1 k=2 (even i count)');
// 独立交叉验证：Lucas/位判定（k=2,3,5 素数）与 BigInt 精确杨辉三角（k=6 合数）
function lucasCount(n,m,k){
	// 返回 #{(i,j): i<=n, j<=min(i,m), k | C(i,j)}，用 k 进制 Lucas 定理（k 素数）
	let cnt=0;
	for(let i=0;i<=n;i++)for(let j=0;j<=Math.min(i,m);j++){
		let a=i,b=j,div=false;
		while(a>0||b>0){const da=a%k,db=b%k;if(db>da){div=true;break;}a=Math.floor(a/k);b=Math.floor(b/k);}
		if(div)cnt++;
	}
	return cnt;
}
for(const k of [2,3,5]){
	const n=2000,m=2000;
	const exp=lucasCount(n,m,k);
	const t0=Date.now();const got=run('2822','1 '+k+'\n'+n+' '+m+'\n');
	const ok=got===String(exp);if(!ok)fail++;
	console.log((ok?'OK  ':'FAIL')+' P2822 k='+k+' n=m=2000 => '+got+' (Lucas '+exp+') '+(Date.now()-t0)+'ms');
}
{
	// 合数 k=6，n=m=300：Node BigInt 精确组合数（阶乘 BigInt）独立验证
	const n=300,m=300,k=6;
	const fac=[1n];for(let i=1;i<=n;i++)fac.push(fac[i-1]*BigInt(i));
	let exp=0;
	for(let i=0;i<=n;i++)for(let j=0;j<=Math.min(i,m);j++){
		const c=fac[i]/(fac[j]*fac[i-j]);
		if(c%BigInt(k)===0n)exp++;
	}
	const got=run('2822','1 '+k+'\n'+n+' '+m+'\n');
	const ok=got===String(exp);if(!ok)fail++;
	console.log((ok?'OK  ':'FAIL')+' P2822 composite k=6 n=m=300 => '+got+' (BigInt factorial '+exp+')');
}
{
	// 合数 k=21，n=m=2000 与 BigInt 杨辉三角（模 21，独立实现，二维）交叉验证
	const n=2000,m=2000,k=21;
	let prev=new Array(n+1).fill(0),cur=new Array(n+1).fill(0),exp=0;
	prev[0]=1%k;
	for(let i=0;i<=n;i++){
		cur=new Array(n+1).fill(0);
		cur[0]=1%k;
		for(let j=1;j<=i;j++)cur[j]=(prev[j-1]+prev[j])%k;
		for(let j=0;j<=Math.min(i,m);j++)if(cur[j]===0)exp++;
		prev=cur;
	}
	const got=run('2822','1 '+k+'\n'+n+' '+m+'\n');
	const ok=got===String(exp);if(!ok)fail++;
	console.log((ok?'OK  ':'FAIL')+' P2822 composite k=21 n=m=2000 => '+got+' (independent 2D Pascal '+exp+')');
}
{
	const t0=Date.now();run('2822','10000 2\n'+Array.from({length:10000},()=> '2000 2000').join('\n')+'\n');
	console.log('     P2822 t=10000 max queries time '+(Date.now()-t0)+'ms');
}
// ---------- 独立算法交叉验证（Prim vs Kruskal；精确 BigInt 组合数 vs 模递推） ----------
{
	let seed=987654321;const rnd=()=> (seed=(seed*1103515245+12345)&0x7fffffff)/0x7fffffff;
	let bad=0;
	for(let tc=0;tc<300;tc++){
		const n=1+Math.floor(rnd()*8),k=1+Math.floor(rnd()*40);
		const es=[];let tot=0;
		for(let i=0;i<k;i++){const u=Math.floor(rnd()*n),v=Math.floor(rnd()*n),w=1+Math.floor(rnd()*20);es.push([u,v,w]);tot+=w;}
		// 独立 Prim：直接在多重图上跑，自环天然被忽略，平行边天然取最小
		const vis=new Array(n).fill(false);let kept=0;
		for(let s=0;s<n;s++){
			if(vis[s])continue;
			vis[s]=true;
			for(;;){
				let bw=Infinity,bv=-1;
				for(const[u,v,w]of es){
					if(vis[u]&&!vis[v]&&w<bw){bw=w;bv=v;}
					if(vis[v]&&!vis[u]&&w<bw){bw=w;bv=u;}
				}
				if(bv<0)break;
				vis[bv]=true;kept+=bw;
			}
		}
		const exp=tot-kept;
		const input=n+' '+k+'\n'+es.map(([u,v,w])=>(u+1)+' '+(v+1)+' '+w).join('\n')+'\n';
		const got=run('2820',input);
		if(got!==String(exp)){bad++;console.log('FAIL P2820 Prim case',tc,input,'got',got,'exp',exp);}
	}
	if(bad)fail+=bad;
	console.log((bad?'FAIL':'OK  ')+' P2820 300 random multigraphs vs independent Prim (loops+parallel), mismatches='+bad);
}
{
	let seed=24680;const rnd=()=> (seed=(seed*1103515245+12345)&0x7fffffff)/0x7fffffff;
	let bad=0,total=0;
	const N=60;
	for(let k=2;k<=21;k++){
		// 独立实现：精确 BigInt 杨辉三角 → 取模 → 二维前缀和
		const C=[];for(let i=0;i<=N;i++){C.push(new Array(N+1).fill(0n));C[i][0]=1n;for(let j=1;j<=i;j++)C[i][j]=C[i-1][j-1]+C[i-1][j];}
		const s=[];for(let i=0;i<=N+1;i++)s.push(new Array(N+2).fill(0));
		for(let i=0;i<=N;i++)for(let j=0;j<=N;j++){
			const b=(j<=i&&C[i][j]%BigInt(k)===0n)?1:0;
			s[i+1][j+1]=s[i][j+1]+s[i+1][j]-s[i][j]+b;
		}
		const qs=[];let inp='20 '+k+'\n';
		for(let q=0;q<20;q++){const n=Math.floor(rnd()*(N+1)),m=Math.floor(rnd()*(N+1));qs.push([n,m]);inp+=n+' '+m+'\n';}
		const out=run('2822',inp).split('\n');
		for(let q=0;q<20;q++){
			const[n,m]=qs[q];const exp=s[n+1][Math.min(n,m)+1];total++;
			if(out[q]!==String(exp)){bad++;console.log('FAIL P2822 k='+k+' n='+n+' m='+m+' got '+out[q]+' exp '+exp);}
		}
	}
	if(bad)fail+=bad;
	console.log((bad?'FAIL':'OK  ')+' P2822 '+total+' random queries k=2..21 vs exact BigInt binomials, mismatches='+bad);
}
console.log(fail===0?'ALL CHECKS PASSED':fail+' CHECKS FAILED');