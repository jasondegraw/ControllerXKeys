/*****************************************************************************/
/*  Copyright (c) 2014, Jason W. DeGraw                                      */
/*                                                                           */
/*  This file is part of ControllerXKeys                                     */
/*                                                                           */
/*  ControllerXKeys is free software: you can redistribute it and/or modify  */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation, either version 3 of the License, or        */
/*  (at your option) any later version.                                      */
/*                                                                           */
/*  ControllerXKeys is distributed in the hope that it will be useful,       */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  GNU General Public License for more details.                             */
/*                                                                           */
/*  You should have received a copy of the GNU General Public License        */
/*  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.          */
/*                                                                           */
/*****************************************************************************/
#include "SDL2/SDL.h"
#include <windows.h>
#include <stdio.h>

unsigned maxButtons;
unsigned maxAxis;
WORD *buttonMap;
WORD *axisMap;
int *axisButtonState;

#define MAXKEYS 1000
INPUT keys[MAXKEYS];
unsigned nInputs;

void setup();
void teardown();

void sendButtonDown(Uint8 button)
{
  keys[nInputs].ki.wVk = buttonMap[button];
  keys[nInputs].ki.dwFlags = 0;
  nInputs++;
  if(nInputs == MAXKEYS) {
    SendInput(nInputs,keys,sizeof(INPUT));
    nInputs = 0;
  }
}

void sendButtonUp(Uint8 button)
{
  keys[nInputs].ki.wVk = buttonMap[button];
  keys[nInputs].ki.dwFlags = KEYEVENTF_KEYUP;
  nInputs++;
  if(nInputs == MAXKEYS) {
    SendInput(nInputs,keys,sizeof(INPUT));
    nInputs = 0;
  }
}

void sendAxisButtonDown(Uint8 axis)
{
  keys[nInputs].ki.wVk = axisMap[axis];
  keys[nInputs].ki.dwFlags = 0;
  nInputs++;
  if(nInputs == MAXKEYS) {
    SendInput(nInputs,keys,sizeof(INPUT));
    nInputs = 0;
  }
}

void sendAxisButtonUp(Uint8 axis)
{
  keys[nInputs].ki.wVk = axisMap[axis];
  keys[nInputs].ki.dwFlags = KEYEVENTF_KEYUP;
  nInputs++;
  if(nInputs == MAXKEYS) {
    SendInput(nInputs,keys,sizeof(INPUT));
    nInputs = 0;
  }
}

int main(int argc, char *argv[])
{
  int i;
  int done=0;
  int index=0;
  SDL_Joystick *joystick;
  SDL_Event event;

  nInputs = 0;
  /* Initialize the input struct array */
  for(i=0;i<MAXKEYS;i++) {
    keys[i].type = INPUT_KEYBOARD;
    keys[i].ki.wVk = VK_SPACE;
    keys[i].ki.wScan = 0;
    keys[i].ki.dwFlags = 0;
    keys[i].ki.time = 0;
    keys[i].ki.dwExtraInfo = 0;
  }
  /* Initialize the button and axis maps */
  setup();

  /* Allocate the axis state array */
  axisButtonState = (int*)malloc(sizeof(int)*maxAxis);
  /* Initialize the axis button state */
  for(i=0;i<maxAxis;i++) {
    axisButtonState[i] = 0;
  }

  if(SDL_Init(SDL_INIT_JOYSTICK) < 0){
    printf("SDL could not initialize! SDL_Error: %s\n",SDL_GetError());
    return EXIT_FAILURE;
  }	else {
    printf("SDL successfully initialized!\n");
  }

  printf("%i joystick(s) found.\n\n", SDL_NumJoysticks() );
  if(SDL_NumJoysticks() == 0) {
    puts("Exiting.\n");
    return EXIT_FAILURE;
  }

  puts("Joystick name(s):");
  for(i=0;i<SDL_NumJoysticks();i++){
    printf("\t%s (%d)\n", SDL_JoystickNameForIndex(i),SDL_IsGameController(i));
  }
  SDL_JoystickEventState(SDL_ENABLE);

  joystick = SDL_JoystickOpen(index);
  printf("Number of axes: %d\n",SDL_JoystickNumAxes(joystick));

  while(!done) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
        done=1;
        break;
      case SDL_JOYBUTTONDOWN:  // Joystick Button Press
        if(event.jbutton.button < maxButtons) {
          if(buttonMap[event.jbutton.button]) {
            printf("Button Down: %d\n",event.jbutton.button);
            sendButtonDown(event.jbutton.button);
          }
        }
        break;
      case SDL_JOYBUTTONUP:  // Joystick Button Release
        if(event.jbutton.button < maxButtons) {
          if(buttonMap[event.jbutton.button]) {
            printf("Button Up: %d\n",event.jbutton.button);
            sendButtonUp(event.jbutton.button);
          }
        }
        break;
      case SDL_JOYAXISMOTION:  // Joystick Motion
        if(event.jaxis.axis < maxAxis) {
          if(axisMap[event.jaxis.axis]) {
            if(event.jaxis.value > 31000 && !axisButtonState[event.jaxis.axis]) {
              axisButtonState[event.jaxis.axis] = 1;
              printf("Axis Button Down: %d\n",event.jaxis.axis);
              sendAxisButtonDown(event.jaxis.axis);
            } else if(event.jaxis.value < -31000 && axisButtonState[event.jaxis.axis]) {
              axisButtonState[event.jaxis.axis] = 0;
              printf("Axis Button Up: %d\n",event.jaxis.axis);
              sendAxisButtonUp(event.jaxis.axis);
            }
          }
          break;
        }
        break;
      }
    }
    if(nInputs) {
      printf("nInputs = %d\n",nInputs);
      SendInput(nInputs,keys,sizeof(INPUT));
      nInputs = 0;
    }
  }

  teardown();
  free(axisButtonState);

  SDL_Quit();
  return EXIT_SUCCESS;
}
