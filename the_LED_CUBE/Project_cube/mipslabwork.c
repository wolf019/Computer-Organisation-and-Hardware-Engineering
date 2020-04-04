/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>


#define P0       0x10
#define P1       0x08
#define P2       0x04
#define P3       0x02
#define P4       0x01
#define BC0       0x10
#define BC1       0x08
#define BC2       0x04
#define BC3       0x02
#define BC4       0x01
#define AC0       0x08
#define AC1       0x10
#define AC2       0x20
#define AC3       0x40
#define AC4       0x80


int ITERCON = 100;
int PLANES = 0x00;
int WALL0A = 0x00;
int WALL1B = 0x00;
int WALL2A = 0x00;
int WALL3A = 0x00;
int WALL4B = 0x00;

int count = 0;
int pat_count = 0;

void wait();
void patt_func();
int plane_count();
int ac_iterator();
int bc_iterator();
void i2c_idl();
void i2c_start();
void i2c_restart() ;
void i2c_stop();
int i2c_send(uint8_t data) ;
uint8_t i2c_recv() ;
void i2c_sendadd(uint8_t data);
int i2c_ch_ack();
int var = 0;


volatile int * porte = (volatile int *) 0xbf886110;


/* Interrupt Service Routine */
void user_isr( void ) {
	return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
    TRISD = (127 << 5);
    TRISF = (1 << 1);
	/* Set up peripheral bus clock */
	OSCCON &= ~0xC0000;  // Set up peripheral bus clock with freq- 40khz
	OSCCON |= 0x080000;
	
	// DEVCFG1 |= (1<<12);
	// double BRG;

    I2C1CON = 0;            // Turn off I2C1 module
    I2C1CON |= (1<<9); // Disable slew rate for 100kHz  /*    BRG = (1 / (2 * 100000)) - 0.000000104; BRG *= (40000000) - 2;  */     
    I2C1BRG = 0xc2;     // Set baud rate
	I2C1STAT = 0;	// clear status register.
    I2C1CONSET = (1<<15);     // Turn on I2C1 module
	I2C1CONSET = 1 << 13; //SIDL = 1

	
	volatile int *trise = (volatile int *)0xbf886100;
	*trise = *trise & 0xffffff00; 
	*porte = 0;
	
	
	i2c_start();  // set gpio b to outputs
	i2c_send(0x40); // 0x22 - 34 , 0x20 - 32
	i2c_send(0x01);	 // 0x01 - iodirb  0x00 - iodira
	i2c_send(0x00);	// 0x 00 - sets to outputs
	i2c_stop(); 
	
	i2c_start();  // set gpio b to outputs
	i2c_send(0x40); // 0x22 - 34 , 0x20 - 32
	i2c_send(0x00);	 // 0x01 - iodirb  0x00 - iodira
	i2c_send(0x00);	// 0x 00 - sets to outputs
	i2c_stop(); 
	
	i2c_start();  // set gpio b to outputs
	i2c_send(0x42); // 0x22 - 34 , 0x20 - 32
	i2c_send(0x01);	 // 0x01 - iodirb  0x00 - iodira
	i2c_send(0x00);	// 0x 00 - sets to outputs
	i2c_stop(); 
	
	i2c_start();  // set gpio b to outputs
	i2c_send(0x42); // 0x22 - 34 , 0x20 - 32
	i2c_send(0x00);	 // 0x01 - iodirb  0x00 - iodira
	i2c_send(0x00);	// 0x 00 - sets to outputs
	i2c_stop(); 
	
	i2c_start();  // set gpio b to outputs
	i2c_send(0x44); // 0x22 - 34 , 0x20 - 32
	i2c_send(0x01);	 // 0x01 - iodirb  0x00 - iodira
	i2c_send(0x00);	// 0x 00 - sets to outputs
	i2c_stop(); 
	
	i2c_start();  // set gpio b to outputs
	i2c_send(0x44); // 0x22 - 34 , 0x20 - 32
	i2c_send(0x00);	 // 0x01 - iodirb  0x00 - iodira
	i2c_send(0x00);	// 0x 00 - sets to outputs
	i2c_stop(); 
	
	return;
}




