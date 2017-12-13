unsigned D0=22;
unsigned D1=23;
unsigned D2=24;
unsigned D3=25;
unsigned D4=26;
unsigned D5=27;
unsigned D6=28;
unsigned D7=29;
unsigned AD0=30;
unsigned AD1=31;
unsigned AD2=32;
unsigned AD3=33;
unsigned AD4=34;
unsigned AD5=35;
unsigned AD6=36;
unsigned AD7=37;
unsigned AD8=38;
unsigned AD9=39;
unsigned AD10=40;
unsigned AD11=41;
unsigned AD12=42;
unsigned AD13=43;
unsigned AD14=44;
unsigned AD15=45;
unsigned WR=46;
unsigned RD=47;
unsigned BUSRQ=48;
unsigned BUSACK=49;
unsigned CLK=50;
unsigned RESET=51;
unsigned MREQ=52;
unsigned IORQ=53;
unsigned HALT=A13;
unsigned RFSH=A14;
unsigned M1=A15;
unsigned DATA[]={D0,D1,D2,D3,D4,D5,D6,D7};
unsigned ADDR[]={AD0,AD1,AD2,AD3,AD4,AD5,AD6,AD7,AD8,AD9,AD10,AD11,AD12,AD13,AD14,AD15};
unsigned long clocks = 0;
#define memsize 0x1000
static unsigned char mem[memsize] = {
0x21, 0x47, 0x00,       // 0000  ld  hl, work
0x3e, 0x53,             // 0003  ld a, 'S'
0x77,               // 0005  ld (hl), a
0x23,               // 0006  inc  hl
0x3e, 0x45-1+2,             // 0007  ld a, 'E'
0x77,               // 0009  ld (hl), a
0x23,               // 000a  inc  hl
0x3e, 0x43,             // 000b  ld a, 'C'
0x77,               // 000d  ld (hl), a
0x23,               // 000e  inc  hl
0x77,               // 000f  ld (hl), a
0x23,               // 0010  inc  hl
0xc6, 0x0c,             // 0011  add  a, 'O'-'C'
0x77,               // 0013  ld (hl)>, a
0x23,               // 0014  inc  hl
0xc6, 0xfe,             // 0015  add  a, 'N'-'O'
0x77,               // 0017  ld (hl), a
0x23,                   // 0018  inc  hl
0x3e, 0x7b,             // 0019  ld a, '{'
0x77,               // 001b  ld (hl), a
0x23,               // 001c  inc  hl
                        // loop1: 
0x3a, 0x44, 0x00,       // 001d  ld a,(seed1)
0x47,               // 0020  ld b,a
0xc6, 0x45,             // 0021  add  a,(seed2)
0x32, 0x44, 0x00,       // 0023  ld (seed1),a
0x78,               // 0026  ld a,b
0x32, 0x45, 0x00,       // 0027  ld (seed2),a
0xfe, 0x21,           // 002a  cp 021h
                        
0xfa, 0x1d, 0x00,       // 002c  jp m, loop1
0xfe, 0x7b,           // 002f  cp 07bh
0xf2, 0x1d, 0x00,       // 0031  jp p, loop1
                
0x77,               // 0034  ld (hl), a
0x23,               // 0035  inc  hl
                
0x3a, 0x46, 0x00,       // 0036  ld a,(count)
0x3d,               // 0039  dec  a
0x32, 0x46, 0x00,       // 003a  ld (count),a
0xc2, 0x1d, 0x00,       // 003d  jp nz, loop1
                
0x3e, 0x7e,             // 0040  ld a, '}'
0x77,       // 0042  ld (hl),a
0x76,               // 0043  halt
                        
0x03,                   // 0044  seed1: defb  3
0x0b,                   // 0045  seed2: defb  11
0x0a,       // 0046  count:  defb  10

#if 0
  0x21, 0x47, 0x00,
  
  0x3a, 0x27, 0x00,  
  0x47,
  0xc6, 0x28,
  0x32, 0x27, 0x00,
  0x78,
  0x32, 0x28, 0x00,
  0xfe, 0x21,
  0xfa, 0x03, 0x00,
  0xfe, 0x7b,
  0xf2, 0x03, 0x00,

  0x77,
  0x23,
  
  0x3a, 0x29, 0x00,
  0x3d,
  0x32, 0x29, 0x00,
  0xc2, 0x03, 0x00,
  0x76,
  
  0x03,    
  0x0b,      
  0x0a,

  0x3e, 0x77,
  0x32, 0x10, 0x00,
  0x3e, 0x88,
  0x32, 0x11, 0x00,
  0x3e, 0xcc,
  0x32, 0x12, 0x00,
  0x76,
  0x03,
  0x05,
  0x07,
  /*
  0x3a, 0x19, 0x00,
  0x06, 0x1a,
  0x48,
  0x80,
  0x32, 0x1a, 0x00,
  0x79,
  0x32, 0x19, 0x00,
  0xfe, 0x00,
  0xfa, 0x00, 0x00,
  0xf2, 0x00, 0x00,
  0x76,
  0x03,
  0x05,
  */
  0x3a, 0x0b, 0x00,
  0x3d,
  0x32, 0x0b, 0x00,
  0xc2, 0x00, 0x00,
  0x76,
  0x0a,
  /*
  0x21, 0x2b, 0x00,
  0x3a, 0x28, 0x00,
  0x06, 0x29,
  0x48,
  0x80,
  0x32, 0x29, 0x00, 
  0x79, 
  0x32, 0x28, 0x00, 
  0xfe, 0x00, 
  0xfa, 0x03, 0x00, 
  0xfe, 0x00,
  0xf2, 0x03, 0x00,
  0x77, 
  0x23, 
  0x3a, 0x2a, 0x00, 
  0x3d,
  0x32, 0x2a, 0x00,
  0xc2, 0x03, 0x00, 
  0x76,
  0x03,
  0x05,
  0x01,
  */
/*0000*/  0x21, 0x41, 0x00,  //  ld  hl, work
/*0003*/  0x3e, 0x53,        // ld  a, 'S'
/*0005*/  0x77,              // ld  (hl), a
/*0006*/  0x23,              // inc hl
/*0007*/  0x3e, 0x45,        // ld  a, 'E'-2+1
/*0009*/  0x77,              // ld  (hl), a
/*000a*/  0x23,              // inc hl
/*000b*/  0x3e, 0x43,        // ld  a, 'C'
/*000d*/  0x77,              // ld  (hl), a
/*000e*/  0x23,              // inc hl
/*000f*/  0x77,              // ld  (hl), a
/*0010*/  0x23,              // inc   hl
/*0011*/  0xc6, 0x0c,        // add a, 'O'-'C'
/*0013*/  0x77,              // ld  (hl), a
/*0014*/  0x23,              // inc hl
/*0015*/  0xc6, 0xff,        // add a, 'N'-'O'
/*0017*/  0x77,              // ld  (hl), a
/*0018*/  0x23,              // inc hl
/*0019*/  0x3e, 0x7b,        // ld  a, '{'
/*001b*/  0x77,              // ld  (hl), a
          0x23,
          
/*003d*/  0x3e, 0x7d,        // ld  a, '}'
/*003f*/  0x77,              // ld  (hl),a
/*0040*/  0x76,              // halt
/*0041*/                     // work: defs  100
/*0003*/                     // seed1:  equ 3
/*0005*/                     // seed2:  equ 5
/*000a*/                     // count:  equ 10
#endif
   /*
  0x21, 0x00, 0x08,   //  ld  hl, worl
  0x3e, 0x01,         //  ld  a, 1
  0x77,               //  ld (hl), a
  0x76,               //  halt
  0x09,
  *//*
  0x21, 0x00, 0x08,   //  ld  hl, work
  0x3e, 0x53,         //  ld  a, 'S'
  0x77,               //  ld  (hl), a
  0x23,               //  inc hl
  0x3e, 0x45,         //  ld  a, 'E'
  0x77,               //  ld  (hl), a
  0x23,               //  inc hl
  0x3e, 0x43,         //  ld  a, 'C'
  0x77,               //  ld  (hl), a
  0x23,               //  inc hl
  0x77,               //  ld  (hl), a
  0x23,               //  inc   hl
  0xc6, 0x0c,         //  add a, 'O'-'C'
  0x77,               //  ld  (hl), a
  0x23,               //  inc hl
  0xc6, 0xff,         //  add a, 'N'-'O'
  0x77,               //  ld  (hl), a
  0x23,               //  inc hl
  0x3e, 0x7b,         //  ld  a, '{'
  0x77,               //  ld  (hl), a 
  0x76,               //  halt
  /*//*
  0x21, 0x00, 0x08,   // 0000 ld  hl,work
  0x7e,               // 0003 ld  a, (hl)
  0x0e, 0x02,         // 0004 ld  c, 2
  0x23,               // 0006 inc hl
  0x86,               // 0007 add a, (hl)
  0x0d,               // 0008 dec c
  0xc2, 0x06, 0x00,   // 0009 jp  nz. loop
  0x32, 0x03, 0x08,   // 000c ld (work+3), a
  0x76,               // 000f halt
  0x11, 0x00, 0x01,  // 0000 ld  de, 0000h
  0x1b,              // 0003 dec de
  0x7a,              // 0004 ld  a, d
  0xb3,              // 0005 or  e
  0xc2, 0x03, 0x00,  // 0006 jp  nz, 0003
  0x76,              // 0009 halt
  0x00,              // 0000: NOP
  0x00,              // 0001: NOP
  0x00,              // 0002: NOP
  0x76,              // 0003: HALT
  0x00, 
  *//*
  0x3e, 0x05,        // 0000: LD   a, 5
  0x47,              // 0002: LD   b, a
  0x80,              // 0003: ADD  a, b
  0x32, 0x00, 0x02,  // 0004: LD   (0x0200), a
  0xc3, 0x03, 0x00,  // 0007: JP   0x0000
  *//*
  0x3E, 0x00,        // 0000: LD A, #0x00
  0x32, 0x00, 0x02,  // 0002: LD (0x0200), A
  0xD6, 0x01,        // 0005: SUB #0x01   
  0xD3, 0xcc,        // 0007: OUT (0xcc), A       
  0xc3, 0x02, 0x00,  // 0009: JP $0002
  0x00,
  */
};

