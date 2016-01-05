/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL OpenGL, standard IO, and, strings
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <stdio.h>
#include <string>
#include <Math.h>
#define PI 3.14
//Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;
struct brick
{
    float x;
    float y;
    float width;
    float height;
    bool alive;
};
const static int BRICKS =54;
brick bricks[BRICKS];
//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Input handler
void handleKeys( unsigned char key, int x, int y );

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

bool checkCollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float radius);
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = true;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window
		gWindow = SDL_CreateWindow( "My first Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL
				if( !initGL() )
				{
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
			}
		}
	}

	return success;
}


bool checkCollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh)
{
    if(Ay+Ah<By) return false;
    else if(Ay>By+Bh) return false;
    else if(Ax+Aw<Bx) return false;
    else if(Ax>Bx+Bw) return false;

    return true;
}


bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	//Initialize Modelview Matrix
	/*glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();*/

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	//Initialize clear color
	glClearColor( 0.f, 1.f, 0.f, 1.f );

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	return success;
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	if( key == 'q' )
	{
		gRenderQuad = !gRenderQuad;
	}
}

void update()
{
	//No per frame update needed
}

void render(float myX, float myY, float width, float height, float ballX, float ballY, float ballWH, float velX, float velY)
{
	//Clear color buffer
	glClear( GL_COLOR_BUFFER_BIT );

	//Render quad
	glPushMatrix();
	glOrtho(0,600,480,0,-1,1);

        glBegin(GL_QUADS);
        glVertex2f(myX,myY);
        glVertex2f(myX+width, myY);
        glVertex2f(myX+width,myY+height);
        glVertex2f(myX,myY+height);
        glEnd();

        glBegin(GL_QUADS);
        for(int i=0;i<BRICKS;i++)
        {
            if(bricks[i].alive == true)
            {
            glVertex2f(bricks[i].x, bricks[i].y);
            glVertex2f(bricks[i].x + bricks[i].width, bricks[i].y);
            glVertex2f(bricks[i].x + bricks[i].width, bricks[i].y + bricks[i].height);
            glVertex2f(bricks[i].x, bricks[i].y + bricks[i].height);

            }

        }

        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(ballX, ballY);
        glVertex2f(ballX + ballWH ,ballY);
        glVertex2f(ballX+ballWH, ballY+ ballWH);
        glVertex2f(ballX, ballY+ballWH);
        glEnd();


	glPopMatrix();
}

void close()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] ){
	//Start up SDL and create window
	        float myX = 270;
			float myY = 450;
			float width = 120;
			float height = 30;
			float ballX = 100;
			float ballY =300;
			float ballWH = 25;
			float velX = 5;
			float velY = -5;

			for(int i=0, x = 4,y = 12;i<BRICKS;i++, x+=60)
            {
                if(x>560)
                {
                    x =4;
                    y+=25;
                }
                bricks[i].x = x;
                bricks[i].y = y;
                bricks[i].width = 50;
                bricks[i].height = 20;
                bricks[i].alive = true;
            }

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//Enable text input
		SDL_StartTextInput();

		//While application is running
		while( !quit )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}

				if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }

                if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_a)
                {
                    glClearColor(1.f,0.f,0.f,1.f);
                }
                bool left_down = false;
                bool right_down = false;

                if(e.type == SDL_KEYDOWN)
                {
                    if(e.key.keysym.sym == SDLK_LEFT)
                    {
                        left_down = true;
                    }
                    if(e.key.keysym.sym == SDLK_RIGHT)
                    {
                        right_down = true;
                    }
                }

                if(e.type == SDL_KEYUP)
                {
                    if(e.key.keysym.sym == SDLK_LEFT)
                    {
                        left_down  = false;
                    }
                    if(e.key.keysym.sym == SDLK_RIGHT)
                    {
                        right_down = false;
                    }
                }

                if(left_down)
                {
                    myX -= 10;
                }

                if(right_down)
                {
                    myX += 10;
                }
                if(myX < 0)
                {
                    myX =0;
                }
                if(myX+width > 600)
                {
                    myX = 600-width;
                }


                ballX += velX;
                for(int i=0;i<BRICKS;i++)
                {
                    if(bricks[i].alive == true)
                    {
                         if(checkCollision(ballX, ballY, ballWH, ballWH, bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height) ==  true)
                            {
                                velX = -velX;
                                bricks[i].alive =false;
                                break;
                            }
                    }

                }
                ballY += velY;

                for(int i=0;i<BRICKS;i++)
                {
                    if(bricks[i].alive == true)
                    {
                         if(checkCollision(ballX, ballY, ballWH, ballWH, bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height) ==  true)
                            {
                                velY = -velY;
                                bricks[i].alive =false;
                                break;
                            }
                    }

                }
                if(ballX <0)
                {
                    velX = - velX;
                }
                else if(ballX+ballWH>600)
                {
                    velX = -velX;
                }
                if(ballY <0)
                {
                    velY = -velY;
                }
                else if(ballY+ballWH>480)
                {
                             myX = 270;
                             myY = 450;
                             width = 120;
                             height = 30;
                             ballX = 100;
                             ballY =300;
                             ballWH = 25;
                             velX = 5;
                             velY = -5;

                            for(int i=0;i<BRICKS;i++)
                            {
                                bricks[i].alive = true;
                            }

                }

                if(checkCollision(myX,myY,width,height,ballX,ballY,ballWH, ballWH)== true)
                {
                    velY = -velY;
                }
				//Handle keypress with current mouse position
				else if( e.type == SDL_TEXTINPUT )
				{
					int x = 0, y = 0;
					SDL_GetMouseState( &x, &y );
					handleKeys( e.text.text[ 0 ], x, y );
				}
			}

			//Render quad

			render(myX,myY,width,height,ballX,ballY,ballWH,velX,velY);

			//Update screen
			SDL_GL_SwapWindow( gWindow );
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}
