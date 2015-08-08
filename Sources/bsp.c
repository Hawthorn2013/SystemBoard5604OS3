/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                 Freescale MPC55xx Board Support Package
*
*                                 (c) Copyright 2007; Micrium; Weston, FL
*                                           All Rights Reserved
*
* File    : BSP.C
* By      : Fabiano Kovalski
*********************************************************************************************************
*/

#include "includes.h"


OS_EVENT *Sem_UART_0_TXI, *Sem_UART_0_RXI;
struct UART_Buffer UART_Buffer_0;
OS_EVENT Sem_DSPI_1;
struct DSPI_Device_Data DSPI_1_Device_Data;


static      int                 Init_DSPI(volatile struct DSPI_tag *dspi);
static      int                 Set_DSPI_1_Pin(void);
static      void                INTC_Handler_DSPI_1_SR_TFUF_RFOF(void);
static      void                INTC_Handler_DSPI_1_SR_EOQF(void);
static      void                INTC_Handler_DSPI_1_SR_TFFF(void);
static      void                INTC_Handler_DSPI_1_SR_TCF(void);
static      void                INTC_Handler_DSPI_1_SR_RFDF(void);
static      int                 Disable_INTC_DSPI_SR_All(volatile struct DSPI_tag *dspi);
static      int                 Enable_INTC_DSPI_SR_TFUF(volatile struct DSPI_tag *dspi);
static      int                 Disable_INTC_DSPI_SR_TFUF(volatile struct DSPI_tag *dspi);
static      int                 Enable_INTC_DSPI_SR_RFOF(volatile struct DSPI_tag *dspi);
static      int                 Disable_INTC_DSPI_SR_RFOF(volatile struct DSPI_tag *dspi);
static      int                 Enable_INTC_DSPI_SR_EOQF(volatile struct DSPI_tag *dspi);
static      int                 Disable_INTC_DSPI_SR_EOQF(volatile struct DSPI_tag *dspi);
static      int                 Enable_INTC_DSPI_SR_TFFF(volatile struct DSPI_tag *dspi);
static      int                 Disable_INTC_DSPI_SR_TFFF(volatile struct DSPI_tag *dspi);
static      int                 Enable_INTC_DSPI_SR_TCF(volatile struct DSPI_tag *dspi);
static      int                 Disable_INTC_DSPI_SR_TCF(volatile struct DSPI_tag *dspi);
static      int                 Enable_INTC_DSPI_SR_RFDF(volatile struct DSPI_tag *dspi);
static      int                 Disable_INTC_DSPI_SR_RFDF(volatile struct DSPI_tag *dspi);
static      int                 DSPI_Push_Data_to_Empty_FIFO(struct DSPI_Device_Data *dev, const uint8_t send_data[], uint8_t rev_data[], int cnt);


/*
 * Init pin to GPIO
 * Output high
 * Success return 0
 */
int Init_GPIO(uint16_t pcrNo)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    SIU.PCR[pcrNo].R = 0x0203;
    SIU.GPDO[pcrNo].R = 1;
    return 0;
}


/*
 * Set GPIO
 * Success return 0
 */
int Set_GPIO(uint16_t pcrNo, int value)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    if (value)
    {
        SIU.GPDO[pcrNo].R = 1;
    }
    else
    {
        SIU.GPDO[pcrNo].R = 0;
    }
    return 0;
}


/*
 * Get GPIO
 * Success return 0
 */
int Get_GPIO(uint16_t pcrNo, int *value)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    *value = SIU.GPDO[pcrNo].R;
    return 0;
}


/*
 * Init BSP LED
 * LED will be turned on
 */
int Init_LED(void)
{
    Init_GPIO(PCR_BSP_LED1);
    Set_GPIO(PCR_BSP_LED1, PIN_VASLUE_LED_TURN_ON);
    Init_GPIO(PCR_BSP_LED2);
    Set_GPIO(PCR_BSP_LED2, PIN_VASLUE_LED_TURN_ON);
    Init_GPIO(PCR_BSP_LED3);
    Set_GPIO(PCR_BSP_LED3, PIN_VASLUE_LED_TURN_ON);
    Init_GPIO(PCR_BSP_LED4);
    Set_GPIO(PCR_BSP_LED4, PIN_VASLUE_LED_TURN_ON);
    return 0;
}


