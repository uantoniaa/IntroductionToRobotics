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

![RGB_ARDUINO_2]([https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/2f1f4875-0148-47ac-bb2a-1f435fe4fd37](https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/0a76ff24-16e9-4a27-9a91-aca6fe7d79e7))


### Code 

[Inspect the code here!](https://github.com/uantoniaa/IntroductionToRobotics/blob/a93bb2d2607094cfef58fd90feabdc4380ea290/Homeworks%20-%20Code/Homework-1.ino)



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

![ELEVATOR_SIMULATOR](https://github.com/uantoniaa/IntroductionToRobotics/assets/93488180/0a76ff24-16e9-4a27-9a91-aca6fe7d79e7)

### Code 

[Inspect the code here!](https://github.com/uantoniaa/IntroductionToRobotics/blob/a93bb2d2607094cfef58fd90feabdc4380ea290/Homeworks%20-%20Code/Homework-2.ino)

### Video 

[Watch the video here!](Link_to_your_video)
</details>