unsigned int swapOneBit(unsigned v) {
  unsigned b0, b1;
  b0 = v & 1;
  b1 = v & 2;
  v = v & 0xfc;
  v |= b0==1 ? 2 : 0;
  v |= b1==2 ? 1 : 0;
  return v;
}

void swapBits()
{
  unsigned int b0=0, b1=0;
  for (unsigned i=0; i < memsize; i++) {
    b0 = mem[i] & 1;
    b1 = mem[i] & 2;
    mem[i] = mem[i] & 0xfc;
    mem[i] |= b0==1 ? 2 : 0;
    mem[i] |= b1==2 ? 1 : 0;
  }
}

void dispWork(int offset) {
  //swapBits();
  Serial.print((char*)(mem+offset));
  Serial.println();
  //swapBits();
}

void memoryDump()
{
  for (unsigned i=0; i < 0x47; i++) {
    char buf[100];
    sprintf(buf, "0x%02x, ", mem[i]);
    Serial.print(buf);
  }
  Serial.println();
}

unsigned memRead(unsigned addr) {
  for (int i=0; i<8; i++) pinMode(DATA[i], OUTPUT);
  int data = mem[addr];
  if (data & 0x80) digitalWrite(D7, HIGH); else digitalWrite(D7, LOW);
  if (data & 0x40) digitalWrite(D6, HIGH); else digitalWrite(D6, LOW);
  if (data & 0x20) digitalWrite(D5, HIGH); else digitalWrite(D5, LOW);
  if (data & 0x10) digitalWrite(D4, HIGH); else digitalWrite(D4, LOW);
  if (data & 0x08) digitalWrite(D3, HIGH); else digitalWrite(D3, LOW);
  if (data & 0x04) digitalWrite(D2, HIGH); else digitalWrite(D2, LOW);
  if (data & 0x02) digitalWrite(D1, HIGH); else digitalWrite(D1, LOW);
  if (data & 0x01) digitalWrite(D0, HIGH); else digitalWrite(D0, LOW);
  //char buf[100];
  //sprintf(buf, "memRead: addr %x data %x", addr, data);
  //Serial.println(buf);
  return data;
}

