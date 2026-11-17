const fs=require('fs');
const names={2842:'纸币问题 1',2843:'暗杀',2847:'[USACO16DEC] Moocast G',2849:'[USACO14DEC] Marathon S'};
for(const id of [2842,2843,2847,2849]){
	const md=fs.readFileSync(`solution/P2000-P2999/P${id}_solution.md`,'utf8');
	const blocks=[...md.matchAll(/```cpp\r?\n([\s\S]*?)```/g)];
	if(blocks.length!==1){console.error(`P${id}: cpp block count = ${blocks.length}`);process.exit(1);}
	fs.writeFileSync(`.scratch/verify/${id}/a.cpp`,blocks[0][1]);
	// 结构校验
	const lines=md.split(/\r?\n/);
	const want=['## 一、解题过程：人类思考叙事','## 二、思维链：完整证明','## 三、模式总结（按实际思考顺序）','## 四、解法说明','## 五、示例代码（C++，Louis Li 风格）'];
	const okTitle=lines[0]===`# P${id} ${names[id]} — 题解`;
	const heads=lines.filter(l=>l.startsWith('## '));
	const okHeads=want.every((w,i)=>heads[i]===w)&&heads.length===5;
	console.log(`P${id}: blocks=1 title=${okTitle} heads=${okHeads}`);
	if(!okTitle||!okHeads){console.error(lines[0]);console.error(heads);process.exit(1);}
}
console.log('extract OK');