/*
 * Turn on LED
 * Success return 0
 */
int Turn_on_LED(uint16_t pcrNo)
{
    return Set_GPIO(pcrNo, PIN_VASLUE_LED_TURN_ON);
}


/*
 * Turn off LED
 * Success return 0
 */
int Turn_off_LED(uint16_t pcrNo)
{
    return Set_GPIO(pcrNo, PIN_VASLUE_LED_TURN_OFF);
}



/*
 * Toggle LED
 * Success return 0
 */
int Toggle_LED(uint16_t pcrNo)
{
    int tmpVal;
    if (Get_GPIO(pcrNo, &tmpVal))
    {
        return 1;
    }
    if (Set_GPIO(pcrNo, !tmpVal))
    {
        return 2;
    }
    return 0;
}


void Init_EMIOS_0(void)
{
    /* eMIOS0初始化80MHz分为10MHz */
    EMIOS_0.MCR.B.GPRE= 7;  /* GPRE+1=分频系数；/* Divide 80 MHz sysclk by 7+1 = 8 for 10MHz eMIOS clk */
    EMIOS_0.MCR.B.GPREN = 1;    /* Enable eMIOS clock */
    EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
    EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
}


int Init_Key(uint16_t pcr, uint16_t uc, uint16_t irq, INTCInterruptFn handler)
{
    EMIOS_0.CH[uc].CCR.B.MODE = EMIOS_MODE_SAIC;
    EMIOS_0.CH[uc].CCR.B.BSL = EMIOS_BSL_INTERNAL_BUS;
    EMIOS_0.CH[uc].CCR.B.EDSEL = EMIOS_EDSEL_BOTH_TRIGGERING;
    SIU.PCR[pcr].R = 0x0100;
    INTC_InstallINTCInterruptHandler(handler, irq, INTC_PRIORITY_BSP_S1_S4);
    EMIOS_0.CH[uc].CCR.B.FEN = 1;
    return 0;
}


void INTC_Handler_BSP_S1_S2(void)
{
    if (EMIOS_0.CH[EMIOS_0_UC_BSP_S1].CSR.B.FLAG)
    {
        EMIOS_0.CH[EMIOS_0_UC_BSP_S1].CSR.B.FLAG = 1;
        if (PIN_VALUE_KEY_UP == EMIOS_0.CH[EMIOS_0_UC_BSP_S1].CSR.B.UCIN)
        {
            LED1 = PIN_VASLUE_LED_TURN_ON;
        }
        else
        {
            LED1 = PIN_VASLUE_LED_TURN_OFF;
        }
    }
    if (EMIOS_0.CH[EMIOS_0_UC_BSP_S2].CSR.B.FLAG)
    {
        EMIOS_0.CH[EMIOS_0_UC_BSP_S2].CSR.B.FLAG = 1;
        if (PIN_VALUE_KEY_UP == EMIOS_0.CH[EMIOS_0_UC_BSP_S2].CSR.B.UCIN)
        {
            LED2 = PIN_VASLUE_LED_TURN_ON;
        }
        else
        {
            LED2 = PIN_VASLUE_LED_TURN_OFF;
        }
    }
}


void INTC_Handler_BSP_S3_S4(void)
{
    if (EMIOS_0.CH[EMIOS_0_UC_BSP_S3].CSR.B.FLAG)
    {
        EMIOS_0.CH[EMIOS_0_UC_BSP_S3].CSR.B.FLAG = 1;
        if (PIN_VALUE_KEY_UP == EMIOS_0.CH[EMIOS_0_UC_BSP_S3].CSR.B.UCIN)
        {
            LED3 = PIN_VASLUE_LED_TURN_ON;
        }
        else
        {
            LED3 = PIN_VASLUE_LED_TURN_OFF;
        }
    }
    if (EMIOS_0.CH[EMIOS_0_UC_BSP_S4].CSR.B.FLAG)
    {
        EMIOS_0.CH[EMIOS_0_UC_BSP_S4].CSR.B.FLAG = 1;
        if (PIN_VALUE_KEY_UP == EMIOS_0.CH[EMIOS_0_UC_BSP_S4].CSR.B.UCIN)
        {
            LED4= PIN_VASLUE_LED_TURN_ON;
        }
        else
        {
            LED4 = PIN_VASLUE_LED_TURN_OFF;
        }
    }
}


