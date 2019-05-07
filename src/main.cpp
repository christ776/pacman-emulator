#include <stdio.h>
#include <GL/glut.h>
#include "parser/parser.h"
#include "renderer/opengl_renderer.h"

 
// Display size 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define TILE_SIZE 16
 
int modifier = 10;
 
// Window size
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;
 
void display();
void reshape_window(GLsizei w, GLsizei h);
 
// Use new drawing method
uint8_t screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3]; 
std::vector<Pixel> pixelData;
void setupTexture();
 
int main(int argc, char **argv) 
{		
    // Setup OpenGL
    glutInit(&argc, argv);          
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 
    glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
    glutCreateWindow("Pacman Emulator by Christian De Martino");

    vector<char> tileData = readTileROM("./pacman/pacman.5e");
    pixelData = decodeTiles(tileData);	

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape_window);
 
    setupTexture();
 
    glutMainLoop(); 
 
    return 0;
}
 
// Setup Texture
void setupTexture()
{
    // Clear screen
    for(int y = 0; y < SCREEN_HEIGHT; ++y)		
        for(int x = 0; x < SCREEN_WIDTH; ++x)
            screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;
 
    // Create a texture 
    glTexImage2D(GL_TEXTURE_2D, 0, 3, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);
 
    // Set up the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
 
    // Enable textures
    glEnable(GL_TEXTURE_2D);
}
 
void updateTexture()
{	
    //Update pixels
	for(int y = 0; y < SCREEN_HEIGHT; y++)	{
        for(int x = 0; x < SCREEN_WIDTH; x++) {
            Pixel pixel = pixelData.at(y * TILE_SIZE * 8 + x);
            screenData[y][x][0] = pixel.color.red;
            screenData[y][x][1] = pixel.color.green;
            screenData[y][x][2] = pixel.color.blue;
        }
    }
        
    // Update Texture
    glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);
 
    glBegin( GL_QUADS );
        glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
        glTexCoord2d(1.0, 0.0); 	glVertex2d(display_width, 0.0);
        glTexCoord2d(1.0, 1.0); 	glVertex2d(display_width, display_height);
        glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  display_height);
    glEnd();
}
 
void display()
{ 
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw pixels to texture
    updateTexture();

    // Swap buffers!
    glutSwapBuffers(); 
}
 
void reshape_window(GLsizei w, GLsizei h)
{
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);        
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
 
    // Resize quad
    display_width = w;
    display_height = h;
}
