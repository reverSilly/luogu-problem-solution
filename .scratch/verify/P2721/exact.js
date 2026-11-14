function pow(b,e){let r=1n;for(let i=0n;i<e;i++)r*=b;return r;}
// exact value = 100000*(368/365)^36 ; in cents = 10^7 * 368^36 / 365^36
const N=10000000n*pow(368n,36n), D=pow(365n,36n);
const cents=(2n*N+D)/(2n*D); // round half up
console.log("exact cents=",cents.toString()," => ",(cents/100n).toString()+"."+ (cents%100n).toString().padStart(2,'0'));
// single 300-day 30%: 100000*(1+900000/3650000)=100000*4550000/3650000
const N2=100000n*4550000n*100n, D2=3650000n;
const c2=(2n*N2+D2)/(2n*D2);
console.log("single300 => ",(c2/100n).toString()+"."+(c2%100n).toString().padStart(2,'0'));
