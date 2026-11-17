const fs=require('fs');
for(const id of [2850,2853,2855,2858]){
	const md=fs.readFileSync(`solution/P2000-P2999/P${id}_solution.md`,'utf8');
	const blocks=[...md.matchAll(/```cpp\r?\n([\s\S]*?)```/g)];
	if(blocks.length!==1){console.log(`P${id}: BLOCK COUNT ${blocks.length} FAIL`);process.exit(1);}
	fs.writeFileSync(`.scratch/verify/${id}/a.cpp`,blocks[0][1]);
	// format checks
	const lines=md.split(/\r?\n/);
	const heads=['## 一、解题过程：人类思考叙事','## 二、思维链：完整证明','## 三、模式总结（按实际思考顺序）','## 四、解法说明','## 五、示例代码（C++，Louis Li 风格）'];
	const okTitle=lines[0]===`# P${id} `+md.match(/^# P\d+ (.+) — 题解$/m)[1]+' — 题解';
	const okHeads=heads.every(h=>lines.includes(h));
	console.log(`P${id}: blocks=1 title=${okTitle} heads=${okHeads}`);
}
