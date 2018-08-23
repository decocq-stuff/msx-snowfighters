function vdp_status_reg(reg:byte):byte;

var waarde:byte;

begin
      inline($3a/reg/                { LD A,(REG)          }
             $fd/$2a/$f7/$fa/        { LD IY,(&HFAF7)      }
             $dd/$21/$31/$01/        { LD IX,&H0131        }
             $cd/$1c/$00/            { CALL &H001C         }
             $32/waarde/             { LD (WAARDE),A       }
             $fb);                   { EI                  }
      vdp_status_reg:=waarde
end;

procedure set_scroll(x:integer;y,stil,aantal,scr:byte);
var reg25,reg26,reg27:byte;

  procedure wrtvdp(reg,data:byte);
    begin
    inline($3a/reg/                { LD A,(REG)          }
           $4f/                    { LD C,A              }
           $3a/data/               { LD A,(DATA)         }
           $47/                    { LD B,A              }
           $fd/$2a/$f7/$fa/        { LD IY,(&HFAF7)      }
           $dd/$21/$2d/$01/        { LD IX,&H012D        }
           $cd/$1c/$00/            { CALL &H001C         }
           $fb)                    { EI                  }
  end;

begin
    if scr=12 then reg25:=8
              else if scr in [10..11] then reg25:=24
              else reg25:=0;
    reg25:=reg25 or (2*stil);
    reg25:=reg25 or aantal;
    reg26:=(x and $fff8) shr 3;
    reg27:=7-(x and $0007);
    wrtvdp(23,y);
    wrtvdp(25,reg25);
    wrtvdp(27,reg27);
    wrtvdp(26,reg26);
end;

procedure line_b(x,y,x2,y2:integer;kleur,log_op:byte);
begin
    mem[$fcb3]:=lo(x2);mem[$fcb4]:=hi(x2);
    mem[$fcb5]:=lo(y2);mem[$fcb6]:=hi(y2);
    mem[$f3f2]:=kleur;
    mem[$fb02]:=log_op;
    inline($ed/$4b/x/              { LD BC,(X)           }
           $ed/$5b/y/              { LD DE,(Y)           }
           $fd/$2a/$f7/$fa/        { LD IY,(&HFAF7)      }
           $dd/$21/$c9/$00/        { LD IX,&H00C9        }
           $cd/$1c/$00/            { CALL &H001C         }
           $fb)                    { EI                  }
end;


procedure line_bf(x,y,x2,y2:integer;kleur,log_op:byte);
begin
    mem[$fcb3]:=lo(x2);mem[$fcb4]:=hi(x2);
    mem[$fcb5]:=lo(y2);mem[$fcb6]:=hi(x2);
    mem[$f3f2]:=kleur;
    mem[$fb02]:=log_op;
    inline($ed/$4b/x/              { LD BC,(X)           }
           $ed/$5b/y/              { LD DE,(Y)           }
           $fd/$2a/$f7/$fa/        { LD IY,(&HFAF7)      }
           $dd/$21/$cd/$00/        { LD IX,&H00CD        }
           $cd/$1c/$00/            { CALL &H001C         }
           $fb)                    { EI                  }
end;

procedure scr_off;
begin
    inline($fd/$2a/$c0/$fc/        { LD IY,(&HFCC0)      }
           $dd/$21/$41/$00/        { LD IX,&H0041        }
           $cd/$1c/$00/            { CALL &H001C         }
           $fb)                    { EI                  }
end;


procedure scr_on;
begin
    inline($fd/$2a/$c0/$fc/        { LD IY,(&HFCC0)      }
           $dd/$21/$44/$00/        { LD IX,&H0044        }
           $cd/$1c/$00/            { CALL &H001C         }
           $fb)                    { EI                  }
end;

procedure switch_to_Z80;
begin
    inline($3e/$00/              { LD A,0           }
           $fd/$2a/$c0/$fc/      { LD IY,(&HFCC0)   }
           $dd/$21/$80/$01/      { LD IX,&H0180     }
           $cd/$1c/$00/          { CALL &H001C      }
           $fb)                  { EI               }
end;

procedure switch_to_R800;
begin
    inline($3e/$00/              { LD A,1           }
           $fd/$2a/$c0/$fc/      { LD IY,(&HFCC0)   }
           $dd/$21/$80/$01/      { LD IX,&H0180     }
           $cd/$1c/$00/          { CALL &H001C      }
           $fb)                  { EI               }
end;

function processor_type:byte;
var soort:byte;

begin
    inline($fd/$2a/$c0/$fc/      { LD IY,(&HFCC0)   }
           $dd/$21/$83/$01/      { LD IX,&H0183     }
           $cd/$1c/$00/          { CALL &H001C      }
           $32/soort/            { LD (SOORT),A     }
           $fb);                 { EI               }
    processor_type:=soort
end;

function msx_version:byte;
var versie:byte;

begin
     inline($3a/$c1/$fc/          { LD A,(&HFCC1) ; slot da ROM BIOS }
            $21/$2d/$00/          { LD HL,&H002D     }
            $cd/$0c/$00/          { CALL &H000C      }
            $32/versie);          { LD (VERSIE),A    }

    msx_version:=versie+1
end;

function ctrl_stop_pressed:boolean;
var ctrl,stop:boolean;

begin
    ctrl:=((mem[$fbeb] and 2)=0);
    stop:=((mem[$fbec] and 16)=0);
    ctrl_stop_pressed:=ctrl and stop
end;

function stop_pressed:boolean;
begin
    stop_pressed:=((mem[$fbec] and 16)=0);
end;

function pressed_function_key:byte;
var nummer:byte;

begin
    nummer:=0;
    if (mem[$fbeb] and 32)=0 then nummer:=1;
    if (mem[$fbeb] and 64)=0 then nummer:=2;
    if (mem[$fbeb] and 128)=0 then nummer:=3;
    if (mem[$fbec] and 1)=0 then nummer:=4;
    if (mem[$fbec] and 2)=0 then nummer:=5;
    if ((mem[$fbeb] and 1)=0) and (nummer<>0) then nummer:=nummer+5;
    pressed_function_key:=nummer
end;

procedure set_adjust(horizontaal,verticaal:integer);
var reg,horiz,verti:byte;

begin
    if horizontaal<=0
      then
        horiz:=-horizontaal
      else
        horiz:=15-horizontaal;
    if verticaal<=0
      then
        verti:=15+verticaal
      else
        verti:=verticaal;
    reg:=16*verti+horiz;
    inline($3a/reg/              { LD A,(REG)       }
           $47/                  { LD B,A           }
           $0e/$12/              { LD C,&H12        }
           $dd/$21/$2d/$01/      { LD IX,&H012D     }
           $fd/$2a/$f7/$fa/      { LD IY,(&HFAF7)   }
           $cd/$1c/$00/          { CALL &H001C      }
           $fb)                  { EI               }
end;

procedure Pause(time:integer);
var filler:byte;
     timer:integer;
begin  
    for timer:=0 to time do begin
        filler:=0;
    end;
end;

