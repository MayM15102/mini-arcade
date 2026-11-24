MINI ARCADE
A Compact ESP32-C3 Handheld Gaming System
A miniature handheld gaming system developed using the Seeed Studio XIAO ESP32-C3 and a 0.96-inch OLED display. The project combines retro-inspired games and a virtual pet interface in a compact, portable design.
1. Project Overview
Mini Arcade is an embedded gaming project designed to explore microcontroller programming, OLED graphics, interactive input systems, and compact hardware integration.
The system uses an ESP32-C3 microcontroller to run multiple interactive applications on a monochrome OLED display, controlled through two physical pushbuttons.
2. Core Features
Feature
Description
Virtual Pet
A Tamagotchi-inspired pixel-art character interface
Pong
A simplified paddle-and-ball arcade game
Dino Run
An obstacle-avoidance game with jump controls
Navigation
Menu-based game selection
Input System
Two-button user interface
Display
128 × 64 monochrome OLED graphics

3. Hardware Architecture
Primary Components
Seeed Studio XIAO ESP32-C3 microcontroller
0.96-inch I2C OLED display
Two tactile pushbuttons
Breadboard and jumper wires
Perfboard for potential permanent assembly
Compact enclosure for the final design
4. Software & Development
Development Environment
Arduino IDE
Arduino ESP32 Core by Espressif Systems
Libraries
Adafruit GFX Library
Adafruit SSD1306 Library
Programming Language: C++
5. Pin Configuration
The following pin assignments are planned for the prototype.
Component
XIAO ESP32-C3 Pin
OLED SDA
D4 / GPIO6
OLED SCL
D5 / GPIO7
OLED VCC
3V3
OLED GND
GND
Button A
D1 / GPIO3
Button B
D2 / GPIO4

6. User Controls
Input
Function
Button A
Select a game / primary game action
Button B
Navigate menu / secondary game action
Both buttons
Return to main menu

7. Repository Structure
mini-arcade/
├── README.md
└── MiniArcade.ino
Additional documentation, wiring diagrams, and project photographs will be added as development progresses.
8. Development Roadmap
Arduino IDE environment configured
ESP32 board support installed
Required display libraries installed
Initial game program developed
Initial code compiled successfully
Source code uploaded to GitHub
Physical hardware integration
OLED display verification
Button input verification
Individual game testing
Permanent circuit assembly
Enclosure development
Final system testing
Demonstration documentation
9. Project Objectives
This project aims to demonstrate practical applications of embedded systems through:
Microcontroller-based application development
Monochrome display graphics
Physical input integration
Basic game programming and logic
Compact electronic system design
10. Project Status
Current Stage: Initial software development completed; hardware integration Completed.
The source code has been compiled successfully in Arduino IDE. Physical hardware testing and verification will be documented as development continues.
Developed as an embedded systems and interactive hardware project.