int Init_UART_0_Ex(void)
{
    Init_UART(&LINFLEX_0);
    Set_UART_Baud_Rate_Ex(&LINFLEX_0, 115200);
    Set_UART_RDFL(&LINFLEX_0, UART_RDFL_MAX);
    Set_UART_0_Pin();
    Set_UART_0_INTC_Handler(INTC_Handler_BSP_UART_0_RXI, INTC_Handler_BSP_UART_0_TXI, INTC_Handler_BSP_UART_0_ERR);
}


int Init_UART(volatile struct LINFLEX_tag *uart)
{
    uart->LINCR1.B.INIT = 1;
    uart->LINCR1.R = 0x00000015;
    uart->LINFBRR.B.DIV_F = 0b1111;
    uart->LINIBRR.B.DIV_M = 0b1111111111111;
    uart->UARTCR.B.UART = 1;
    uart->UARTCR.R = 0x00000033;
    uart->LINCR1.B.INIT = 0;
    return 0;
}


int Set_UART_RDFL(volatile struct LINFLEX_tag *uart, int rdfl)
{
    if (rdfl > UART_RDFL_MAX)
    {
        return 1;
    }
    uart->UARTCR.B.RDFL = rdfl;
    return 0;
}


int Set_UART_0_Pin(void)
{
    SIU.PCR[PCR_BSP_RS232_TX].R = 0x0400;
    SIU.PCR[PCR_BSP_RS232_RX].R = 0x0103;
    return 0;
}


/*
 * Success return 0
 */
