{
  Snow Fighters
  ==============================================
  (c) 2002 MSX Files
  Programmed by SLotman
  Enjoy this x-mas demo! =)
  Version 2.0 By Sander de Cocq 2018
  From Demo to Game: it's ON
}
{$C-}


{$i misc.inc}
{$i pal.inc }
{$i MsxDskIO.inc}
{$i msx.inc }
{$i sprite.inc}


Const AvatarsXoffset: array[0..7] of byte=(0,64,128,192,0,64,128,192);
      AvatarsYOffset: array[0..7] of byte=(176,176,176,176,144,144,144,144);
      AvatarsHeadroom: array[0..7] of byte=(1,6,6,6,6,6,10,32);
      PlayerYOffset: integer=120;
      maxhealth: byte=4;
      HighestLevel:byte=25;
      CPUStartSpeed:byte=1 ;
      PlayerStartSpeed:byte=3;
      CPUinitialAvatar:byte=6;
      MinimumBallSpeed: byte=4;
      IntroTimeOut:integer=156;
      DemoLength:integer=1000;
      const Page_Title:byte=2;
      const Page_Game:byte=3;

{ Global variables }
var PlayerX, PlayerY: integer;
  Black, Red, LightBlue, White, Yellow, LightGrey, Green: byte;
  PrevPlayerX, PrevPlayerY: array[0..1] of integer;
  PrevCPUX, PrevCPUY: array [0..1] of integer;
  PlayerChar, PlayerAnim: byte;  
  PlayerBalls: byte;
  PlayerDirection:byte;
 { Speed: byte;}
  PlayerBallSpeed, CPUBallSpeed: byte;
  PlayerSpeed: byte;{integer?}
  CPUSpeed: byte;
  Level,NewLevel: byte;{integer?}
  Score,HighScore: integer;{integer?}
  PlayerHealth: byte;
  CPUHealth:byte;
  healthblock:byte;
  CPUX, CPUY: integer;
  CPUChar, CPUAnim: byte;
  CPUMoveX, CPUMoveY: integer;
  CPUBallX, CPUBallY: integer;
  CPUBalls:byte;{integer?}
  tr:byte;
  NewCPUWait:byte;
  IntroText: TPstring;
  sLevel:String[3];
  sScore:String[8];
  DemoCounter:integer;
  DemoPlayerSteps:byte;
  PlayerBallX, PlayerBallY: integer;
  ExitTitle, GameOver, CheatMode, DemoMode: boolean;
  S,i,j: byte;                
  T: boolean;
  PlayerMoved, CPUMoved: boolean;
  Acertou, ResetPlayers: boolean;
  fr:integer;               
  Clique:byte;
  c:char;
  GameBitmapLoaded, TitleBitmapLoaded:boolean;
  WPage,VPage: byte; { Working and visual pages }
    
                         
{ Sprite graphic and colors }                         
Const LineColor : SpriteColorArrayType=(7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7);
      BallColor : SpriteColorArrayType=(15,15,15,15,11,11,11,2,2,2,2,2,2,6,6,6);
      Ball      : Sprite8Type=($00,$3c,$7e,$7e,$7e,$7e,$3c,$00);
      BallLine  : Sprite8Type=($3c,$42,$81,$81,$81,$81,$42,$3c);




procedure setgamepal; 
{ set the game palette }
begin
SetRGB(00,7,0,7);
SetRGB(01,0,1,3); {dark blue}
SetRGB(02,2,2,2);{olive drab}
SetRGB(03,5,4,4);{skin dark}
SetRGB(04,0,0,5); {blue}
SetRGB(05,0,4,0); Green:=5 ; {green}
SetRGB(06,7,2,2); red:=6 ; {red}
SetRGB(07,0,0,0); black:=7 ;{black}
SetRGB(08,7,5,0); yellow:=8 ; {now orange, khaki 4,4,0}
SetRGB(09,5,5,5); {medium grey}
SetRGB(10,3,6,3); {greenish grey (trees)}
SetRGB(11,6,6,6); LightGrey:=11; {light grey}
SetRGB(12,0,4,7); LightBlue:= 12; {sky blue}
SetRGB(13,6,5,5); {skin light}
SetRGB(14,6,6,5); {yellowish}
SetRGB(15,7,7,7); White:=15 ;{white}
end;                         

