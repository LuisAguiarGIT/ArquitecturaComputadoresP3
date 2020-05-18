#include <reg51.h>

// Semáforo 1
sbit S1_VERM = P1 ^ 0;
sbit S1_AMAR = P1 ^ 1;
sbit S1_VERD = P1 ^ 2;

// Semáforo 2
sbit S2_VERM = P1 ^ 3;
sbit S2_AMAR = P1 ^ 4;
sbit S2_VERD = P1 ^ 5;

// Semáforo 3
sbit S3_VERM = P2 ^ 2;
sbit S3_AMAR = P2 ^ 1;
sbit S3_VERD = P2 ^ 0;

// Semáforo peões
sbit P3_VERM = P2 ^ 4;
sbit P4_VERD = P2 ^ 3;

// Botão dos peões
sbit B3 = P3 ^ 2;

// Contadores de tempo
int conta_um = 0;
int conta_cinco = 0;
int conta_dez = 0;
int conta_quinze = 0;

void InitTimer0(void);

void main(void)
{
    InitTimer();

    while (1)
    {
        //conta = 1    -> 250us
        //conta = 4    -> 1ms
        //conta = 40   -> 10ms
        //conta = 400  -> 100ms
        //conta = 4000 -> 1s
    }
}

void InitTimer0(void)
{
    // Ativa interrupções globais e do timer 0
    EA = 1;
    ET0 = 1;

    // Configura o timer 0 no modo 2
    // 12MHz / 12 = 1MHz
    // 1s/1MHz = 1us
    TMOD &= 0xF0;
    TMOD |= 0x02;

    // Configura o tempo de contagem (250us)
    TH0 = 0x06;
    TL0 = 0x06;

    //Inicia o timer0
    TRO = 1;
}

void Timer0_ISR(void) interrupt 1
{               // Interrupt indica um evento que requer atenção imediata, quando isto ocorre, o controlador completa a execução da instrução atual
    conta_um++; // e começa a a execução do interrupt service routine, isto comunica ao controlador o que fazer quando a interrupção occorre
    conta_cinco++;
    conta_dez++;
    conta_quinze++;
}

void mudaEstadoS1_e_S2(void)
{
    if (conta_um <= 40000)
    {
        S1_VERD = ~S1_VERD
    }
}