// ArcadeSlam1 : Retro arcade gaming on an MCU.
// This program is designed to run on the TIMSP430G2553 and was developed in the Energia Integrated Development Environment
// For construction details and update please visit http://roboslam.com/arcadeslam
// This program is relased under the GNU Public License Version 2 a copy of which should be included with 
// this collection of software
// Changelog:
// April 2017: Modified to work with the S6D1121 TFT display
// Lots more pins used as it is a parallel interface display
// Low level I/O routines and display initialization rewritten


#include "reversebits.h"

#include "Sprite.h"
#include "Display.h"
#include "Images.h"
extern Display Screen;


void setup()
{
  SetupButtons();
  Screen.begin();
  
  randomSeed(12345); // initialize the random number generator
}


uint8_t GameStarted=0;
void loop()
{
  GameStarted = 0;  
  Screen.fillRectangle(0,0,240,320,0);
  Screen.putText("Arcade",6,70,50, RGBToWord(0x1f,0x1f,0xff), 0);
  Screen.putText("Slam!",5,125,50, RGBToWord(0xff,0xff,0x0), 0);
  Screen.putText("Press Left for Brici", 20, 5, 100, RGBToWord(0xff,0x3f,0x3f), 0);
  Screen.putText("Press Right for Invaders", 24, 5, 120, RGBToWord(0x1f,0xff,0x1f), 0);
  while (GameStarted == 0)
  {
    if (LeftPressed())
    {
      GameStarted = 1;
      PlayBrici();
    }
    if (RightPressed())
    {
      GameStarted = 1;
      PlayInvaders();
    }
    random(100); // cycle the random number generator
  }
}
void PlayBrici()
{
  // This game is pushing the limits of memory.  It seems to be about 1 or 2 bytes short of blowing the stack up. :o)
  #define BLOCKCOUNT 30
  Sprite Blocks[BLOCKCOUNT] = { 
    { YELLBLK,0,50,24,10 },{ YELLBLK,24,50,24,10 },{ YELLBLK,48,50,24,10 },{ YELLBLK,72,50,24,10 },{ YELLBLK,96,50,24,10 },{ YELLBLK,120,50,24,10 },{ YELLBLK,144,50,24,10 },{ YELLBLK,168,50,24,10 },{ YELLBLK,192,50,24,10 },{ YELLBLK,216,50,24,10 },
    { GRENBLK,0,60,24,10 },{ GRENBLK,24,60,24,10 },{ GRENBLK,48,60,24,10 },{ GRENBLK,72,60,24,10 },{ GRENBLK,96,60,24,10 },{ GRENBLK,120,60,24,10 },{ GRENBLK,144,60,24,10 },{ GRENBLK,168,60,24,10 },{ GRENBLK,192,60,24,10 },{ GRENBLK,216,60,24,10 },
    { BLUEBLK,0,70,24,10 },{ BLUEBLK,24,70,24,10 },{ BLUEBLK,48,70,24,10 },{ BLUEBLK,72,70,24,10 },{ BLUEBLK,96,70,24,10 },{ BLUEBLK,120,70,24,10 },{ BLUEBLK,144,70,24,10 },{ BLUEBLK,168,70,24,10 },{ BLUEBLK,192,70,24,10 },{ BLUEBLK,216,70,24,10 } };
  Sprite Bat(BAT,0,280,20,3);
  Sprite Ball(BALL,120,90,2,2);
  uint8_t Index;
  int8_t BallXVelocity=1;
  int8_t BallYVelocity=1;
  int8_t BallCount = 5;
  uint8_t Victory = 0;
  // put your setup code here, to run once:
  Screen.fillRectangle(0,0,240,320,0);  
  Screen.putText("Brici", 5, 10, 40, RGBToWord(0xff,0xff,0xff), RGBToWord(0,0,0));
  delay(500);
  Screen.fillRectangle(0,0,240,320,0);
  for (Index = BallCount; Index > 0; Index--)
    Screen.fillRectangle(240-Index*15,300,10,10,RGBToWord(0xff,0xf,0xf));
  for (Index = 0; Index < BLOCKCOUNT; Index++)
  {
    Blocks[Index].show();
  }
  Ball.show();
  Bat.show();
  while(1)
  {
    if (RightPressed())
    {      
      // Move right
      if (Bat.getX() < (SCREEN_WIDTH - Bat.getWidth()))
      {
        Bat.move(Bat.getX()+2,Bat.getY()); // Move the bat faster than the ball
      }
    }

    if (LeftPressed())
    {
      // Move left            
      if (Bat.getX() > 0)
      {
        Bat.move(Bat.getX()-2,Bat.getY()); // Move the bat faster than the ball     
      }
    }
    if (Bat.touching(Ball.getX(),Ball.getY()))
    {
      BallYVelocity = -BallYVelocity;
    }    
    Bat.redraw(); // redraw bat as it might have lost a pixel due to collisions

    Ball.move(Ball.getX()+BallXVelocity,Ball.getY()+BallYVelocity);
    if (Ball.getX() == 2)
      BallXVelocity = -BallXVelocity;
    if (Ball.getX() == SCREEN_WIDTH-2)
      BallXVelocity = -BallXVelocity;
    if (Ball.getY() == 2)
      BallYVelocity = -BallYVelocity;
      
    if (Ball.getY() >= SCREEN_HEIGHT)
    {
      BallCount--;
      if (BallCount == 0)
      {
        Screen.fillRectangle(0,0,240,320,0);
        Screen.putText("GAME OVER", 9, 40, 100, RGBToWord(0xff,0xff,0xff), 0);
        Screen.putText("Press Fire to restart", 21, 8, 120, RGBToWord(0xff,0xff,0), RGBToWord(0,0,0));
        while(!FirePressed());
        return;
      }
      if (random(100) & BIT0)
        BallXVelocity=1;
      else
        BallXVelocity=-1;
      
      BallYVelocity=1;
      Ball.move(random(10,SCREEN_WIDTH-10),random(90,120));
      Screen.fillRectangle(120,300,120,10,0);
      for (Index = BallCount; Index > 0; Index--)
        Screen.fillRectangle(240-Index*15,300,10,10,RGBToWord(0xff,0xf,0xf));
      
      
    }
    Victory = 1;
    for (Index = 0; Index < BLOCKCOUNT; Index++)
    {
      if (Blocks[Index].visible()) // any blocks left?
        Victory = 0;
      int touch = Blocks[Index].touching(Ball.getX(),Ball.getY());
      if (touch)
      {
        Blocks[Index].hide();
        if ( (touch == 1) || (touch == 3) )
          BallYVelocity = -BallYVelocity;    
        if ( (touch == 2) || (touch == 4) )
          BallXVelocity = -BallXVelocity;    
      }
    }
    if (Victory)
    {
      Screen.fillRectangle(0,0,240,320,RGBToWord(0,0,0xff));
      Screen.putText("VICTORY!", 8, 40, 100, RGBToWord(0xff,0xff,0), RGBToWord(0,0,0xff));
      Screen.putText("Press Fire to restart", 21, 8, 120, RGBToWord(0xff,0xff,0), RGBToWord(0,0,0xff));
      while(!FirePressed());
      return;
    }
    delay(10); // Slow the game to human speed    
  }
}
void PlayInvaders()
{
  // Variables for the Invaders game
#define MAX_INVADERS 10

  Sprite  Invaders[MAX_INVADERS]= {
  { InvaderImage,3,3,10,12   },
  { InvaderImage,23,3,10,12  },
  { InvaderImage,43,3,10,12  }, 
  { InvaderImage,63,3,10,12  },
  { InvaderImage,83,3,10,12  },
  { InvaderImage,83,3,10,12  },
  { InvaderImage,103,3,10,12 },
  { InvaderImage,123,3,10,12 },
  { InvaderImage,143,3,10,12 },
  { InvaderImage,163,3,10,12 }
  };
  Sprite  Defender(DefenderImage,195,280,10,6);
#define MAX_MISSILES 5
  Sprite Missiles[MAX_MISSILES]={
  { MissileImage,0,0,5,8  },
  { MissileImage,0,0,5,8  }, 
  { MissileImage,0,0,5,8  }, 
  { MissileImage,0,0,5,8  },
  { MissileImage,0,0,5,8  },
  }; 
  uint8_t GameOver = 0;
  uint8_t MissileCounter = 0;
  uint8_t InvaderCounter = 0;
  int DirectionChange=0;
  int Victory = 0;
  uint16_t x_step = 2;

  // Initialization for the Invaders game
  int Index;
  int Toggle = 1;

  Screen.fillRectangle(0,0,240,320,0);
  Screen.putText("MCU INVADERS", 12, 10, 40, RGBToWord(0xff,0xff,0xff), RGBToWord(0,0,0));
  delay(500);
  Screen.fillRectangle(0,0,240,320,0);
  for (Index = 0; Index < MAX_INVADERS;Index++)
  {
    Invaders[Index].show();
  }
  GameOver = 0;
  MissileCounter = 0;
  InvaderCounter = 0;
  DirectionChange=0;
  Victory = 0;
  x_step = 2;
  SetupButtons();

  while(1)
  {
    // put your main code here, to run repeatedly:
    // Show the defender
    Defender.show();

    if (RightPressed())
    {      
      // Move right
      if (Defender.getX() < (SCREEN_WIDTH - 11))
      {
        Defender.move(Defender.getX()+2,Defender.getY());
      }
    }

    if (LeftPressed())
    {
      // Move left            
      if (Defender.getX() > 1)
      {
        Defender.move(Defender.getX()-2,Defender.getY());

      }
    }
    if (FirePressed() )
    {
      // User pressed Fire!
      // See if there is an empty missile-in-flight slot
      for (MissileCounter = 0; MissileCounter < MAX_MISSILES; MissileCounter++)
      {
        if (Missiles[MissileCounter].visible()==0)
        {
          // Found an empty slot, place a missile just above the centre of the defender
          Missiles[MissileCounter].show();
          Missiles[MissileCounter].move(Defender.getX()+Defender.getWidth()/2-Missiles[MissileCounter].getWidth()/2,Defender.getY()-Missiles[MissileCounter].getHeight()/2);
          break; // exit remaining for loop cycles
        }
      }
    }
    // Update missiles
    for (MissileCounter = 0; MissileCounter < MAX_MISSILES; MissileCounter++)
    {
      if ((Missiles[MissileCounter].getY() > 0) && (Missiles[MissileCounter].visible()))
      {
        Missiles[MissileCounter].move(Missiles[MissileCounter].getX(),Missiles[MissileCounter].getY()-2);
        // Did any missile hit an invader?
        for (InvaderCounter = 0; InvaderCounter < MAX_INVADERS; InvaderCounter++)
        {
          if (Invaders[InvaderCounter].within(Missiles[MissileCounter].getX(),Missiles[MissileCounter].getY()))
          {
            // Missile meets invader!
            Invaders[InvaderCounter].hide();
            Missiles[MissileCounter].hide();
            // Do an explosion at that location
            Explode(Missiles[MissileCounter].getX(),Missiles[MissileCounter].getY());
          }
        }
      }
      else
      {
        if (Missiles[MissileCounter].visible())
          Missiles[MissileCounter].hide();
      }

    }

    // Update invaders
    DirectionChange = 0;
    Victory = 1;
    for (InvaderCounter = 0; InvaderCounter < MAX_INVADERS; InvaderCounter++)
    {
      if (Invaders[InvaderCounter].visible()) 
      {
        Victory = 0; // didn't win yet, there are still invaders
        Invaders[InvaderCounter].move(Invaders[InvaderCounter].getX()+x_step,Invaders[InvaderCounter].getY()+2);

        if (Invaders[InvaderCounter].getX() >=(SCREEN_WIDTH-Invaders[InvaderCounter].getWidth()))
          DirectionChange = 1;
        if (Invaders[InvaderCounter].getX() == 0)
          DirectionChange = 1;
        if (Invaders[InvaderCounter].getY() > SCREEN_HEIGHT)
        {
          Invaders[InvaderCounter].move(Invaders[InvaderCounter].getX(),0);

        }
        if (Defender.within(Invaders[InvaderCounter].getX()+Invaders[InvaderCounter].getWidth()/2,Invaders[InvaderCounter].getY()+Invaders[InvaderCounter].getHeight()/2))
        {
          Explode(Defender.getX(),Defender.getY());
          Screen.fillRectangle(0,0,240,320,0);
          Screen.putText("GAME OVER", 9, 40, 100, RGBToWord(0xff,0xff,0xff), 0);
          Screen.putText("Press Fire to restart", 21, 8, 120, RGBToWord(0xff,0xff,0), RGBToWord(0,0,0));
          while(!FirePressed());
          return;
        }
      }

    }
    if (Victory)
    {
      Screen.fillRectangle(0,0,240,320,RGBToWord(0,0,0xff));
      Screen.putText("VICTORY!", 8, 40, 100, RGBToWord(0xff,0xff,0), RGBToWord(0,0,0xff));
      Screen.putText("Press Fire to restart", 21, 8, 120, RGBToWord(0xff,0xff,0), RGBToWord(0,0,0xff));
      while(!FirePressed());
      return;
    }
    if (DirectionChange) // Did an invader hit either edge?
      x_step = -x_step; // if so, then reverse direction
  }
}
void SetupButtons()
{
  // Button layout:
  // P1_6 : Left
  // P2_7 : Fire
  // P2_6 : Right
    // Set up the buttons
  pinMode(P1_6,INPUT);
  pinMode(P2_6,INPUT);
  pinMode(P2_7,INPUT);
  // enable pull-up resistors
  P1REN |= (BIT6);
  P1OUT |= (BIT6);
  P2REN |= (BIT6 + BIT7);
  P2OUT |= (BIT6 + BIT7);
}
int LeftPressed()
{
  if ( (P1IN & BIT6) == 0)
    return 1;
  else 
    return 0;
}
int RightPressed()
{
  if ( (P2IN & BIT6) == 0)
    return 1;
  else 
    return 0;
}
int FirePressed()
{
  static int PreviousState = 0;
  if ((P2IN & BIT7) == 0)
  {
    if (PreviousState == 0)
    {
      PreviousState = 1;
      return 1;
    }
    else
      return 0;

  }
  else 
  {
    PreviousState = 0;    
    return 0;  
  }
}
void Explode(uint16_t X,uint16_t Y)
{
  Sprite Bang(ExplosionImage,X-5,Y-6,10,12);
  Bang.show();
  delay(100);
  Bang.hide();
}