int Set_UART_Baud_Rate(volatile struct LINFLEX_tag *uart, int32_t baudrate)
{
    int baud_rate_not_found = 0;
    
    uart->LINCR1.B.INIT = 1;
    switch (baudrate)
    {
        case (2400) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_2400;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_2400;
            break;
        case (9600) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_9600;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_9600;
            break;
        case (10417) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_10417;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_10417;
            break;
        case (19200) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_19200;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_19200;
            break;
        case (57600) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_57600;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_57600;
            break;
        case (115200) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_115200;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_115200;
            break;
        case (230400) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_230400;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_230400;
            break;
        case (460800) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_460800;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_460800;
            break;
        case (921600) :
            uart->LINFBRR.B.DIV_F = LINFLEX_DIV_F_BAUD_RATE_921600;
            uart->LINIBRR.B.DIV_M = LINFLEX_DIV_M_BAUD_RATE_921600;
            break;
        default :
            baud_rate_not_found = 1;
            break;
    }
    uart->LINCR1.B.INIT = 0;
    if (baud_rate_not_found)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int Set_UART_Baud_Rate_Ex(volatile struct LINFLEX_tag *uart, int32_t baudrate)
{
    float lfdiv, f_linfbrr;
    int linfbrr, linibrr;
    
    if (baudrate > F_PERIPH_SET_1_CLK / 24 + 1)
    {
        uart->LINCR1.B.INIT = 1;
        uart->LINFBRR.B.DIV_F = 0b1000;
        uart->LINIBRR.B.DIV_M = 0b0000000000001;
        uart->LINCR1.B.INIT = 0;
        return 1;
    }
    else if (baudrate < (int)((float)F_PERIPH_SET_1_CLK / 16 / 8191.9375))
    {
        uart->LINCR1.B.INIT = 1;
        uart->LINFBRR.B.DIV_F = 0b1111;
        uart->LINIBRR.B.DIV_M = 0b1111111111111;
        uart->LINCR1.B.INIT = 0;
        return 2;
    }
    lfdiv = (float)F_PERIPH_SET_1_CLK / 16 / baudrate;
    linibrr = (int)lfdiv;
    f_linfbrr = (lfdiv - linibrr) * 16;
    linfbrr = (int)f_linfbrr;
    if (linfbrr >= 15)
    {
        if ((f_linfbrr - linfbrr) * 16 >= 15.5)
        {
            linfbrr = 0;
            linibrr++;
        }
        else
        {
            linfbrr = 15;
        }
    }
    else if (f_linfbrr - linfbrr >= 0.5)
    {
        linfbrr++;
    }
    if (linibrr > 8191)
    {
        linibrr = 8191;
        linfbrr = 15;
    }
    else if (linfbrr <= 8 && linibrr <= 1)
    {
        linfbrr = 8;
        linibrr = 1;
    }
    uart->LINCR1.B.INIT = 1;
    uart->LINFBRR.B.DIV_F = linfbrr;
    uart->LINIBRR.B.DIV_M = linibrr;
    uart->LINCR1.B.INIT = 0;
    return 0;
}


int Set_UART_0_INTC_Handler(INTCInterruptFn handler_rxi, INTCInterruptFn handler_txi, INTCInterruptFn handler_err)
{
    INTC_InstallINTCInterruptHandler((INTCInterruptFn)handler_rxi, IRQ_BSP_RS232_RXI, INTC_PRIORITY_BSP_RS232_RXI);
    INTC_InstallINTCInterruptHandler((INTCInterruptFn)handler_txi, IRQ_BSP_RS232_TXI, INTC_PRIORITY_BSP_RS232_TXI);
    INTC_InstallINTCInterruptHandler((INTCInterruptFn)handler_err, IRQ_BSP_RS232_ERR, INTC_PRIORITY_BSP_RS232_ERR);    
    return 0;
}


int Enable_UART_RXI(volatile struct LINFLEX_tag *uart)
{
    uart->LINIER.B.DRIE = 1;
    return 0;
}


int Disable_UART_RXI(volatile struct LINFLEX_tag *uart)
{
    uart->LINIER.B.DRIE = 0;
    return 0;
}


int Enable_UART_TXI(volatile struct LINFLEX_tag *uart)
{
    uart->LINIER.B.DTIE = 1;
    return 0;
}


int Disable_UART_TXI(volatile struct LINFLEX_tag *uart)
{
    uart->LINIER.B.DTIE = 0;
    return 0;
}


/*
 * Success return 0
 */
int Post_Date_to_UART_Buffer(volatile struct LINFLEX_tag *uart, const uint8_t data[], int cnt)
{
    uint8_t send[UART_TDFL_MAX] = {0x00, 0x00, 0x00, 0x00};

    if (cnt > UART_TDFL_MAX || cnt < 1)
    {
        return 1;
    }
    uart->UARTCR.B.TDFL = cnt - 1;
    switch (cnt)
    {
    case 4 :
        send[0] = data[3];
    case 3 :
        send[1] = data[2];
    case 2 :
        send[2] = data[1];
    case 1 :
        send[3] = data[0];
    }
    uart->BDRL.R = *(uint32_t *)send;
    return 0;
}


void INTC_Handler_BSP_UART_0_RXI(void)
{
    uint8_t data[UART_RDFL_MAX] = {0x00, 0x00, 0x00, 0x00};

    *(uint32_t *)data = (uint8_t)LINFLEX_0.BDRM.R;
    LINFLEX_0.UARTSR.B.DRF = 1;
    if (OSSemAccept(Sem_UART_0_RXI))
    {
        OSSemPost(Sem_UART_0_RXI);
        return;
    }
    UART_Buffer_0.length = (int)(LINFLEX_0.UARTCR.B.RDFL) + 1;
    UART_Buffer_0.data[0] = data[UART_RDFL_MAX -1 - 0];
    UART_Buffer_0.data[1] = data[UART_RDFL_MAX -1 - 1];
    UART_Buffer_0.data[2] = data[UART_RDFL_MAX -1 - 2];
    UART_Buffer_0.data[3] = data[UART_RDFL_MAX -1 - 3];
    OSSemPost(Sem_UART_0_RXI);
}


void INTC_Handler_BSP_UART_0_TXI(void)
{
    LINFLEX_0.UARTSR.B.DTF = 1;
    OSSemPost(Sem_UART_0_TXI);
}


void INTC_Handler_BSP_UART_0_ERR(void)
{
    
}


void Delay_us(uint32_t us)
{
    volatile int i, j;

    for (i = 0; i < us; i++)
    {
        for (j = 0; j < 9; j++) {}
    }
}


void Delay_ms(uint32_t ms)
{
    int i;

    for (i = 0; i < ms; i++)
    {
        Delay_us(1000);
    }
}


void Init_OSTickISR(void)
{
    /* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
    PIT.PITMCR.R = 0x00000001;  /* Enable PIT and configure timers to stop in debug modem */
    PIT.CH[PIT_CH_BSP_OSTickISR].LDVAL.R = 1600000;    /* 800000==10ms */
    INTC_InstallINTCInterruptHandler(INTC_Handler_OSTickISR, IRQ_BSP_OSTickISR, INTC_PRIORITY_BSP_OSTickISR); /* PIT 1 interrupt vector with priority 1 */
    PIT.CH[PIT_CH_BSP_OSTickISR].TCTRL.R = 0x00000003; /* Enable PIT1 interrupt and make PIT active to count */
}


void INTC_Handler_OSTickISR(void)
{
    PIT.CH[PIT_CH_BSP_OSTickISR].TFLG.B.TIF = 1;   // MPC56xxB/P/S: Clear PIT 1 flag by writing 1
    OSTickISR();
    PIT.PITMCR.R=0x00000000;
}


static int Init_DSPI(volatile struct DSPI_tag *dspi)
{
    dspi->MCR.R = 0x803f0001;
    dspi->RSER.R = 0x00000000;
    dspi->MCR.B.HALT = 0x0;
}


int Init_DSPI_1(void)
{
    Init_DSPI(&DSPI_1);
    Set_DSPI_1_Pin();
//    INTC_InstallINTCInterruptHandler(INTC_Handler_DSPI_1_SR_TFUF_RFOF, IRQ_DSPI_1_SR_TFUF_RFOF, INTC_PRIORITY_DSPI_1_SR_TFUF_RFOF);
    INTC_InstallINTCInterruptHandler(INTC_Handler_DSPI_1_SR_EOQF, IRQ_DSPI_1_SR_EOQF, INTC_PRIORITY_DSPI_1_SR_EOQF);
//    INTC_InstallINTCInterruptHandler(INTC_Handler_DSPI_1_SR_TFFF, IRQ_DSPI_1_SR_TFFF, INTC_PRIORITY_DSPI_1_SR_TFFF);
//    INTC_InstallINTCInterruptHandler(INTC_Handler_DSPI_1_SR_TCF, IRQ_DSPI_1_SR_TCF, INTC_PRIORITY_DSPI_1_SR_TCF);
//    INTC_InstallINTCInterruptHandler(INTC_Handler_DSPI_1_SR_RFDF, IRQ_DSPI_1_SR_RFDF, INTC_PRIORITY_DSPI_1_SR_RFDF);
    DSPI_1_Device_Data.CB_TX_Complete = NULL;
    DSPI_1_Device_Data.dspi = &DSPI_1;
    return DSPI_ERR_NONE;
}


static int Set_DSPI_1_Pin(void)
{
    SIU.PCR[34].R = 0x0604; //PC2 SCK_1
    //SIU.PSMI[7].R = 0;    //SCK_1 PCR[34]
    SIU.PCR[35].R = 0x0503; //PC3 CS0_1
    //SIU.PSMI[9].R = 0;    //CS0_1 PCR[35]
    SIU.PCR[36].R = 0x0104; //PC4 SIN_1
    //SIU.PSMI[8].R = 0;    //SIN_1 PCR[36]
    SIU.PCR[62].R = 0x0604; //PD14 CS1_1
    SIU.PCR[63].R = 0x0604; //PD15 CS2_1
    SIU.PCR[67].R = 0x0A04; //PE3 SOUT_1
    SIU.PCR[74].R = 0x0A04; //PE10 CS3_1
    SIU.PCR[75].R = 0x0A04; //PE11 CS4_1
}


static void INTC_Handler_DSPI_1_SR_TFUF_RFOF(void)
{
    
}


static void INTC_Handler_DSPI_1_SR_EOQF(void)
{
    Disable_INTC_DSPI_SR_EOQF(&DSPI_1);
    DSPI_1.SR.B.EOQF = 1;
    if (NULL != DSPI_1_Device_Data.CB_TX_Complete)
    {
        DSPI_1_Device_Data.CB_TX_Complete();
    }
}


static void INTC_Handler_DSPI_1_SR_TFFF(void)
{
    
}


static void INTC_Handler_DSPI_1_SR_TCF(void)
{
    
}


static void INTC_Handler_DSPI_1_SR_RFDF(void)
{
    
}


static int Disable_INTC_DSPI_SR_All(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.R            = 0x00000000;
    dspi->MCR.B.HALT        = halt;
}


static int Enable_INTC_DSPI_SR_TFUF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.TFUFRE     = 1;
    dspi->MCR.B.HALT        = halt;
}


