#ifndef MYHEADER
#define MYHEADER

#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define color1 0xC
#define color2 0xA
#define color3 0x6
#define color4 0xE
#define LIGHTBLUE 0xC
#define YELLOW 0xA
#define VIOLET 0x6
#define WHITE 0xE

//float temp = 0; //this will keep track of the temparature
#define RCGCGPIO       (*((volatile uint32_t *)0x400FE108))
#define GPIODIR_PORT_F       (*((volatile uint32_t *) 0x40025400)) //Register to set all the pins in port F to Input/Output mode for port F
#define GPIODEN_PORT_F       (*((volatile uint32_t *) 0x4002551C)) //Register to set corresponding pin to Input/Output mode for port F
#define GPIODATA_PORT_F       (*((volatile uint32_t *)0x400253FC)) // Register to write/read data from the corresponding pin for port F
#define GPIOLOCK_PORT_F       (*((volatile uint32_t *)0x40025520)) //enable writing to GPIOCR register
#define GPIOCR_PORT_F       (*((volatile uint32_t *)0x40025524)) 
#define GPIOPUR_PORT_F       (*((volatile uint32_t *)0x40025510))
//port A configs
#define GPIOPCTL_PORT_A        (*((volatile uint32_t *)0x4000452C))
#define GPIOAFSEL_PORT_A        (*((volatile uint32_t *)0x40004420))
#define GPIOAMSEL_PORT_A        (*((volatile uint32_t *)0x40004528)) //to configure analog functionality
#define GPIODEN_PORT_A       (*((volatile uint32_t *) 0x4000451C)) //Register to set corresponding pin to Input/Output mode for port A
#define GPIODIR_PORT_A       (*((volatile uint32_t *) 0x40004400)) //Register to set all the pins in port F to Input/Output mode for port A
#define GPIODATA_PORT_A       (*((volatile uint32_t *)0x400043FC)) // Register to write/read data from the corresponding pin for port A
//GPIO port B configs
#define GPIODIR_PORT_B       (*((volatile uint32_t *) 0x40005400)) //Register to set all the pins in port F to Input/Output mode for port B
#define GPIODEN_PORT_B       (*((volatile uint32_t *) 0x4000551C)) //Register to set corresponding pin to Input/Output mode for port B
#define GPIOAFSEL_PORT_B        (*((volatile unsigned long *)0x40005420))

// global timer configuration -- this deals with timer 0,1,2,3,4
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))

//timer 0 configurations
#define GPTMCTL (*((volatile uint32_t *)0x4003000C))
#define GPTMCFG (*((volatile uint32_t *)0x40030000))
#define GPTMTAMR (*((volatile uint32_t *)0x40030004))
#define GPTMTAILR (*((volatile uint32_t *)0x40030028))
#define GPTMRIS (*((volatile uint32_t *)0x4003001C))
#define GPTMICR (*((volatile uint32_t *)0x40030024))

//timer 1 configurations
#define GPTMCTL_timer1 (*((volatile uint32_t *)0x4003100C))
#define GPTMCFG_timer1 (*((volatile uint32_t *)0x40031000))
#define GPTMTAMR_timer1 (*((volatile uint32_t *)0x40031004))
#define GPTMTAILR_timer1 (*((volatile uint32_t *)0x40031028))
#define GPTMRIS_timer1 (*((volatile uint32_t *)0x4003101C))
#define GPTMICR_timer1 (*((volatile uint32_t *)0x40031024))
//timer 2 configurations
#define GPTMCTL_timer2 (*((volatile uint32_t *)0x4003200C))
#define GPTMCFG_timer2 (*((volatile uint32_t *)0x40032000))
#define GPTMTAMR_timer2 (*((volatile uint32_t *)0x40032004))
#define GPTMTAILR_timer2 (*((volatile uint32_t *)0x40032028))
#define GPTMRIS_timer2 (*((volatile uint32_t *)0x4003201C))
#define GPTMICR_timer2 (*((volatile uint32_t *)0x40032024))

//Interrupts
#define GPTMIMR (*((volatile uint32_t *)0x40030018)) //for timer 0
#define GPTMIMR_timer1 (*((volatile uint32_t *)0x40031018)) //for timer 1
#define GPTMIMR_timer2 (*((volatile uint32_t *)0x40032018)) //for timer 2
#define EN0 (*((volatile uint32_t *)0xE000E100))
#define EN1 (*((volatile uint32_t *)0xE000E104))

//GPIO configs
//PORT F
#define GPIOIS_PORT_F (*((volatile uint32_t *)0x40025404))
#define GPIOIBE_PORT_F (*((volatile uint32_t *)0x40025408))
#define GPIOIEV_PORT_F (*((volatile uint32_t *)0x4002540C))
#define GPIOIM_PORT_F (*((volatile uint32_t *)0x40025410))

//PORT A
#define GPIOIS_PORT_A (*((volatile uint32_t *)0x40004404))
#define GPIOIBE_PORT_A (*((volatile uint32_t *)0x40004408))
#define GPIOIEV_PORT_A (*((volatile uint32_t *)0x4000440C))
#define GPIOIM_PORT_A (*((volatile uint32_t *)0x40004410))

#define RCC (*((volatile uint32_t *)0x400FE060))
#define RCC2 (*((volatile uint32_t *)0x400FE070))

//raw interrupt status Regsiter(RIS)
#define RIS (*((volatile uint32_t *)0x400FE050)) //6th bit contain PLLLRIS, 1 -> READY TO USE, 0 -> not ready

#define RCGCADC (*((volatile uint32_t *)0x400FE638))
#define ADCACTSS (*((volatile uint32_t *)0x40038000))
#define ADCEMUX (*((volatile uint32_t *)0x40038014))
#define ADCSSMUX3 (*((volatile uint32_t *)0x400380A0))
#define ADCSSCTL3 (*((volatile uint32_t *)0x400380A4))
#define ADCPSSI (*((volatile uint32_t *)0x40038028))
#define ADCRIS (*((volatile uint32_t *)0x40038004))
#define ADCSSFIFO3 (*((volatile uint32_t *)0x400380A8))

#define ADCIM (*((volatile uint32_t *)0x40038008))


//UART defintions
#define RCGCUART (*((volatile uint32_t *)0x400FE618))
#define UARTCTL (*((volatile uint32_t *)0x4000C030))
#define UARTIBRD (*((volatile uint32_t *)0x4000C024))
#define UARTFBRD (*((volatile uint32_t *)0x4000C028))
#define UARTCC (*((volatile uint32_t *)0x4000CFC8))
#define UARTLCRH (*((volatile uint32_t *)0x4000C02C))
#define UARTFR (*((volatile uint32_t *)0x4000C018))
#define UARTDR (*((volatile uint32_t *)0x4000C000))



#endif 
