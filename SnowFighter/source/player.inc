type playstring= string[2];
                            
Var
   { Music Variables }
   Envelope, EFreqL, EFreqH: byte;
   Channels,Mixer: byte;

   MusTimer: integer;
   SongPos: integer;
   tempo: byte;

   PatternA: array[1..8] of string[128];
   PatternB: array[1..8] of string[128];
   PatternC: array[1..8] of string[128];  
 
   mScore: array[1..8] of byte;
   ScorePos:byte;
   ScrVal: byte;
 
   Ch1,Ch2,Ch3: playstring; 
   VolA,VolB, VolC: byte;
   ShortNotes: boolean;
   

Procedure Play(Ch1, Ch2, Ch3: playstring);
var s0,s1,s2,s3,s4,s5: integer;
    Chn1,Chn2, Chn3: playstring; 
    Volume,Code:integer;
    Tempstr: string[3];
    fr: byte;
    
begin

s0:=-1;s1:=-1;
s2:=-1;s3:=-1;
s4:=-1;s5:=-1;

chn1:=ch1; chn2:=ch2; chn3:=ch3;


  { Case Chn1 of }   
  if (Chn1<>'..') and (Chn1<>'MT') then begin
       if chn1='C1' then begin s0:=98;  s1:=13; end;
       if chn1='D1' then begin s0:=245; s1:=11; end;
       if chn1='E1' then begin s0:=149; s1:=10; end;
       if chn1='F1' then begin s0:=3;   s1:=10; end;
       if chn1='G1' then begin s0:=235; s1:=8;  end;
       if chn1='A1' then begin s0:=240; s1:=7;  end;
       if chn1='B1' then begin s0:=18;  s1:=7;  end;
       if chn1='C2' then begin s0:=178; s1:=6;  end;
       if chn1='D2' then begin s0:=242; s1:=5;  end;
       if chn1='E2' then begin s0:=78;  s1:=5;  end;
       if chn1='F2' then begin s0:=1;   s1:=5;  end;
       if chn1='G2' then begin s0:=117; s1:=4;  end;
       if chn1='A2' then begin s0:=249; s1:=3;  end;
       if chn1='B2' then begin s0:=137; s1:=3;  end;
       if chn1='C3' then begin s0:=87;  s1:=3;  end;
       if chn1='D3' then begin s0:=250; s1:=2;  end;
       if chn1='E3' then begin s0:=167; s1:=2;  end;
       if chn1='F3' then begin s0:=129; s1:=2;  end;
       if chn1='G3' then begin s0:=59;  s1:=2;  end;
       if chn1='A3' then begin s0:=253; s1:=1;  end;
       if chn1='B3' then begin s0:=197; s1:=1;  end;
       if chn1='C4' then begin s0:=172; s1:=1;  end;
       if chn1='D4' then begin s0:=125; s1:=1;  end;
       if chn1='E4' then begin s0:=83;  s1:=1;  end;
       if chn1='F4' then begin s0:=64;  s1:=1;  end;
       if chn1='G4' then begin s0:=29;  s1:=1;  end;
       if chn1='A4' then begin s0:=254; s1:=0;  end;
       if chn1='B4' then begin s0:=226; s1:=0;  end;
       if chn1='C5' then begin s0:=214; s1:=0;  end;
       if chn1='D5' then begin s0:=190; s1:=0;  end;
       if chn1='E5' then begin s0:=170; s1:=0;  end;
       if chn1='F5' then begin s0:=160; s1:=0;  end;
       if chn1='G5' then begin s0:=143; s1:=0;  end;
       if chn1='A5' then begin s0:=127; s1:=0;  end;
       if chn1='B5' then begin s0:=113; s1:=0;  end;
       if chn1='C6' then begin s0:=107; s1:=0;  end;
       if chn1='D6' then begin s0:=95;  s1:=0;  end;
       if chn1='E6' then begin s0:=85;  s1:=0;  end;
       if chn1='F6' then begin s0:=80;  s1:=0;  end;
       if chn1='G6' then begin s0:=71;  s1:=0;  end;
       if chn1='A6' then begin s0:=64;  s1:=0;  end;
       if chn1='B6' then begin s0:=57;  s1:=0;  end;
       if chn1='C7' then begin s0:=53;  s1:=0;  end;
       if chn1='D7' then begin s0:=48;  s1:=0;  end;
       if chn1='E7' then begin s0:=42;  s1:=0;  end;
       if chn1='F7' then begin s0:=40;  s1:=0;  end;
       if chn1='G7' then begin s0:=36;  s1:=0;  end;
       if chn1='A7' then begin s0:=32;  s1:=0;  end;
       if chn1='B7' then begin s0:=28;  s1:=0;  end;
       if chn1='C8' then begin s0:=27;  s1:=0;  end;
       if chn1='D8' then begin s0:=24;  s1:=0;  end;
       if chn1='E8' then begin s0:=21;  s1:=0;  end;
       if chn1='F8' then begin s0:=20;  s1:=0;  end;
       if chn1='G8' then begin s0:=18;  s1:=0;  end;
       if chn1='A8' then begin s0:=16;  s1:=0;  end;
       if chn1='B8' then begin s0:=14;  s1:=0;  end;
       if chn1='AB' then begin s0:=224; s1:=1;  end; { A#3 }
       if chn1='AC' then begin s0:=240; s1:=0;  end; { A#4 }
       IF chn1='DB' then begin s0:=207; s1:=2;  end; { D#3 }
       if chn1='DC' then begin s0:=104; s1:=1;  end; { D#4 }
       if chn1='DD' then begin s0:=180; s1:=0;  end; { D#5 }
       if chn1='FB' then begin s0:=93;  s1:=2;  end; { F#3 }
       if chn1='GB' then begin s0:=27;  s1:=2;  end; { G#3 }
       if chn1='GC' then begin s0:=13;  s1:=1;  end; { G#4 }
  end;
  if (Chn2<>'..') and (Chn2<>'MT') then begin
       if chn2='C1' then begin s2:=98;  s3:=13; end;
       if chn2='D1' then begin s2:=245; s3:=11; end;
       if chn2='E1' then begin s2:=149; s3:=10; end;
       if chn2='F1' then begin s2:=3;   s3:=10; end;
       if chn2='G1' then begin s2:=235; s3:=8;  end;
       if chn2='A1' then begin s2:=240; s3:=7;  end;
       if chn2='B1' then begin s2:=18;  s3:=7;  end;
       if chn2='C2' then begin s2:=178; s3:=6;  end;
       if chn2='D2' then begin s2:=242; s3:=5;  end;
       if chn2='E2' then begin s2:=78;  s3:=5;  end;
       if chn2='F2' then begin s2:=1;   s3:=5;  end;
       if chn2='G2' then begin s2:=117; s3:=4;  end;
       if chn2='A2' then begin s2:=249; s3:=3;  end;
       if chn2='B2' then begin s2:=137; s3:=3;  end;
       if chn2='C3' then begin s2:=87;  s3:=3;  end;
       if chn2='D3' then begin s2:=250; s3:=2;  end;
       if chn2='E3' then begin s2:=167; s3:=2;  end;
       if chn2='F3' then begin s2:=129; s3:=2;  end;
       if chn2='G3' then begin s2:=59;  s3:=2;  end;
       if chn2='A3' then begin s2:=253; s3:=1;  end;
       if chn2='B3' then begin s2:=197; s3:=1;  end;
       if chn2='C4' then begin s2:=172; s3:=1;  end;
       if chn2='D4' then begin s2:=125; s3:=1;  end;
       if chn2='E4' then begin s2:=83;  s3:=1;  end;
       if chn2='F4' then begin s2:=64;  s3:=1;  end;
       if chn2='G4' then begin s2:=29;  s3:=1;  end;
       if chn2='A4' then begin s2:=254; s3:=0;  end;
       if chn2='B4' then begin s2:=226; s3:=0;  end;
       if chn2='C5' then begin s2:=214; s3:=0;  end;
       if chn2='D5' then begin s2:=190; s3:=0;  end;
       if chn2='E5' then begin s2:=170; s3:=0;  end;
       if chn2='F5' then begin s2:=160; s3:=0;  end;
       if chn2='G5' then begin s2:=143; s3:=0;  end;
       if chn2='A5' then begin s2:=127; s3:=0;  end;
       if chn2='B5' then begin s2:=113; s3:=0;  end;
       if chn2='C6' then begin s2:=107; s3:=0;  end;
       if chn2='D6' then begin s2:=95;  s3:=0;  end;
       if chn2='E6' then begin s2:=85;  s3:=0;  end;
       if chn2='F6' then begin s2:=80;  s3:=0;  end;
       if chn2='G6' then begin s2:=71;  s3:=0;  end;
       if chn2='A6' then begin s2:=64;  s3:=0;  end;
       if chn2='B6' then begin s2:=57;  s3:=0;  end;
       if chn2='C7' then begin s2:=53;  s3:=0;  end;
       if chn2='D7' then begin s2:=48;  s3:=0;  end;
       if chn2='E7' then begin s2:=42;  s3:=0;  end;
       if chn2='F7' then begin s2:=40;  s3:=0;  end;
       if chn2='G7' then begin s2:=36;  s3:=0;  end;
       if chn2='A7' then begin s2:=32;  s3:=0;  end;
       if chn2='B7' then begin s2:=28;  s3:=0;  end;
       if chn2='C8' then begin s2:=27;  s3:=0;  end;
       if chn2='D8' then begin s2:=24;  s3:=0;  end;
       if chn2='E8' then begin s2:=21;  s3:=0;  end;
       if chn2='F8' then begin s2:=20;  s3:=0;  end;
       if chn2='G8' then begin s2:=18;  s3:=0;  end;
       if chn2='A8' then begin s2:=16;  s3:=0;  end;
       if chn2='B8' then begin s2:=14;  s3:=0;  end;
       if chn2='AB' then begin s2:=224; s3:=1;  end; { A#3 }
       if chn2='AC' then begin s2:=240; s3:=0;  end; { A#4 }
       IF chn2='DB' then begin s2:=207; s3:=2;  end; { D#3 }
       if chn2='DC' then begin s2:=104; s3:=1;  end; { D#4 }
       if chn2='DD' then begin s2:=180; s3:=0;  end; { D#5 }
       if chn2='FB' then begin s2:=93;  s3:=2;  end; { F#3 }
       if chn2='GB' then begin s2:=27;  s3:=2;  end; { G#3 }
       if chn2='GC' then begin s2:=13;  s3:=1;  end; { G#4 }
  end;
  if (Chn3<>'..') and (Chn3<>'MT') then begin
       if chn3='C1' then begin s4:=98;  s5:=13; end;
       if chn3='D1' then begin s4:=245; s5:=11; end;
       if chn3='E1' then begin s4:=149; s5:=10; end;
       if chn3='F1' then begin s4:=3;   s5:=10; end;
       if chn3='G1' then begin s4:=235; s5:=8;  end;
       if chn3='A1' then begin s4:=240; s5:=7;  end;
       if chn3='B1' then begin s4:=18;  s5:=7;  end;
       if chn3='C2' then begin s4:=178; s5:=6;  end;
       if chn3='D2' then begin s4:=242; s5:=5;  end;
       if chn3='E2' then begin s4:=78;  s5:=5;  end;
       if chn3='F2' then begin s4:=1;   s5:=5;  end;
       if chn3='G2' then begin s4:=117; s5:=4;  end;
       if chn3='A2' then begin s4:=249; s5:=3;  end;
       if chn3='B2' then begin s4:=137; s5:=3;  end;
       if chn3='C3' then begin s4:=87;  s5:=3;  end;
       if chn3='D3' then begin s4:=250; s5:=2;  end;
       if chn3='E3' then begin s4:=167; s5:=2;  end;
       if chn3='F3' then begin s4:=129; s5:=2;  end;
       if chn3='G3' then begin s4:=59;  s5:=2;  end;
       if chn3='A3' then begin s4:=253; s5:=1;  end;
       if chn3='B3' then begin s4:=197; s5:=1;  end;
       if chn3='C4' then begin s4:=172; s5:=1;  end;
       if chn3='D4' then begin s4:=125; s5:=1;  end;
       if chn3='E4' then begin s4:=83;  s5:=1;  end;
       if chn3='F4' then begin s4:=64;  s5:=1;  end;
       if chn3='G4' then begin s4:=29;  s5:=1;  end;
       if chn3='A4' then begin s4:=254; s5:=0;  end;
       if chn3='B4' then begin s4:=226; s5:=0;  end;
       if chn3='C5' then begin s4:=214; s5:=0;  end;
       if chn3='D5' then begin s4:=190; s5:=0;  end;
       if chn3='E5' then begin s4:=170; s5:=0;  end;
       if chn3='F5' then begin s4:=160; s5:=0;  end;
       if chn3='G5' then begin s4:=143; s5:=0;  end;
       if chn3='A5' then begin s4:=127; s5:=0;  end;
       if chn3='B5' then begin s4:=113; s5:=0;  end;
       if chn3='C6' then begin s4:=107; s5:=0;  end;
       if chn3='D6' then begin s4:=95;  s5:=0;  end;
       if chn3='E6' then begin s4:=85;  s5:=0;  end;
       if chn3='F6' then begin s4:=80;  s5:=0;  end;
       if chn3='G6' then begin s4:=71;  s5:=0;  end;
       if chn3='A6' then begin s4:=64;  s5:=0;  end;
       if chn3='B6' then begin s4:=57;  s5:=0;  end;
       if chn3='C7' then begin s4:=53;  s5:=0;  end;
       if chn3='D7' then begin s4:=48;  s5:=0;  end;
       if chn3='E7' then begin s4:=42;  s5:=0;  end;
       if chn3='F7' then begin s4:=40;  s5:=0;  end;
       if chn3='G7' then begin s4:=36;  s5:=0;  end;
       if chn3='A7' then begin s4:=32;  s5:=0;  end;
       if chn3='B7' then begin s4:=28;  s5:=0;  end;
       if chn3='C8' then begin s4:=27;  s5:=0;  end;
       if chn3='D8' then begin s4:=24;  s5:=0;  end;
       if chn3='E8' then begin s4:=21;  s5:=0;  end;
       if chn3='F8' then begin s4:=20;  s5:=0;  end;
       if chn3='G8' then begin s4:=18;  s5:=0;  end;
       if chn3='A8' then begin s4:=16;  s5:=0;  end;
       if chn3='B8' then begin s4:=14;  s5:=0;  end;
       if chn3='AB' then begin s4:=224; s5:=1;  end; { A#3 }
       if chn3='AC' then begin s4:=240; s5:=0;  end; { A#4 }
       if chn3='DB' then begin s4:=207; s5:=2;  end; { D#3 }
       if chn3='DC' then begin s4:=104; s5:=1;  end; { D#4 }
       if chn3='DD' then begin s4:=180; s5:=0;  end; { D#5 }
       if chn3='FB' then begin s4:=93;  s5:=2;  end; { F#3 }
       if chn3='GB' then begin s4:=27;  s5:=2;  end; { G#3 }
       if chn3='GC' then begin s4:=13;  s5:=1;  end; { G#4 }
  end;  
  
  if ShortNotes then begin                
     for fr:=0 to 13 do begin
         sound(fr,0);
     end;            
  end;
     
  if (s0>-1) or (s2>-1) or (s4>-1) then begin
     sound(6,Mixer); sound(7,Channels); 
     sound(11,EfreqL); sound(12,EfreqH); sound(13,Envelope); 
  end;   

  { Mute command }             
  if Chn1='MT' then VolA:=0;
  if Chn2='MT' then VolB:=0;
  if Chn3='MT' then VolC:=0;
  
  { do volume }        
  if Chn1[1]='V' then begin
     if chn1[2]='G' then 
        volA:=16 else tempstr:='$'+chn1[2];
     Val(tempstr, volume, code);
     VolA:=volume;
  end;
  if Chn2[1]='V' then begin
     if chn2[2]='G' then 
        volB:=16 else tempstr:='$'+chn2[2];
     Val(tempstr, volume, code);
     VolB:=volume;
  end;
  if Chn3[1]='V' then begin
     if chn3[2]='G' then 
        volC:=16 else tempstr:='$'+chn3[2];
     Val(tempstr, volume, code);
     VolC:=volume;
  end;
  
  { Finally play the notes } 
  if s0 > -1 then begin 
     sound(8,VolA); sound(0,s0); sound(1,s1); 
  end;                      
  if s2 > -1 then begin 
     sound(9,VolB); sound(2,s2); sound(3,s3); 
  end;
  if s4 > -1 then begin 
     sound(10,VolC); sound(4,s4); sound(5,s5); 
  end;
end;

Procedure InitMusic;
begin
  SongPos:=1; ScorePos:=1; MusTimer:=0;
  sound(8,0); sound(9,0); sound(10,0);
  sound(0,0); sound(1,0);
  sound(2,0); sound(3,0);
  sound(4,0); sound(5,0);             
  sound(6,0); sound(7,32+16+8+4+2+1);
  sound(13,0); sound(11,0); sound(12,0)
end;

Procedure PlayScore;
begin
     
     if ScorePos=1 then begin
        sound(8,VolA);
        Sound(9,VolB);
        Sound(10,VolC);
     end;   
     if MusTimer>Tempo then begin      
        {
        gotoxy(1,18); writeln('Score: ', ScorePos, '  ');
        gotoxy(1,19); writeln('Pos  : ', SongPos, '  '); 
        }
        If (ScorePos>21) or (mScore[ScorePos]=$FF) then begin 
           ScorePos:=1;
        end Else begin
         
           Ch1:=copy(PatternA[mScore[ScorePos]],SongPos,2);
           Ch2:=copy(PatternB[mScore[ScorePos]],SongPos,2);  
           Ch3:=copy(PatternC[mScore[ScorePos]],SongPos,2);  
           play(ch1,ch2,ch3);
           
           SongPos:=SongPos+2;
           if SongPos>Length(PatternA[mScore[ScorePos]])-1 then begin
              SongPos:=1;
              ScorePos:=ScorePos+1;
           end;   
        End;
        MusTimer:=0;
     end;                     
end;
