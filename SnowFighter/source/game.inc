function Collision(x1,y1,w,h,x2,y2,w2,h2:integer):boolean;
begin
  Collision:=false;
  if ( x1 < x2 + w2 ) and ( x1+w > x2) and (y1<y2+h2) and (y1+h>y2) then Collision:=true;
end;

{procedure delay(valor:integer);
var OJ: integer;
begin
  OJ:=Jiffy;
  Jiffy:=0;
  repeat
  until Jiffy>=valor;
  Jiffy:=OJ;
end;

function RND(vr: byte):byte;
var j:byte;
begin
  for j:=0 to random(100) do begin
      RND:=random(vr);
  end;
end;
}