/* This function is called repetitively from the main program */
void labwork( void ) {
    
    
	/*Previous code */
	{
	// while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)){};
	// I2C1CON = I2C1CON | 1 | (1<<19);  // start seq
	// PORTE = 1;

		
	// I2C1ADD = 0x40; //------> try 0x40;
	// PORTE = PORTE +1;
	 // I2C1STAT |= (0x004 ); // write (/read)

	//I2C1STAT &= (0xffff7fff ); // ack to 0
	// PORTE = PORTE +1;
	// while(I2C1STAT & 0x8000){}
	
	// I2C1TRN = 0x12;
	// PORTE = PORTE +1;

	// I2C1CON = I2C1CON | 0x4;
	// PORTE = PORTE +1;
	}
  
    int sw = getsw();
    
    if(sw == 0x00){
        no_led();
        ITERCON = 0;
    }
    else if(sw == 0x01){
        ITERCON = 600;
    }
    else if(sw == 0x02){
        ITERCON = 500;
    }
    else if(sw == 0x03){
        ITERCON = 400;
    }
    else if(sw == 0x4){
         ITERCON = 200;
     }
    else if(sw == 0x05){
        ITERCON = 100;
    }
    else if(sw == 0x06){
         ITERCON = 50;
     }
    else if(sw == 0x07){
         ITERCON = 15;
     }
    else if(sw == 0x08){
        ITERCON = 10;
    }
    
    else if(sw == 0xF){
        every_led();
    }
    
    patt_func();

    i2c_start();        // output 01010101 to gpio A controller 2
    i2c_send(0x44);     // resend
    i2c_send(0x12);     // 0x12 - 18 gpioa , 0x13 -19 gpiob
    i2c_send(WALL0A);     // Wall 0
    i2c_stop();
    
    i2c_start();        // output 10101010 to the gpio B controller 1
    i2c_send(0x42);     // resend
    i2c_send(0x13);     // 0x12 - 18 gpioa , 0x13 -19 gpiob
    i2c_send(WALL1B);   // Wall 1
    i2c_stop();
    
    i2c_start();       // output 01010101 to gpio A controller 1
    i2c_send(0x42);    // resend
    i2c_send(0x12);    // 0x12 - 18 gpioa , 0x13 -19 gpiob
    i2c_send(WALL2A);    // Wall 2
    i2c_stop();
    
    i2c_start();       // output 01010101 to gpio A controller 0
    i2c_send(0x40);    // resend
    i2c_send(0x12);    // 0x12 - 18 gpioa , 0x13 -19 gpiob
    i2c_send(WALL3A);    // Wall 3
    i2c_stop();
        
	i2c_start();       // output 10101010 to the gpio B controller 0
	i2c_send(0x40);    // resend
	i2c_send(0x13);	   // 0x12 - 18 gpioa , 0x13 -19 gpiob
	i2c_send(WALL4B);	   //  Wall 4
	i2c_stop();
	
	i2c_start();       // output 10101010 to the gpio B controller 2
	i2c_send(0x44);    // resend
	i2c_send(0x13);	   // 0x12 - 18 gpioa , 0x13 -19 gpiob
	i2c_send(PLANES);	   // Planes
	i2c_stop();
	
}

/* Wait for I2C bus to become idle */
void i2c_idle() 
{
	while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)); //----> check if any start stop ack is on, else it is 0 and stat checks for 
													//     	transmission on progress.
}

/* Send start conditon on the bus */
void i2c_start() {
	i2c_idle();
	I2C1CONSET = 1 << 0; //SEN
	// while (I2C2CON & 1);
	// *porte |= (1<<0);
	i2c_idle();
}

/* Send restart conditon on the bus */
void i2c_restart() {
	i2c_idle();
	I2C1CONSET = 1 << 1; //RSEN
	// while (I2C2CON & 0x2);
	// *porte |= (1<<4);
	i2c_idle();
}

/* Send stop conditon on the bus */
void i2c_stop() {
	i2c_idle();
	 I2C1CONSET = 1 << 2; //PEN
	// *porte |= (1<<5);
	i2c_idle();
}

/* Send one byte on I2C bus, return ack/nack status of transaction */
int i2c_send(uint8_t data) {
	i2c_idle(); //
	I2C1TRN = data;
	// while(I2C2STAT & (1));
	// *porte |= (1<<1);
	i2c_idle();
	while (I2C2STAT &( 1<<15));
}

/* Receive one byte from I2C bus */    // todo******
uint8_t i2c_recv() {
	i2c_idle();
	I2C2CONSET = 1 << 3; //RCEN = 1
	i2c_idle();
	I2C2STATCLR = 1 << 6; //I2COV = 0
	return I2C2RCV;
}

/* Send acknowledge conditon on the bus */     //todo*****
void i2c_ack() {
	i2c_idle();
	I2C2CONCLR = 1 << 5; //ACKDT = 0
	I2C2CONSET = 1 << 4; //ACKEN = 1
}

/* Send not-acknowledge conditon on the bus */		//todo*****
void i2c_nack() {
	i2c_idle();
	I2C2CONSET = 1 << 5; //ACKDT = 1
	I2C2CONSET = 1 << 4; //ACKEN = 1
}

//int plane_count(){
//
//    if(count == 0){
//        count++;
//        return 1;
//    }
//    else if (count != 5){
//        count++;
//    }
//    else{
//        count_send = 1;
//        count = 0;
//    }
//
//    return count_send *= 2;
//}

int getsw( void ){
    
    return (PORTD >> 8) & 0x0F;
}



int getbtns( void ){
    
    return (PORTD >> 5) & 0x07;
}

