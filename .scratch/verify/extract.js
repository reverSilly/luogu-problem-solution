// 通用：从题解 md 抽取 ```cpp 代码块到 .scratch/verify/{id}/from_md.cpp
// 用法: node extract.js <id1> <id2> ...
const fs=require('fs');
const ids=process.argv.slice(2);
const base='D:/Information/My/OI/luogu_problems';
for(const id of ids){
	const md=fs.readFileSync(`${base}/solution/P2000-P2999/P${id}_solution.md`,'utf8');
	const m=md.match(/```cpp\r?\n([\s\S]*?)```/);
	if(!m){console.log(id+' NO CPP BLOCK');process.exit(1);}
	fs.writeFileSync(`${base}/.scratch/verify/${id}/from_md.cpp`,m[1]);
	console.log(id+' extracted '+m[1].split('\n').length+' lines');
}