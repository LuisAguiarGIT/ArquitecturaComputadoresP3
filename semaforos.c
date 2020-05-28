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
sbit P3_VERD = P2 ^ 3;

// Botão dos peões
sbit B3 = P3 ^ 2;

// Contadores de tempo
unsigned int contadorS1_S2 = 0;
unsigned int contadorS3 = 0;
unsigned int contadorP3 = 0;

// Verifica se já foi da cor anterior S1
int S1_VERD_VERIF = 0;
int S1_AMAR_VERIF = 0;

// Verifica se já foi da cor anterior S3
int S3_AMAR_VERIF = 0;
int S3_VERM_VERIF = 0;

// Verifica se já passaram 10 segundos
int S3_DEZ_FLAG = 0;

void InitTimer0(void);
void Timer0_ISR(void);
void InitTimer1(void);
void Timer1_ISR(void);
void desligaSemaforos(void);
void mudaEstadoS1_e_S2(void);
void mudaEstadoS3(void);
void mudaEstadoP3(void);

void main(void)
{
    desligaSemaforos();
    InitTimer0();
    S1_VERD = ~S1_VERD;
    S2_VERD = ~S2_VERD;
    S3_VERM = ~S3_VERM;

    while (1)
    {
        mudaEstadoS1_e_S2();
        mudaEstadoS3();
        mudaEstadoP3();
    }
}

void InitTimer0(void)
{
    EA = 1;  // Ativa interrupções globais e do timer 0
    ET0 = 1; // Ativa interrupção do timer 0
    EX0 = 1; // Ativa interrupção externa 0

    // Configura o timer 0 no modo 2
    // 12MHz / 12 = 1MHz
    // 1s/1MHz = 1us
    TMOD &= 0xF0; // Limpa os 4 bits do timer 0
    TMOD |= 0x02; // Timer a funcionar no modo 2

    // Configura o tempo de contagem (250us)
    TH0 = 0x06;
    TL0 = 0x06;

    //Inicia o timer0
    TR0 = 1;
}

void Timer0_ISR(void) interrupt 1
{
    contadorS1_S2++;
    contadorS3++;
}

void InitTimer1(void)
{
    ET1 = 1;
    EX1 = 1;

    TMOD = 0x06;
    TL0 = 0x06;

    TR1 = 1;
}

void Timer1_ISR(void) interrupt 1
{
    contadorP3++;
}

// void interrupcaoBotao(void) interrupt 0 {
//     // Código para quando clicam no botão
// }

void mudaEstadoS1_e_S2(void)
{   // Se passaram 10s E o S1 foi verde
    // DESLIGA VERDE E LIGA AMARELO
    if (contadorS1_S2 == 40000 && S1_VERD_VERIF == 0)
    {
        // Desligar a luz verde
        S1_VERD = ~S1_VERD;
        S2_VERD = ~S2_VERD;

        // Verde foi ligado
        S1_VERD_VERIF = 1;

        // Reset do contador
        contadorS1_S2 = 0;

        // Ligar a luz amarela
        S1_AMAR = ~S1_AMAR;
        S2_AMAR = ~S2_AMAR;
    }
    // Se passaram 5s E S1 foi amarelo foi ligado durante esse tempo
    // DESLIGA AMARELO E LIGA VERMELHO
    if (contadorS1_S2 == 20000 && S1_VERD_VERIF == 1 && S1_AMAR_VERIF == 0)
    {
        // Desligar a luz amarela
        S1_AMAR = ~S1_AMAR;
        S2_AMAR = ~S2_AMAR;

        //Amarelo foi ligado
        S1_AMAR_VERIF = 1;

        contadorS1_S2 = 0;

        // Ligar a luz vermelha S1 e S2
        S1_VERM = ~S1_VERM;
        S2_VERM = ~S2_VERM;
    }
    // DESLIGA VERMELHO E LIGA VERDE
    if (contadorS1_S2 == 60000 && S1_AMAR_VERIF == 1)
    {
        // Desligar a luz vermelha
        S1_VERM = ~S1_VERM;
        S2_VERM = ~S2_VERM;

        S1_VERD_VERIF = 0;

        S1_AMAR_VERIF = 0;

        contadorS1_S2 = 0;

        S1_VERD = ~S1_VERD;
        S2_VERD = ~S2_VERD;
    }
}

void mudaEstadoS3(void)
{
    if (S1_VERD == 0 || S1_AMAR == 0)
    {
        S3_VERM = 0;
        S3_AMAR = 1;
        S3_VERD = 1;
    }
    else
    {
        if (contadorS3 == 40000)
        {
            S3_DEZ_FLAG = 1;
        }
        // Logo S1 e S2 está vermelho
        // DESLIGA VERMELHO E LIGA VERDE
        if (contadorS3 == 60000 && S3_VERM_VERIF == 0)
        {
            // Desligar a luz verde
            S3_VERM = ~S3_VERM;

            S3_DEZ_FLAG = 0;

            // Vermelho foi ligado
            S3_VERM_VERIF = 1;

            // Reset do contador
            contadorS3 = 0;

            // Ligar a luz amarela
            S3_VERD = ~S3_VERD;
        }
        // DESLIGA VERDE E LIGA AMARELO
        if (contadorS3 == 20000 && S3_VERM_VERIF == 1 && S3_AMAR_VERIF == 0)
        {
            // Desligar a luz amarela
            S3_VERD = ~S3_VERD;

            //Amarelo foi ligado
            S3_AMAR_VERIF = 1;

            contadorS3 = 0;

            // Ligar a luz amarela S3
            S3_AMAR = ~S3_AMAR;
        }
        // DESLIGA AMARELO E LIGA VERMELHO
        if (contadorS3 == 40000 && S3_AMAR_VERIF == 1)
        {
            // Desligar a luz amarela
            S3_AMAR = ~S3_AMAR;

            S3_VERM_VERIF = 0;
            S3_AMAR_VERIF = 0;
            S3_DEZ_FLAG = 0;

            contadorS3 = 0;

            // Voltar ao ciclo
            S3_VERM = ~S3_VERM;
        }
    }
}

void mudaEstadoP3(void)
{
    if (S3_VERD == 0 || S3_AMAR == 0)
    {
        P3_VERM = 0;
        P3_VERD = 1;
    }
    else
    {
        if (S3_DEZ_FLAG = 1 && S3_VERM == 0)
        {
            InitTimer1();
            if (contadorP3 == 4000)
            {
                P3_VERM = ~P3_VERM;
                contadorP3 = 0;
            }
        }
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
    P3_VERD = 1;
}