int plane_count(){
    if(count <= ITERCON){
        count++;
        return P4;
    }
    else if (count <= ITERCON * 2){
        count++;
        return P3;
    }
    else if(count <= ITERCON * 3){
        count++;
        return P2 | P4;
    }
    else if(count <= ITERCON * 4){
        count++;
        return P1 | P2;
    }
    else if(count <= ITERCON * 5){
        count++;
        return P0 | P3 | P2;
    }
    else{
        count = 0;
        return 0;
    }

}

int plane_count_down(){
    
    if(count <= ITERCON){
        count++;
        return P4;
    }
    else if (count <= ITERCON * 2){
        count++;
        return P3;
    }
    else if(count <= ITERCON * 3){
        count++;
        return P2;
    }
    else if(count <= ITERCON * 4){
        count++;
        return P1;
    }
    else if(count <= ITERCON * 5){
        count++;
        return P0;
    }
    else{
        count = 0;
        return 0;
    }

}


int ac_iterator(){
    
    if(count == ITERCON || count == 200 || count == 300 || count == 400 || count == 500 ){
        return 0x00;
    }
    else if(count < ITERCON){
        return AC0 | AC4;
    }
    else if (count < 200){
        return AC1 | AC3;
    }
    else if(count < 300){
        return AC2 | AC0 | AC4;
    }
    else if(count < 400){
        return AC3 | AC1;
    }
    else if(count < 500){
        return AC4 | AC0;
    }

}

int bc_iterator(){
    
    if(count == ITERCON || count == ITERCON * 2 || count == ITERCON * 3 || count == ITERCON * 4 || count == ITERCON * 5 ){
        return 0x00;
    }
    else if(count < ITERCON){
        return BC1 | BC3;
    }
    else if (count < ITERCON * 2){
        return BC4 | BC1;
    }
    else if(count < ITERCON * 3){
        return BC2;
    }
    else if(count < ITERCON * 4){
        return BC4 | BC0;
    }
    else if(count < ITERCON * 5){
        return BC3 | BC1;
    }

}

void patt_func(){
    
    int btns = getbtns();
    
    
    if(btns & 1){
    
    PLANES = 0xff;
    
    if(pat_count < ITERCON){
        pat_count++;
        WALL0A = 0xff;
    }
    else if(pat_count < ITERCON * 2){
        WALL0A = 0x00;
        pat_count++;
        WALL1B = 0xff;
    }
    else if(pat_count < ITERCON * 3){
        WALL1B = 0x00;
        pat_count++;
        WALL2A = 0xff;
    }
    else if (pat_count < ITERCON * 4){
        WALL2A = 0x00;
        pat_count++;
        WALL3A = 0xff;
    }
    else if (pat_count < ITERCON * 5){
        WALL3A = 0x00;
        pat_count++;
        WALL4B = 0xff;
    }
    else{
        WALL4B = 0x00;
        pat_count = 0;
    }
    }
    
    if(btns & 2){
    
    PLANES = plane_count();
    
    if(pat_count < ITERCON ){
        WALL0A = ac_iterator();
        pat_count++;
    }
    else if(pat_count < ITERCON * 2){
        WALL1B = bc_iterator();
        pat_count++;
    }
    else if(pat_count < ITERCON * 3){
        WALL2A = ac_iterator();
        pat_count++;
    }
    else if (pat_count < ITERCON * 4){
        pat_count++;
        WALL3A = ac_iterator();
    }
    else if (pat_count < ITERCON * 5){
        pat_count++;
        WALL4B = bc_iterator();
    }
    else{
        pat_count = 0;
    }
    }
    
    if(btns & 4){
    
    PLANES = plane_count();
    
    if(pat_count < ITERCON ){
        pat_count++;
        WALL0A = 0xff;
    }
    else if(pat_count < ITERCON * 2){
        pat_count++;
        WALL1B = 0xff;
    }
    else if(pat_count < ITERCON * 3){
        pat_count++;
        WALL2A = 0xff;
    }
    else if (pat_count < ITERCON * 4){
        pat_count++;
        WALL3A = 0xff;
    }
    else if (pat_count < ITERCON * 5){
        pat_count++;
        WALL4B = 0xff;
    }
    else{
        pat_count = 0;
    }
    }
    
    if(((PORTF >> 1) & 0x1) & (getsw() != 0X00)){
        
        PLANES = plane_count_down();
        WALL0A = 0xff;
        WALL1B = 0xff;
        WALL2A = 0xff;
        WALL3A = 0xff;
        WALL4B = 0xff;
    
    }
}

void every_led(){
    
    PLANES = P0 || P1 || P2 || P3 || P4;
    WALL0A = 0xff;
    WALL1B = 0xff;
    WALL2A = 0xff;
    WALL3A = 0xff;
    WALL4B = 0xff;

}

void no_led(){
    
    PLANES = 0x00;
    WALL0A = 0x00;
    WALL1B = 0x00;
    WALL2A = 0x00;
    WALL3A = 0x00;
    WALL4B = 0x00;

}

