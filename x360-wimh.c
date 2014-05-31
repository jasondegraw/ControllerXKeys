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
#include <stdlib.h>
#include <windows.h>

extern unsigned maxButtons;
extern unsigned maxAxis;
extern WORD *buttonMap;
extern WORD *axisMap;

void setup()
{
  /* Set the button map size */
  maxButtons = 15;
  buttonMap = (WORD*)malloc(sizeof(WORD)*maxButtons);
  /* Initialize the button map */
  buttonMap[0] = VK_UP;      /* D-up */
  buttonMap[1] = VK_DOWN;    /* D-down */
  buttonMap[2] = VK_LEFT;    /* D-left */
  buttonMap[3] = VK_RIGHT;   /* D-right */
  buttonMap[4] = VK_ESCAPE;  /* back */
  buttonMap[5] = VK_ESCAPE;  /* start */
  buttonMap[6] = 0;          /* LS */
  buttonMap[7] = 0;          /* RS */
  buttonMap[8] = 0;          /* LB */
  buttonMap[9] = 0;          /* RB */
  buttonMap[10] = VK_SPACE;  /* A */
  buttonMap[11] = 0x44;      /* B */
  buttonMap[12] = 0x44;      /* X */
  buttonMap[13] = 0;         /* Y */
  buttonMap[14] = VK_ESCAPE; /* guide */

  /* Set the axis map size */
  maxAxis = 6;
  axisMap = (WORD*)malloc(sizeof(WORD)*maxAxis);
  /* Initialize the axis map */
  axisMap[0] = 0;       /* ? */
  axisMap[1] = 0;       /* ? */
  axisMap[2] = 0;       /* ? */
  axisMap[3] = 0;       /* ? */
  axisMap[4] = 0x41;    /* LT */
  axisMap[5] = 0x53;    /* RT */
}

void teardown()
{
  free(buttonMap);
  free(axisMap);
}
