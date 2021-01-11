
/*-----------------------------------------
by Kang Woo Jung

2014.09.22

-------------------------------------------*/

#include "Melody_Source.h"
#include "Melody.h"

/*
MELODY Format :  VOICE, LENGTH, VOICE, LENGTH ...... VOICE_END

VOICEs List
  MUT,
  C3,C3S,D3,D3S,E3,F3,F3S,G3,G3S,A3,A3S,B3,
  C4,C4S,D4,D4S,E4,F4,F4S,G4,G4S,A4,A4S,B4,
  C5,C5S,D5,D5S,E5,F5,F5S,G5,G5S,A5,A5S,B5,
  C6,C6S,D6,D6S,E6,F6,F6S,G6,G6S,A6,A6S,B6,
  C7,C7S,
  VOICE_END

LENGTHs List
  M32=2,  //32분음표
  M24=3,  //24분음표
  M16=4,  //16분음표
  M12=6,  //12분음표
  M8=8,   //8분음표
  M6=12,  //6분음표
  M4=16,  //4분음표
  M3=24,  //3분음표
  M2=32,  //2분음표
  M0=64   //온음표 
*/

u8 MD_BEEP[]={
  C7,M16,
  VOICE_END
};

u8 MD_BEEP2[]={
  C7,M16,
  C8,M8,
  VOICE_END
};

u8 MD_FAULT[]={
  B6,M16,
  C7,M16,
  B6,M16,
  C7,M16,
  VOICE_END
};

u8 MD_POW_ON[]={
  C5,M16,
  D5,M16,
  E5,M16,
  F5,M16,
  G5,M16,
  A5,M16,
  B5,M16,
  C6,M16,
  VOICE_END
};

u8 MD_POW_OFF[]={
  C6,M16,
  B5,M16,
  A5,M16,
  G5,M16,
  F5,M16,
  E5,M16,
  D5,M16,
  C5,M16,
  VOICE_END
};

u8 MD_TIME_OVER[]={
  C5,M16,
  E5,M16,
  G5,M16,
  C6,M16,
  MUT,M16,
  VOICE_END
};


u8 MD_ON[]={
  C5,M16,
  C6,M16,
  VOICE_END
};


u8 MD_OFF[]={
  C6,M16,
  C5,M16,
  VOICE_END
};


u8 MD_BAT_LOW[]={
  C5,M16,
  C6,M16,
  MUT,M8,
  VOICE_END
};

u8 MD_CHARGE_END[]={
  E5,M8,
  G5,M32,
  E5,M16,
  G5,M16,
  C6,M16,
  MUT,M16,
  VOICE_END
};





u8 PPONG_DANG[]={
  C4,M8,
  D4,M8,
  E4,M8,
  E4,M8,
  C4,M8,
  E4,M8,
  G4,M16,
  A4,M16,
  G4,M8,
  
  C4,M8,
  D4,M8,
  E4,M8,
  E4,M8,
  C4,M8,
  E4,M8,
  G4,M16,
  A4,M16,
  G4,M8,
  
  A4,M4,
  G4,M8,
  E4,M8,
  A4,M4,
  G4,M8,
  E4,M8,

  D4,M8,
  D4,M8,
  C4,M8,
  D4,M8,
  E4,M8,
  G4,M8,
  G4,M4,

  A4,M6,
  A4,M16,
  G4,M8,
  A4,M8,
  C5,M8,
  C5,M8,
  C5,M8,
  MUT,M8,
    
  G4,M6,
  G4,M16,
  E4,M8,
  D4,M8,
  C4,M3,
  MUT,M8,
    
  D4,M6,
  E4,M16,
  C4,M8,
  E4,M8,
  G4,M8,
  G4,M8,
  G4,M4,
    
  D4,M8,
  E4,M16,
  F4,M16,
  E4,M8,
  D4,M8,
  C4,M3,

  VOICE_END
};


