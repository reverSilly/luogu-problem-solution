// P2942 独立核对：BigInt 最小堆枚举 closure（与双指针归并完全不同的实现）
const {execFileSync}=require('child_process');
const exe=__dirname+'/a.exe';
function f1(x,a,b,d){return a*x/d+b;}
function brute(c,N,a1,b1,d1,a2,b2,d2){
	// 最小堆（数组实现）+ seen 集合，逐个弹出最小未扩展值
	const heap=[],seen=new Set();
	const push=v=>{if(seen.has(v))return;seen.add(v);heap.push(v);let i=heap.length-1;
		while(i>0){const p=(i-1)>>1;if(heap[p]<=heap[i])break;[heap[p],heap[i]]=[heap[i],heap[p]];i=p;}};
	const pop=()=>{const top=heap[0],last=heap.pop();if(heap.length){heap[0]=last;let i=0;
		for(;;){let l=2*i+1,r=l+1,m=i;if(l<heap.length&&heap[l]<heap[m])m=l;if(r<heap.length&&heap[r]<heap[m])m=r;
			if(m===i)break;[heap[m],heap[i]]=[heap[i],heap[m]];i=m;}}return top;};
	push(c);
	let last=0n;
	for(let k=0n;k<N;k++){
		if(!heap.length)throw new Error('set exhausted');
		const v=pop();last=v;
		push(f1(v,a1,b1,d1));push(f1(v,a2,b2,d2));
	}
	return last;
}
function run(c,N,a1,b1,d1,a2,b2,d2){
	const inp=`${c} ${N}\n${a1} ${b1} ${d1}\n${a2} ${b2} ${d2}\n`;
	return BigInt(execFileSync(exe,{input:inp}).toString().trim());
}
let ok=0;
// 固定用例
const fixed=[[3,10,4,3,3,17,8,2],[1,5,3,0,2,2,0,1],[7,1,4,3,3,17,8,2],[1,450,20,0,1,2,0,1],[100,300,20,20,19,20,20,19]];
for(const[c,N,a1,b1,d1,a2,b2,d2]of fixed){
	const e=brute(BigInt(c),BigInt(N),BigInt(a1),BigInt(b1),BigInt(d1),BigInt(a2),BigInt(b2),BigInt(d2));
	const g=run(c,N,a1,b1,d1,a2,b2,d2);
	if(e!==g){console.log('MISMATCH fixed',c,N,a1,b1,d1,a2,b2,d2,'expect',e.toString(),'got',g.toString());process.exit(1);}
	++ok;
}
// 随机小数据（刻意覆盖 b=0 不动点、a/d 接近 1、两式相同）
let seed=12345;
const rnd=n=>{seed=(seed*1103515245+12345)&0x7fffffff;return seed%n;};
for(let t=0;t<300;++t){
	const c=1+rnd(6),N=1+rnd(250);
	const d1=1+rnd(4),a1=d1+1+rnd(4),b1=rnd(3);
	let d2,a2,b2;
	if(t%5===0){d2=d1;a2=a1;b2=b1;}	// 两式相同
	else{d2=1+rnd(4);a2=d2+1+rnd(4);b2=rnd(3);}
	const e=brute(BigInt(c),BigInt(N),BigInt(a1),BigInt(b1),BigInt(d1),BigInt(a2),BigInt(b2),BigInt(d2));
	if(e>=(1n<<63n))continue;	// 题目保证所有时长 <2^63，超界的是非法输入，跳过
	const g=run(c,N,a1,b1,d1,a2,b2,d2);
	if(e!==g){console.log('MISMATCH rand',c,N,a1,b1,d1,a2,b2,d2,'expect',e.toString(),'got',g.toString());process.exit(1);}
	++ok;
}
console.log('ALL OK,',ok,'cases');