static int Disable_INTC_DSPI_SR_TFUF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.TFUFRE     = 0;
    dspi->MCR.B.HALT        = halt;
}


static int Enable_INTC_DSPI_SR_RFOF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.RFOFRE     = 1;
    dspi->MCR.B.HALT        = halt;
}


static int Disable_INTC_DSPI_SR_RFOF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.RFOFRE     = 0;
    dspi->MCR.B.HALT        = halt;
}


static int Enable_INTC_DSPI_SR_EOQF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.EOQFRE     = 1;
    dspi->MCR.B.HALT        = halt;
}


static int Disable_INTC_DSPI_SR_EOQF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.EOQFRE     = 0;
    dspi->MCR.B.HALT        = halt;
}


static int Enable_INTC_DSPI_SR_TFFF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.TFFFRE     = 1;
    dspi->MCR.B.HALT        = halt;
}


static int Disable_INTC_DSPI_SR_TFFF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.TFFFRE     = 0;
    dspi->MCR.B.HALT        = halt;
}


static int Enable_INTC_DSPI_SR_TCF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.TCFRE      = 1;
    dspi->MCR.B.HALT        = halt;
}


static int Disable_INTC_DSPI_SR_TCF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.TCFRE      = 0;
    dspi->MCR.B.HALT        = halt;
}


