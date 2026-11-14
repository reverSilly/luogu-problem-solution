const fs=require('fs');
const path=require('path');
const{execFileSync}=require('child_process');
const base='D:/Information/My/OI/luogu_problems';
const ids=['P2648','P2649','P2651','P2655'];
for(const id of ids){
	const md=fs.readFileSync(path.join(base,'solution/P2000-P2999',id+'_solution.md'),'utf8');
	const m=md.match(/```cpp\n([\s\S]*?)\n```/);
	if(!m){console.log(id,'NO CPP BLOCK');continue;}
	const code=m[1];
	const dir=path.join(base,'.scratch/verify',id);
	const src=path.join(dir,'embedded.cpp');
	fs.writeFileSync(src,code);
	const exe=path.join(dir,'embedded.exe');
	try{
		execFileSync('D:/mingw64/bin/g++.exe',['-std=c++20','-O2',src,'-o',exe],{encoding:'utf8'});
		console.log(id,'embedded COMPILE OK');
	}catch(e){console.log(id,'COMPILE FAIL',e.stderr||e.message);}
}