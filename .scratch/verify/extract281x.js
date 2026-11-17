const fs=require('fs');
for(const id of [2814,2815,2816,2817]){
  const md=fs.readFileSync(`solution/P2000-P2999/P${id}_solution.md`,'utf8');
  const blocks=[...md.matchAll(/```cpp\r?\n([\s\S]*?)```/g)].map(m=>m[1]);
  if(blocks.length!==1){console.error(`P${id}: ${blocks.length} cpp blocks!`);process.exit(1);}
  fs.writeFileSync(`.scratch/verify/${id}/a.cpp`,blocks[0]);
  console.log(`P${id}: extracted 1 block, ${blocks[0].length} chars`);
}
