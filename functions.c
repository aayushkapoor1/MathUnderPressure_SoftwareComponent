#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include <time.h>
#include "ece198.h"
#include "functions.h"

// calculates all the divisors of the fist number displayed
// to ensure we have integer division (i.e 9/3 not 9/5)
int divisiors(int *array, int number){ 
    // adds all the divisors to an array
    for(int i = number; i > 0; --i){
        if(number%i==0){
            array[i]=i;
        }else{
            array[i]=0;
        }
    }

    // chooses a random divisor to return that 
    // is not 0 (since we cannot have n/0 where n is an integer)
    int randomIndex = rand() % 10;
    while(array[randomIndex]==0){
        randomIndex = rand() % 10;
    }

    return array[randomIndex];
}

int displayOperation(int *colours, int *divisorOfNums, char *buff){
    int r1; 
    int r2;
    int randomColour = rand() % 4; //picks a random number from 0-3 which corresponds to a colour for the led
    if(randomColour == 1){
        r1 = rand() % 10;
        r2 = rand() % 10;
        while(r1-r2<0){     //ensures the first number will never be smaller than the second number for subtraction
            r1 = rand() % 10;
            r2 = rand() % 10;
        }
    }else if(randomColour == 3){
        r1 = rand() % 10;
        r2 = rand() % 9 + 1;    //creates a random number between 1-9 since we do not n/0 where n is an integer
        while(r1%r2!=0){
            r2 = divisiors(divisorOfNums, r1); //ensures that the result of the two display integers is an integer
        }
    }else{
        r1 = rand() % 10;    //creates a random number between 0-9
        r2 = rand() % 10;    //creates a random number between 0-9
    }

    //initalizes variables to hold the result of the operation
    int operation = 0;

    //computes the operation based on the sign
    if(randomColour == 0){
        operation = r1+r2;
    }
    else if (randomColour == 1){
        operation = r1-r2;
    }else if (randomColour == 2){
        operation = r1*r2;
    }else if (randomColour == 3){
        operation = r1/r2;
    }
    // code to display elements to board
    Display7Segment1(r1);     // displays random number on 7-segment display
    Display7Segment2(r2);     // displays random number on 7-segment display
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, colours[randomColour] & 0x01);  // blue  (hex 1 == 0001 binary)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, colours[randomColour] & 0x02);  // green (hex 2 == 0010 binary)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, colours[randomColour] & 0x04);  // red   (hex 4 == 0100 binary)
    return operation;
}

bool timerState(uint32_t timer, bool loss){
    timer = HAL_GetTick();
    // checks if the game is being played for less than 9 seconds
    if(timer<9000){
        // converts the timer value from milliseconds to seconds
        // ensures the timer counts down from 9 based on the elapsed game time
        Display7SegmentTimer(9-((timer-1)/1000));
        // since the game has only been played for less than 9 seconds, set
        // the loss state to false
        loss = false;
    }else if(timer>=9000){        // if the elapsed game time is larger than 9 seconds 
                                  // then we set the loss state to be true
        Display7SegmentTimer(0);  // sets the timer to display 0
        Display7Segment1(11);     // sets the 7-segment display to display nothing
        Display7Segment2(11);     // sets the 7-segment display to display nothing
        // display red on the multi-colour led
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);  // red
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);  // green
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);  // blue
        loss = true;
    }
    return loss;
}

void winningState(){
    // sets the winning state of the game
    Display7Segment1(11);     // sets the 7-segment display to display nothing
    Display7Segment2(11);     // sets the 7-segment display to display nothing
    // display green on the multi-colour led
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1); 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0); 
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}