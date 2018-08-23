Type Fcb = Record
                 Drive       :Byte;
                 FileName    :Array[1..11] Of Char;
                 CurBlock    :Integer;
                 RecSize     :Integer;
                 FileSizeLo  :Integer;
                 FileSizeHi  :Integer;
                 Date        :Integer;
                 Time        :Integer;
                 DeviceId    :Byte;
                 DirLoc      :Byte;
                 TopCluster  :Integer;
                 LastAccessd :Integer;
                 RelLoc      :Integer;
                 CurRec      :Integer;
                 RanRecLo    :Integer;
                 RanRecHi    :Integer;
           End; {Fcb}

    OpenString     = String[255];
    FcbPointerType = ^Fcb;

Var FcbPointer :FcbPointerType;
    MSXIOResult :Byte;

Function MSXOpen(Filename:OpenString):FcbPointerType;
Var Drv,Ch     :Char;
    i,j        :Byte;
    FcbPointer :FcbPointerType;

Begin

     New(FcbPointer);

     With FcbPointer^ Do  { Clear FCB }
       Begin
       Drive       := 0 ;
       For i := 1 To 11 Do FileName[i] := Chr(32);
       CurBlock    := 0 ;
       RecSize     := 0 ;
       FileSizeLo  := 0 ;
       FileSizeHi  := 0 ;
       Date        := 0 ;
       Time        := 0 ;
       DeviceId    := 0 ;
       DirLoc      := 0 ;
       TopCluster  := 0 ;
       LastAccessd := 0 ;
       RelLoc      := 0 ;
       CurRec      := 0 ;
       RanRecLo    := 0 ;
       RanRecHi    := 0 ;
     End;

   { Prepare FileName for FCB }

    If Pos(':',FileName) = 0 Then FcbPointer^.Drive := 0
     Else
         Begin
             Drv := UpCase(Filename[1]);
             FcbPointer^.Drive := Ord(Drv) - Ord('A') + 1;
         End;
    j:= 1;
    i := Pos(':',FileName) + 1 ;
    While (FileName[i] <> '.')  And
          (i < (Pos(':',FileName)+9 )) And ( i <= Length(FileName) ) Do
      Begin
            FcbPointer^.FileName[j] := FileName[i];
            i := succ(i);
            j := succ(j);
      End; {While}
     If Pos('.', FileName) <> 0 Then
      Begin
         i := Pos('.',FileName) + 1;
         j := 9;
         While (i < (Pos('.',FileName)+4 )) And ( i <= Length(FileName) ) Do
           Begin
            FcbPointer^.FileName[j] := FileName[i];
            i := succ(i);
            j := succ(j);
           End; {While}
     End; {If}

    Inline ( $ed/$5b/ FcbPointer /
             $0e/$0f/                 { LD C,OPEN }
             $cd/05/00/
             $32/MSXIOResult
           );
    If MSXIOResult = 0 Then 
     MSXOpen := FcbPointer
    Else 
    MsxOpen := Nil;

End; {MsxOpen}

Function  MSXCreate(Filename:OpenString):FcbPointerType;
Var Drv,Ch     :Char;
    i,j        :Byte;
    FcbPointer :FcbPointerType;

Begin
     New(FcbPointer);
     With FcbPointer^ Do  { Clear FCB }
       Begin
       Drive       := 0 ;
       For i := 1 To 11 Do FileName[i] := Chr(32);
       CurBlock    := 0 ;
       RecSize     := 0 ;
       FileSizeLo  := 0 ;
       FileSizeHi  := 0 ;
       Date        := 0 ;
       Time        := 0 ;
       DeviceId    := 0 ;
       DirLoc      := 0 ;
       TopCluster  := 0 ;
       LastAccessd := 0 ;
       RelLoc      := 0 ;
       CurRec      := 0 ;
       RanRecLo    := 0 ;
       RanRecHi    := 0 ;
     End;

   { Prepare FileName for FCB }

    If Pos(':',FileName) = 0 Then FcbPointer^.Drive := 0
     Else
         Begin
             Drv := UpCase(Filename[1]);
             FcbPointer^.Drive := Ord(Drv) - Ord('A') + 1;
         End;
    j:= 1;
    i := succ(Pos(':',FileName));
    While (FileName[i] <> '.')  And
          (i < (Pos(':',FileName)+9 )) And ( i <= Length(FileName) ) Do
      Begin
            FcbPointer^.FileName[j] := FileName[i];
            i := succ(i) ;
            j := succ(j);
      End; {While}
     If Pos('.', FileName) <> 0 Then
      Begin
         i := succ(Pos('.',FileName));
         j := 9;
         While (i < (Pos('.',FileName)+4 )) And ( i <= Length(FileName) ) Do
           Begin
            FcbPointer^.FileName[j] := FileName[i];
            i := succ(i);
            j := succ(j);
           End; {While}
     End; {If}

    Inline
          (  $ed/$5b/ FcbPointer /
             $0e/$16/                { LD C,CREATE }
             $cd/$05/$00/
             $32/MSXIOResult
          );
    If MsxIOResult = 0 Then
      MSXCreate := FcbPointer
    Else 
      MsxCreate := Nil;
End; {MsxCreate}


Procedure MSXClose(FcbPointer:FcbPointerType);
Begin
     Inline ($ed/$5b/ FcbPointer /
             $0e/$10/                { LD C,Close }
             $cd/$05/$00/
             $32/MSXIOResult
          );
     Dispose(FcbPointer);
End; {MsxClose}

Procedure MSXWrite(
FcbPointer:FcbPointerType;RecordSize,RecordCount,BufferAddr:Integer);
{BufferAddr = DMA in DOS }
Begin
       Inline ( $ed/$5b/ BufferAddr /
                $0e/$1a/                { LD C,SET_DMA }
                $cd/$05/$00
              );
       FcbPointer^.RecSize := RecordSize; {You could as well set the Sandom
                                           Rec Lo and Hi ie. record number
                                           in FCB.}
       Inline ( $ed/$5b/ FcbPointer /
                $0e/$26/                { LD C,Random_Write }
                $2a/ RecordCount /
                $cd/$05/$00/
                $32/MSXIOResult
              );
       Inline ( $0e/$0d/               { Reset disk drive parameters }
                $cd/$05/$00
              );
End; {MsxWrite}

Procedure MSXRead(
  FcbPointer:FcbPointerType;RecordSize,RecordCount,BufferAddr:Integer);
  {BufferAddr = DMA in DOS }
  Begin
       Inline ( $ed/$5b/ BufferAddr /
                $0e/$1a/                { LD C,SET_DMA }
                $cd/$05/$00
              );

       FcbPointer^.RecSize := RecordSize; {You could as well set the Sandom
                                           Rec Lo and Hi ie. record number
                                           in FCB.}
       Inline ( $ed/$5b/ FcbPointer /
                $0e/$27/                { LD C,Random_Read }
                $2a/ RecordCount /
                $cd/$05/$00/
                $32/MSXIOResult
              );
End; {MsxRead}

Procedure MSXSeek(FcbPointer:FcbPointerType;RecNumberLo,RecNumberHi:Integer);
Begin
     FcbPointer^.RanRecLo := RecNumberLo;
     FcbPointer^.RanRecHi := RecNumberHi;
End;

