// 校验四篇题解：标题 / 五个二级标题 / 恰好一份 cpp 代码块 / 抽出代码与已验证的 a.cpp 一致
const fs=require('fs');
const ids=['P2677','P2678','P2681','P2683'];
const heads=['## 一、解题过程：人类思考叙事','## 二、思维链：完整证明','## 三、模式总结（按实际思考顺序）','## 四、解法说明','## 五、示例代码（C++，Louis Li 风格）'];
const root='D:/Information/My/OI/luogu_problems';
for(const id of ids){
	const p=`${root}/solution/P2000-P2999/${id}_solution.md`;
	const t=fs.readFileSync(p,'utf8');
	const lines=t.split(/\r?\n/);
	const h1=lines[0];
	const got=lines.filter(l=>l.startsWith('## '));
	const okHead=got.length===5&&got.every((g,i)=>g===heads[i]);
	const blocks=[...t.matchAll(/```cpp\n([\s\S]*?)```/g)].map(m=>m[1]);
	const allFence=[...t.matchAll(/```/g)].length;
	const out=`${root}/.scratch/verify/${id}/from_md.cpp`;
	if(blocks.length===1)fs.writeFileSync(out,blocks[0]);
	const a=fs.readFileSync(`${root}/.scratch/verify/${id}/a.cpp`,'utf8');
	const same=blocks.length===1&&blocks[0].replace(/\s+$/,'')===a.replace(/\s+$/,'');
	const chars=t.length;
	const p2=t.indexOf('## 二、'),p3=t.indexOf('## 三、');
	console.log(`${id}: h1="${h1}" headingsOK=${okHead} cppBlocks=${blocks.length} fences=${allFence} codeMatchesVerified=${same} chars=${chars} part2chars=${p3-p2} part2ratio=${((p3-p2)/chars*100).toFixed(1)}%`);
	if(!okHead)console.log('  headings:',JSON.stringify(got));
}