static int Enable_INTC_DSPI_SR_RFDF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.RFDFRE     = 1;
    dspi->MCR.B.HALT        = halt;
}


static int Disable_INTC_DSPI_SR_RFDF(volatile struct DSPI_tag *dspi)
{
    int halt;
    
    halt                    = (int)dspi->MCR.B.HALT;
    dspi->MCR.B.HALT        = 0b1;
    dspi->RSER.B.RFDFRE     = 0;
    dspi->MCR.B.HALT        = halt;
}


DSPI_RES Open_DSPI_Dev(struct DSPI_Device_Data *dev)
{
    OS_CPU_SR  cpu_sr = 0;
    
    OS_ENTER_CRITICAL();
    if (DSPI_STATUS_IDEL == dev->status)
    {
        dev->status = DSPI_STATUS_OPENED;
        OS_EXIT_CRITICAL();
        Disable_INTC_DSPI_SR_All(dev->dspi);
        dev->dspi->SR.R         = 0x00000000;
        dev->CB_TX_Complete     = NULL;
        return DSPI_RES_OK;
    }
    else
    {
        OS_EXIT_CRITICAL();
        return DSPI_RES_BUS_BUSY;
    }
}


DSPI_RES Close_DSPI(struct DSPI_Device_Data *dev)
{
    if (DSPI_STATUS_OPENED == dev->status)
    {
        Disable_INTC_DSPI_SR_All(dev->dspi);
        dev->dspi->SR.R         = 0x00000000;
        dev->CB_TX_Complete     = NULL;
        dev->status = DSPI_STATUS_IDEL;
        return DSPI_RES_OK;
    }
    else
    {
        return DSPI_RES_ERR;
    }
}


