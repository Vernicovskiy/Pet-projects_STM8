#include <stm8s.h>
uint8_t count;
void pause(uint32_t p);
int main( void ){
//Настройка кнопки и светодиода.
GPIO_Init( GPIOG, GPIO_PIN_0, GPIO_MODE_IN_FL_NO_IT);
GPIO_Init( GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_OD_HIZ_SLOW);
//Настраиваем выводы для SPI.
GPIO_Init( GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
GPIO_Init( GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
GPIO_Init( GPIOC, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
//Настройка самого SPI как MASTER.
SPI_Init( SPI_FIRSTBIT_LSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 7);
//Включаем SPI.
SPI_Cmd( ENABLE);
while(1){
//Проверяем нажатие кнопки, если нажата зажигаем светодиод и отправляем байт 111.
if(!GPIO_ReadInputPin( GPIOG, GPIO_PIN_0)){ GPIO_WriteLow( GPIOD, GPIO_PIN_0); SPI_SendData(111);}
//Проверяем пришли данные.
if( SPI_GetFlagStatus( SPI_FLAG_RXNE)){
//Если пришли проверяем байт данных, если 111, ждем паузу и гасим светодиод.
   count = (uint8_t)SPI->DR;
if( SPI_ReceiveData()==111){ pause( 100000);; GPIO_WriteHigh( GPIOD, GPIO_PIN_0);count=0;}
}
}
}
void pause( uint32_t p){
for( uint32_t i=0; i<p; i++){ }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
while (1){ }
}
#endif