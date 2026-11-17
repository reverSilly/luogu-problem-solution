const {execFileSync}=require('child_process');
const dir='D:/Information/My/OI/luogu_problems/.scratch/verify/2803/';
let seed=20260601;
function rnd(m){seed=(seed*1103515245+12345)&0x7fffffff;return seed%m;}
let bad=0;
for(let it=0;it<300;++it){
	const n=1+rnd(7),k=1+rnd(8);
	const a=Array.from({length:n},()=>1+rnd(100));
	const d=Array.from({length:n-1},()=>1+rnd(100));
	const inp=`${n} ${k}\n${a.join(' ')}\n${d.join(' ')}\n`;
	const o1=execFileSync(dir+'a.exe',{input:inp}).toString().trim();
	const o2=execFileSync(dir+'brute.exe',{input:inp}).toString().trim();
	if(o1!==o2){bad++;console.log('MISMATCH',JSON.stringify(inp),o1,o2);if(bad>3)break;}
}
console.log(bad===0?'ALL 300 MATCH':bad+' mismatches');