void memWrite(unsigned addr, unsigned data) {
  char buf[100];
  //sprintf(buf, "memWrite: addr %x data %x", addr, data);
  //Serial.println(buf);
  mem[addr] = data;
}

void memProtect() {
  for (int i=0; i<8; i++) pinMode(DATA[i], INPUT);
}

void setup() {
  // Initalize serial --------------------------------------------------------------------
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  // Set Up Z80 NMI vector----------------------------------------------------------------
  mem[0x66] = 0xc3;
  mem[0x67] = 0x00;
  mem[0x68] = 0x00;

  swapBits();
  memoryDump();
  // Set Up Z80 Input Pins ---------------------------------------------------------------  
  memProtect();
  for (int i=0; i < 16; i++)
    pinMode(ADDR[i], INPUT);
  pinMode(WR, INPUT);
  pinMode(RD, INPUT);
  pinMode(MREQ, INPUT);
  pinMode(IORQ, INPUT);
  pinMode(BUSACK, INPUT);
  pinMode(RFSH, INPUT);
  pinMode(M1, INPUT);
  // Set Up Z80 Output Pins --------------------------------------------------------------
  pinMode(CLK, OUTPUT);     digitalWrite(CLK, LOW);
  pinMode(BUSRQ, OUTPUT);   digitalWrite(BUSRQ, HIGH);
  pinMode(RESET, OUTPUT);   digitalWrite(RESET, LOW);
  // Reset Z80 ---------------------------------------------------------------------------
  reset();
}