procedure settitlepal;  
{ set the title palette }
{begin
SetRGB(0 , 7,7,7);
SetRGB(4 , 0,7,7);
SetRGB(2 , 7,0,7);
SetRGB(3, 0,0,7);
SetRGB(1, 7,7,0); yellow :=1;
SetRGB(5, 0,7,0);
SetRGB(9, 7,0,0); red:=9 ;
SetRGB(7 , 0,0,0); black:=7 ;
SetRGB(8 , 7,7,7); White:=8; 
SetRGB(6 , 0,0,7);
SetRGB(10 , 5,5,5);
SetRGB(11 , 2,2,2);
SetRGB(14 , 4,3,0);
SetRGB (13, 6,6,6); LightGrey:= 13;
SetRGB(15 , 6,4,0);
SetRGB(12 , 0,4,7); LightBlue:=12;
end;}begin
SetRGB(0 , 7,7,7);
SetRGB(4 , 0,7,7);
SetRGB(2 , 2,2,2);
SetRGB(3, 0,0,7);
SetRGB(1, 7,7,0); yellow :=1;
SetRGB(5, 0,7,0);
SetRGB(9, 7,0,0); red:=9 ;
SetRGB(7 , 0,0,0); black:=7 ;
SetRGB(8 , 6,4,0);  
SetRGB(6 , 7,2,2);
SetRGB(10 , 5,5,5);
SetRGB(11 , 6,6,6); LightGrey:= 11;
SetRGB(14 , 4,3,0);
SetRGB (13, 6,6,6); 
SetRGB(12 , 0,4,7); LightBlue:=12;
SetRGB(15,7,7,7); White:=15 ;
end;

