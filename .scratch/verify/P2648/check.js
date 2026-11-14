const{execFileSync}=require('child_process');
const fs=require('fs');
function ri(a,b){return a+Math.floor(Math.random()*(b-a+1));}
function run(exe,input){return execFileSync(exe,{input,encoding:'utf8'}).trim();}
let mismatch=0,orz=0,fin=0;
const N=3000;
for(let t=0;t<N;++t){
	const C=ri(2,6),D=ri(1,20),P=ri(0,8),F=ri(0,8);
	let lines=[`${D} ${P} ${C} ${F}`];
	for(let i=0;i<P;++i){let a=ri(1,C),b=ri(1,C);while(b===a)b=ri(1,C);lines.push(`${a} ${b}`);}
	for(let i=0;i<F;++i){let j=ri(1,C),k=ri(1,C);while(k===j)k=ri(1,C);lines.push(`${j} ${k} ${ri(1,30)}`);}
	const input=lines.join('\n')+'\n';
	const a=run('./P2648.exe',input),b=run('./brute.exe',input);
	if(a!==b){mismatch++;console.log('MISMATCH\n'+input+'main='+a+' brute='+b);if(mismatch>5)break;}
	if(a==='orz')orz++;else fin++;
}
console.log(`done ${N} cases, mismatch=${mismatch}, orz=${orz}, finite=${fin}`);