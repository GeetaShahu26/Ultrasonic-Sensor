Distance Measurement using HC-SR04 and 16x2 LCD with 8051 Microcontroller
1. Overview
This project demonstrates how to interface an HC-SR04 Ultrasonic Sensor and a 16x2 LCD with an 8051 microcontroller (AT89C51/52 or similar). The system measures the distance to an object in front of the sensor and displays it in centimeters on the LCD screen in real-time.

This is a fundamental embedded systems project ideal for learning about sensor interfacing, timing calculations, and peripheral communication with the 8051 architecture.

2. Features
Real-time Distance Measurement: Continuously measures distance using the HC-SR04 ultrasonic sensor.

LCD Display: Shows the measured distance in centimeters on a 16x2 character LCD.

Simple Design: Built using basic components, making it easy to understand and replicate.

Educational Value: Excellent for learning about:

Timer programming on the 8051.

External interrupt usage.

LCD interfacing and commands.

Sensor data acquisition and processing.

3. Components Required
Component	Quantity
8051 Microcontroller (AT89C51/52)	1
16x2 LCD Module (HD44780 compatible)	1
HC-SR04 Ultrasonic Distance Sensor	1
11.0592 MHz Crystal Oscillator	1
33pF Ceramic Capacitors	2
10µF Electrolytic Capacitor (for LCD)	1
10kΩ Potentiometer (for LCD contrast)	1
10kΩ Resistor (Pull-up for EA pin)	1
330Ω Resistor (for LCD backlight - optional)	1
Push Button (for Reset)	1
Breadboard / PCB	1
Connecting Wires (Jumper cables)	As required
4. Circuit Diagram & Schematic
Connection Summary
8051 to LCD Connections
8051 Pin	LCD Pin	Function
P2.0	D4	Data Bit 4
P2.1	D5	Data Bit 5
P2.2	D6	Data Bit 6
P2.3	D7	Data Bit 7
P1.0	RS	Register Select
P1.1	RW	Read/Write (Connect to GND for Write-only)
P1.2	E	Enable
VSS, RW, K: Connect to GND.

VDD, A: Connect to +5V.

VEE: Connect to the middle pin of a 10kΩ potentiometer (used to adjust contrast) whose other pins are connected to +5V and GND.

8051 to HC-SR04 Connections
HC-SR04 Pin	8051 Pin
VCC	+5V
Trig	P3.0
Echo	P3.2 (INT0 - External Interrupt 0)
GND	GND
(A detailed schematic diagram (Fritzing or Proteus) should be placed here in the actual repository)

5. How It Works
Trigger Pulse: The 8051 sends a 10µs HIGH pulse to the Trig pin of the HC-SR04.

Echo Pulse: The HC-SR04 emits an ultrasonic burst and sets its Echo pin HIGH.

Time Measurement: The Echo pin stays HIGH for a duration proportional to the time taken for the sound wave to travel to the object and back. This pin is connected to the External Interrupt 0 (INT0) pin of the 8051.

An interrupt service routine (ISR) is triggered on the rising edge (start of echo) and starts Timer 1.

Another ISR is triggered on the falling edge (end of echo) and stops Timer 1.

Distance Calculation: The value in Timer 1 registers is used to calculate the time interval. This time is then converted into distance using the formula:
Distance (in cm) = (Time measured in µs) / 58
Why 58? The speed of sound is ~343 m/s or 0.0343 cm/µs. The time measured is for the round trip. So, Distance = (Time * 0.0343) / 2 = Time / 58.3 ≈ Time / 58.

Display Result: The calculated distance is converted into a string and displayed on the LCD.

6. Software Explanation
The code is written in C language using the Keil µVision IDE.

Key Functions:
lcd_init(): Initializes the LCD in 4-bit mode.

lcd_cmd(unsigned char): Sends a command byte to the LCD.

lcd_data(unsigned char): Sends a data byte (character) to the LCD.

lcd_string(char *): Displays a string on the LCD.

lcd_clear(): Clears the LCD display.

delay(unsigned int): Creates a software delay in milliseconds.

get_distance(): Manages the trigger pulse, calculates distance from timer value, and returns it.

INT0_ISR(): Interrupt Service Routine for INT0 that handles starting and stopping Timer 1 based on the Echo signal.

7. Installation & Setup
A. Hardware Setup
Build the basic 8051 minimum system circuit (Crystal, capacitors, reset switch).

Wire the LCD to the 8051 ports as described in the connection table.

Connect the HC-SR04 sensor to the specified pins and power lines.

Double-check all connections to avoid short circuits.

B. Software Setup
Install Keil µVision: Ensure you have the Keil C51 compiler installed.

Open Project: Open the .uvproj file in Keil.

Build Project: Click on 'Build Target' (F7) to compile the code. Ensure there are no errors.

Flash Code: Use a dedicated 8051 programmer (e.g., USB ASP, dedicated flash kit) and software (e.g., Flash Magic) to burn the generated .hex file into the microcontroller.

Power On: Power up the circuit. The LCD should light up and display the distance.

8. Files in this Repository
text
├── Ultrasonic_LCD_8051.c      # Main C source code file
├── Ultrasonic_LCD_8051.hex    # Pre-compiled HEX file (ready to burn)
├── Ultrasonic_LCD_8051.uvproj # Keil µVision project file
├── Schematic.png              / .pdf (Circuit diagram)
└── README.md                  # This file
9. Troubleshooting
Problem	Possible Solution
LCD shows blank boxes	Adjust the contrast potentiometer. Check if RS, RW, E, and data pins are connected correctly.
Distance reading is 0 or inaccurate	Check Trig and Echo connections. Ensure the object is within the sensor's range (2cm - 400cm).
Code doesn't compile	Ensure you are using the Keil C51 compiler, not the ARM or C166 compiler.
LCD is not initializing	Add more delay in the lcd_init() function. The LCD might need more time to power on.
Sensor not responding	Ensure the HC-SR04 is getting a stable 5V supply. Check for loose connections.
10. Applications
Obstacle detection for simple robots.

Proximity sensing and alert systems.

Parking assistance system model.

Liquid level measurement in a tank.

A learning base for more advanced IoT projects.