function Collision(x1,y1,w,h,x2,y2,w2,h2:byte):byte;
{ detect the collision between two rectangles }          
var hit: byte;
begin         
  {
  Collision:=false;
  if (x1<x2+w2) and (x1+w>x2) and (y1<y2+h2) and (y1+h>y2) then Collision:=true;
  }
  INLINE (
  $F3/$3E/$00/$32/hit/$3A/x2/$47/$3A/w2/$80/$47/$3A/x1 /$B8/$F2/*+$0031/$47 /$3A/w/$80
  /$47/$3A/x2/$B8/$F2/*+$0024/$3A/y2/$47/$3A/h2/$80/$47/$3A/y1/$B8/$F2/*+$0014/$47
  /$3A/h/$80/$47/$3A/y2/$B8/$F2/*+$0007/$3E/$01/$32/hit/$FB);
  collision:=hit;
end;

Procedure DrawChar(pchar,anim,PosX,PosY:byte);
{ Draw player or enemy char }
  Var CharY,CharX: integer;
  begin
    if pchar=8 then begin
      if anim=3 then begin
        copyL_xy(192,PlayerYOffset,Page_Game,PosX,PosY,WPage,16,24,tpset,0);
      end else begin
        copyL_xy((48*playerdirection)+(anim*16),PlayerYOffset,Page_Game,PosX,PosY,WPage,16,24,tpset,0);
      end;
    end else begin
      CopyL_xy(AvatarsXoffset[pchar]+(anim*16),AvatarsYOffset[pchar],Page_Game,PosX,PosY,WPage,16,32,tpset,0);
    end;
  end;                  

Procedure Flip;              
{ show the current working page on screen and make the old visual page the current working one }
  var TempPage: byte;
  begin              
    TempPage:=VPage;
    VPage:=WPage;
    WPage:=TempPage;
    setpage(VPage,WPage);
  end;

procedure UpdatePlayers;
begin
 
  line_bf(PrevPlayerX[wpage],prevplayerY[wpage],PrevPlayerX[wpage]+16,prevplayerY[wpage]+24,White,0);
  DrawChar(PlayerChar,PlayerAnim,PlayerX,PlayerY);
  PrevPlayerX[wpage]:=PlayerX;
  prevplayerY[wpage]:=PlayerY;
  line_bf(prevCPUX[wpage],prevCPUY[wpage],PrevCPUX[wpage]+16,prevCPUY[wpage]+32,White,0);
  DrawChar(CPUChar,CPUAnim,CPUX,CPUY);
  prevCPUX[wpage]:=cpux;
  prevCPUY[wpage]:=cpuY;
end;

Procedure UpdateCPUHealthBar;
begin
  line_bf(202,9,202+((maxhealth-cpuhealth)*healthblock),14,Black,0);
  if CPUHealth>1 then begin
    line_bf(202+((maxhealth-cpuhealth)*healthblock),9,253,14,Green,0);
  end else begin
    line_bf(202+((maxhealth-cpuhealth)*healthblock),9,253,14,Red,0);
  end;
  Copy_xy(202,9,wpage,202,9,vpage,253,14,0); 
end;

Procedure UpdatePlayerHealthBar;
begin
  line_bf(54,9,54-((maxhealth-playerhealth)*(healthblock)),14,7,0);
  if playerhealth>1 then begin
    line_bf(5,9,5+(playerhealth*healthblock),14,Green,0);
  end else begin
    line_bf(5,9,5+(playerhealth*healthblock),14,Red,0);
  end;
  Copy_xy(5,9,wpage,5,9,vpage,54,14,0); 
end;

Procedure UpdateLevel;
begin
  setpage(vpage,wpage);
  str(level,sscore);
  color (White,LightBlue,Black);
    
    graphicprint40('Level:' + sscore,29,2,8,false,0);
  color(Black,White,Black);
  copy (174,24,255,31,wpage,174,24,vpage)
end;

Procedure UpdateScore;
begin
  setpage(vpage,wpage);
  str(score,sscore);
  color (White,LightBlue,Black);
  graphicprint40('Score:' + sscore +'0',29,3,8,false,0);
  color(Black,White,Black);
  copy (174,32,255,40,wpage,174,32,vpage);
end;

Function HighScoreString:TPstring;
var hs:string[8];
begin
   str(score,hs);
   HighScoreString:=hs+'0';
end;

Procedure DrawSnowballs;
begin
       putsprite(0,0,PlayerBallX,PlayerBallY);
       putsprite(1,1,PlayerBallX,PlayerBallY);
       putsprite(2,0,CPUBallX,CPUBallY);
       putsprite(3,1,CPUBallX,CPUBallY);
       { Make sure the sprites will appear on page 1 }
       { thanks for the tip, Fudeba!                 }
       if (WPage=1) then Copy_y (0,212,0,212,43,1,0);
end;

procedure SetLevelParameters(newLevel:byte);
begin
  CPUBallSpeed:=MinimumBallSpeed+( newlevel div 2);
  if cpuballSpeed>20 then cpuballSpeed:=20;
  {playerhealth:=4;}
  cpuhealth:=maxhealth;
  CPUMoveY:=cpustartspeed+(newlevel div 3);
  if random(100)>50 then CPUMoveY:=-CPUMoveY;
  updatelevel;updatescore;
  cpuchar:=CPUinitialAvatar-(level mod 7);
  CPUAnim:=0;
  randomize;
  resetPlayers:=true;
end;

procedure FadeScreen(FadeColor:byte);
var i:byte;
begin
  for i:=0 to 110 do begin
    line_b(i,i,256-i,211-i,FadeColor,0);
  end;
end;

procedure FadeScreenInvert(FadeColor,Slowness:byte);
var i,j:byte;
begin
  for j:=0 to 110 do begin
    i:=110-j;
    line_b(i,i,256-i,211-i,FadeColor,0);
    pause(Slowness);
  end;
end;

procedure EndGameScreen;
var i:integer;
    s,j, topX,topY,bottomX,bottomY: byte;
begin
  topX:=30;topY:=90;bottomX:=220;bottomY:=130;
  for i:= 0 to ((bottomY-topY)  div 2) do begin
    line_b(50-i,109-i,200+i,109+i,15,0);
  end;
  line_B(topX+1,topY+1,bottomX-1,bottomY-1,6,0);
  str(score,sscore);
  str(level,slevel);
  graphicprint40('Game Over!',17,11,15,false,0);
  graphicprint40('Score:' + sscore + '0     Level:'+slevel,10,13,15,false,0);
  i:=0;
  pause(300);
  if keypressed then repeat
    c:=inkey;
  until (keypressed=false);
  repeat
    repeat 
      s:=random(256);
      j:=random(211);
    until (s>topX) and (s<bottomX) and (j>topY) and (j<bottomY)=false;
    i:=succ(i);
    line_b(s,j,s,j,15,0);
  until (i=2000) or (keypressed);
  if keypressed then c:=inkey;
  fadescreen(Black);
 
end;
    
begin  {Main|}     
  introtext := '                              ';                        
  IntroText := introtext + 'SLotman/msX-Files and SdC wish you happy holidays! ';
  introtext := introtext + 'Snow Fighters Version 1.1 BETA Turbo Pascal Demo. ';
  introtext := introtext + 'Visit http://www.msx.org';
  introtext := introtext + '                                 ';
  { Detect R-800 mode and force Z80 mode if finds it }
  if msx_version>3 then switch_to_Z80;
    {remove the keyboard click }
  if ClickSW>0 then begin Clique:=ClickSW; ClickSW:=0; end;
  { Start screen 5 }
  screen(5);        
  { Set sprites }
  enablesprites; spritesize8; ClearAllSprites;
  setspritepattern(0, ball); 
  setspritepattern(1, ballLine); 

  { Ball colors } 
  setspritecolors(0,ballcolor); 
  setspritecolors(1,linecolor);
  setspritecolors(2,ballcolor); 
  setspritecolors(3,linecolor);
  
  ExitTitle:=false;
  GameBitmapLoaded:=false;
  TitleBitmapLoaded:=false;
  HighScore:=0;
  { Title screen loop }

  repeat            
     GameOver:=false;
     cheatmode:=false;
     DemoMode:=false; DemoCounter:=0;
     
     
     
    { Load title and sel.screen graphics }
     if TitleBitmapLoaded=false then begin
         
         LoadGraphic('snowf2-1.gfx',256,212,0,0,Page_Title,0,1,5);
         TitleBitmapLoaded:=true;
    end;
    setpage(0,0);
     { remove sprites from screen }
     PlayerBallX:=-32;PlayerBallY:=-32;
     CPUBallX:=-32;CPUBallY:=-32;
    drawsnowballs;
     settitlepal;
     color(White,Black,black);
    FadeScreen(black);
     
   { copy(0,0,255,211,Page_Title,0,0,0);}
    for tr:=1 to 106 do begin
      copy(106-tr,106-tr,149+tr,106-tr,Page_Title,106-tr,106-tr,0);
      copy(106-tr,106-tr,107-tr,105+tr,Page_Title,106-tr,106-tr,0);
      copy(106-tr,105+tr,149+tr,105+tr,Page_Title,106-tr,105+tr,0);
      copy(148+tr,106-tr,149+tr,105+tr,Page_Title,148+tr,106-tr,0);
      
    end;

    
     {for tr:=0 to 15 do begin
         line_bf((tr*16),20,13+(tr*16),30,tr,0);
        end;}
    
     
     
   
    color(White,LightBlue,White);
    graphicprint40('  ...Loading game...',20,0,8,false,0);
    s:=1;
    { Load graphic bkg to page 3 }
    if GameBitmapLoaded=false then begin
      LoadGraphic('snowf2-2.gfx',256,212,0,0,Page_Game,0,0,5);
      GameBitmapLoaded:=true;
    end;
    { prevent accidental keypress detection from previous}
    if keypressed then repeat 
      c:=inkey;
    until (keypressed=false);
    { wait for a keypress... }
    repeat
      graphicprint40(copy(introtext,s,20),20,0,8,false,0);
      s:=succ(s);
      if (s>(length(introtext)-20)) then s:=1;
      DemoCounter:=succ(DemoCounter);
    until keypressed or (DemoCounter=IntroTimeOut); 
    { if ESC pressed, get out of the game }
    if keypressed then begin 
      c:=inkey;
        if c=chr(27) then begin
           GameOver:=true;
           ExitTitle:=true;
        end;
        if c=chr(188) then CheatMode:=True;   {CODE+c enables cheat mode}
        if c='d' then begin
          DemoMode:=True;
          DemoCounter:=0;
        end; 
       
    end;   
     if DemoCounter=IntroTimeOut then begin
          DemoMode:=True;
          DemoCounter:=0;
        end;

    PlayerChar:=8;
    
    WPage:=1; VPage:=0;
    randomize;
    { Set -game variables }
    GameOver:=false;
    CPUHealth:=maxhealth;
    PlayerHealth:=maxhealth;
    if DemoMode=true then begin
      NewLevel:=random(7)+4;
      playerSpeed:=random(2)+3;
      PlayerBallSpeed:=MinimumBallSpeed+random(4);
      S:=random(3)+1;
    end else begin
      NewLevel:=1;
      PlayerSpeed:=3;
      PlayerBallSpeed:=MinimumBallSpeed;
    end;
    Level:=0;
    Score:=0;
    
    healthblock:=12;
    PlayerX:=10; PlayerY:=150;PlayerDirection:=0;         
    PrevPlayerX[0]:=10;PrevPlayerX[1]:=10;
    PrevPlayerY[0]:=150;prevplayerY[1]:=150;
    PlayerAnim:=0;
    PlayerBalls:=1;
    PlayerBallX:=-32; PlayerBallY:=-32;
    CPUX:=229; CPUY:=150;
    PrevCPUX[0]:=229;PrevCPUX[1]:=229;
    prevCPUY[0]:=150;prevCPUY[1]:=150;
    Acertou:=false;
    
    CPUBalls:=1;
    CPUBallX:=-32; CPUBallY:=-32;
    { set the game palette }
   
    color(Black,White,Black);
    FadeScreen(black);
    setgamepal;
    { Draw everything once }
    setpage(VPage,WPage);
    Copy_xy(0,0,Page_Game,0,0,wpage,256,119,0); {Copy from snowf2-2.gfx}
    line_bf(0,119,255,211,White,0);

    
    
    line_b(4,8,55,15,4,0); {box around health bar L}
    {line_bf(5,9,54,14,5,0)};
    line_b(201,8,254,15,4,0); {box around health bar R}
    {line_bf(202,9,253,14,5,0)}
    if cheatmode=true then begin
      color(6,15,7);
      graphicprint40(' CHEATING!',2,2,Red,false,0);
    end;
     if DemoMode=true then begin
      color(6,15,7);
      graphicprint40(' *** DEMO ***',2,2,Red,false,0);
    end;
    {Completed drawing; copy to visual page}
    {
      Copy_xy(0,0,wpage,0,0,vpage,256,212,0); }
     
     
    for tr:=1 to 106 do begin
      copy(106-tr,106-tr,149+tr,106-tr,WPage,106-tr,106-tr,VPage);
      copy(106-tr,106-tr,107-tr,105+tr,WPage,106-tr,106-tr,VPage);
      copy(106-tr,105+tr,149+tr,105+tr,WPage,106-tr,105+tr,VPage);
      copy(148+tr,106-tr,149+tr,105+tr,WPage,148+tr,106-tr,VPage);
      
    end;
        
    { Draw Chars on working page }
      
    updateplayers;updatelevel;updatescore;
    { swap working page with visual page }
    Flip; 
    
    { start main game loop }
    while not GameOver do begin 
      if newlevel<>level then begin
        level:=newlevel;
        SetLevelParameters(level);
        UpdateCPUHealthBar;
        UpdatePlayerHealthBar;
      end;
      PlayerMoved:=false; CPUMoved:=false;
      {}
      { read the cursor/ joystick }
      if DemoMode=true then begin
         democounter:=succ(DemoCounter);
         if democounter=DemoLength then gameover:=true;
         if (playerX<8) or (playerY>180) or (playerX>190) or (playerY<100) or (DemoPlayerSteps=0) then begin
           S:=random(12);
           if s>8 then S:=0;
           DemoPlayerSteps:=random(25);
           if s=0 then DemoPlayerSteps:=DemoPlayerSteps div 2;
         end;
         T:=false;
         DemoPlayerSteps:=pred(DemoPlayerSteps);
        if random(20)=5 then begin
           T:=true;
        end;
      end else begin
        S:=stick(0) or stick(1);
        T:=(strig(0) or strig(1));
      end;
        if S<>0 then begin
          { Ok, player moved! } 
          if ((s<3) or (s=8)) then begin 
              PlayerY:=PlayerY-playerSpeed;
              if playerY<110 then playerY:=110;
              playerdirection:=3;
          end;
          if ((s>3) and (s<7)) then begin 
            PlayerY:=PlayerY+playerSpeed;
            if playerY>188 then playerY:=188;
            PlayerDirection:=0;
          end;
          if ((s>1) and (s<5)) then begin
            PlayerX:=PlayerX+playerSpeed;
            if playerX>200 then playerX:=200;
            PlayerDirection:=2;
          end;
          if (s>5) then begin
            PlayerX:=PlayerX-playerSpeed;
            if playerX<1 then playerX:=1;
            PlayerDirection:=1;
          end;
          PlayerMoved:=true;
          PlayerAnim:=succ(PlayerAnim);
          if PlayerAnim>2 then PlayerAnim:=0;
        end;
                         
      { Read triggers }               
      
      if (T) and (PlayerBalls>0) then begin
        PlayerAnim:=2;
        PlayerBallX:=PlayerX;
        PlayerBallY:=PlayerY+10;
        PlayerBalls:=0;
        PlayerDirection:=2;
        PlayerMoved:=true;   
       end;              
       if PlayerBalls=0 then begin
          PlayerBallX:=PlayerBallX+PlayerBallSpeed;
          if PlayerBallX>255 then begin
             PlayerBalls:=1;
             PlayerBallX:=-32;
             PlayerBallY:=-32;
          end else begin
             if Collision(PlayerBallX,PlayerBallY,8,8,CPUX,CPUY+AvatarsHeadroom[CPUChar],16,32)=1 then begin
                PlayerBallX:=-32; PlayerBallY:=-32;
                {CPUBallX:=-32; CPUBallY:=-32;}
                Score:=Score+10;
                CPUHealth:=pred(cpuhealth);
                Acertou:=true;
                UpdateCPUHealthBar;
                CPUAnim:=3;       
                PlayerBalls:=1;
                CPUMoved:=true;
                drawsnowballs;
                if cpuhealth=0 then begin  {level up}
                  CPUBallX:=-32;CPUBallY:=-32;
                  newlevel:=succ(newlevel);
               
                 { resetplayers:=true;}
                  for i:= 0 to 20 do begin
                    flip;
                    DrawSnowballs;
                    graphicprint40('Level Up!',18,13,7,false,0);
                    UpdatePlayers;
                    color(6,15,7);
                    flip;
                    DrawSnowballs;
                    color(7,15,7);
                    graphicprint40('Level Up!',18,13,7,false,0);
                    updateplayers;
                  end;
                  UpdateCPUHealthBar; 
                  line_bf (107,108,163,120,15,0);
                  copy(107,108,163,120,wpage,107,108,vpage);
                  
                 { updatelevel;}
                  score:=score+100;
                  updatelevel;
                end;
                updatescore;
              end;   
            end;
         end;
                       
         CPUMoved:=true;
         if not acertou then begin
           if CPUBalls>0 then begin
             if random(30)<3 then begin
               { CPU shoots! }
               CPUBalls:=0;
               CPUAnim:=2;             
               CPUBallX:=CPUX;
               CPUBallY:=CPUY+18;
             end;
           end else begin
             { CPU walks }
             CPUAnim:=succ(CPUAnim);
             if CPUAnim>2 then CPUAnim:=0;                                  
             CPUY:=CPUY+CPUMoveY;
             if (CPUY>179) or (CPUY<114) or (random(highestlevel*8)<level) then CPUMoveY:=-CPUMoveY;
             if CPUBalls=0 then begin
               CPUBallX:=CPUBallX-cpuballSpeed;
               if CPUBallX<0 then begin
                 CPUBallX:=-32;
                 CPUBallY:=-32;
                 CPUBalls:=1;
                 score:=score+5;
                 updatescore;
               end else begin
                 if Collision(CPUBallX,CPUBallY,8,8,PlayerX,PlayerY,16,24)=1 then begin
                   {PlayerBallX:=-32; PlayerBallY:=-32;}
                   CPUBallX:=-32; CPUBallY:=-32;
                   CPUBalls:=1;
                   PlayerAnim:=3;
                   Acertou:=true;
                   ResetPlayers:=false;
                   PlayerHealth:=pred(PlayerHealth);
                   if PlayerHealth > 0 then begin
                     UpdatePlayerHealthBar;
                   end else begin
                     {game over}
                     if cheatmode=false then begin
                        GameOver:=true;
                        ResetPlayers:=true;
                      end else begin 
                        playerhealth:=maxhealth;
                      end;
               end;
               PlayerMoved:=true;
               {PlayerBalls:=0;}
               for i:=0 to 100 do begin
                 color (7,15,6);pause(5);
                 color (7,15,7);pause(5);
               end;
          
             end;
           end;   
         end else  begin 
           {NewCPUWait:=succ(newCPUWait)}  
         end;      
       end;                    
    END;
       {Balls collide: nah, that is statistically improbable IRL,  just ignore; 
       If (CPUBalls=0) and (PlayerBalls=0) then begin
         if Collision(PlayerBallX,PlayerBallY,8,8,CPUBallX,CPUBallY,8,8)=1 then begin
           PlayerBallX:=-32; PlayerBallY:=-32;
           CPUBallX:=-32; CPUBallY:=-32;
           PlayerBalls:=1;
           CPUBalls:=1;          
         end;                            
       end;}
       if acertou then  Acertou:=false;
            
        if keypressed then begin 
          c:=inkey;
          if (c=chr(27)) or (DemoMode=true) then GameOver:=true;
          if CheatMode=true then begin
            if c='l' then begin
              cpuhealth:=0;
              newlevel:=succ(newLevel);
            end;
            if c='i' then begin
              {draw palette hint}
              for tr:=0 to 15 do begin
                 line_bf((tr*16),64,13+(tr*16),78,tr,0);
              end;
              copy_xy(0,64,wpage,0,64,vpage,256,78,0);
            end;
          end;
          if (c='p') then begin
            repeat until keypressed;
         end
       end;
      if ResetPlayers then begin
         PlayerX:=10; PlayerY:=150;          
         PlayerAnim:=0;
         PlayerBalls:=1;
         PlayerBallX:=-32; PlayerBallY:=-32;
         CPUX:=229; CPUY:=150;
         CPUMoveY:=random(100);
         CPUBalls:=1;
         CPUBallX:=-32; CPUBallY:=-32;
         if CPUMoveY>50 then CPUMoveY:=4 else CPUMoveY:=-4;
         ResetPlayers:=false;
      end;   
     
      
       if PlayerMoved or CPUMoved then UpdatePlayers;
       
       flip;
       DrawSnowballs;

       
     end; { GameOver; }     
        
     setpage (vpage,vpage);
     
     
     if demomode=false then begin
       fadescreen(black);
       if score>HighScore then begin
         HighScore:=score;

       end;
       IntroText:= '                    High Score:'+Highscorestring;
       introtext:= introtext + '  Thank you for playing.                   ';
       {end game animation}
       
       
       EndGameScreen;
     end;
   until ExitTitle;

  { reset palette and go back to screen 0 }
  color(15,4,4); inipalette; screen(0); 
  { put the keyboard click as it was before the }
  { game started                                }
  ClickSW:=Clique;
end.
