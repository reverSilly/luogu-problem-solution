const fs=require('fs'),path=require('path');
const root='D:/Information/My/OI/luogu_problems';
const stmtDir=path.join(root,'statement/P2000-P2999');
const solDir=path.join(root,'solution/P2000-P2999');
const want=new Set(['入门','普及-','普及/提高-']);
const targets=[],already=[],nodiff=[];
const diffCount={};
for(let id=2501;id<=2800;id++){
  const sf=path.join(stmtDir,`P${id}.md`);
  if(!fs.existsSync(sf))continue;
  const t=fs.readFileSync(sf,'utf8');
  const m=t.match(/##\s*Difficulty\s*\n+\s*([^\n]+)/);
  const d=m?m[1].trim():'(none)';
  diffCount[d]=(diffCount[d]||0)+1;
  if(!want.has(d))continue;
  const solf=path.join(solDir,`P${id}_solution.md`);
  if(fs.existsSync(solf))already.push(id);
  else targets.push(id);
}
console.log('DIFF DISTRIBUTION 2501-2800:',JSON.stringify(diffCount));
console.log('ALREADY SOLVED (below 提高):',already.join(' '));
console.log('TARGET COUNT:',targets.length);
console.log('TARGETS:',targets.join(' '));