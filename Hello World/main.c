//�������������
#include  <iostm8s105c6.h>
#include  <intrinsics.h>

//����������
unsigned char status=0;                 //������� ������� ����� 
unsigned int msecond=0;
unsigned int milesecond=0;              //������� RTC
unsigned int second=0;
const unsigned char HSE=0xb4;           //������� ���������
const unsigned char HSI=0xe1;           //���������� ���������

//��������� �������
void delay(unsigned int n);             //��������� ��������
void TIM4_setup(void);                  //��������� ������� ���4
//void CLK_SW(unsigned char);
//void knopka(void);



int main( void )
{
  __enable_interrupt();                 //��������� ���������� ����������(������������ ������� RIM)
  
  //����� ���� ��������� ����������
  CLK_SWCR_SWEN=1;                      //��������� ������������ ����� ��
  CLK_SWR=HSE;                          //���������� ����� �������� ���� ��
 
  //����������� ������ TIM4
  TIM4_setup();   
  
  //������������ PORTD 0
  PD_DDR_bit.DDR0 = 1;                  // ����� PD0 ��������������� �� �����
  PD_CR1_bit.C10 = 1;                   // ����� ���� Push-pull
  PD_CR2_bit.C20 = 1;                   // �������� ������������ - �� 10 ���.
 hello_world: 
   if(milesecond>=50000){                 //����� 0,5� ������
     PD_ODR_bit.ODR0=0;
   }
   else{                                //����� 0,5� �� ������
     PD_ODR_bit.ODR0=1;
   }
  goto hello_world;
}
 
void delay(unsigned int n)              //��������� ��������
{
    while (n--> 0);
}

//��������� ������� ���4
void TIM4_setup(void){
  TIM4_CR1_CEN=1;                       //��������� ������
  TIM4_PSCR=4;                          //����� ������������ �������
  TIM4_CR1_ARPE=1;                      //��������� ����������
  TIM4_ARR=201;                         //����� ��������(�� ������� ��������, � ������� �������))
  TIM4_IER_UIE=1;                       //��������� ���������� �� �������
}



//������� ��������� ����������
#pragma vector=TIM4_OVR_UIF_vector      //������� ��������� ����������
__interrupt void TIMR4_ISR(void){
  TIM4_SR_UIF=0;                        //���������� ���� ���������� �� �������
  ++msecond;
  if(msecond>=5){                       //������� �����.
    msecond=0;
    ++milesecond;
    if(milesecond>=5000){
      milesecond=0;
      ++second;
    }
  }
  
}
 