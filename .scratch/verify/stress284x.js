const {spawnSync}=require('child_process');
function run(exe,input){
	const r=spawnSync(exe,{input,encoding:'utf8',timeout:10000});
	if(r.status!==0)throw new Error(`${exe} exit ${r.status} ${r.stderr}`);
	return r.stdout.trim();
}
let seed=20261125;
function rnd(n){seed=(seed*1103515245+12345)&0x7fffffff;return seed%n;}
function check(id,gen,cases){
	const A=`.scratch/verify/${id}/a.exe`,B=`.scratch/verify/${id}/b.exe`;
	for(let t=0;t<cases;++t){
		const inp=gen();
		const ra=run(A,inp),rb=run(B,inp);
		if(ra!==rb){console.error(`P${id} MISMATCH case ${t}\n${inp}got ${ra} want ${rb}`);process.exit(1);}
	}
	console.log(`P${id}: ${cases} random cases OK`);
}
check(2843,()=>{
	const n=1+rnd(14),k=1+rnd(5);
	let s=`${n} ${k}\n`;
	for(let i=0;i<n;++i)s+=rnd(1<<k)+(i+1<n?' ':'\n');
	return s;
},300);
check(2849,()=>{
	const n=3+rnd(7),k=rnd(n);	// k 含 n-1，测口径钳制
	let s=`${n} ${k}\n`;
	for(let i=0;i<n;++i)s+=`${rnd(9)-4} ${rnd(9)-4}\n`;
	return s;
},300);
check(2847,()=>{
	const n=1+rnd(8);
	let s=`${n}\n`;
	for(let i=0;i<n;++i)s+=`${rnd(9)} ${rnd(9)}\n`;
	return s;
},200);
console.log('all stress OK');