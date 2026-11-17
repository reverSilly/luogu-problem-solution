const {execFileSync}=require('child_process');
const dir='D:/Information/My/OI/luogu_problems/.scratch/verify/2813/';
let seed=987654321;
function rnd(m){seed=(seed*1103515245+12345)&0x7fffffff;return seed%m;}
let bad=0;
for(let it=0;it<400;++it){
	const m=1+rnd(4),n=1+rnd(6);
	const def=Array.from({length:m},()=>1+rnd(15));
	const atk=Array.from({length:n},()=>1+rnd(15));
	const inp=`${m} ${n}\n${def.join(' ')}\n${atk.join(' ')}\n`;
	const o1=execFileSync(dir+'a.exe',{input:inp}).toString().trim();
	const o2=execFileSync(dir+'brute.exe',{input:inp}).toString().trim();
	if(o1!==o2){bad++;console.log('MISMATCH',JSON.stringify(inp),o1,o2);if(bad>3)break;}
}
console.log(bad===0?'ALL 400 MATCH':bad+' mismatches');