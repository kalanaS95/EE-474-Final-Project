#include <stdbool.h>
#include "SSD2119.c"

void intitialize_interrupts(void);
void initialize_timers(void);
void initialize_Timer0A(void);
void initialize_Timer1A(void);
void Timer_Handler_A(void);
void initialize_GPIOC(void);
void initialize_GPIOE(void);
void initialize_GPIOF(void);
void UART4_Init(void);
void UART3_Init(void);
void TransmitData(char* data);
char readFromBluetoothModule(void);
void AndroidToTivaController(void);
void mainScreen();
void drawStatus(void);
void set80Mhz(void);
uint32_t distance_measurement(void);
void ControllerHelper(void);


/*Varaibles needed to Ultrasonic sensor*/
uint32_t    pulseWidth;
double      distance;
uint16_t    count = 0;

char* defaultBell_tone = "a"; //this will keep track of default door bell tone
int doorBellVol = 204; //always starts at maximum volume --> this is how sound board is default to when start, also keep track of door bell volume
bool doorLockStatus = false; //false -> unlocked, true-> locked

int main()
{
  //initializing
  set80Mhz();
  LCD_Init();
  initialize_GPIOC();
  initialize_GPIOE();
  initialize_GPIOF();
  initialize_timers();
  intitialize_interrupts();
  UART4_Init();
  UART3_Init();
  //end of initialzing

  //when program starts we make sure it starts with door lock unlocked
  TransmitData("0");
  while(true)
  {
    mainScreen();
  }

  return 0;
}


/*This function acts as a helper function that constantly checks for any data from Bluetooth or
button presses and draws initial information on the screen when system is started*/
void mainScreen()
{
  LCD_ColorFill(Color4[0]); //change background to black
  LCD_DrawRect(0,0,318,180,Color4[15]);//draw a rectangle
  drawStatus();
  while(true)
  {

    AndroidToTivaController(); //calling this function infinitly, so it can detect information coming from bluetooth module

  }
}

/*This function draws the door bell status (locked or unlocked), user selected door bell tone
and user selected door bell volume to the screen*/
void drawStatus(void)
{
  //drawing dor lock status
  LCD_SetTextColor(255,255,255); //text in white
  LCD_SetCursor(15,192);
  printf("Door Lock Status:");

  LCD_SetCursor(120,192);
  if(!doorLockStatus)
  {
    LCD_SetTextColor(0,255,0); //print in green
    printf("Unlocked\r\n");
  }
  else
  {
    LCD_SetTextColor(255,0,0); //print in red
    printf("Locked  \r\n");
  }

  //drawing user selcted door bell tone
  LCD_SetCursor(15,207);
  LCD_SetTextColor(255,255,255); //text in white
  printf("Door Bell Tone  :");
  LCD_SetCursor(120,207);
  printf("             ");
  LCD_SetCursor(120,207);
  printf("Tone #%d\r\n",defaultBell_tone[0]-96);

  //drawing user selected door bell volume
  LCD_SetCursor(15,222);
  LCD_SetTextColor(255,255,255); //text in white
  printf("Door Bell Volume:");
  LCD_SetCursor(120,222);
  LCD_SetTextColor(255,255,0); //text in Yellow
  printf("        ");
  LCD_SetCursor(120,222);
  printf("%d\r\n",doorBellVol);

}


/*This function will enable interrupts for the Timer OA*/
void intitialize_interrupts(void)
{
  TIMER0_IMR_R = 0x01; //turning on Timer A Time-Out Interrupt Mask - Timer 0
  NVIC_EN0_R = 0x00080008;  //turning interrupts on for timer0A and PORT D at 3rd position

}

/*This function bascally turns on the clock for Timer 0A and call initialize Timer 0A function to initialize
timer 0A to tick each second when processor clock is 16Mhz (default clock)*/
void initialize_timers(void)
{
  SYSCTL_RCGCTIMER_R = 0x01; //enabling timer 0
  initialize_Timer0A();
  initialize_Timer1A();
}

/*This function will configure timer 0A to be count down mode and tick each second when
processor clock is 16Mhz*/
void initialize_Timer0A(void)
{
  TIMER0_CTL_R = 0x00; // disable timer for configuration
  TIMER0_CFG_R = 0x0; //selecting 32 bit mode
  TIMER0_TAMR_R = 0x002; // TAMR to periodic timer mode and TACDIR to count down mode
  TIMER0_TAILR_R = 0x00F42400; //0x00F42400; // setting 16000000 --> 1Hz
  TIMER0_CTL_R = 0x01; //finally enabling the timer
}


