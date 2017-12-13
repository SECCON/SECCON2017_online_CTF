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
  0x22, 0x47, 0x00, 0x3d, 0x53, 0x77, 0x23, 0x3d, 0x45, 0x77, 0x23, 0x3d, 0x43, 
  0x77, 0x23, 0x77, 0x23, 0xc5, 0x0c, 0x77, 0x23, 0xc5, 0xfd, 0x77, 0x23, 0x3d, 
  0x7b, 0x77, 0x23, 0x39, 0x44, 0x00, 0x47, 0xc5, 0x46, 0x31, 0x44, 0x00, 0x78, 
  0x31, 0x46, 0x00, 0xfd, 0x22, 0xf9, 0x1e, 0x00, 0xfd, 0x7b, 0xf1, 0x1e, 0x00, 
  0x77, 0x23, 0x39, 0x45, 0x00, 0x3e, 0x31, 0x45, 0x00, 0xc1, 0x1e, 0x00, 0x3d, 
  0x7d, 0x77, 0x75, 0x03, 0x0b, 0x09, 
};

void dispWork(int offset) {
  Serial.print((char*)(mem+offset));
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
  return data;
}

void memWrite(unsigned addr, unsigned data) {
  char buf[100];
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
  mem[0x66] = 0xc3;
  mem[0x67] = 0x00;
  mem[0x68] = 0x00;
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
  pinMode(CLK, OUTPUT);     digitalWrite(CLK, LOW);
  pinMode(BUSRQ, OUTPUT);   digitalWrite(BUSRQ, HIGH);
  pinMode(RESET, OUTPUT);   digitalWrite(RESET, LOW);
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
    mem[0x44] = 0x03;
    mem[0x45] = 0x0b;
    mem[0x46] = 0x09;
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
  clocks++;
}


void loop() {
  clock();
}
