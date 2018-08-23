{-----------------------------------------------------------------------------}
{        General Declarations                                                 }
{-----------------------------------------------------------------------------}
Type TPString = String[255];
Const 
  Color_Transparent =0;
  Color_Black = 1;
  Color_Green = 2;
  Color_LightGreen = 3;
  Color_DarkBlue = 4;
  Color_Blue = 5;
  Color_DarkRed = 6;
  Color_LightBlue = 7;
  Color_Red = 8;
  Color_LightRed = 9;
  Color_Yellow = 10;
  Color_LightYellow = 11;
  Color_DarkGreen = 12;
  Color_Magenta = 13;
  Color_Grey = 14;
  Color_White = 15;
  

{Command array for the Vdp access }

Var _sx  :Integer Absolute $f562 ;
    _sy  :Integer Absolute $f564 ;
    _dx  :Integer Absolute $f566 ;
    _dy  :Integer Absolute $f568 ;
    _nx  :Integer Absolute $f56a ;
    _ny  :Integer Absolute $f56c ;
    _col :Byte Absolute    $f56e ;
    _arg :Byte Absolute    $f56f ;
    _cmd :Byte Absolute    $f570 ;

Var Jiffy: integer absolute $FC9E; 
    ClickSW: integer absolute $F3DB;

{Vdp Copy commands }

Const HMMC = $F0 ; {High speed Byte Cpu  -> Vram }
      YMMM = $E0 ; {High speed Byte Vram -> Vram in y-axis }
      HMMM = $D0 ; {High speed Byte Vram -> Vram in x- and y-axis }
      HMMV = $C0 ; {High speed Byte Vdp  -> Vram = paint a box }

      LMMC = $B0 ; {Logical Dot     Cpu  -> Vram }
      LMCM = $A0 ; {Logical Dot     Vram -> Cpu  }
      LMMM = $90 ; {Logical Dot     Vram -> Vram }
      LMMV = $80 ; {Logical Dot     Vdp  -> Vram = paint a box }


{Copy directions for the ARG-register }

      Copy_Right = 0;
      Copy_Down  = 0;
      Copy_Left  = 4;
      Copy_Up    = 8;
      ExpansionVram   = 32 ;
      Vram  = 0 ;

Const TPSET=8;
      Mult256: array[0..3] of integer=(0,256,512,768);

Procedure Screen(mode:Byte);
{ Igual ao screen do Basic }
Const EXPTBL = $fcc0;
      CALSLT = $001C;
      CHGMOD = $005F;

