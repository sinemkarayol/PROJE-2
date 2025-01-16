#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.c"
#include "driverlib/gpio.c"
#include "driverlib/gpio.h"
#include"inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/timer.c"
#include <stdio.h>




#define LCD_DATA_MASK 0xF0 // D4-D7 (PB4-PB7) i�in mask

void init();
void timerkesmefonksiyonu();
void LCD_command(unsigned command);
void LCD_write(unsigned char data);
void LCD_Print(const char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_init(void);
int saat=0;
int dakika=0;
int saniye=0;
int *str=0;




void main(void)

{
       init();
       LCD_init();
       //LCD_command(0x7F); // �lk sat�ra git
       //LCD_Print("mert 5 yasinda");


       while(1)
     {


     }

}
void init()
{
    //Sistem �al��ma frekans� ayarlamas�(40MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //Port C,E,F enerjilendirilmesi
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)) {}  //Kontrol
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}  //Kontrol
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}  //Kontrol

    //Pinleri output ve inputunu ayarlanmas�
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //Timer�n ayarlanmas�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //timer0 � enerjilendirdik
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) {}   //timer0 sifir enerjilendirdimi diye kontrol ettik
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); //timer0 � 32 bit  periyodic ayarlad�k
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1); //1sn i�in timera de�er y�kledik

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);   // Kesme bayra��n� temizle
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Timer A kesmesini etkinle�tir
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerkesmefonksiyonu); //Vekt�r adresi belirledik
    IntEnable(INT_TIMER0A);                           // NVIC'te Timer0A kesmesini etkinle�tir
    IntMasterEnable();                                // Genel kesme sistemini a�
    TimerEnable(TIMER0_BASE, TIMER_A); // setb tr0

}

void LCD_pulseEnable(void)
{
    SysCtlDelay(6000);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 32);
    SysCtlDelay(6000);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 1);
    SysCtlDelay(6000);

}

void LCD_command(unsigned command)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 1); // RS = 0 (komut i�in)

    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~LCD_DATA_MASK) | (command & 0xF0); // Y�ksek nibble
    LCD_pulseEnable();
    SysCtlDelay(6000);// Komut i�in gecikme
    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~LCD_DATA_MASK) | ((command << 4) & 0xF0); // D���k nibble
    LCD_pulseEnable();
    SysCtlDelay(6000); // Komut i�in gecikme
}

void LCD_init(void)
{
    SysCtlDelay(1500000); // LCD'nin a��lmas� i�in gecikme
    LCD_command(0x28); // 4-bit mod ve 2 sat�r
    SysCtlDelay(80000); // Komut i�in gecikme
    LCD_command(0x01); // Ekran� temizle
    SysCtlDelay(80000); // Komut sonras� gecikme
    LCD_command(0x0F); // Ekran� a�, imle� kapal�
    SysCtlDelay(80000); // Komut i�in gecikme
    LCD_command(0x06); // �mleci sa�a kayd�r
    SysCtlDelay(80000); // Komut i�in gecikme

    LCD_command(0x1F); // Ekran� KAYDIR
    SysCtlDelay(80000); // Komut sonras� gecikme
}

void LCD_write(unsigned char data)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 16); // RS = 1 (veri i�in)

    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~LCD_DATA_MASK) | (data & 0xF0); // Y�ksek nibble
    LCD_pulseEnable();
    SysCtlDelay(6000); // Komut i�in gecikme
    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~LCD_DATA_MASK) | ((data << 4) & 0xF0); // D���k nibble
    LCD_pulseEnable();
    SysCtlDelay(6000); // Veri i�in gecikme
}

void LCD_Print(const char *str)
{

    while (*str) LCD_write(*str++);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    LCD_command(0x80 | (row ? 0x40 : 0x00) + col);
}


void timerkesmefonksiyonu()
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // bayrak temizlendi

    uint32_t deger=GPIOPinRead(GPIO_PORTF_BASE, 2);
    GPIOPinWrite(GPIO_PORTF_BASE, 2,~deger);  //ledi yak�p s�nd�rme

    saniye++;
    if(saniye==60)
    {
        saniye=0;
        dakika++;
        if(dakika==60)
        {
            dakika=0;
            saat++;
            if(saat==24)
            {
                saat=0;
            }

        }
    }
    char timeStr[9]; // "HH:MM:SS\0" format�
    sprintf(timeStr, "%02d:%02d:%02d", saat, dakika, saniye);
    //LCD_command(0x80); // �lk sat�ra git
    LCD_SetCursor(1, 0); // LCD'nin ilk sat�r�na yaz
    LCD_Print(timeStr);




}