void busRq(boolean x) {
  digitalWrite(BUSRQ, !x);
}

void reset() {
  digitalWrite(RESET, LOW);
  digitalWrite(RESET, HIGH);
  Serial.println("**RESET**");
}

unsigned readAddr() {
  unsigned result = 0;
  for (int i=15; i >= 0; i--) {
    result += result;
    if (digitalRead(ADDR[i])==HIGH) result |= 1;
  }
  return result;
}

unsigned readData() {
  unsigned result = 0;
  for (int i=7; i >=0; i--) {
    result += result;
    if (digitalRead(DATA[i])==HIGH) result |= 1;
  }
  return result;
}

void clock() {
  char buf[200];
  
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);

  if (digitalRead(HALT)==0) {
    Serial.println("HALT");
    dispWork(0x47);
    mem[0x44] = swapOneBit(0x03);
    mem[0x45] = swapOneBit(0x0b);
    mem[0x46] = swapOneBit(0x0a);
    clocks++;
    while (digitalRead(HALT)==0) {
      digitalWrite(CLK, HIGH);
      digitalWrite(CLK, LOW);
      clocks++;
    }
    return;
  }

  if (digitalRead(RFSH)==0) {
    clocks++;
    return;
  }

  int rfsh   = digitalRead(RFSH);
  int mreq   = digitalRead(MREQ);
  int iorq   = digitalRead(IORQ);
  int busack = digitalRead(BUSACK);
  int rd     = digitalRead(RD);
  int wr     = digitalRead(WR);
  int m1     = digitalRead(M1);
 
  unsigned addr = readAddr();
  unsigned data = readData();
  
  if (mreq==0) {
    if (rd==0) {
      data = memRead(addr);
    } else if (wr==0) {
      memWrite(addr, data);
    } 
  }
  memProtect();
  
  //sprintf(buf, "%08lu %04x %02x mr:%d io:%d rd:%d wr:%d m1:%d rf:%d ", 
  //        clocks, addr, data, mreq, iorq, rd, wr, m1, rfsh);
  //Serial.print(buf);
  //dispWork(0x47);
  clocks++;
}


void loop() {
  clock();
  //delay(100);
}
