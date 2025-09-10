#include<reg51.h>
#include <stdio.h>

// Define LCD connections
#define display_port P2      // Data pins connected to port 2 on microcontroller

sbit rs = P3^2;  // RS pin connected to pin 2 of port 3
sbit rw = P3^3;  // RW pin connected to pin 3 of port 3
sbit e  = P3^4;  // E pin connected to pin 4 of port 3

// Define Ultrasonic Sensor Pins (using Port 1 to avoid conflict with LCD on Port 2)
sbit Trigger_pin = P1^6;
sbit Echo_pin    = P1^7;

// Function to create delay in ms
void msdelay(unsigned int time) {
    unsigned int i, j;
    for(i = 0; i < time; i++)
        for(j = 0; j < 1275; j++);
}

// Delay of ~10 microseconds
void Delay_us() {
    TL0 = 0xF5;
    TH0 = 0xFF;
    TR0 = 1;
    while (TF0 == 0);
    TR0 = 0;
    TF0 = 0;
}

// LCD command function
void lcd_cmd(unsigned char command) {
    display_port = command;
    rs = 0;
    rw = 0;
    e = 1;
    msdelay(1);
    e = 0;
}

// LCD data function
void lcd_data(unsigned char disp_data) {
    display_port = disp_data;
    rs = 1;
    rw = 0;
    e = 1;
    msdelay(1);
    e = 0;
}

// LCD initialization
void lcd_init() {
    lcd_cmd(0x38);  // 2 lines, 5x7 matrix
    msdelay(10);
    lcd_cmd(0x0E);  // Display on, cursor blinking
    msdelay(10);
    lcd_cmd(0x01);  // Clear display
    msdelay(10);
    lcd_cmd(0x06);  // Cursor shift right
    msdelay(10);
    lcd_cmd(0x80);  // Cursor to first line
    msdelay(10);
}

// Send a string to LCD
void lcd_string(unsigned char *str) {
    while(*str) {
        lcd_data(*str++);
    }
}

// Move cursor to position
void lcd_cmd_xy(unsigned char row, unsigned char col) {
    unsigned char pos;
    if(row == 1)
        pos = 0x80 + (col - 1);
    else
        pos = 0xC0 + (col - 1);
    lcd_cmd(pos);
}

// Timer initialization
void init_timer() {
    TMOD = 0x01; // Timer0 mode1
    TF0 = 0;
    TR0 = 0;
}

// Send 10us trigger pulse
void send_trigger_pulse() {
    Trigger_pin = 1;
    Delay_us();
    Trigger_pin = 0;
}

void main() {
    float distance_measurement, value;
    unsigned int timer_count;
    unsigned char distance_in_cm[10];

    lcd_init();                // Initialize LCD
    init_timer();              // Initialize Timer

    lcd_cmd_xy(1,1);
    lcd_string("Distance:");

    while(1) {
        send_trigger_pulse();            // Send 10us pulse

        while(!Echo_pin);                // Wait for echo HIGH
        TR0 = 1;                         // Start Timer0

        while(Echo_pin && !TF0);         // Wait until echo LOW or timeout
        TR0 = 0;                         // Stop timer

        timer_count = (TH0 << 8) | TL0;  // Read timer value
        TH0 = 0;                         // Clear timer
        TL0 = 0;

        value = 34300.0 / 2.0;           // Sound speed (cm/s) / 2 (round trip)
        distance_measurement = (float)timer_count * 1.085e-6 * value; // (clock period × count × speed)

        sprintf(distance_in_cm, "%4.2f cm", distance_measurement);

        lcd_cmd_xy(2,1);
        lcd_string("            ");      // Clear line
        lcd_cmd_xy(2,1);
        lcd_string(distance_in_cm);

        msdelay(200);  // Small delay between measurements
    }
}