int Set_DSPI_CTAR(struct DSPI_Device_Data *dev, int dbr, int cpol, int cpha,int lsbfe,int pcssck,int pasc,int pdt,int pbr,int cssck,int asc,int dt,int br)
{
    dev->CTAR.B.DBR = dbr;
    dev->CTAR.B.FMSZ = DSPI_CTAR_FMSZ_2BYTES;                              /* Avoid reserved value */
    dev->CTAR.B.CPOL = cpol;
    dev->CTAR.B.CPHA = cpha;
    dev->CTAR.B.LSBFE = lsbfe;
    dev->CTAR.B.PCSSCK = pcssck;
    dev->CTAR.B.PASC = pasc;
    dev->CTAR.B.PDT = pdt;
    dev->CTAR.B.PBR = pbr;
    dev->CTAR.B.CSSCK = cssck;
    dev->CTAR.B.ASC = asc;
    dev->CTAR.B.DT = dt;
    dev->CTAR.B.BR = br;
    dev->dspi->MCR.B.HALT = 1;
    dev->dspi->CTAR[0].R = dev->CTAR.R;
    dev->CTAR.B.FMSZ = DSPI_CTAR_FMSZ_1BYTE;
    dev->dspi->CTAR[1].R = dev->CTAR.R;
    dev->dspi->MCR.B.HALT = 0;
    return DSPI_ERR_NONE;
}


int Set_DSPI_PUSHR(struct DSPI_Device_Data *dev, int cont, int pcs)
{
    if (DSPI_PUSHR_PCS_ALL < pcs || DSPI_PUSHR_PCS_NONE > pcs)
    {
        return DSPI_ERR_PUSHR_PCS_INVALID;
    }
    dev->PUSHR.B.CONT = cont;
    dev->PUSHR.R &= 0xFFC0FFFF;
    dev->PUSHR.R |= (uint32_t)pcs << 16;
    return DSPI_ERR_NONE;
}


int DSPI_SYNC_Send_and_Receive_Data(struct DSPI_Device_Data *dev, const uint8_t send_data[], uint8_t rev_data[], int cnt)
{
    int res = DSPI_ERR_NONE;
    uint8_t popr[DSPI_ASYNC_SEND_DATA_MAX_LENGTH];
    
    dev->PUSHR.B.EOQ = 0;
    Disable_INTC_DSPI_SR_EOQF(dev->dspi);
    if (DSPI_ERR_NONE != (res = DSPI_Push_Data_to_Empty_FIFO(dev, send_data, rev_data, cnt)))
    {
        return res;
    }
    while(!dev->dspi->SR.B.EOQF) {}
    dev->dspi->SR.B.EOQF = 1;
    {
        int quotient, remainder, i, rev_cnt = 0;
        
        quotient = cnt / DSPI_PUSHR_MAX_BYTE_AMOUNT;
        remainder = cnt % DSPI_PUSHR_MAX_BYTE_AMOUNT;
        for (i = 0; i < quotient; i++)
        {
            *(uint16_t *)(popr + rev_cnt) = (uint16_t)(dev->dspi->POPR.R);
            rev_cnt += 2;
        }
        if (remainder)
        {
            *(uint8_t *)(popr + rev_cnt) = (uint8_t)(dev->dspi->POPR.R);
            rev_cnt++;
        }
        if (NULL != rev_data)
        {
            while (rev_cnt--)
            {
                rev_data[rev_cnt] = popr[rev_cnt];
            }
        }
    }
    return DSPI_ERR_NONE;
}


/*
 * If you don't need the callback function, then set it NULL
 */
int Set_DSPI_Callback_TX_Complete(struct DSPI_Device_Data *dev, void(*fun)(void))
{
    dev->CB_TX_Complete = fun;
    return DSPI_ERR_NONE;
}


int DSPI_ASYNC_Send_and_Receive_Data(struct DSPI_Device_Data *dev, uint8_t send_data[], uint8_t rev_data[], int cnt)
{
    int res = DSPI_ERR_NONE;
    
    dev->PUSHR.B.EOQ = 0;
    Enable_INTC_DSPI_SR_EOQF(dev->dspi);
    if (DSPI_ERR_NONE != (res = DSPI_Push_Data_to_Empty_FIFO(dev, send_data, rev_data, cnt)))
    {
        return res;
    }
    return DSPI_ERR_NONE;
}