/*Interrupt handler for timer 0A */
void Timer_Handler_A(void)
{

  if (TIMER0_RIS_R & (1<<0) == 1) //means timer has ended
    TIMER0_ICR_R = 0x001;


}

/*This function will initialize GPIO PORT E 1,2 and 3 pins to recognize button inputs
PE1 -> Unlock btn
PE2-> Lock btn
PE3 -> door bell btn
*/
void initialize_GPIOE(void)
{
  unsigned int delay=0;
  SYSCTL_RCGC2_R |= (1<<4); //providing clock to GPIO E
  delay++;

  GPIO_PORTE_PCTL_R = 0xFFFF000F;
  GPIO_PORTE_AFSEL_R = 0xF1;
  GPIO_PORTE_AMSEL_R = 0xF1;
  GPIO_PORTE_DEN_R = 0x0E;
  GPIO_PORTE_DIR_R &= ~(1<<1);
  GPIO_PORTE_DIR_R &= ~(1<<2);
  GPIO_PORTE_DIR_R &= ~(1<<3);

  GPIO_PORTE_RIS_R = 0x00;
  GPIO_PORTE_IBE_R = 0x00;
  GPIO_PORTE_IEV_R = 0x00;
  GPIO_PORTE_IM_R = 0xFF;

}


//------ UART ------------------------------------- FOR communication with Arduino
void initialize_GPIOC(void) {
  unsigned int delay = 0;
  // STEP 1:
  // Congigure the GPIO pins to enable UART function


  SYSCTL_RCGC2_R |= (1<<2);
  delay++;

  /*Configuring PC4 and 5 for UART with Arduino --> PC4 - RX and PC5 - TX*  --- UART4*/
  GPIO_PORTC_AFSEL_R |= (11<<4);//0x03;           // allow alternative function for PB 0&1
  GPIO_PORTC_DEN_R |= (11<<4);//0x03;             // enable digital pins
  GPIO_PORTC_PCTL_R |= (1<<20)|(1<<16);  // set alternative function to UART

  /*Configuring PC6 and 7 for UART with Arduino --> PC6 - RX and PC7 - TX   ---- UART3*/
  GPIO_PORTC_AFSEL_R |= (11<<6);//0x03;           // allow alternative function for PB 0&1
  GPIO_PORTC_DEN_R |= (11<<6);//0x03;             // enable digital pins
  GPIO_PORTC_PCTL_R |= (1<<28)|(1<<24);  // set alternative function to UART
}

/*Initialization of UART module 4 -- FOR communication with Arduino*/
void UART4_Init(void)
{
  // Configure the UART Module

  SYSCTL_RCGCUART_R |= (1<<4);                   // enable clock to UART Module 1
  int32_t delay = SYSCTL_RCGCUART_R;   // delay to wait for clock setup

  UART4_CTL_R = 0x0;                   // disable module before configuration

  // set the baud rate (integer & fraction parts)
  UART4_IBRD_R = 43;//104;//8;
  UART4_FBRD_R = 26;//11;//44;

  UART4_LCRH_R = 0x60;                  // set the transmission line control
  UART4_CC_R =0x0;                    // use system clock

  UART4_CTL_R =0x0301;//|= (1<<0);                 // enable UART Module


}

/*Initialization of UART module 3 -- FOR communication with Bluetooth module*/
void UART3_Init(void) //
{
  // Configure the UART Module

  SYSCTL_RCGCUART_R |= (1<<3);                   // enable clock to UART Module 1
  int32_t delay = SYSCTL_RCGCUART_R;   // delay to wait for clock setup

  UART3_CTL_R = 0x0;                   // disable module before configuration

  // set the baud rate (integer & fraction parts)
  UART3_IBRD_R = 43;//104;//8;
  UART3_FBRD_R = 26;//11;//44;

  UART3_LCRH_R = 0x60;                  // set the transmission line control
  UART3_CC_R =0x0;                    // use system clock

  UART3_CTL_R =0x0301;//|= (1<<0);                 // enable UART Module


}


/*This function allows to transmitt data through UART 4 --- communication with Arduino*/
void TransmitData(char* data)
{
  char* temp = data;

  while(*temp != '\0') {
    // Here you check the flag to see
      while((UART4_FR_R & (1<<5)) != 0){} //wait if the holding register becomes empty
      UART4_DR_R = *temp;

    temp++;
  }
}

/*This function allows to read recieved data from bluetooth module*/
char readFromBluetoothModule(void)
{
  char temp = 0;
  while(UART3_FR_R & (1<<4) !=0){} //wait till we got something to read
  temp = UART3_DR_R;

  return temp;
}

