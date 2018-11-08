#define PPMMAXCHANNELNUM 8

struct channel{
  unsigned long lastChange;
  unsigned int value;
  };

volatile channel channelList[PPMMAXCHANNELNUM];
volatile int chnum;
//volatile int PPMOut Fix PPM out to PIN 2
volatile unsigned int currentLength;

volatile unsigned long inputChange;

volatile byte lastPinStateB;
volatile byte lastPinStateD;




ISR(PCINT0_vect){
  byte currPins,changeMask;
  inputChange=micros();
  currPins=PINB&((1<<PCINT0)|(1<<PCINT1));
  changeMask=lastPinStateB^currPins;
  if( changeMask & (1<<PCINT0) )
  {    
    if ( currPins & (1<<PCINT0) ) {
      channelList[4].lastChange=inputChange;
    } else  {
      if (channelList[4].lastChange !=0) {
        channelList[4].value=(inputChange-channelList[4].lastChange)*2;
      }
    }
  }

  if( changeMask & (1<<PCINT1) )
  {    
    if ( currPins & (1<<PCINT1) ) {
      channelList[5].lastChange=inputChange;
    } else  {
      if (channelList[5].lastChange !=0) {
        channelList[5].value=(inputChange-channelList[5].lastChange)*2;
      }
    }
  }
/*
  if( changeMask & (1<<PCINT2) )
  {    
    if ( currPins & (1<<PCINT2) ) {
      channelList[2].lastChange=inputChange;
    } else  {
      if (channelList[2].lastChange !=0) {
        channelList[2].value=(inputChange-channelList[2].lastChange)*2;
      }
    }
  }

  if( changeMask & (1<<PCINT3) )
  {    
    if ( currPins & (1<<PCINT3) ) {
      channelList[3].lastChange=inputChange;
    } else  {
      if (channelList[3].lastChange !=0) {
        channelList[3].value=(inputChange-channelList[3].lastChange)*2;
      }
    }  
  }

  if( changeMask & (1<<PCINT4) )
  {    
    if ( currPins & (1<<PCINT4) ) {
      channelList[4].lastChange=inputChange;
    } else  {
      if (channelList[4].lastChange !=0) {
        channelList[4].value=(inputChange-channelList[4].lastChange)*2;
      }
    }
  }
  
  if( changeMask & (1<<PCINT5) )
  {    
    if ( currPins & (1<<PCINT5) ) {
      channelList[5].lastChange=inputChange;
    } else  {
      if (channelList[5].lastChange !=0) {
        channelList[5].value=(inputChange-channelList[5].lastChange)*2;
      }
    }
  }     
*/
  
   lastPinStateB=currPins;
 } 





ISR(PCINT2_vect){
  byte currPins,changeMask;
  inputChange=micros();
  currPins=PIND&((1<<PCINT20)|(1<<PCINT21)|(1<<PCINT22)|(1<<PCINT23));
  changeMask=lastPinStateD^currPins;
  if( changeMask & (1<<PCINT20) )
  {    
    if ( currPins & (1<<PCINT20) ) {
      channelList[0].lastChange=inputChange;
    } else  {
      if (channelList[0].lastChange !=0) {
        channelList[0].value=(inputChange-channelList[0].lastChange)*2;
      }
    }
  }

  if( changeMask & (1<<PCINT21) )
  {    
    if ( currPins & (1<<PCINT21) ) {
      channelList[1].lastChange=inputChange;
    } else  {
      if (channelList[1].lastChange !=0) {
        channelList[1].value=(inputChange-channelList[1].lastChange)*2;
      }
    }
  }

  if( changeMask & (1<<PCINT22) )
  {    
    if ( currPins & (1<<PCINT22) ) {
      channelList[2].lastChange=inputChange;
    } else  {
      if (channelList[2].lastChange !=0) {
        channelList[2].value=(inputChange-channelList[2].lastChange)*2;
      }
    }
  }

  if( changeMask & (1<<PCINT23) )
  {    
    if ( currPins & (1<<PCINT23) ) {
      channelList[3].lastChange=inputChange;
    } else  {
      if (channelList[3].lastChange !=0) {
        channelList[3].value=(inputChange-channelList[3].lastChange)*2;
      }
    }  
  }

/*
  if( changeMask & (1<<PCINT4) )
  {    
    if ( currPins & (1<<PCINT4) ) {
      channelList[4].lastChange=inputChange;
    } else  {
      if (channelList[4].lastChange !=0) {
        channelList[4].value=(inputChange-channelList[4].lastChange)*2;
      }
    }
  }
  
  if( changeMask & (1<<PCINT5) )
  {    
    if ( currPins & (1<<PCINT5) ) {
      channelList[5].lastChange=inputChange;
    } else  {
      if (channelList[5].lastChange !=0) {
        channelList[5].value=(inputChange-channelList[5].lastChange)*2;
      }
    }
  }     
*/
  
   lastPinStateD=currPins;
 } 





