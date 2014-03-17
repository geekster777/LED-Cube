
#define clock_pin 10
#define latch_pin 9
#define data_pin 8

byte val[8][8];

void setup() {
    
      pinMode(clock_pin, OUTPUT);
      digitalWrite(clock_pin, LOW);
      pinMode(latch_pin, OUTPUT);
      pinMode(data_pin, OUTPUT);
      pinMode(12, OUTPUT);
      pinMode(13, OUTPUT);
                
      pinMode(1, OUTPUT);
      pinMode(2, OUTPUT);
      pinMode(3, OUTPUT);
      pinMode(4, OUTPUT);
      pinMode(5, OUTPUT);
      pinMode(6, OUTPUT);
      pinMode(7, OUTPUT);
      pinMode(11, OUTPUT);
      digitalWrite(11, HIGH);
      for(int i=0; i<8; i++)
        for(int j=0; j<7; j++)
          val[i][j]=255;
      
      for(int i=0; i<8; i++)
        val[i][7]=0;
}

byte offset = 0;
int G=0;

void make_rain() {
  for(int i=0; i<6; i++)
    for(int j=0; j<8; j++)
      val[j][i]=val[j][i+1];
    
  for(int i=0; i<8; i++) {
    if(random(10)<7) {
      val[i][6]=255;
      continue;
    }
    int raindrop_pos=random(8);
    byte raindrop=1;
    for(int j=0; j<raindrop_pos; j++)
      raindrop*=2;
    raindrop=255-raindrop;
    val[i][6]=raindrop;
  }
}

void loop() {  
  offset = (offset+1)%8;
  
  make_rain();
      
  G=(G+1)%8;       
  for(byte b=0; b<4; b++)
                                
  for(byte layer=0; layer<8; layer++) {
    digitalWrite(latch_pin, LOW);
    
    for(byte pos=0; pos<9; pos++) {
      byte store_pos = pos%8;
      
      digitalWrite(7,LOW);
      for(int i = 6; i>3; i--) {
        digitalWrite(i, store_pos%2 ? LOW:HIGH);
        store_pos/=2;
      }
      digitalWrite(7,HIGH);
      
      byte data_pos = pos%2?pos+2:pos;

      digitalWrite(latch_pin, HIGH);

      shiftOut(data_pin,clock_pin,pos%2?MSBFIRST:LSBFIRST
        ,val[(data_pos+4)%8][(layer+1)%8]);//val[(layer+offset)%8]);

      digitalWrite(latch_pin, LOW);
      
    }
    digitalWrite(7,HIGH);
    byte curr = layer;
    
    if(curr ==7) curr=5;
    else if(curr==5) curr=7;
      
    for(int i = 3; i>0; i--) {
      digitalWrite(i, curr%2 ? LOW:HIGH);
      curr/=2;
    }
    digitalWrite(7,LOW);
    digitalWrite(11,LOW);
    delay(2);
    digitalWrite(11, HIGH);
  }
  digitalWrite(7,LOW);
  digitalWrite(11,HIGH);
}


