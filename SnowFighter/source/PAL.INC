type str=string[11];

{ ******************************************************************** }

procedure setpalette (nr,r,g,b: byte);

begin
  inline ($ed/$5b/g/        { ld de,(g)          }
          $3a/nr/$57/       { ld a,(nr): ld d,a  }
          $3a/r/            { ld a,(rood)        }
          $07/$07/$07/$07/  { 4* rlca            }
          $47/              { ld b,a             }
          $3a/b/$b0/        { ld a,(b): or b     }
          $fd/$2a/$f7/$fa/  { ld iy,(#faf7)      }
          $dd/$21/$4d/$01/  { ld ix,#014d        }
          $cd/$1c/$00);     { call #1c           }
end;

{ ******************************************************************** }

Procedure SetRGB(Color,r,g,b:Byte);
Var rb,gr,colour :Byte;
Begin
   rb := (r SHL 4)+b;
   gr := g;
   Colour := color;
   Inline ($F3/$3A/colour /$D3/$99/$3E/$10/$F6/$80/$D3/$99/$3A/ rb /
           $D3/$9A/$3A/ gr /$D3/$9A/$FB);
End;


{ ******************************************************************** }

procedure getpalette (nr: Byte; var r,g,b: Byte);

var
  r2,g2,b2: Byte;

begin
  (*
  inline ($3a/nr/
          $fd/$2a/$f7/$fa/  { ld iy,(#faf7)      }
          $dd/$21/$49/$01/  { ld ix,#0149        }
          $cd/$1c/$00/      { call #1c           }
          $79/$e6/$0f/$32/g2/       { ld a,c: ld (g2),a  }
          $78/$e6/$0f/      { ld a,b: and 15     }
          $32/b2/           { ld (b2),a          }
          $78/$07/$07/      { ld a,b: rlca: rlca }
          $07/$07/$e6/$0f/  { rlca: rlca: and 15 }
          $32/r2);          { ld (r2),a          }

  r := r2;
  g := g2;
  b := b2;
  *)
  inline ($3a/nr/
          $fd/$2a/$f7/$fa/  { ld iy,(#faf7)      }
          $dd/$21/$49/$01/  { ld ix,#0149        }
          $cd/$1c/$00/      { call #1c           }
          $79/$32/g2/       { ld a,c: ld (g2),a  }
          $78/$32/b2);      { ld a,b: ld (b2),a  }

  G:=G2 and 15;
  R:=(B2 SHR 4);
  B:= B2 and 15;
end;

{ ******************************************************************** }

procedure inipalette;

begin
  inline ($fd/$2a/$f7/$fa/  { ld iy,(#faf7)      }
          $dd/$21/$41/$01/  { ld ix,#0141        }
          $cd/$1c/$00);     { call #1c           }
end;

{ ******************************************************************** }
procedure LoadPal(fname: str);
type PBuffer = array[1..39] of byte;

var arq: file of PBuffer;
    PalBuffer: PBuffer;
    fr,cor:integer;
    
begin
  assign(arq, fname);
  reset(arq);
  read(arq,palbuffer);
  If (Palbuffer[1]=$FE) and (Palbuffer[2]=$80) and (Palbuffer[4]=$9F) then begin
     fr:=8; cor:=0;
     while cor<16 do begin                     
         setRGB(cor,
                   Round(palbuffer[fr] shr 4),
                   Round(palbuffer[fr+1]),
                   Round(palbuffer[fr] and $0F)
                    );
         fr:=fr+2;
         cor:=cor+1;
     end;
  end;
  close(arq);
end;

procedure FadeOUT;
var fk, fr,fl: integer;
    bR, bG, bB: byte;
    OTime:integer;

begin
   fl:=0;
   While fl<8 do begin
         for fr:=0 to 15 do begin
             getpalette(15-fr,bR,bG,bB);
             if bR>0 then bR:=bR-1;
             if bG>0 then bG:=bG-1;
             if bB>0 then bB:=bB-1;
             setpalette(15-fr,bR,bG,bB);
         end;
         fl:=fl+1;
   end;
end;

procedure BlackOut;
var fr: integer;
begin
   for fr:=0 to 15 do begin
       setpalette(15-fr,0,0,0);
   end;
end;




