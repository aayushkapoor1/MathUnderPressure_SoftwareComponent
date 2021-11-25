#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ece198.h"
#include "functions.h"

int divisiors(int *array, int number);
int displayOperation(int *colours, int *divisorOfNums, char *buff);

int main(void)
{
    HAL_Init();


    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    SerialSetup(9600);

    InitializeKeypad();      //initializes the keypad
    Initialize7Segment1();   //initializes the 7-segment display
    Initialize7Segment2();   //initializes the 7-segment display
    Initialize7SegmentTimer();  //initializes the 7-segment display
    InitializePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // initialize color LED output pins
    SerialSetup(9600);  //initializes the port to print the random num to the console

    int colours[4]={3,4,5,6}; //yellow, dark blue, magenta, light blue
    //yellow is addition
    //dark blue is subtraction
    //magenta is multiplication
    //light blue is division

    int divisorOfNums[10]={0};

    char *keypad_symbols = "123A456B789C*0#D";
    int inputtedAnswer=-1;
    int solution;
    int timesPlayed=0;
    char buff[100];
    bool loss = false;

    while(true){
        if(timesPlayed==0){
            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button press
        }
        srand(HAL_GetTick());   //initializes the algorithm for the random num gen
        if(timesPlayed!=3){
            solution = displayOperation(colours, divisorOfNums, buff);
            char array[10]="";
            int counter = 0;
            while(inputtedAnswer != solution){ //continuously runs
                // starts timer
                uint32_t timer = HAL_GetTick();

                // code for keypad input
                while (ReadKeypad() < 0){
                    loss = timerState(timer, loss);
                }

                while(loss){
                    // continuously updates the timer
                    timerState(timer, loss);
                }  

                // resets the state of the input array is '*' is clicked on the keypad
                if(keypad_symbols[ReadKeypad()]=='*'){
                    for(int i=0; i<counter; i++){
                        array[i]=' ';
                    }
                    inputtedAnswer=-1;
                    counter=0;
                }
                // concatinates the value pressed to the input array
                else{
                    if(counter==0){
                        inputtedAnswer=0;
                    }
                    array[counter]=keypad_symbols[ReadKeypad()];
                    inputtedAnswer = (inputtedAnswer*10)+(array[counter]-'0');
                    counter++;
                }
        
                while (ReadKeypad() >= 0){ 
                    // continuously updates the timer
                    loss = timerState(timer, loss);
                } 
            }
            // if the game is won, the number of wins variable is incremented
            timesPlayed++;

        }else if (timesPlayed == 3){
            // displays the winning state to the board if the user wins
            winningState();
        }
    }
    
    return 0;
}