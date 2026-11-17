const fs=require('fs');
const path=require('path');
const{execFileSync}=require('child_process');
const base='D:/Information/My/OI/luogu_problems';
const ids=['2818','2819','2820','2822'];
for(const id of ids){
	const md=fs.readFileSync(path.join(base,'solution/P2000-P2999','P'+id+'_solution.md'),'utf8');
	const blocks=md.match(/```cpp\n([\s\S]*?)\n```/g)||[];
	if(blocks.length!==1){console.log(id,'CPP BLOCK COUNT =',blocks.length);continue;}
	const code=blocks[0].replace(/^```cpp\n/,'').replace(/\n```$/,'');
	const dir=path.join(base,'.scratch/verify',id);
	fs.mkdirSync(dir,{recursive:true});
	const src=path.join(dir,'a.cpp');
	fs.writeFileSync(src,code);
	try{
		execFileSync('D:/mingw64/bin/g++.exe',['-std=c++20','-O2','-w','-o',path.join(dir,'a.exe'),src],{encoding:'utf8'});
		console.log(id,'COMPILE OK');
	}catch(e){console.log(id,'COMPILE FAIL',e.stderr||e.message);}
	// 结构校验
	const heads=['## 一、解题过程：人类思考叙事','## 二、思维链：完整证明','## 三、模式总结（按实际思考顺序）','## 四、解法说明','## 五、示例代码（C++，Louis Li 风格）'];
	const miss=heads.filter(h=>!md.includes(h));
	const title=md.split('\n')[0];
	console.log(id,'title:',title,'| missing heads:',miss.length?miss:'none');
}