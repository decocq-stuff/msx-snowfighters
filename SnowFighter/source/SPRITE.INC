Type Sprite8Type = Array[0..7] Of Byte;
     SpriteColorArrayType=Array[0..15] Of Byte;

Const ArrowSprite :Sprite8Type = ($fe,$fc,$f0,$f8,$dc,$ce,$87,$03);
      ArrowSprite2:Sprite8Type = ($fe,$84,$88,$88,$b4,$ca,$85,$02);
      FingerSprite:Sprite8Type = ($06,$06,$06,$57,$7f,$7f,$7e,$3c);
      SquareSprite:Sprite8Type = (255,129,129,129,129,129,129,255);
      TimeGlassSprite:Sprite8Type =($7e,$7e,$3c,$18,$18,$3c,$7e,$7e);

      WhiteArray  :SpriteColorArrayType = (15,15,15,15,15,15,15,15,
                                           15,15,15,15,15,15,15,15);
      YellowArray  :SpriteColorArrayType = (10,10,10,10,10,10,10,10,
                                             10,10,10,10,10,10,10,10);

      RedArray  :SpriteColorArrayType = (8,8,8,8,8,8,8,8,
                                           8,8,8,8,8,8,8,8);
      BlackArray  :SpriteColorArrayType = (0,0,0,0,0,0,0,0,
                                           0,0,0,0,0,0,0,0);

Var  
     Vdp1 :Byte Absolute  $f3e0;
     Vdp5 :Byte Absolute  $f3e4;
     Vdp6 :Byte absolute  $f3e5;
     Vdp8 :Byte Absolute  $ffe7; 
     Vdp11:Byte Absolute  $ffea;

Procedure _WrtVdp(VdpRegister,Value:Byte);

 Begin   
    Inline (
             $F3/$3A/ Value  /$D3/$99/$3A/ VdpRegister /$F6/$80/$D3/$99/$fb
            );
 End;

Procedure _WriteVram(VramBlockUsed:Byte;
                     VramAddr,RamAddr,ByteCount:Integer);

{ VramBlocUsed  = 0 = first 64 Kb
                  1 = second 64 Kb
                  2 = Expansion Vram 64 Kb }


Var Expansion, AddrHi, AddrMid, AddrLo :Byte;

Begin
  If VramBlockUsed = 2 Then Expansion := $40 {Set the bit 6 (MXC) }
   Else Expansion := 0 ; {No expansion vram used}
  AddrHi := Hi(VramAddr) Div $40;
  If VramBlockUsed = 1 Then AddrHi := AddrHi + 4 ;
    {Set addr bit 16 in reg 14 = bit 2 }
  AddrLo := Lo(VramAddr);
  AddrMid := Hi(Vramaddr) And 63 ;

  Inline(
      $F3/$3A/ Expansion /$D3/$99/$3E/$2D/$F6/$80/$D3/$99/$3A/ AddrHi /$D3/
      $99/$3E/$0E/$F6/$80/$D3/$99/$3A/ AddrLo /$00/$D3/$99/$3A/ AddrMid /$F6/
      $40/$D3/$99/$2A/ RamAddr /$ED/$4B/ ByteCount /$7E/$D3/$98/$23/$0B/$79/
      $B0/$20/$F7/ $F3/$3e/ 0  /$D3/$99/$3E/$2D/$F6/$80/$D3/$99/$fb
        );
End;

Procedure _FillVram(VramBlockUsed:Byte;
                    VramAddr:Integer;FillValue :Byte;
                    ByteCount:Integer);

{ VramBlocUsed  = 0 = first 64 Kb
                  1 = second 64 Kb
                  2 = Expansion Vram 64 Kb }


Var Expansion, AddrHi, AddrMid, AddrLo :Byte;
    RamAddr :Integer;
