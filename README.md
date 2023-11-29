# IntroductionToRobotics

 This repository contains all the assignments for the 'Introduction to Robotics' course for Group 361 (Year 3) from the Faculty of Mathematics and Computer Science. Every assignment encompasses implementation insights, code, as well as image and video files.

<details>
  <summary> 
    Homework 1 : RGB LED Control with Potentiometers
  </summary>  

 ## RGB LED Control with Potentiometers


The homework contains:
  - The project requirements and description; 
  - A photo of the hardware setup;
  - A link to the video showcasing the functionality;
  - A link to the uploaded code;
 

### Objective
* Interface potentiometers with Arduino to control an RGB LED's individual channels - Red, Green, Blue.
* Learn to harness analog readings from potentiometers and utilize digital electronics techniques for LED control.
* Adhere to a consistent and clean coding style, ensuring the code is well-commented and easily understandable by peers and reviewers.

  
### Components Used
* 1 RGB LED 
* 3 potentiometers 
* Resistors and wires as necessary


### Technical Implementation
* Control each RGB channel of the LED using dedicated potentiometers.
* The Arduino interprets the analog readings from the potentiometers and then produces a mapped output to the RGB LED pins for precise color adjustments.


### Photos of the Hardware Setup
* A detailed view of the Arduino setup on a breadboard with the illuminated LED indicating its active state.

![RGB_ARDUINO_2](https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/2f1f4875-0148-47ac-bb2a-1f435fe4fd37)


### Code 

