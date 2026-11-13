const fs=require('fs'),path=require('path'),cp=require('child_process');
const root='D:/Information/My/OI/luogu_problems';
const ids=process.argv.slice(2);
const heads=['## 一、解题过程：人类思考叙事','## 二、思维链：完整证明','## 三、模式总结（按实际思考顺序）','## 四、解法说明','## 五、示例代码（C++，Louis Li 风格）'];
let bad=0;
for(const id of ids){
  const md=path.join(root,'solution/P2000-P2999',`P${id}_solution.md`);
  if(!fs.existsSync(md)){console.log(`${id} MISSING`);bad++;continue;}
  const t=fs.readFileSync(md,'utf8');
  const lines=t.split('\n');
  const miss=heads.filter(h=>!t.includes(h));
  const blocks=[...t.matchAll(/```cpp\s*\n([\s\S]*?)```/g)];
  const title=lines[0];
  const titleOk=title.startsWith(`# P${id} `)&&title.endsWith(' — 题解');
  let cOK='-',cErr='';
  if(blocks.length!==1){cErr=`cpp_blocks=${blocks.length}`;}
  else{
    const dir=path.join(root,'.scratch','verify',id);fs.mkdirSync(dir,{recursive:true});
    const cf=path.join(dir,'a.cpp');fs.writeFileSync(cf,blocks[0][1]);
    const r=cp.spawnSync('D:/mingw64/bin/g++.exe',['-std=c++20','-O2','-w','-o',path.join(dir,'a.exe'),cf],{encoding:'utf8'});
    cOK=r.status===0?'OK':'FAIL';if(r.status!==0)cErr=(r.stderr||'').slice(0,300);
  }
  const sec2=t.indexOf(heads[1]),sec3=t.indexOf(heads[2]);
  const len2=sec3-sec2;
  const total=t.length;
  const ok=miss.length===0&&titleOk&&cOK==='OK';
  if(!ok)bad++;
  console.log(`${id} ${ok?'OK':'BAD'} len=${total} part2=${len2} title=${titleOk} compile=${cOK}${miss.length?' miss='+miss.join('/'):''}${cErr?' err='+cErr.replace(/\n/g,' '):''}`);
}
console.log(`BAD=${bad}/${ids.length}`);