Begin
  If VramBlockUsed = 2 Then Expansion := $40 {Set the bit 6 (MXC) }
   Else Expansion := 0 ; {No expansion vram used}
  AddrHi := Hi(VramAddr) Div $40;
  If VramBlockUsed = 1 Then AddrHi := AddrHi + 4 ;
    {Set addr bit 16 in reg 14 = bit 2 }
  AddrLo := Lo(VramAddr);
  AddrMid := Hi(Vramaddr) And 63 ;
  RamAddr := Addr(FillValue);
  Inline(
      $F3/$3A/ Expansion /$D3/$99/$3E/$2D/$F6/$80/$D3/$99/$3A/ AddrHi /$D3/
      $99/$3E/$0E/$F6/$80/$D3/$99/$3A/ AddrLo /$00/$D3/$99/$3A/ AddrMid /$F6/
      $40/$D3/$99/$2A/ RamAddr /$ED/$4B/ ByteCount /$7E/$D3/$98/0/$0B/$79/
      $B0/$20/$F7/ $F3/$3e/ 0  /$D3/$99/$3E/$2D/$F6/$80/$D3/$99/$fb
        );
End;

Procedure SetSpritePattern(PatternNumber:Byte;Var PatternArray:Sprite8Type);

  Var  VramAddr :Integer;
       A  :Sprite8Type;
  Begin
    A := PatternArray;
    VramAddr := (Vdp6 * $800) + (PatternNumber * 8);
    _WriteVram(0,VramAddr,Addr(A),8);
  End;

Procedure PutSprite(SpriteNum,SpritePatternNum,x,y:Byte);
Var VramAddr :Integer;
    Buf      :Array[0..3] Of Byte;

Begin
    VramAddr := ((Vdp5 And 252) * $80) +(Vdp11 * $8000) + (SpriteNum * 4);
    Buf[0] := y;
    Buf[1] := x;
    Buf[2] := SpritePatternNum;

    _WriteVram(0,VramAddr,Addr(Buf),4);
End;


Procedure SetSpriteColors(SpriteNum:Byte; AttributeA:SpriteColorArrayType);

{Attribute bit 7 = EC  32 bit shift 1=on 0=off}
{          bit 6 = CC  Change priority (read vdp manual }
{          bit 5 = IC  this line ,bit set 0, does not conflict other sprites}
{          bit 4 = 0   not used}
{          bits 3..0   colour code 0 to 15;}

   Var Vdp5  :Byte Absolute $f3e4;
       Vdp11 :Byte Absolute $ffea;
       AttrA :SpriteColorArrayType ;
       VramAddr :Integer;
   Begin
       AttrA := AttributeA;
       VramAddr := ((Vdp5 And 252) * $80) + (Vdp11 * $8000) - 512
                    + (SpriteNum * 16) ; {Colour table}
       _WriteVram(0,VramAddr,Addr(AttrA),16);
   End;

Procedure ClearAllSprites;
 Const FillValue = 0;
 Var VramAddr :Integer;
 Begin
   VramAddr := Vdp6 * $800;
   _FillVram(0,VramAddr,FillValue,8*256); { Zero the Sprite Pattern table }
 End;

Procedure DisableSprites; {Get some speed to graphic operations. }
  Begin
     Vdp8 := Vdp8 Or 2;
     _WrtVdp(8,Vdp8);
  End;

Procedure EnableSprites; {Loose some speed in graphic operations. }
  Begin
     Vdp8 := Vdp8 And 253;
     _WrtVdp(8,Vdp8);
  End;

Procedure SpriteSize8; {Default situation }
  Begin
     Vdp1 := Vdp1 And 253;
     _WrtVdp(1,Vdp1);
  End;

Procedure SpriteSize16;
  Begin
     Vdp1 := Vdp1 Or 2;
     _WrtVdp(1,Vdp1);
  End;

Procedure SpriteSizeNormal; {Default situation }
  Begin
     Vdp1 := Vdp1 And 254;
     _WrtVdp(1,Vdp1);
  End;

Procedure SpriteSizeMagnified;
  Begin
     Vdp1 := Vdp1 Or 1;
     _WrtVdp(1,Vdp1);
  End;

function sprite_collision:boolean;
var reg:byte;

begin
    inline($fd/$2a/$f7/$fa/        { LD IY,(&HFAF7)      }
           $dd/$21/$31/$01/        { LD IX,&H0131        }
           $3e/$00/                { LD A,0              }
           $cd/$1c/$00/            { CALL &H001C         }
           $32/reg/                { LD (REG),A          }
           $fb);                   { EI                  }
    sprite_collision:=((reg and 32)=32)
end;