void ControllerHelper(void)
{
    static char previousChar = 0;
  	static int counter = 0;
  	char info = (char)readFromBluetoothModule();

  	/*The following conditional statements deals with physical button presses*/
    if((GPIO_PORTE_DATA_R & (1<<1)) == 0x2) //button connected to PE1
    {
      TransmitData("0"); //sending 0 to Ardunio through UART will trigger door unlock
      doorLockStatus = false; //manipulating global variable
      drawStatus(); //making updates in the screen
      return;
    }
    else if((GPIO_PORTE_DATA_R & (1<<2)) == 0x4)//button connected to PE2
    {
      TransmitData("1"); //sending 1 to Ardunio through UART will trigger door lock
      doorLockStatus = true; //manipulating global variable
      drawStatus(); //making updates in the screen
      return;
    }
    else if ((GPIO_PORTE_DATA_R & (1<<3)) == 0x8)//button connected to PE3
    {
      info = defaultBell_tone[0]; //on button press we change info variable to default tone such that it will trigger correct case in switch to produce door bell sound
    }

    /*This make sure same command is not called twice. E.g if user decides to press unlock twice
This helps to prevent motor/servo from damaging*/
  if(previousChar == info)
    return;

 /*This switch statememt deals with data coming from bluetooth module*/
  switch(info)
  {
    case '0': TransmitData("0"); //sending 0 to Ardunio through UART will trigger door unlock
              doorLockStatus = false; //manipulating global variable
              drawStatus(); //making updates in the screen
              break;
    case '1': TransmitData("1");//sending 1 to Ardunio through UART will trigger door lock
              doorLockStatus = true;
              drawStatus();
              break;
    case 'a': TransmitData("a"); //sending char "a" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "a"; //changing default tone to this one
              drawStatus(); // draw changes
              break;
    case 'b': TransmitData("b"); //sending char "b" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "b";
              drawStatus();
              break;
    case 'c': TransmitData("c"); //sending char "c" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "c";
              drawStatus();
              break;
    case 'd': TransmitData("d"); //sending char "d" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "d";
              drawStatus();
              break;
    case 'e': TransmitData("e"); //sending char "e" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "e";
              drawStatus();
              break;
    case 'f': TransmitData("f"); //sending char "f" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "f";
              drawStatus();
              break;
    case 'g': TransmitData("g"); //sending char "g" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "f";
              drawStatus();
              break;
    case 'h': TransmitData("h"); //sending char "h" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "h";
              drawStatus();
              break;
    case 'i': TransmitData("i"); //sending char "i" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "i";
              drawStatus();
              break;
    case 'j': TransmitData("j"); //sending char "j" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "j";
              drawStatus();
              break;
    case 'k': TransmitData("k"); //sending char "k" to arduino triggers first sound track in Adafruit sound board
              defaultBell_tone = "k";
              drawStatus();
              break;
    case '+': if(doorBellVol<204) //check to make sure we dont send sound board instructions to increase volume when its at maximum
              {
                TransmitData("+"); //sending char "+" to arduino triggers sound board to increase the volume
                doorBellVol = doorBellVol + 2; //sound is incremented by 2, according to the spec sheet of sound baord
              }
              drawStatus(); //draw changes
              break;
    case '-': if(doorBellVol>0) //check to make sure we dont send sound board instructions to increase volume when its at maximum
              {
                TransmitData("-"); //sending char "-" to arduino triggers sound board to decrease the volume
                doorBellVol = doorBellVol - 2; //sound is decremented by 2, according to the spec sheet of sound baord
              }
              drawStatus();//draw changes
              break;

  }

  previousChar = info;
}

/*This function acts as a controller. Depending on information recieved from bluetooth module. Tiva board will send data
to arduino to accomplish different tasks*/
void AndroidToTivaController(void)
{

    ControllerHelper();

    //---- ultra sonic
           // Set Trig pin to Low (PF3)
        GPIO_PORTF_DATA_R &= ~0x08;
        for (int j = 0; j < 200000; j++) {ControllerHelper();}

        // Set Trig pin to High for 10ns
        GPIO_PORTF_DATA_R |= 0x08;
        for (int j = 0; j < 500; j++) {ControllerHelper();}

        // Set "Trig" pin to Low
        GPIO_PORTF_DATA_R &= ~0x08;

        pulseWidth = distance_measurement();    // Call distance_measurement() to get the delta t on the "echo" pin

        // Calculate distance in inches
        distance = pulseWidth/148.0;

        // if less than 2ft, ring doorbell every so often and notify 
        if (distance <= 24){
          count++;
          if (count % 10 == 0){
            LCD_SetCursor(100,80);
            printf("Someone's at the door !\r\n");
            TransmitData(defaultBell_tone);

          }
        }else
          {
            LCD_SetCursor(100,80);
            printf("                        \r\n");
          }

        ControllerHelper();
    //-------------------
}

