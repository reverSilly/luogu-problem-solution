const fs=require('fs');
const path=require('path');
const base='D:/Information/My/OI/luogu_problems';
const ids=['P2640','P2642','P2645','P2646'];
for(const id of ids){
  const md=fs.readFileSync(path.join(base,'solution/P2000-P2999',id+'_solution.md'),'utf8');
  const m=md.match(/```cpp\r?\n([\s\S]*?)```/);
  if(!m){console.log(id+' NO CPP BLOCK');process.exit(1);}
  const out=path.join(base,'.scratch/verify',id,'embedded.cpp');
  fs.writeFileSync(out,m[1]);
  console.log(id+' extracted '+m[1].split('\n').length+' lines -> '+out);
}