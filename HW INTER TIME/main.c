#include "iostm8s105c6.h"

void portD_init(void);
void timer_init(void);
void interrupt_init(void);

#pragma vector = 0x0D

__interrupt void TIM1_OVR_UIF(void);

int main(void) {
  portD_init();
  timer_init();
  interrupt_init();
  for(;;){}
}
void portD_init(void) {
  PD_DDR_bit.DDR0=1;
  PD_CR1_bit.C10=1;
  PD_CR2_bit.C20=0;
  PD_ODR_bit.ODR0=0;
}
void timer_init(void){
  CLK_PCKENR2=0xff; //Включаем тактирование таймера 1
  TIM1_CR1_bit.URS=1; // прерывание только по переполнению
  //TIM1_ARRH_bit.ARR=0x36;
  TIM1_CR1_bit.DIR=0; // считаем вверх до ARR(0xff)
  TIM1_CR1_bit.CMS=0; // выбор режима счета 00: режим с выравниванием по краю; счёт ведётся вверх или вниз (режим суммирующего или вычитающего счётчика) в зависимости от значения бита направления DIR
  TIM1_IER_bit.UIE=1; // разрешаем прерывания
  TIM1_PSCRH=0x00; // старший регистр
  TIM1_PSCRL=0x10; // младший регистр
  TIM1_CR1_bit.CEN=1; // запустили
}
void interrupt_init(void){
  asm("rim");
}

__interrupt void TIM1_OVR_UIF(void){
  TIM1_SR1_bit.UIF=0; //очистка флага переполнения иначе вернемся в обработчик
  PD_ODR_bit.ODR0=!PD_ODR_bit.ODR0; // инверсия бита
  TIM1_CR1_bit.CEN=1; // запустили таймер
}
  
  


  
 
  



  

