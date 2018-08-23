
type
  caracter = array[1..8] of byte;
  sprite1  = caracter;
  sprite2  = array[1..32] of byte;

var
  a: byte;
  hl,bc,de: integer;
  keybuf:array[1..11] of byte;


Procedure ClPrim(ix:integer);
begin
  InLine($F3/
         $3A/A/
         $2A/HL/
         $ED/$4B/BC/
         $ED/$5B/DE/
         $DD/$2A/IX/
         $CD/*+18/
         $32/A/
         $22/HL/
         $ED/$43/BC/
         $ED/$53/DE/
         $FB/
         $C9/
         $08/
         $DB/$A8/
         $F5/
         $E6/$F0/
         $ED/$73/$9A/$F3/
         $C3/$F38C
         )
end;

type LinhaBas = string[255];
var Handler: string[7];

Procedure InstHook (x:integer);
begin
  Mem[x]:=$C3;
  Mem[x+1]:=Lo(Addr(Handler));
  Mem[x+2]:=Hi(Addr(Handler));
end;

Procedure Basic(x:LinhaBas);
begin
  x:=x+#0;
  InLine($21/x/$22/HL);
  ClPrim($4187);
end;

Procedure InstallBasic;
begin
  Handler:=#$ED+#$7B+#$9A+#$F3+#$C3+#$92+#$F3;
  InstHook($FF02);
  InstHook($FFB1);
  InstHook($FEF8);
end;
