(() => {
'use strict';
const $ = id => document.getElementById(id);
const figs = window.FIGUREN, byId = Object.fromEntries(figs.map(f => [f.id,f]));
const ages = ['baby','teenager','erwachsen','senior'];
const labels = {ohren:'Ohren',schwanz:'Schwanz',besonderheit:'Besonderheit',ton:'Fellton',kranzton:'Kranzton',struktur:'Fellstruktur',muster:'Fellmuster',zubehoer:'Zubehör'};
const traits = Object.keys(labels), layers = ['schwanz','besonderheit-hinten','basis','struktur','muster','ohren','besonderheit-vorn','zubehoer'];
const size = {baby:[94,89],teenager:[81,107],erwachsen:[114,116],senior:[115,110]};
const state = {family:byId.evi,partner:byId.bit,child:null,locked:new Set(),history:[],visible:new Set(layers),missing:new Set(),all:false};
const images = {};
function effectiveKranzton(ton,wanted){const fur=187+(2-ton)*17,palette=[255,136,119,102];let k=wanted;if(fur<=170)while(k>0&&palette[k]<=170&&Math.abs(palette[k]-fur)<51)k--;return k}
function url(key){return (window.TEILE_DATEN||{})[key]||null}
function getImage(key){if(!url(key))return Promise.resolve(null);if(!images[key])images[key]=new Promise(resolve=>{let im=new Image();im.onload=()=>resolve(im);im.onerror=()=>resolve(null);im.src=url(key)});return images[key]}
async function colored(im,key,creature){
  const kranz=effectiveKranzton(creature.ton,creature.kranzton||0);
  const map=await getImage(key.replace(/\.png$/,'.bereiche.png'));
  if(!map||map.width!==im.width||map.height!==im.height)return im;
  const c=document.createElement('canvas');c.width=im.width;c.height=im.height;
  const x=c.getContext('2d',{willReadFrequently:true});x.drawImage(im,0,0);
  const d=x.getImageData(0,0,c.width,c.height);
  const m=document.createElement('canvas');m.width=im.width;m.height=im.height;
  const mx=m.getContext('2d',{willReadFrequently:true});mx.drawImage(map,0,0);
  const md=mx.getImageData(0,0,c.width,c.height).data;
  for(let i=0;i<md.length;i+=4){
    if(d.data[i+3]===0){if(md[i]===68&&kranz>0){d.data[i]=d.data[i+1]=d.data[i+2]=[255,136,119,102][kranz];d.data[i+3]=255}continue;}
    const region=md[i],v=d.data[i];let next=v;
    if(region===51||region===119||region===85)next=v+(2-creature.ton)*17;
    if(region===68)next=v-(255-[255,136,119,102][kranz]);
    if(region===85)next=Math.min(next,204);
    if(next!==v){next=Math.max(0,Math.min(255,Math.round(next/17)*17));d.data[i]=d.data[i+1]=d.data[i+2]=next}
  }
  x.putImageData(d,0,0);return c;
}
function familyBase(sex){return sex==='weiblich'?'evi':'evi-m'}
function value(f,t){return f[t]}
function random(max){return Math.floor(Math.random()*max)}
function partners(){return figs.filter(f=>f.id!=='evi'&&f.id!=='evi-m'&&f.geschlecht!==state.family.geschlecht)}
function keyOf(c){return traits.map(t=>String(c[t])).join('|')+'|'+c.geschlecht}
function drawChild(a,b){
  const c={geschlecht:random(2)?'maennlich':'weiblich'};
  for(const t of traits){
    if(t==='ton'){let low=Math.min(a.ton,b.ton),high=Math.max(a.ton,b.ton);c.ton=Math.max(0,Math.min(4,low+random(high-low+3)-1));continue}
    const av=value(a,t),bv=value(b,t);c[t]=random(2)?av:bv;
    if(c[t]==='keins'&&av!==bv)c[t]=av==='keins'?bv:av;
  }
  if(state.child)for(const t of state.locked)c[t]=state.child[t];
  return c;
}
function roll(){let c,tries=0;do{c=drawChild(state.family,state.partner);tries++}while(tries<20&&(keyOf(c)===keyOf(state.family)||keyOf(c)===keyOf(state.partner)));
  state.child=c;$('meldung').textContent=tries===20?'Nach 20 Versuchen ähnelt das Kind einem Elternteil vollständig.':'Neues Kind ausgewürfelt.';render()}
function labelFor(id){return byId[id]?.name||id}
function card(title,creature,age,opts={}){
  const box=document.createElement('div');box.className='card';const head=document.createElement('strong');head.textContent=title;box.append(head);
  const wrap=document.createElement('div');wrap.className='canvas-wrap';box.append(wrap);
  const foot=document.createElement('div');foot.className='muted';foot.textContent=opts.note||'';box.append(foot);
  paint(wrap,creature,age,opts);return box;
}
function markMissing(sex,age,part,id){state.missing.add(`${sex}, ${age}, ${part}, ${id}`)}
async function paint(wrap,creature,age,opts){
  // Parkfiguren als Elternteil: immer ihre eigene Zeichnung unverändert zeigen, nie auf Evis Basis
  if(creature.id&&byId[creature.id]&&creature.id!=='evi'&&creature.id!=='evi-m'){
    const own=await getImage(`original/${creature.id}/${age}`);
    if(!own){wrap.innerHTML=`<div class="missing">Bild von ${creature.name} fehlt</div>`;return}
    const z=opts.small?1:Math.max(1,+$('zoom').value),c=document.createElement('canvas');c.width=own.width;c.height=own.height;c.style.width=own.width*z+'px';c.style.height=own.height*z+'px';
    const x=c.getContext('2d');x.imageSmoothingEnabled=false;x.fillStyle=$('hintergrund').value;x.fillRect(0,0,c.width,c.height);x.drawImage(own,0,0);
    wrap.replaceChildren(c);const n=document.createElement('div');n.className='muted';n.textContent='Originalzeichnung';wrap.append(n);return;
  }
  const sex=creature.geschlecht,baseId=familyBase(sex),basisKey=`teile/${sex}/${age}/basis.png`;
  let base=await getImage(basisKey),original=await getImage(`original/${baseId}/${age}`);
  if(!original&&!base){wrap.innerHTML='<div class="missing">männliche Evi fehlt noch</div>';markMissing(sex,age,'basis',baseId);updateTodo();return}
  const w=base?.width||original.width,h=base?.height||original.height;
  const canvas=document.createElement('canvas');canvas.width=w;canvas.height=h;canvas.style.width=(w*(opts.small?1:Math.max(1,+$('zoom').value)))+'px';canvas.style.height=(h*(opts.small?1:Math.max(1,+$('zoom').value)))+'px';
  const ctx=canvas.getContext('2d',{willReadFrequently:true});ctx.imageSmoothingEnabled=false;
  ctx.fillStyle=$('hintergrund').value;ctx.fillRect(0,0,w,h);
  function seam(im,color){
    if(!$('naehte').checked)return;
    let c=document.createElement('canvas');c.width=w;c.height=h;let x=c.getContext('2d',{willReadFrequently:true});x.drawImage(im,0,0);
    let d=x.getImageData(0,0,w,h).data;ctx.fillStyle=color;
    for(let y=0;y<h;y++)for(let px=0;px<w;px++){
      let i=(y*w+px)*4;if(d[i+3]===0)continue;
      if(px===0||px===w-1||y===0||y===h-1||d[i-4+3]===0||d[i+4+3]===0||d[i-w*4+3]===0||d[i+w*4+3]===0)ctx.fillRect(px,y,1,1)
    }
  }
  let missing=[];
  if(!base){base=original;missing.push('Original-Ohren und -Schwanz noch in der Basis');markMissing(sex,age,'basis',baseId)}
  const path=(type,id)=>`teile/${sex}/${age}/${type}/${id}.png`;
  const draw=async (layer,key,tinted=true)=>{
    if(!state.visible.has(layer))return;
    const im=await getImage(key);
    if(!im){missing.push(`${layer}: ${key}`);markMissing(sex,age,layer,key);return}
    if(im.width!==w||im.height!==h){missing.push(`Größe passt nicht: ${key}`);return}
    ctx.drawImage(tinted?await colored(im,key,creature):im,0,0);
    seam(im,layer==='basis'?'#00bcd4':'#e40085');
  };
  if(base!==original){if(creature.schwanz!==baseId)await draw('schwanz',path('schwanz',creature.schwanz));if(creature.besonderheit!=='keins'&&creature.besonderheit!=='maehne')await draw('besonderheit-hinten',path('besonderheit',creature.besonderheit+'-hinten'))}
  if(state.visible.has('basis'))ctx.drawImage(base!==original?await colored(base,basisKey,creature):base,0,0);
  if(state.visible.has('basis'))seam(base,'#00bcd4');
  if(base!==original){
    await draw('struktur',path('struktur',creature.struktur||'punkte'));
    if(creature.schwanz===baseId)await draw('schwanz',path('schwanz',creature.schwanz));
    if(creature.muster!=='keins'&&state.visible.has('muster')){
      const key=path('muster',creature.muster),mask=await getImage(key);
      if(!mask){missing.push(`muster: ${key}`);markMissing(sex,age,'muster',key)}
      else if(mask.width===w&&mask.height===h){
        const mc=document.createElement('canvas');mc.width=w;mc.height=h;
        const mx=mc.getContext('2d',{willReadFrequently:true});mx.drawImage(mask,0,0);
        const md=mx.getImageData(0,0,w,h).data,bd=ctx.getImageData(0,0,w,h);
        const bc=document.createElement('canvas');bc.width=w;bc.height=h;
        const bx=bc.getContext('2d',{willReadFrequently:true});bx.drawImage(base,0,0);
        const ba=bx.getImageData(0,0,w,h).data;
        const region=await getImage(basisKey.replace(/\.png$/,'.bereiche.png'));
        const rc=document.createElement('canvas');rc.width=w;rc.height=h;
        const rx=rc.getContext('2d',{willReadFrequently:true});if(region)rx.drawImage(region,0,0);
        const rd=rx.getImageData(0,0,w,h).data;
        const shift=creature.muster==='helle-brust'?68:creature.muster==='punkte'?-34:creature.muster==='maske'?-51:-68;
        for(let i=0;i<md.length;i+=4)if(md[i+3]>0&&ba[i+3]>0&&rd[i]===51){let g=Math.max(0,Math.min(255,Math.round((bd.data[i]+shift*md[i]/255)/17)*17));bd.data[i]=bd.data[i+1]=bd.data[i+2]=g}
        ctx.putImageData(bd,0,0);
      }else missing.push(`Größe passt nicht: ${key}`);
    }
    if(creature.besonderheit==='maehne'&&sex==='maennlich')await draw('besonderheit-vorn',path('besonderheit','leo-vorn'));
    await draw('ohren',path('ohren',creature.ohren));
    if(creature.besonderheit!=='keins'&&creature.besonderheit!=='maehne')await draw('besonderheit-vorn',path('besonderheit',creature.besonderheit+'-vorn'));
    if(creature.zubehoer!=='keins')await draw('zubehoer',path('zubehoer',creature.zubehoer),false);
  }else if(creature.ohren!==baseId||creature.schwanz!==baseId||creature.besonderheit!=='keins'||creature.muster!=='keins'||creature.zubehoer!=='keins'){
    missing.push('Merkmale noch nicht als Bauteile darstellbar');
    for(const t of ['ohren','schwanz','besonderheit','muster','zubehoer'])if(creature[t]!=='keins')markMissing(sex,age,t,creature[t]);
  }
  if($('raster').checked){ctx.save();ctx.strokeStyle='rgba(130,130,130,.18)';ctx.lineWidth=.15;for(let x=0;x<w;x++){ctx.beginPath();ctx.moveTo(x+.5,0);ctx.lineTo(x+.5,h);ctx.stroke()}for(let y=0;y<h;y++){ctx.beginPath();ctx.moveTo(0,y+.5);ctx.lineTo(w,y+.5);ctx.stroke()}ctx.restore()}
  wrap.replaceChildren(canvas);if(missing.length){let small=document.createElement('div');small.className='muted';small.textContent=missing.join(' · ');wrap.append(small)}
  if($('vergleich').checked&&original&&base!==original){let im=document.createElement('img');im.src=url(`original/${baseId}/${age}`);im.alt='Original';im.style.cssText='image-rendering:pixelated;max-width:100px';wrap.append(im)}
  updateTodo();
}
function renderFamily(){let sel=$('familie');sel.replaceChildren();let fam=[byId.evi,byId['evi-m'],...state.history.map((x,i)=>({...x,id:'kind-'+i,name:'Kind, Generation '+(i+2)}))];for(const f of fam){let op=new Option(f.name,f.id);sel.add(op)}sel.value=state.family.id;let possible=partners();if(!possible.some(x=>x.id===state.partner.id))state.partner=possible[0];$('partner-hinweis').textContent=`${possible.length} Figuren des anderen Geschlechts`;
  $('partner').replaceChildren(...possible.map(f=>{let b=document.createElement('button');b.type='button';b.setAttribute('aria-pressed',String(f.id===state.partner.id));let img=document.createElement('img');img.src=url(`original/${f.id}/baby`)||'';img.alt='';b.append(img,document.createTextNode(f.name));b.onclick=()=>{state.partner=f;render()};return b}))}
function renderTraits(){
  let host=$('merkmale');host.replaceChildren();
  for(const t of traits){
    let box=document.createElement('div');box.className='trait';let title=document.createElement('strong');title.textContent=labels[t];box.append(title);
    let buttons=document.createElement('div');buttons.className='row';
    for(const [name,parent] of [['Von A',state.family],['Von B',state.partner]]){
      let b=document.createElement('button');b.type='button';b.textContent=name;b.onclick=()=>{state.child[t]=parent[t];render()};buttons.append(b)
    }
    box.append(buttons);
    let sel=document.createElement('select');
    if(t==='ton')for(let v=0;v<=4;v++)sel.add(new Option(`${v} – ${['sehr hell','hell','mittel','dunkel','sehr dunkel'][v]}`,String(v)));
    else if(t==='kranzton')for(let v=0;v<=3;v++)sel.add(new Option(`${v} – ${['weiß','hellgrau','mittelgrau','dunkelgrau'][v]}`,String(v)));
    else{sel.add(new Option('keins','keins'));for(const f of figs)sel.add(new Option(`${f.name} (${f[t]})`,f.id));
      const extras=t==='struktur'?['punkte','glatt','gesprenkelt','gestrichelt','wuschelig']:t==='muster'?['flecken','ringe','helle-brust','maske','punkte']:t==='zubehoer'?['halstuch']:[];
      for(const extra of extras)sel.add(new Option(`Direkt: ${extra}`,`direkt:${extra}`));
    }
    sel.value=t==='ton'||t==='kranzton'?String(state.child[t]):(state.child[t]==='keins'?'keins':figs.find(f=>f[t]===state.child[t])?.id||`direkt:${state.child[t]}`);
    sel.onchange=()=>{state.child[t]=t==='ton'||t==='kranzton'?+sel.value:(sel.value.startsWith('direkt:')?sel.value.slice(7):(byId[sel.value]?.[t]||'keins'));render()};box.append(sel);
    let src=document.createElement('div');src.className='muted';src.textContent=`A: ${state.family[t]} · B: ${state.partner[t]}`;box.append(src);
    let lock=document.createElement('label');lock.className='check';let check=document.createElement('input');check.type='checkbox';check.checked=state.locked.has(t);check.onchange=()=>check.checked?state.locked.add(t):state.locked.delete(t);lock.append(check,document.createTextNode('Sperren'));box.append(lock);host.append(box)
  }
}
function renderCode(){const c=state.child;$('code').value=`geschlecht=${c.geschlecht}, ohren=${c.ohren}, schwanz=${c.schwanz}, besonderheit=${c.besonderheit}, ton=${c.ton}, kranzton=${c.kranzton}, struktur=${c.struktur}, muster=${c.muster}, zubehoer=${c.zubehoer}`}
function updateTodo(){$('todo').textContent=[...state.missing].sort().join('\n')||'Für diese Ansicht sind alle benötigten Bilder vorhanden.'}
function render(){if(!state.child)state.child=drawChild(state.family,state.partner);state.missing.clear();renderFamily();$('geschlecht').value=state.child.geschlecht;renderTraits();renderCode();$('trio').replaceChildren(card('Elternteil A',state.family,'erwachsen',{note:state.family.name}),card('Kind',state.child,'erwachsen',{note:state.child.geschlecht==='weiblich'?'Mädchen':'Junge'}),card('Elternteil B',state.partner,'erwachsen',{note:state.partner.name}));$('alter').replaceChildren(...ages.map(a=>card(a[0].toUpperCase()+a.slice(1),state.child,a)));$('stammbaum').replaceChildren(...state.history.map((x,i)=>card('Generation '+(i+2),x,'erwachsen',{small:true})));
  if(state.all){$('kombinationen').replaceChildren(...partners().map(p=>card(p.name,drawChild(state.family,p),'erwachsen',{small:true})))}else $('kombinationen').replaceChildren();updateTodo()}
$('familie').onchange=()=>{let id=$('familie').value;state.family=byId[id]||state.history[+id.slice(5)];state.partner=partners()[0];state.child=drawChild(state.family,state.partner);render()};
$('wuerfeln').onclick=roll;
$('geschlecht').onchange=()=>{state.child.geschlecht=$('geschlecht').value;render()};
$('generation').onclick=()=>{let c={...state.child,id:'kind-'+state.history.length,name:'Kind, Generation '+(state.history.length+2)};state.history.push(c);state.family=c;state.partner=partners()[0];state.child=drawChild(c,state.partner);$('meldung').textContent='Das Kind ist jetzt Elternteil A.';render()};
$('alle').onclick=()=>{state.all=!state.all;$('alle').textContent=state.all?'Kombinationen ausblenden':'Alle Kombinationen zeigen';render()};
for(const id of ['zoom','hintergrund','naehte','raster','vergleich'])$(id).addEventListener('change',render);
for(const layer of layers){let label=document.createElement('label');label.className='check';let c=document.createElement('input');c.type='checkbox';c.checked=true;c.onchange=()=>{c.checked?state.visible.add(layer):state.visible.delete(layer);render()};label.append(c,document.createTextNode(layer));$('ebenen').append(label)}
$('kopieren').onclick=()=>navigator.clipboard.writeText($('code').value);
$('einfuegen').onclick=()=>{try{let obj=Object.fromEntries($('code').value.split(',').map(s=>s.trim().split('=')));if(!['maennlich','weiblich'].includes(obj.geschlecht))throw Error('Geschlecht fehlt');for(const t of traits)if(obj[t]===undefined)throw Error(labels[t]+' fehlt');obj.ton=+obj.ton;obj.kranzton=+obj.kranzton;if(!Number.isInteger(obj.ton)||obj.ton<0||obj.ton>4)throw Error('Fellton muss 0–4 sein');if(!Number.isInteger(obj.kranzton)||obj.kranzton<0||obj.kranzton>3)throw Error('Kranzton muss 0–3 sein');state.child=obj;$('meldung').textContent='Merkmalliste wiederhergestellt.';render()}catch(e){$('meldung').textContent='Liste nicht lesbar: '+e.message}};
$('todo-kopieren').onclick=()=>navigator.clipboard.writeText($('todo').textContent);
$('export').onclick=async()=>{let sex=$('schablone-geschlecht').value,age=$('schablone-alter').value,base=await getImage(`teile/${sex}/${age}/basis.png`)||await getImage(`original/${familyBase(sex)}/${age}`);if(!base){$('meldung').textContent='Für diese Schablone fehlt die Basis.';return}let c=document.createElement('canvas');c.width=base.width;c.height=base.height;let x=c.getContext('2d');x.fillStyle='#fff';x.fillRect(0,0,c.width,c.height);x.globalAlpha=.25;x.drawImage(base,0,0);x.globalAlpha=1;x.strokeStyle='rgba(100,100,100,.25)';x.lineWidth=.1;for(let px=0;px<c.width;px++){x.beginPath();x.moveTo(px+.5,0);x.lineTo(px+.5,c.height);x.stroke()}for(let py=0;py<c.height;py++){x.beginPath();x.moveTo(0,py+.5);x.lineTo(c.width,py+.5);x.stroke()}let a=document.createElement('a');a.download=`schablone-${sex}-${age}.png`;a.href=c.toDataURL('image/png');a.click()};
render();
})();