[Inspect the code here!](https://github.com/uantoniaa/IntroductionToRobotics/blob/80f7c4016df804830b67b5f0ae75f78f5ca3f558/Homeworks%20-%20Code/Homework-1.ino)



### Video 


[Watch the video here!](https://youtube.com/shorts/LWe2Eiy1l58?feature=share)


</details>

<details>
  <summary> 
    Homework 2 : Elevator simulator wannabe
  </summary>  
 
 ## Elevator simulator wannabe

The homework contains:
  - The project requirements and description; 
  - A photo of the hardware setup;
  - A link to the video showcasing the functionality;
  - A link to the uploaded code;

### Objective
* Simulate a 3-floor elevator control system using LEDs, buttons, and optionally, a buzzer with Arduino.
* Implement button debouncing techniques and coordinate multiple components to represent a real-world elevator system.

### Components Used
* LEDs (At least 4: 3 for the floors and 1 for the elevator’s operational state)
* Buttons (At least 3 for floor calls)
* Buzzer (1) - optional for Computer Science, mandatory for CTI
* Resistors and wires as necessary

### Technical Implementation
* Use LEDs to indicate the elevator's current floor and operational state.
* Incorporate buttons to simulate floor calls, leading to the elevator's movement towards the designated floor after a brief interval.
* Use the buzzer for feedback, indicating elevator movement, door closures, and arrival at the desired floor.
* Implement button debounce techniques to prevent accidental multiple calls.
* Ensure the elevator’s operational LED blinks during movement and remains static when stationary.

### Photos of the Hardware Setup
* A detailed view of the Arduino setup displaying the LEDs in their active state, buttons, and the buzzer.

![ELEVATOR_SIMULATOR](https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/983b39c9-f2fc-4554-9f30-db669c79cd1c)


### Code 

[Inspect the code here!](https://github.com/uantoniaa/IntroductionToRobotics/blob/80f7c4016df804830b67b5f0ae75f78f5ca3f558/Homeworks%20-%20Code/Homework-2.ino)

### Video 

[Watch the video here!](https://youtube.com/shorts/piyIdvtJ--A?feature=share)
</details>


<details>
  <summary> 
    Homework 3 : 7 segment display drawing
  </summary>  
 
 ## 7 segment display drawing

The homework contains:
  - The project requirements and description; 
  - A photo of the hardware setup;
  - A link to the video showcasing the functionality;
  - A link to the uploaded code;

### Task Requirements

- **General description:** Control a 7-segment display using a joystick to "draw" and toggle segments. Movement should be restricted to adjacent segments only.
- **Components:**
  - 1x 7-segment display
  - 1x Joystick module
  - Resistors and wires as needed
- **Initial Position:** Start with the decimal point (DP) on the display.
- **Functionality:**
  - The current position blinks irrespective of segment state.
  - Use the joystick to move between segment positions without "jumping" over segments.
  - Short pressing the joystick button toggles the segment state (ON/OFF).
  - Long pressing the joystick button resets the display, turning all segments OFF and returning to DP.
- **Interrupts:** Required for CTI students. 



## Objectives

The primary goals of this homework are to:

1. **Understand and Implement Direct Digital Control**: Utilize the digital input from a joystick to control the state of a digital output device, a 7-segment display, in real-time.

2. **Develop Efficient Code**: Create a program that responds to user input without the use of blocking functions such as `delay()`, which can prevent real-time interaction. Instead, use `millis()` or `micros()` to manage timing without halting the execution of the code.

3. **Practice Debouncing**: Implement software debouncing for the joystick button to ensure accurate detection of short and long presses without false triggering due to contact bounce.

4. **Learn to Use Interrupts**: For CTI students, an objective is to correctly utilize interrupts to process the joystick input, thereby learning about interrupt service routines and their role in embedded systems. For Computer Science students, this serves as an additional challenge.

5. **Algorithm Development**: Devise algorithms to navigate between adjacent segments on the 7-segment display in a controlled and predictable manner, as per the given movement table, ensuring that the transitions are logical and do not "jump" over segments.

6. **Implement State Toggle through User Input**: Use the joystick button to toggle the state of the segments on the display, allowing the user to "draw" by turning segments on and off.

7. **Design a User Reset Feature**: Implement a reset functionality that, upon a long press of the joystick button, turns off all segments and resets the display position to the decimal point (DP).

8. **Creative Expansion**: Encourage creative thinking by allowing for bonus points on the addition of extra features such as sounds, lights, or other interactive components.

Through the completion of these objectives, students will gain hands-on experience with direct control of hardware, software debouncing, interrupt handling, and state management, all of which are foundational concepts in robotics and embedded systems programming.

### Photos of the Hardware Setup
* A detailed view of the Arduino setup.

![7 segment display drawing](https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/80b6ad45-ab9a-4d28-a5f8-fe9cdf6f4b23)
)


### Code 

[Inspect the code here!](https://github.com/uantoniaa/IntroductionToRobotics/blob/c60f9609c446237807a38a9e6778f6c4cd2ae9d2/Homeworks%20-%20Code/Homework-3.ino)



### Video 

[Watch the video here!](https://youtube.com/shorts/gSbU59CDnUo?feature=share)
</details>


<details>
  <summary> 
    Homework 4:  Stopwatch Timer
  </summary>  


 ## Stopwatch timer

The homework contains:
  - The project requirements and description; 
  - A photo of the hardware setup;
  - A link to the video showcasing the functionality;
  - A link to the uploaded code;

### Objective
* The primary goal of this assignment is to create a functional and interactive stopwatch timer using a 4-digit 7-segment display and three buttons. This task will challenge students to apply their skills in digital electronics, programming logic, and user interface design.

  ### Technical Implementation
*The 4-digit 7-segment display should start at "000.0".
Button functionalities:
*Button 1: Start/pause.
*Button 2: Reset (in pause mode) or reset saved laps (in lap viewing mode).
*Button 3: Save lap (in counting mode) or cycle through last saved laps (up to 4 laps).

### Photos of the Hardware Setup
* A detailed view of the Arduino setup displaying the LEDs in their active state, buttons, and the buzzer.

![Stopwatch Timer](https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/6278af40-7dc6-45fc-ad39-4c50e5614d98)


### Code 

[Inspect the code here!](https://github.com/uantoniaa/IntroductionToRobotics/blob/a64797d8e8da7022f92dc4f8bf1d0e35e4d69b4c/Homeworks%20-%20Code/Homework-4.ino)

### Video 

[Watch the video here!](https://youtube.com/shorts/PJygiogoYvA?si=OAt7j-lUY0WDj-IZ)
</details>

<details>
  <summary> 
    Homework 5: Smart Environment Monitor and Logger
  </summary>  

## Smart Environment Monitor and Logger

The homework contains:
  - The project requirements and description; 
  - A photo of the hardware setup;
  - A link to the video showcasing the functionality;
  - A link to the uploaded code;

### Objective
* Develop a "Smart Environment Monitor and Logger" using Arduino.
* Utilize various sensors to gather environmental data, log this data into EEPROM, and provide feedback via an RGB LED.
* Implement a user interface through a Serial Menu.

### Components Used
* Arduino Uno Board
* Ultrasonic Sensor (HC-SR04)
* LDR (Light-Dependent Resistor)
* RGB LED
* Resistors as needed
* Breadboard and connecting wires
* (Optional) Additional sensors/components for extended functionality

### Technical Implementation
* Integrate sensor readings and memory management for environmental monitoring.
* Implement Serial Communication for user interaction and system configuration.
* Establish a menu system for adjusting sensor settings and viewing logged data.
* Provide visual feedback using an RGB LED based on sensor readings and thresholds.



### Photos of the Hardware Setup
* A detailed view of the Arduino setup with sensors and the RGB LED.

![Smart_Environment_Monitor_Setup](https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/8bd71ee8-0d3c-4478-af17-2ac16ae51ef6)


### Code 

[Inspect the code here!](https://github.com/uantoniaa/IntroductionToRobotics/blob/main/Homeworks%20-%20Code/Homework-5.ino)

### Video 

[Watch the video here!](https://youtu.be/6L7syGTK26M?si=1O16G_nDEn6O_oeO)


</details>

<details>
  <summary> 
    Homework 6: Mini 8x8 LED Matrix Game
  </summary> 

## Mini 8x8 LED Matrix Game

The homework contains:

 - The project requirements and description;
 - A photo of the hardware setup;
 - A link to the video showcasing the functionality;
 - A link to the uploaded code;

### Objective
* Create an interactive game on an 8x8 LED matrix, incorporating elements like a player, bombs/bullets, and walls.
* Explore creativity and programming skills by designing a game with a unique concept and engaging mechanics.


### Components Used
* Arduino Uno Board
* Joystick
* 8x8 LED Matrix
* MAX7219 LED Driver
* Resistors and capacitors as needed
* Breadboard and connecting wires
* (Optional) Additional sensors/components for extended functionality (I didn't use)

### Technical Implementation
* Utilize the 8x8 LED matrix for game display.
* Implement controls using a joystick to navigate the player across the matrix.
* Program different blinking patterns for various game elements (player, bombs/bullets, walls).
* Ensure smooth control and logical placement of walls and other elements in the game.

 
### Photos of the Hardware Setup
* A detailed view of the Arduino setup with the 8x8 LED Matrix and controls.
8x8 LED Matrix Game

![8x8 LED Matrix Game](https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/03a89de9-a496-478c-b2b9-3daf4b44c0b8)



### Code 

[Inspect the code here!](https://github.com/uantoniaa/IntroductionToRobotics/blob/main/Homeworks%20-%20Code/Homework-6.ino)

### Video 

[Watch the video here!](https://youtu.be/q8Z4gM8RU_o)


</details>


Note: The repository is continually updated with new homework and materials. 