int DSPI_Push_Data_to_Empty_FIFO(struct DSPI_Device_Data *dev, uint8_t const send_data[], uint8_t rev_data[], int cnt)
{
    int quotient, remainder, i, rev_cnt = 0;
    
    (void)rev_data;
    if (dev->dspi->SR.B.TXCTR)
    {
        return DSPI_ERR_DEVICE_BUSY;
    }
    if (cnt > DSPI_ASYNC_SEND_DATA_MAX_LENGTH)
    {
        return DSPI_ERR_TX_DATA_TOO_LONG;
    }
    dev->PUSHR.B.CTAS = 0;
    dev->PUSHR.B.EOQ = 0;
    quotient = cnt / DSPI_PUSHR_MAX_BYTE_AMOUNT;
    remainder = cnt % DSPI_PUSHR_MAX_BYTE_AMOUNT;
    for (i = 0; i < quotient; i++)
    {
        if (!remainder && i == quotient - 1)
        {
            dev->PUSHR.B.EOQ = 1;
        }
        dev->PUSHR.B.TXDATA = *((uint16_t *)(send_data) +i);
        dev->dspi->PUSHR.R = dev->PUSHR.R;
    }
    if (remainder)
    {
        dev->PUSHR.B.EOQ = 1;
        dev->PUSHR.B.CTAS = 1;
        dev->PUSHR.B.TXDATA = (uint32_t)*(send_data + i * DSPI_PUSHR_MAX_BYTE_AMOUNT);
        dev->dspi->PUSHR.R = dev->PUSHR.R;
    }
    return DSPI_ERR_NONE;
}


void Disable_Watchdog(void)
{
    SWT.SR.R = 0x0000c520;  /* rite keys to clear soft lock bit */
    SWT.SR.R = 0x0000d928;
    SWT.CR.R = 0x8000010A;  /* Clear watchdog enable (WEN) */
}


void Init_ModesAndClock(void)
{
    ME.MER.R = 0x0000001D;  /* Enable DRUN, RUN0, SAFE, RESET modes */
    /* 设置sysclk */
    //CGM.FMPLL_CR.R = 0x02400100;  /* 8 MHz xtal: Set PLL0 to 64 MHz */
    CGM.FMPLL_CR.R = 0x01280000;    /* 8 MHz xtal: Set PLL0 to 80 MHz */
    //CGM.FMPLL_CR.R = 0x013C0000;  /* 8 MHz xtal: Set PLL0 to 120 MHz */
    ME.RUN[0].R = 0x001F0064;   /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclk选择锁相环时钟 */
    ME.RUNPC[0].R = 0x00000010; /* Peri. Cfg. 1 settings: only run in RUN0 mode      选择RUN0模式 */

    /* PCTL[?] 选择需要时钟模块(默认即可，不用设置) */
    //ME.PCTL[32].R = 0x00; /* MPC56xxB/P/S ADC 0: select ME.RUNPC[0] */
    ME.PCTL[32].B.DBG_F = 0;
    /* Mode Transition to enter RUN0 mode */
    ME.MCTL.R = 0x40005AF0; /* Enter RUN0 Mode & Key */
    ME.MCTL.R = 0x4000A50F; /* Enter RUN0 Mode & Inverted Key */

    while (ME.GS.B.S_MTRANS) {} /* Wait for mode transition to complete 等待模式转换完成 */
    while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode 等待选择RUN0模式 */

    /* 开peri0、1、2 */
    /* 外设时钟总线 可用于分频 */
    CGM.SC_DC[0].R = 0x84;  /* LIN */
    CGM.SC_DC[1].R = 0x80;  /* FLEXCAN,DSPI */
    CGM.SC_DC[2].R = 0x80;  /* eMIOS,CTU,ADC */
}


void Enable_IRQ(void)
{
    INTC.CPR.B.PRI = 0; /* Single Core: Lower INTC's current priority */
    asm(" wrteei 1");   /* Enable external interrupts */
}


void Dummy(void *)
{
    
}
