//заголовочники
#include  <iostm8s105c6.h>
#include  <intrinsics.h>

//переменные
unsigned char status=0;                 //регистр статуса порта 
unsigned int msecond=0;
unsigned int milesecond=0;              //региста RTC
unsigned int second=0;
const unsigned char HSE=0xb4;           //внешний генератор
const unsigned char HSI=0xe1;           //внутренний генератор

//прототипы функций
void delay(unsigned int n);             //временная задержка
void TIM4_setup(void);                  //настройка таймера ТИМ4
//void CLK_SW(unsigned char);
//void knopka(void);



int main( void )
{
  __enable_interrupt();                 //разрешаем глобальные прерывания(ассемблерная команда RIM)
  
  //смена типа тактового генератора
  CLK_SWCR_SWEN=1;                      //разрешаем переключение смену ТГ
  CLK_SWR=HSE;                          //записываем новое значение типа ТГ
 
  //настраиваем таймер TIM4
  TIM4_setup();   
  
  //настраиваема PORTD 0
  PD_DDR_bit.DDR0 = 1;                  // Вывод PD0 конфигурируется на вывод
  PD_CR1_bit.C10 = 1;                   // Выход типа Push-pull
  PD_CR2_bit.C20 = 1;                   // Скорость переключения - до 10 МГц.
 hello_world: 
   if(milesecond>=50000){                 //менее 0,5с светим
     PD_ODR_bit.ODR0=0;
   }
   else{                                //более 0,5с не светим
     PD_ODR_bit.ODR0=1;
   }
  goto hello_world;
}
 
void delay(unsigned int n)              //временная задержка
{
    while (n--> 0);
}

//настройка таймера ТИМ4
void TIM4_setup(void){
  TIM4_CR1_CEN=1;                       //разрешаем таймер
  TIM4_PSCR=4;                          //задаём предделитель таймера
  TIM4_CR1_ARPE=1;                      //разрешаем автолоадер
  TIM4_ARR=201;                         //задаём значение(не сколько загрузит, а сколько считать))
  TIM4_IER_UIE=1;                       //разрешаем прерывания от таймера
}



//функция обработки прерывания
#pragma vector=TIM4_OVR_UIF_vector      //функция обработки прерываний
__interrupt void TIMR4_ISR(void){
  TIM4_SR_UIF=0;                        //сбрасываем флаг прерывания от таймера
  ++msecond;
  if(msecond>=5){                       //считаем время.
    msecond=0;
    ++milesecond;
    if(milesecond>=5000){
      milesecond=0;
      ++second;
    }
  }
  
}
 