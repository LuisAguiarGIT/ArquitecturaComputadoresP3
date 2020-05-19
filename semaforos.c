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

// Verifica se já foi da cor anterior
bool S1_VERD_VERIF = false;
bool S2_VERD_VERIF = false;
bool S1_AMAR_VERIF = false;
bool S2_AMAR_VERIF = false;

void InitTimer0(void);

void main(void)
{
    desligaSemaforos();
    InitTimer();

    while (1)
    {
        mudaEstadoS1_e_S2();
    }
}

void InitTimer0(void)
{
    EA = 1;  // Ativa interrupções globais e do timer 0
    ET0 = 1; //Ativa interrupção do timer 0
    EX0 = 1; // Ativa interrupção externa 0

    // Configura o timer 0 no modo 2
    // 12MHz / 12 = 1MHz
    // 1s/1MHz = 1us
    TMOD &= 0xF0; // Limpa os 4 bits do timer 0
    TMOD |= 0x02; // Timer a funcionar no modo 2

    // Configura o tempo de contagem (200us)
    TH0 = 0x38;
    TL0 = 0x38;

    //Inicia o timer0
    TRO = 1;
}

void Timer0_ISR(void) interrupt 1
{               // Interrupt indica um evento que requer atenção imediata, quando isto ocorre, o controlador completa a execução da instrução atual
    conta_um++; // e começa a a execução do interrupt service routine, isto comunica ao controlador o que fazer quando a interrupção occorre
}

// void interrupcaoBotao(void) interrupt 0 {
//     // Código para quando clicam no botão
// }

void mudaEstadoS1_e_S2(void)
{
    S1_VERD = ~S1_VERD;
    S2_VERD = ~S2_VERD;
    // // Fica verde durante 10 segundos
    if (conta_um == 50000 && !S1_VERD_VERIF && !S2_VERD_VERIF)
    {
        S1_VERD = ~S1_VERD;
        S2_VERD = ~S2_VERD;
        // Reset no contador pois passou para amarelo
        conta_um = 0;
        // Já foi verde
        S1_VERD_VERIF = !S1_VERD_VERIF;
        S2_VERD_VERIF = !S1_VERD_VERIF;
    }

    // Fica amarelo durante 5 segundos
    if (conta_um == 25000 && !S1_VERD_VERIF && !S2_VERD_VERIF)
    {
        S1_AMAR = ~S1_AMAR;
        S2_AMAR = ~S2_AMAR;
        // Reset no contador pois passou para vermelho
        conta_um = 0;
        // Já foi amarelo
        S1_AMAR_VERIF = !S1_AMAR_VERIF;
        S2_AMAR_VERIF = !S2_AMAR_VERIF;
    }

    // Fica vermelho durante 15 segundos
    if (conta_um == 75000 && !S1_AMAR_VERIF && !S2_AMAR_VERIF)
    {
        S1_VERM = ~S1_VERM;
        S2_VERM = ~S2_VERM;
        // Reset no contador pois passou para vermelho
        conta_um = 0;
        // Já foi vermelho
        S1_VERD_VERIF = !S1_VERD_VERIF;
        S2_VERD_VERIF = !S1_VERD_VERIF;
    }
}

void desligaSemaforos(void)
{
    S1_VERM = 1;
    S1_AMAR = 1;
    S1_VERD = 1;

    // Semáforo 2
    S2_VERM = 1;
    S2_AMAR = 1;
    S2_VERD = 1;

    // Semáforo 3
    S3_VERM = 1;
    S3_AMAR = 1;
    S3_VERD = 1;

    // Semáforo peões
    P3_VERM = 1;
    P4_VERD = 1;
}