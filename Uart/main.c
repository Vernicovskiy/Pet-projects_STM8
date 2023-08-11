

#include <iostm8s105c6.h>
#include <intrinsics.h>
#include <stdbool.h>


 unsigned char inChar = 0;
 bool rxtrue = false;

void UART_TX(unsigned char val) { // отправка байта
  UART2_DR = val;
  while (!(UART2_SR & (1 << 6))); // пока Transmission не complete
}


unsigned char UART_RX(void) { //прием байта
  while (!(UART2_SR & (1 << 5))); //пока пустой 
 return UART2_DR; // регистр данных 
}

#pragma vector=UART2_R_RXNE_vector 
__interrupt void UART2_RXE(void)
{
  inChar = UART2_DR;
  rxtrue = true;    
}



void config(){
  CLK_CKDIVR = 0x00;  //set clock 16MHZ
  CLK_PCKENR1 = 0xFF;  // Enable peripherals
  UART2_BRR2 = 0x03;
  UART2_BRR1 = 0x68;  // 9600 baud
  UART2_CR3 &= ~((1 << 4) | (1 << 5)); //1 STOP BIT
  UART2_CR2 |= (1 << 2); 
  UART2_CR2 |= (1 << 3); //ENABLE TX AND RX
  UART2_CR2_RIEN=1; // Receiver interrupt enable
}

main()
{
  __disable_interrupt (); 
  config();
  __enable_interrupt (); 
  
  
  
  

  while (1) 
  {
    if(rxtrue)
    {
      inChar++;
      UART_TX(inChar);
      rxtrue = false;
    }
    
  }
}