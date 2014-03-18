
#define clock_pin 10
#define latch_pin 9
#define data_pin 8

byte val[8][8];

void setup() {
  //initializes the pins 
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

  //Sets the top layer to be lit up, to look like a cloud
  for(int i=0; i<8; i++)
    for(int j=0; j<7; j++)
      val[i][j]=255;
      
  for(int i=0; i<8; i++)
    val[i][7]=0;
}


//Creates raindrops on the 7th layer, and drops each drop down
void make_rain() {

  //drops each all of the drops down a layer
  for(int i=0; i<6; i++)
    for(int j=0; j<8; j++)
      val[j][i]=val[j][i+1];
    
  //Creates new raindrops on the 7th layer
  for(int i=0; i<8; i++) {

    //Only makes a raindrop 30% of the time. Makes it look neater
    if(random(10)<7) {
      val[i][6]=255;
      continue;
    }

    //Gets the position of the raindrop (0 based)
    int raindrop_pos=random(8);

    //some bit shifting magic to place the raindrop in the binary number
    byte raindrop=1 << raindrop_pos;
    
    //Replace all ones with zeros and visa-versa
    raindrop=255-raindrop;

    //set the raindrop to the 7th layer (second highest)
    val[i][6]=raindrop;
  }
}

void loop() {  
  
  //creates raindrops, and moves them down in the cube
  make_rain();

  //small delay loop that gets the animation speed just right
  for(byte i=0; i<4; i++) {
   
    //updates the cube to display the new set of values
    update_cube();      
  }
}



void update_cube() {

  //iterates through each layer
  for(byte layer=0; layer<8; layer++) {

    //makes sure the latch pin for the analog data is set to its default
    digitalWrite(latch_pin, LOW);
    
    //loops through each position in the array
    for(byte pos=0; pos<9; pos++) {

      //uses a temporary storage of the position to write out
      byte store_pos = pos%8;
      
      //sets the position ic to not display
      digitalWrite(7,LOW);

      //tells the position ic which position to switch to
      for(int i = 6; i>3; i--) {
        digitalWrite(i, store_pos%2 ? LOW:HIGH);
        store_pos/=2;
      }

      //turns on the position ic to display
      digitalWrite(7,HIGH);
      
      //corrects the position because the pins are a pain to fix
      byte data_pos = pos%2?pos+2:pos;
      
      //sets the analog chip to start accepting data
      digitalWrite(latch_pin, HIGH);
      
      //shifts out the data
      shiftOut(data_pin,clock_pin,pos%2?MSBFIRST:LSBFIRST
        ,val[(data_pos+4)%8][(layer+1)%8]);//val[(layer+offset)%8]);

      //tells the analog chip to stop accepting data
      digitalWrite(latch_pin, LOW);
      
    }
    
    digitalWrite(7,HIGH);

    //stores the layer byte 
    byte curr = layer;
    
    //switches 2 layers, because I haven't gotten around to fixing them in
    //the hardware yet. My bad
    if(curr ==7) curr=5;
    else if(curr==5) curr=7;
    
    //sets the layer of the cube to be displayed
    for(int i = 3; i>0; i--) {
      digitalWrite(i, curr%2 ? LOW:HIGH);
      curr/=2;
    }
    
    digitalWrite(7,LOW);

    //starts displaying the cube
    digitalWrite(11,LOW);
    //gives it 2 miliseconds of light (any more and it starts to flicker)
    delay(2);

    //stops displaying the layer
    digitalWrite(11, HIGH);
  }

  //resets the pins to make sure the cube does not do anything funky
  digitalWrite(7,LOW);
  digitalWrite(11,HIGH);
}

