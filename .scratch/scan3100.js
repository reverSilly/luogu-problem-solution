const fs=require('fs'),path=require('path');
const root='D:/Information/My/OI/luogu_problems';
const want=new Set(['入门','普及-','普及/提高-']);
const bucket=id=>{const b=Math.floor(id/1000)*1000;return `P${b}-P${b+999}`;};
const targets=[],already=[],nodiff=[];
const diffCount={};
for(let id=2801;id<=3100;id++){
  const sf=path.join(root,'statement',bucket(id),`P${id}.md`);
  if(!fs.existsSync(sf)){nodiff.push(id);continue;}
  const t=fs.readFileSync(sf,'utf8');
  const m=t.match(/##\s*Difficulty\s*\n+\s*([^\n]+)/);
  const d=m?m[1].trim():'(none)';
  diffCount[d]=(diffCount[d]||0)+1;
  if(!want.has(d))continue;
  const solf=path.join(root,'solution',bucket(id),`P${id}_solution.md`);
  if(fs.existsSync(solf))already.push(id);
  else targets.push(id);
}
console.log('DIFF DISTRIBUTION 2801-3100:',JSON.stringify(diffCount));
console.log('MISSING STATEMENT:',nodiff.join(' '));
console.log('ALREADY SOLVED (below 提高):',already.join(' '));
console.log('TARGET COUNT:',targets.length);
console.log('TARGETS:',targets.join(' '));