Begin
     Inline  (
              $f3                { di }
             /$3a/mode           { LD A,(MODE) }
             /$fd/$2a/$c0/$fc    { LD IY,(EXPTBL - 1 }
             /$DD/$21/$5f/00     { LD IX,CHGMOD }
             /$CD/$1c/00         { CALL CALSLT }
             /$fb                { EI }
             );

End;

Procedure Color(ForeGroundColor,BackGroudColor,BorderColor:Byte);
{ Igual ao color do Basic }

Var FORCLR :Byte Absolute $F3E9;
    BAKCLR :Byte Absolute $f3ea;
    BDRCLR :Byte Absolute $f3eb;

Begin
    FORCLR := ForeGroundColor;
    BAKCLR := BackGroudColor;
    BDRCLR := BorderColor;
    Inline (  $f3/
              $fd/$2a/$c0/$fc    { LD IY,(EXPTBL - 1 }
             /$DD/$21/$62/00     { LD IX,CHGCLR }
             /$CD/$1c/00/$fb     { CALL CALSLT }
           );
End;



Procedure GraphicPrint(Str:TPString;X,Y:Integer;Color:Byte;LogOp:Byte);
{ Escreve uma string em modo grafico }

Var StoreX,StoreY :Integer ;
    ch            :Char;
    i,StoreColor  :Byte;
    GRPACX        :Integer Absolute $FCB7;
    GRPACY        :Integer Absolute $FCB9;
    ATRBYT        :Byte Absolute $F3F2;
    LOGOPER       :Byte Absolute $f570;

Begin
     StoreColor := ATRBYT;
     ATRBYT := Color;
     StoreX := GRPACX;
     StoreY := GRPACY;
     GRPACX := x;
     GRPACY := y;
     LOGOPER :=LogOp;
     For i := 1 to Length(Str) Do
        Begin
           ch := Str[i];
           Inline (
              $f3/
              $3a/ch/
              $fd/$2a/$c0/$fc    { LD IY,(EXPTBL - 1 }
             /$DD/$21/$10/$15    { LD IX,grpprnt }
             /$CD/$1c/00         { CALL CALSLT }
             /$fb
                   );
         End;
     GRPACX :=StoreX;
     GRPACY :=StoreY;
     ATRBYT :=StoreColor;
     LOGOPER := 0; {Normal condition}
End;

Procedure GraphicPrintChar(Str:Char;X,Y:Integer;Color:Byte;LogOp:Byte);
{ Escreve uma string em modo grafico }

Var StoreX,StoreY :Integer ;
    ch            :Char;
    StoreColor  :Byte;
    GRPACX        :Integer Absolute $FCB7;
    GRPACY        :Integer Absolute $FCB9;
    ATRBYT        :Byte Absolute $F3F2;
    LOGOPER       :Byte Absolute $f570;

Begin
     StoreColor := ATRBYT;
     ATRBYT := Color;
     StoreX := GRPACX;
     StoreY := GRPACY;
     GRPACX := x;
     GRPACY := y;
     LOGOPER :=LogOp;
     ch := Str;
           Inline (
              $f3/
              $3a/ch/
              $fd/$2a/$c0/$fc    { LD IY,(EXPTBL - 1 }
             /$DD/$21/$10/$15    { LD IX,grpprnt }
             /$CD/$1c/00         { CALL CALSLT }
             /$fb
                   );
    
    LOGOPER := 0; {Normal condition}
End;

Procedure GraphicPrint40(Str:TPString;Column,Row:Integer;Color:Byte;Wrap:Boolean;LogOp:Byte);
var i:integer;
    currX,currY:integer;
    CurrColumn, CurrRow:byte;
    
begin
    for i:=0 to pred(Length(Str)) do
      begin
      if (wrap=true) or ((wrap=false)and(column+i<40)) then begin
        CurrColumn := (column+i) mod 40 ;
        CurrRow := Row +(i div 40);
        if column+i>39  then begin
          if currColumn<column then begin
            CurrRow:=succ(CurrRow);
          end;
        end;

        GraphicPrintChar(str[succ(i)],currColumn*6,succ(CurrRow)*8,Color,Logop);
      end {if}
    end; {for i}

end; {GrapiphicPrint40}

Procedure SetPage(DisplayPage,ActivePage:Byte);
Const SETPAGE = $013D; {SubRom routine}

Var   DPPAGE :Byte Absolute  $FAF5;
      ACPAGE :Byte Absolute  $FAF6;

Begin
    DpPage := DispLayPage;
    AcPage := ActivePage;

    Inline (
             $f3
            /$fd/$2a/$f7/$fa    { LD IY,(EXbrsa - 1 }
            /$DD/$21/$3d/$01    { LD IX,setpage }
            /$CD/$1c/00         { CALL CALSLT }
            /$fb
             );
End;

Procedure  Copy_xy (Source_x,Source_y:Integer;SourcePage:Byte;
                   Destination_x,Destination_y:Integer;DestinationPage:Byte;
                   Number_x,Number_y:Integer;Direction:Byte);

Begin
   _cmd:= HMMM ;
   _sy := Mult256[SourcePage] + Source_y ;
   _dy := Mult256[DestinationPage] + Destination_y ;
   _arg:= Direction ;
   _dx := Destination_x;
   _sx := Source_x;
   _nx := Number_x;
   _ny := Number_y;

   Inline (
     $F3/$3E/$20/$D3/$99/$3E/$11/$F6/$80/$D3/$99/$06/$0F/$0E/$9B/$21/
     $62/$F5/$ED/$B3/$3E/$02/$D3/$99/$3E/$0F/$F6/$80/$D3/$99/$DB/$99/
     $CB/$47/$20/$F0/$AF/$D3/$99/$3E/$8F/$D3/$99/$FB
          );
End ; {Copy_xy}

Procedure  Copy_y (Source_x,Source_y:Integer;SourcePage:Byte;
                   Destination_y,Number_y:Integer;DestinationPage:Byte;
                   Direction:Byte);


Begin
   _cmd:= YMMM ;
   _sy := Mult256[SourcePage] + Source_y ;
   _dy := Mult256[DestinationPage] + Destination_y ;
   _arg:= Direction ;
   _dx := Source_x; {It`s not an error ! It's really like this.}
   {_sx := sx; <- Not used ! }
   {_nx := Number_x; <- Not used ! }
   _ny := Number_y;

   Inline (
     $F3/$3E/$20/$D3/$99/$3E/$11/$F6/$80/$D3/$99/$06/$0F/$0E/$9B/$21/
     $62/$F5/$ED/$B3/$3E/$02/$D3/$99/$3E/$0F/$F6/$80/$D3/$99/$DB/$99/
     $CB/$47/$20/$F0/$AF/$D3/$99/$3E/$8F/$D3/$99/$FB
          );
End ; {Copy_y}

Procedure  CopyL_xy (Source_x,Source_y:Integer;SourcePage:Byte;
                     Destination_x,Destination_y:Integer;DestinationPage:Byte;
                     Number_x,Number_y:Integer;
                     LogicalOperation,Direction:Byte);


Begin
   _cmd:= LMMM + LogicalOperation;
   _sy := Mult256[SourcePage] + Source_y ;
   _dy := Mult256[DestinationPage] + Destination_y ;
   _arg:= Direction ;
   _dx := Destination_x;
   _sx := Source_x;
   _nx := Number_x;
   _ny := Number_y;

   Inline (
     $F3/$3E/$20/$D3/$99/$3E/$11/$F6/$80/$D3/$99/$06/$0F/$0E/$9B/$21/
     $62/$F5/$ED/$B3/$3E/$02/$D3/$99/$3E/$0F/$F6/$80/$D3/$99/$DB/$99/
     $CB/$47/$20/$F0/$AF/$D3/$99/$3E/$8F/$D3/$99/$FB
          );
End ; {CopyL_xy}

Var
   VA:byte;
   VHL,VBC,VDE:integer;

Const
   GTSTCK=$D5;     {retorna o estado do joystick selecionado}
   GTTRIG=$D8;     {retorna o estado do botao de disparo selecionado}
   WRTPSG=$93;     {grava um byte num registro do PSG}

Procedure CallRom(VIX:integer);
BEGIN
inline($F3/$CD/*+19/$FB/$32/VA/$22/VHL/$ED/$43/VBC/$ED/$53/VDE/$18/$1B/
       $DD/$2A/VIX/$3A/VA/$2A/VHL/$ED/$4B/VBC/$ED/$5B/VDE/$08/$DB/$A8/
       $F5/$E6/$F0/$C3/$8C/$F3/$FB);
END;

Function Stick(n:byte):byte;
BEGIN
  VA:=n;
  CallRom(GTSTCK);
  Stick:=VA;
END;

Function Strig(n:byte):boolean;
BEGIN
   VA:=n;
   CallRom(GTTRIG);
   Strig:=(va<>0);
END;

Procedure Sound(r,v:byte);
BEGIN
   VA:=r;
   VDE:=v;
   CallRom(WRTPSG);
END;

Procedure _MoveDotToVram(SourceAddress :Integer;Destination_x,
Destination_y:Integer;DestinationPage:Byte;Number_x,Number_y:Integer;
LogicalOperation,Direction:Byte);
Var Address :Integer;

Begin

   Address := SourceAddress+1; {Because the firstbyte is set in CMD array. }
   _cmd:= LMMC + LogicalOperation;
   _dy := Mult256[DestinationPage] + Destination_y ;
   _arg:= Direction ;
   _dx := Destination_x;
   _nx := Number_x;
   _ny := Number_y;
   _col:= Mem[Address-1];
   Inline ($f3/$DD/$2A/Address/
          $3E/$02/$D3/$99/$3E/$8F/$D3/$99/$DB/$99/$CB/$47/
          $20/$F2/$F3/$3E/$20/$D3/$99/$3E/$11/$F6/$80/$D3/$99/$06/$0F/$0E/
          $9B/$21/$62/$F5/$ED/$B3/$3E/$2C/$F6/$80/$D3/$99/$3E/$11/$F6/$80/
          $D3/$99/$3E/$02/$D3/$99/$3E/$8F/$D3/$99/$DB/$99/$CB/$47/$28/$0D/
          $CB/$7F/$28/$EE/$DD/$7E/$00/$DD/$23/$D3/$9B/$18/$E5/$3E/$00/$D3/
          $99/$3E/$8F/$D3/$99/$DB/$99/$fb);
end ; {Lmmc}

Type StringType = String[80];

Procedure LoadGraphic(FileName :StringType;
                      Width, Height: Integer;
                      x,y:Integer;
                      Page:Byte;LogOp,Direction, ScrMod :Byte);

Var ReadBuf        :^Integer;
    Temp           :Integer Absolute ReadBuf;
    DotBuf         :^Integer;
    DotTemp        :Integer Absolute DotBuf;
    Counter,yy     :Integer;
    BufSize        :Integer;
Begin
     Case ScrMod Of
       0,1,2,3,4 :BufSize := 256 ; {Dummy option}
       5,7       :BufSize := 512 ;
       6         :BufSize := 1024;
       8         :BufSize := 256 ;
     End;
     GetMem(ReadBuf,256);
     GetMem(DotBuf,BufSize);
     FcbPointer := MsxOpen(filename);
     Counter :=Width Div (BufSize Div 256) ;
     yy :=y;

     MsxRead(FcbPointer,counter,1,Temp);
     While  (MsxIOResult = 0) Do Begin
       Case ScrMod Of
         5,7 :Inline($f3/$2A/Temp/$ED/$5B/DotTemp/$ED/$4B/Counter
                     /$7E/$F5/$CB/$3F/$CB/$3F/$CB/$3F/$CB/$3F/$12/$13/$F1/$E6/
                      $0F/$12/$13/$23/$0B/$78/$B1/$20/$E9/$fb);
                      {Split one 8-byte to two 4-bit bytes}
         6  :Inline ($f3/$DD/$2A/Temp /$FD/$2A/DotTemp
                      /$ED/$5B/Counter /$DD/$7E/$00/$06/$04/$0E/$00/$17/$CB/
                       $11/$17/$CB/$11/$FD/$71/$00/$FD/$23/$10/$F1/$1B/$DD/$23/
                       $7A/$B3/$20/$E5/$fb);
                       {Split one 8-byte to four 2-bit bytes}
         8   :Inline ($f3/
                      $2a/Temp /$ed/$5b/DotTemp /$ed/$4b /Counter /
                      $ed/$b0/$f3/$fb);
                       {Moves 8-bit data directly from readbuf to Vrambuf.}
       End; {Case}
       _MoveDotToVram(DotTemp,x,yy,Page,256,1,LogOp,Direction);
       yy :=yy+1;
       MsxRead(FcbPointer,counter,1,Temp);
     End;
     MsxClose(FcbPointer);
     FreeMem(ReadBuf,256);
     FreeMem(DotBuf,BufSize);
End;

Procedure LoadBMP(FileName :StringType;
                      Width, Height: Integer;
                      x,y:Integer;
                      Page:Byte;LogOp,Direction, ScrMod :Byte);
{Load a Windows 4 bit bitmap; palette ignored}
Var ReadBuf        :^Integer;
    Temp           :Integer Absolute ReadBuf;
    DotBuf         :^Integer;
    DotTemp        :Integer Absolute DotBuf;
    Counter,yy     :Integer;
    BufSize        :Integer;
    i              :integer;
Begin
     Case ScrMod Of
       0,1,2,3,4 :BufSize := 256 ; {Dummy option}
       5,7       :BufSize := 512 ;
       6         :BufSize := 1024;
       8         :BufSize := 256 ;
     End;
     GetMem(ReadBuf,256);
     GetMem(DotBuf,BufSize);
     FcbPointer := MsxOpen(filename);
     Counter :=Width Div (BufSize Div 256) ;
     yy :=y;

  {Read and ignore file header}
 { for i:=0 to 1 do begin
      MsxRead(FcbPointer,70,1,Temp);
 end;}


     MsxRead(FcbPointer,counter,1,Temp);
     While  (MsxIOResult = 0) Do Begin
       Case ScrMod Of
         5,7 :Inline($f3/$2A/Temp/$ED/$5B/DotTemp/$ED/$4B/Counter
                     /$7E/$F5/$CB/$3F/$CB/$3F/$CB/$3F/$CB/$3F/$12/$13/$F1/$E6/
                      $0F/$12/$13/$23/$0B/$78/$B1/$20/$E9/$fb);
                      {Split one 8-byte to two 4-bit bytes}
         6  :Inline ($f3/$DD/$2A/Temp /$FD/$2A/DotTemp
                      /$ED/$5B/Counter /$DD/$7E/$00/$06/$04/$0E/$00/$17/$CB/
                       $11/$17/$CB/$11/$FD/$71/$00/$FD/$23/$10/$F1/$1B/$DD/$23/
                       $7A/$B3/$20/$E5/$fb);
                       {Split one 8-byte to four 2-bit bytes}
         8   :Inline ($f3/
                      $2a/Temp /$ed/$5b/DotTemp /$ed/$4b /Counter /
                      $ed/$b0/$f3/$fb);
                       {Moves 8-bit data directly from readbuf to Vrambuf.}
       End; {Case}
       _MoveDotToVram(DotTemp,x,yy,Page,256,1,LogOp,Direction);
       yy :=succ(yy);
       MsxRead(FcbPointer,counter,1,Temp);
     End;
     MsxClose(FcbPointer);
     FreeMem(ReadBuf,256);
     FreeMem(DotBuf,BufSize);
End;


procedure CLS;
begin
  Inline  (
           $f3                 { di }
           /$fd/$2a/$c0/$fc    { LD IY,(EXPTBL - 1 }
           /$DD/$21/$c3/00     { LD IX,CHGMOD }
           /$CD/$1c/00         { CALL CALSLT }
           /$fb                { EI }
           );
  
end;

procedure cls2;

begin
  inline ($f3/$af/          { xor a         }
          $fd/$2a/$c0/$fc/  { ld iy,(#fcc0) }
          $dd/$21/$c3/$00/  { ld ix,#c3     }
          $cd/$1c/$00/$fb); { call #1c      }
end;

Procedure MirrorCopy(SourceX,SourceY,SourcePage,DestX,DestY,DestPage,Width,Height,LogOP:integer);
var fx:integer;
begin
  DestX:=DestX+Width;
  Width:=SourceX+Width;
  fx:=SourceX;
  while fx<=Width do begin
        CopyL_xy(fx,SourceY,SourcePage,
                 DestX,DestY,DestPage,
                 1,Height,LogOp,0);
        DestX:=pred(DestX);
        fx:=succ(fx);
  end;
end;

Procedure Copy(X1,Y1,X2,Y2,SrcPage,DestX,DestY,DestPage:integer);
begin
   _cmd:= HMMM ;
   _sy := Mult256[SrcPage] + y1 ;
   _dy := Mult256[DestPage]+ Desty ;
   _arg:= 0;
   _dx := Destx;
   _sx := x1;
   _nx := succ(x2-x1);
   _ny := succ(y2-y1);

   Inline (
     $F3/$3E/$20/$D3/$99/$3E/$11/$F6/$80/$D3/$99/$06/$0F/$0E/$9B/$21/
     $62/$F5/$ED/$B3/$3E/$02/$D3/$99/$3E/$0F/$F6/$80/$D3/$99/$DB/$99/
     $CB/$47/$20/$F0/$AF/$D3/$99/$3E/$8F/$D3/$99/$FB
          );
end;

Procedure LCopy(X1,Y1,X2,Y2,SrcPage,DestX,DestY,DestPage,LogOp:integer);
begin
   _cmd:= LMMM + LogOp;
   _sy := Mult256[SrcPage] + y1 ;
   _dy := Mult256[DestPage] + Desty ;
   _arg:= 0;
   _dx := Destx;
   _sx := x1;
   _nx := succ(x2-x1);
   _ny := succ(y2-y1);

   Inline (
     $F3/$3E/$20/$D3/$99/$3E/$11/$F6/$80/$D3/$99/$06/$0F/$0E/$9B/$21/
     $62/$F5/$ED/$B3/$3E/$02/$D3/$99/$3E/$0F/$F6/$80/$D3/$99/$DB/$99/
     $CB/$47/$20/$F0/$AF/$D3/$99/$3E/$8F/$D3/$99/$FB
          );
end;

function Inkey:char;
var bt:byte;
begin
     Inkey:=chr(0);
     Mem[$FCA9]:=1;
     Inline($f3/$fd/$2a/$c0/$fc/$DD/$21/$9F/00     
            /$CD/$1c/00/$32/bt/$fb);
     Inkey:=chr(bt);
     Mem[$FCA9]:=0;
end;
