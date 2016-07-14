
// Variables
// static volatile uint32_t tick = 0;


// Functions
// void tickConfig(void);  // initializes timer 2 on the 328

// ISR(TIMER0_COMPA_vect){//timer0 interrupt 1kHz toggles pin 8
//  tick ++; // increment our tick
// // //generates pulse wave of frequency 2kHz/2 = 500Hz (takes two cycles for full wave- toggle high then toggle low)
// //   if (toggle0){
// //     digitalWrite(8,HIGH);
// //     toggle0 = 0;
// //   }
// //   else{
// //     digitalWrite(8,LOW);
// //     toggle0 = 1;
// //   }
// }
//
// uint32_t getSystemTick(){
//   return tick;
// }