ISR(TIMER1_COMPA_vect) // timer compareA interrupt service routine
{
  PORTD |= (1<<PCINT18);
  TCNT1  = 0;
//  digitalWrite(PPMOut, HIGH);
  chnum++; if (chnum > PPMMAXCHANNELNUM) { chnum=0;}
  if (chnum ==PPMMAXCHANNELNUM){
        OCR1A = 45000-currentLength;
        currentLength=0;
    } else {
      OCR1A = channelList[chnum].value; 
      currentLength+=channelList[chnum].value;
    } 

}

ISR(TIMER1_COMPB_vect) // timer compareB interrupt service routine
{
  PORTD &= ~(1<<PCINT18);
//    digitalWrite(PPMOut, LOW);
}

void setChannel(int channel, int pin, unsigned int initValue) {
  if (pin!=0)
    {pinMode(pin,INPUT);}
  channelList[channel-1].value = initValue<<1;
  channelList[channel-1].lastChange = 0;
  }

  
void setup() {
  chnum =PPMMAXCHANNELNUM;
  setChannel(1,4,1500); //aileron
  setChannel(2,5,1500); //elevator
  setChannel(3,6,1000); //throttle
  setChannel(4,7,1500); //rudder
  setChannel(5,8,1500); //aux1
  setChannel(6,9,1500); //aux2
  setChannel(7,0,1000); //RSSI or any ADC
  setChannel(8,0,1000); // FailSafe
//  PPMOut=2;
  pinMode(2,OUTPUT);

//  digitalWrite(PPMOut, HIGH);

  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1C = 0;               // Clear TCCR1C
  TCNT1  = 0;

  OCR1B = 600;            // compare match register PPM pin width
  OCR1A = 1200; 
  currentLength = 0;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11);    // 8 prescaler 
//  TCCR1A = B11110000;   
  TIMSK1 |= (1 << OCIE1B )|( 1 << OCIE1A);  // enable timer compare interrupt

  PCICR |= (1<<PCIE0)|(1<<PCIE2);
  PCMSK0 |= (1<<PCINT0)|(1<<PCINT1);
  PCMSK2 |= (1<<PCINT20)|(1<<PCINT21)|(1<<PCINT22)|(1<<PCINT23);

  
  lastPinStateB= PINB&(1<<PCINT0)|(1<<PCINT1);
  lastPinStateD= PIND&(1<<PCINT20)|(1<<PCINT21)|(1<<PCINT22)|(1<<PCINT23);
  
  inputChange=micros();  
  
  interrupts();             // enable all interrupts

//  PORTD |= (1<<PCINT18);


}






void loop() {
  // put your main code here, to run repeatedly:
int i;
int chPin;


if (( micros()- inputChange)>200000)
{
  channelList[7].value=4000;
} else {
  channelList[7].value=2000;    
}

channelList[6].value=map(analogRead(0),0,1023,2000,4000 );
/*
for(i=0;i<PPMMAXCHANNELNUM;i++) {
  if (channelList[i].pin != 0){
  chPin=digitalRead(channelList[i].pin);
  if ((chPin==LOW) && !channelList[i].clearInput) {
    channelList[i].clearInput=true;
    } else {    
      if (chPin!=channelList[i].state){
          channelList[i].state=chPin;
          if (chPin==LOW){
              channelList[i].value=(micros()-channelList[i].lastChange)*2;

            } else {
              channelList[i].lastChange=micros();
            }
          }
        }
    }
}
*/
}