/*This will over clock the processor to work at 80Mhz*/
void set80Mhz(void)
{
  SYSCTL_RCC_R = 0x00000000; //reset register to remove garbage values
  SYSCTL_RCC2_R = 0x00000000; //reset register to remove garbage values
  //setting BYPASs
  SYSCTL_RCC_R |= (1<<11);
  //clear USESYSDIV
  SYSCTL_RCC_R &= ~(1<<22);
  //selecting crystal value from XTAL 0x15 . i.e 21 in decimal
  SYSCTL_RCC_R |= (1<<10);
  SYSCTL_RCC_R |= (1<<8);
  SYSCTL_RCC_R |= (1<<6);
  //getting write access to RCC2
  SYSCTL_RCC2_R |= (1<<31);
  //select oscillator source 0x0 for main oscillator
  SYSCTL_RCC2_R &= ~(1<<4);
  SYSCTL_RCC2_R &= ~(1<<5);
  SYSCTL_RCC2_R &= ~(1<<6);
  //clear power down bit
  SYSCTL_RCC2_R &= ~(1<<13);
  //select DIV400 to 1 to use 400mhz
  SYSCTL_RCC2_R |= (1<<30);
  //inorder to divide by 5 SYSDIVLSB should be 0
  SYSCTL_RCC2_R &= ~ (1<<22);
  //and value of SYSDIV2 should be 0x2
  SYSCTL_RCC2_R |= (1<<24);
  //set USESYSDIV in RCC -***
  SYSCTL_RCC_R |= (1<<22);
  //wait for the PLL to lock by polling PLLRIS
  while(!SYSCTL_RIS_R & (1<<6))
  {
  }
  //finally clear bypass2
  SYSCTL_RCC2_R &= ~(1<<11);

}

void initialize_GPIOF(void)
{
    int delay = 0;
	//Enable Clock to the GPIO_F Modules 
    SYSCTL_RCGCGPIO_R |= (1<<5);

    // allow time for clock to stabilize 
    delay++;

    // set all GPIO_PORTF are not analog
    GPIO_PORTF_AMSEL_R = 0x00;

    // set GPIOPCTL's bit 8,9,10 in PMC2 of GPIO_PORTF PCTL
    GPIO_PORTF_PCTL_R |= 0x0700;

    // SET Alternative function for PF2
    GPIO_PORTF_AFSEL_R |= 0x4;

    // PF2 input, PF3 output
    GPIO_PORTF_DIR_R &= ~(0x4);
    GPIO_PORTF_DIR_R |= 0x8;

    // Enable digital I/O for GPIO_F
    GPIO_PORTF_DEN_R |= 0xC;

}

void initialize_Timer1A(void)
{
    // Edge-timer capture mode

    // Enable Clock for TIMER Module
    SYSCTL_RCGCTIMER_R |= 0x2;
    while ((SYSCTL_PRTIMER_R & 0x2) == 0) {}

    // Disable TimerA & TimerB
    TIMER1_CTL_R &= ~(0x101);  

    // Split into two 16-bit timers
    TIMER1_CFG_R = 0x04;         

    // Configure Timer to capture mode (0x3) edge-time mode (0x4)
    TIMER1_TAMR_R = 0x7;

    // Configure Timer Event Mode to both-edges
    TIMER1_CTL_R &= ~0xFFFFFFFF; // 0 all
    TIMER1_CTL_R |=  (0x3<<2);  // Both edges

    // Configure Load
    TIMER1_TAILR_R = 0xFFFF;

    // Set the prescaler to 0xFF
    TIMER1_TAPR_R = 0xFF;
    TIMER1_IMR_R = 0;

    // Enable GPTM Timer
    TIMER1_CTL_R |= 0x101;  // Enable TimerB
}


uint32_t distance_measurement(void)
{
    uint32_t highEdge,lowEdge;
    uint32_t deltaT;

    // clear interrupt status
    TIMER1_ICR_R |= 0x4;

    // check interrupt status
    while ((TIMER1_RIS_R & 0x4) == 0){}

    // once interrupt status is set, capture the high edge

    highEdge = TIMER1_TAR_R;

    // clear interrupt status
    TIMER1_ICR_R |= 0x4;

    // check interrupt status
    while ((TIMER1_RIS_R & 0x4) == 0){}

    // once interrupt status is set, capture the low edge
    lowEdge = TIMER1_TAR_R;

    // the following returns the time. 0.0125ns corresponds to 80MHz
    deltaT = (highEdge - lowEdge) * 0.0125;
    return deltaT;
}
