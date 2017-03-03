/*
===============================================================================================
Name    : Mun Hao Ran
Date    : 11-9-2016
Purpose : Simple 2D Paint Program using OpenGL/GLUT
===============================================================================================
*/

#include <gl/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

// The following statement is to hide the console window
#pragma comment (linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

// Maximum primitives
const int MAX = 500;

// Window width and height
int window_w;
int window_h;

bool dragging = false;

// Color structure
struct Color
{
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;
};

// Shape structure
struct Shape
{
	string type;
	float startX, startY;
	float endX, endY;
	float pointSize;
	float lineWidth;
	bool isFilled = false;
	bool isActivated = false;
	Color color;
};

Color color;
Shape shapeList[MAX];
int shapeCount = 0;

// Draw mode enum
enum DRAW_MODE
{
	NONE, POINT_MODE, LINE_MODE,
	W_TRIANGLE_MODE, F_TRIANGLE_MODE, W_RECTANGLE_MODE, F_RECTANGLE_MODE, W_CIRCLE_MODE, F_CIRCLE_MODE
};

// Right-click menu enum
enum RIGHT_CLICK_MENU
{
	CLEAR, EXIT
};

// Color code enum
enum COLOR_CODE
{
	RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, BLACK
};

DRAW_MODE mode = NONE;

float point_size = 1.0;
float line_width = 1.0;

const float PI = 3.142;

// Handle start drawing process
void handleStartDraw(float x, float y)
{
	if (mode == NONE)
	{
		return;
	}
	else
	{
		// Start drawing
		dragging = true;

		// Active current shape
		shapeList[shapeCount].isActivated = true;

		if (mode == POINT_MODE) // Point mode
		{
			shapeList[shapeCount].type = "Point";
		}
		if (mode == LINE_MODE) // Line mode
		{
			shapeList[shapeCount].type = "Line";
		}
		if (mode == W_TRIANGLE_MODE || mode == F_TRIANGLE_MODE) // Triangle mode
		{
			if (mode == F_TRIANGLE_MODE) // Filled triangle
			{
				shapeList[shapeCount].isFilled = true;
			}
			else // Wireframe triangle
			{
				shapeList[shapeCount].isFilled = false;
			}

			shapeList[shapeCount].type = "Triangle";
		}
		if (mode == W_RECTANGLE_MODE || mode == F_RECTANGLE_MODE) // Rectangle mode
		{
			if (mode == F_RECTANGLE_MODE) // Filled rectangle
			{
				shapeList[shapeCount].isFilled = true;
			}
			else // Wireframe triangle
			{
				shapeList[shapeCount].isFilled = false;
			}

			shapeList[shapeCount].type = "Rectangle";
		}
		if (mode == W_CIRCLE_MODE || mode == F_CIRCLE_MODE) // Circle mode
		{
			if (mode == F_CIRCLE_MODE) // Filled circle
			{
				shapeList[shapeCount].isFilled = true;
			}
			else // Wireframe circle
			{
				shapeList[shapeCount].isFilled = false;
			}

			shapeList[shapeCount].type = "Circle";
		}

		// Set shape coordinates
		shapeList[shapeCount].startX = x;
		shapeList[shapeCount].startY = y;
		shapeList[shapeCount].endX = x;
		shapeList[shapeCount].endY = y;

		// Set shape point size and line width
		shapeList[shapeCount].pointSize = point_size;
		shapeList[shapeCount].lineWidth = line_width;

		// Set shape color
		shapeList[shapeCount].color.r = color.r;
		shapeList[shapeCount].color.g = color.g;
		shapeList[shapeCount].color.b = color.b;

		shapeCount++;
	}
}

// Handle continue drawing process
void handleContinueDraw(float x, float y)
{
	if (!dragging)
	{
		return;
	}

	// Get current position of end X and Y
	int current = shapeCount - 1;

	shapeList[current].endX = x;
	shapeList[current].endY = y;

	glutPostRedisplay();
}

// Handle finish drawing process
void handleFinishDraw(float x, float y)
{
	if (!dragging)
	{
		return;
	}

	// Finish drawing
	dragging = false;

	// Get current position of all coordinates
	int current = shapeCount - 1;

	if (shapeList[current].startX == shapeList[current].endX &&
		shapeList[current].startX == shapeList[current].endY)
	{
		shapeCount--;
	}

	glutPostRedisplay();
}

// Clear all primivites on drawing area
void clearPrimitives()
{
	// Reset shape count to zero
	shapeCount = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
	glutPostRedisplay();
}

// Color menu
void colorMenu(int code)
{
	if (code == RED) // Red
	{
		color.r = 1.0;
		color.g = 0.0;
		color.b = 0.0;
	}
	else if (code == GREEN) // Green
	{
		color.r = 0.0;
		color.g = 1.0;
		color.b = 0.0;
	}
	else if (code == BLUE) // Blue
	{
		color.r = 0.0;
		color.g = 0.0;
		color.b = 1.0;
	}
	else if (code == CYAN) // Cyan
	{
		color.r = 0.0;
		color.g = 1.0;
		color.b = 1.0;
	}
	else if (code == MAGENTA) // Magenta
	{
		color.r = 1.0;
		color.g = 0.0;
		color.b = 1.0;
	}
	else if (code == YELLOW) // Yellow
	{
		color.r = 1.0;
		color.g = 1.0;
		color.b = 0.0;
	}
	else if (code == WHITE) // White
	{
		color.r = 1.0;
		color.g = 1.0;
		color.b = 1.0;
	}
	else if (code == BLACK) // Black
	{
		color.r = 0.0;
		color.g = 0.0;
		color.b = 0.0;
	}

	int current = shapeCount - 1;

	// Change color of current shape
	if (shapeList[current].isActivated)
	{
		shapeList[current].color.r = color.r;
		shapeList[current].color.g = color.g;
		shapeList[current].color.b = color.b;
	}

	glutPostRedisplay();
}

// Point size menu
void pointSizeMenu(int size)
{
	if (size == 1.0) // 1.0
	{
		point_size = 1.0;
	}
	else if (size == 2.0) // 2.0
	{
		point_size = 2.0;
	}
	else if (size == 3.0) // 3.0
	{
		point_size = 3.0;
	}
	else if (size == 4.0) // 4.0
	{
		point_size = 4.0;
	}
	else if (size == 5.0) // 5.0
	{
		point_size = 5.0;
	}
	else if (size == 6.0) // 6.0
	{
		point_size = 6.0;
	}
}

// Line width menu
void lineWidthMenu(int width)
{
	if (width == 1.0) // 1.0
	{
		line_width = 1.0;
	}
	else if (width == 2.0) // 2.0
	{
		line_width = 2.0;
	}
	else if (width == 3.0) // 3.0
	{
		line_width = 3.0;
	}
	else if (width == 4.0) // 4.0
	{
		line_width = 4.0;
	}
	else if (width == 5.0) // 5.0
	{
		line_width = 5.0;
	}
	else if (width == 6.0) // 6.0
	{
		line_width = 6.0;
	}

	int current = shapeCount - 1;

	// Change line width of current shape
	if (shapeList[current].isActivated)
	{
		shapeList[current].lineWidth = line_width;
	}

	glutPostRedisplay();
}

// Right-click menu
void rightClickMenu(int option)
{
	switch (option)
	{
	case CLEAR:	// Clear	
		clearPrimitives();
		break;

	case EXIT: // Exit
		exit(0);
	}
}

// Create right-click mouse menu
void createMouseMenu()
{
	int c_menu, ps_menu, lw_menu;

	// Create color sub-menu
	c_menu = glutCreateMenu(colorMenu);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Cyan", CYAN);
	glutAddMenuEntry("Magenta", MAGENTA);
	glutAddMenuEntry("Yellow", YELLOW);
	glutAddMenuEntry("White", WHITE);
	glutAddMenuEntry("Black", BLACK);

	// Create point size sub-menu
	ps_menu = glutCreateMenu(pointSizeMenu);
	glutAddMenuEntry("1.0", 1.0);
	glutAddMenuEntry("2.0", 2.0);
	glutAddMenuEntry("3.0", 3.0);
	glutAddMenuEntry("4.0", 4.0);
	glutAddMenuEntry("5.0", 5.0);
	glutAddMenuEntry("6.0", 6.0);

	// Create point size sub-menu
	lw_menu = glutCreateMenu(lineWidthMenu);
	glutAddMenuEntry("1.0", 1.0);
	glutAddMenuEntry("2.0", 2.0);
	glutAddMenuEntry("3.0", 3.0);
	glutAddMenuEntry("4.0", 4.0);
	glutAddMenuEntry("5.0", 5.0);
	glutAddMenuEntry("6.0", 6.0);

	// Create main menu
	glutCreateMenu(rightClickMenu);
	glutAddSubMenu("Colors", c_menu);
	glutAddSubMenu("Point size", ps_menu);
	glutAddSubMenu("Line width", lw_menu);
	glutAddMenuEntry("Clear", CLEAR);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Select drawing mode from mouse click
void selectMode(int x, int y, int modifiers)
{
	if (y > window_h - 50) // Point mode
	{
		mode = POINT_MODE;
	}
	else if (y > window_h - 100) // Line mode
	{
		mode = LINE_MODE;
	}
	else if (y > window_h - 150) // Wireframe triangle mode
	{
		mode = W_TRIANGLE_MODE;
	}
	else if (y > window_h - 200) // Filled triangle mode
	{
		mode = F_TRIANGLE_MODE;
	}
	else if (y > window_h - 250) // Wireframe rectangle mode
	{
		mode = W_RECTANGLE_MODE;
	}
	else if (y > window_h - 300) // Filled rectangle mode
	{
		mode = F_RECTANGLE_MODE;
	}
	else if (y > window_h - 350) // Wireframe circle mode
	{
		mode = W_CIRCLE_MODE;
	}
	else if (y > window_h - 400) // Filled circle mode
	{
		mode = F_CIRCLE_MODE;
	}
}

// Draw outline around selected mode
void drawSelected()
{
	if (mode == NONE)
	{
		return;
	}
	if (mode == POINT_MODE) // Point mode
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(2, window_h - 2);
		glVertex2i(49, window_h - 2);
		glVertex2i(49, window_h - 49);
		glVertex2i(2, window_h - 49);
		glEnd();
	}
	else if (mode == LINE_MODE) // Line mode
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(2, window_h - 51);
		glVertex2i(49, window_h - 51);
		glVertex2i(49, window_h - 99);
		glVertex2i(2, window_h - 99);
		glEnd();
	}
	else if (mode == W_TRIANGLE_MODE) // Wireframe triangle mode
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(2, window_h - 101);
		glVertex2i(49, window_h - 101);
		glVertex2i(49, window_h - 149);
		glVertex2i(2, window_h - 149);
		glEnd();
	}
	else if (mode == F_TRIANGLE_MODE) // Filled triangle mode
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(2, window_h - 151);
		glVertex2i(49, window_h - 151);
		glVertex2i(49, window_h - 199);
		glVertex2i(2, window_h - 199);
		glEnd();
	}
	else if (mode == W_RECTANGLE_MODE) // Wireframe rectangle mode
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(2, window_h - 201);
		glVertex2i(49, window_h - 201);
		glVertex2i(49, window_h - 249);
		glVertex2i(2, window_h - 249);
		glEnd();
	}
	else if (mode == F_RECTANGLE_MODE) // Filled rectangle mode
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(2, window_h - 251);
		glVertex2i(49, window_h - 251);
		glVertex2i(49, window_h - 299);
		glVertex2i(2, window_h - 299);
		glEnd();
	}
	else if (mode == W_CIRCLE_MODE) // Wireframe circle mode
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(2, window_h - 301);
		glVertex2i(49, window_h - 301);
		glVertex2i(49, window_h - 349);
		glVertex2i(2, window_h - 349);
		glEnd();
	}
	else if (mode == F_CIRCLE_MODE) // Filled circle mode
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(2, window_h - 351);
		glVertex2i(49, window_h - 351);
		glVertex2i(49, window_h - 399);
		glVertex2i(2, window_h - 399);
		glEnd();
	}
}

// Draw menu icon
void drawMenuIcon()
{
	// Draw point icon
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(4);
	glBegin(GL_POINTS);
	glVertex2i(25, window_h - 25);
	glEnd();

	// Draw line icon
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(10, window_h - 90);
	glVertex2i(40, window_h - 60);
	glEnd();

	// Draw wireframe triangle icon
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(25, window_h - 110);
	glVertex2i(10, window_h - 135);
	glVertex2i(40, window_h - 135);
	glEnd();

	// Draw filled triangle icon
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex2i(25, window_h - 160);
	glVertex2i(10, window_h - 185);
	glVertex2i(40, window_h - 185);
	glEnd();

	// Draw wireframe rectangle icon
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(10, window_h - 210);
	glVertex2i(40, window_h - 210);
	glVertex2i(40, window_h - 240);
	glVertex2i(10, window_h - 240);
	glEnd();

	// Draw filled rectangle icon
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2i(10, window_h - 260);
	glVertex2i(40, window_h - 260);
	glVertex2i(40, window_h - 290);
	glVertex2i(10, window_h - 290);
	glEnd();

	int segment = 300;
	float twoPI = PI * 2.0;
	float radius = 15.0;

	// Draw wireframe circle icon
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);

	for (int i = 0 ; i < segment; i++)
	{	
		glVertex2f(25 + (radius * cos(i *  twoPI / segment)), 
			(window_h - 325) + (radius * sin(i * twoPI / segment)));
	}

	glEnd();

	// Draw filled circle icon
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < segment; i++)
	{
		glVertex2f(25 + (radius * cos(i *  twoPI / segment)),
			(window_h - 375) + (radius * sin(i * twoPI / segment)));
	}

	glEnd();

	// Draw color status box
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	glVertex2i(2, window_h - 401);
	glVertex2i(49, window_h - 401);
	glVertex2i(49, window_h - 449);
	glVertex2i(2, window_h - 449);
	glEnd();
}

// Draw paint menu
void drawPaintMenu()
{
	// Draw menu bar
	glColor3f(0.4, 0.4, 0.4);
	glRectf(0, 0, 50, window_h);

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0);
	glBegin(GL_LINES);

	// Draw bottom line
	glVertex2i(1, 1);
	glVertex2i(50, 1);

	// Draw left line
	glVertex2i(1, 1);
	glVertex2i(1, window_h);

	// Draw right line
	glVertex2i(50, 1);
	glVertex2i(50, window_h);

	// Draw top line
	glVertex2i(1, window_h - 1);
	glVertex2i(50, window_h - 1);

	int distance = 50;

	// Draw 9 lines for each icon
	for (int i = 1; i <= 9; i++)
	{
		glVertex2i(1, window_h - distance);
		glVertex2i(50, window_h - distance);

		distance = distance + 50;
	}

	glEnd();
}

// Draw menu interface
void drawInterface()
{
	// Draw paint menu
	drawPaintMenu();

	// Draw outline around selected mode
	drawSelected();

	// Draw menu icon
	drawMenuIcon();

	glutPostRedisplay();
}

// Draw all primitives on drawing area
void drawPrimitives()
{
	for (int i = 0; i < shapeCount; i++)
	{
		// Set primitives color based on selected color
		glColor3f(shapeList[i].color.r, shapeList[i].color.g, shapeList[i].color.b);

		if (shapeList[i].type == "Point") // Point
		{
			// Draw point
			glPointSize(shapeList[i].pointSize);
			glBegin(GL_POINTS);
			glVertex2f(shapeList[i].startX, shapeList[i].startY);
			glEnd();
		}
		if (shapeList[i].type == "Line") // Line
		{
			// Draw line
			glLineWidth(shapeList[i].lineWidth);
			glBegin(GL_LINES);
			glVertex2f(shapeList[i].startX, shapeList[i].startY);
			glVertex2f(shapeList[i].endX, shapeList[i].endY);
			glEnd();
		}

		if (shapeList[i].type == "Triangle") // Triangle
		{
			if (shapeList[i].isFilled) // Filled
			{
				glBegin(GL_TRIANGLES);
			}
			else // Wireframe
			{
				glLineWidth(shapeList[i].lineWidth);
				glBegin(GL_LINE_LOOP);
			}

			// Draw triangle
			glVertex2f(shapeList[i].startX, shapeList[i].startY);
			glVertex2f(((shapeList[i].endX - shapeList[i].startX) / 2) +
				shapeList[i].startX, shapeList[i].endY);
			glVertex2f(shapeList[i].endX, shapeList[i].startY);
			glEnd();
		}
		if (shapeList[i].type == "Rectangle") // Rectangle
		{
			if (shapeList[i].isFilled) // Filled
			{
				glBegin(GL_QUADS);
			}
			else // Wireframe
			{
				glLineWidth(shapeList[i].lineWidth);
				glBegin(GL_LINE_LOOP);
			}

			// Draw rectangle
			glVertex2f(shapeList[i].startX, shapeList[i].startY);
			glVertex2f(shapeList[i].endX, shapeList[i].startY);
			glVertex2f(shapeList[i].endX, shapeList[i].endY);
			glVertex2f(shapeList[i].startX, shapeList[i].endY);
			glEnd();
		}
		if (shapeList[i].type == "Circle") // Circle
		{	
			if (shapeList[i].isFilled) // Filled
			{
				glBegin(GL_TRIANGLE_FAN);
			}
			else // Wireframe
			{
				glLineWidth(shapeList[i].lineWidth);
				glBegin(GL_LINE_LOOP);
			}

			// Calculate half width and height
			float halfWidth = (shapeList[i].endX - shapeList[i].startX) / 2;
			float halfHeight = (shapeList[i].endY - shapeList[i].startY) / 2;

			// Calculate center X and Y
			float centerX = shapeList[i].startX + halfWidth;
			float centerY = shapeList[i].startY + halfHeight;

			// Draw circle
			for (int j = 0; j < 360; j++) 
			{
				float angle = j * PI / 180.0;
				float x = centerX + (cos(angle) * halfWidth);
				float y = centerY + (sin(angle) * halfHeight);
				glVertex2f(x, y);
			}

			glEnd();
		}

		// Create frame around primitives when drawing
		if (shapeList[i].isActivated)
		{
			glColor3f(1.0, 1.0, 1.0);
			glLineWidth(1.0);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.0, 0xF0F0);
			glBegin(GL_LINE_LOOP);

			// Line frame
			if (shapeList[i].type == "Line")
			{
				glVertex2f(shapeList[i].startX, shapeList[i].startY);
				glVertex2f(shapeList[i].endX, shapeList[i].endY);
			}
			// Triangle/Rectangle/Circle frame
			if (shapeList[i].type == "Triangle" || shapeList[i].type == "Rectangle" ||
				shapeList[i].type == "Circle")
			{
				glVertex2f(shapeList[i].startX, shapeList[i].startY);
				glVertex2f(shapeList[i].endX, shapeList[i].startY);
				glVertex2f(shapeList[i].endX, shapeList[i].endY);
				glVertex2f(shapeList[i].startX, shapeList[i].endY);
			}

			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
	}
}

// Initialize menu area and drawing area
void initArea(float x1, float x2, float y1, float y2)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(x1, x2, y1, y2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Initialize OpenGL/GLUT settings
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_w, 0, window_h);
}

// Display callback
void display()
{
	glClearColor(0.5, 0.5, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(50, 0, window_w - 50, window_h);

	// Initialize drawing area
	initArea(0, 1, 0, 1);

	drawPrimitives();

	glViewport(0, 0, 50, window_h);

	// Initialize paint menu area
	initArea(0, 51, 0, window_h);

	drawInterface();

	glutSwapBuffers();
}

// Reshape callback
void reshape(int w, int h)
{
	// Adjust clipping
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Adjust viewport and clear
	glViewport(0, 0, w, h);
	glClearColor(0.8, 0.8, 0.8, 0.8);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display();
	glFlush();

	// Set new window width and height value
	window_w = w;
	window_h = h;
}

// Mouse click callback
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (x < 50 && state == GLUT_DOWN)
		{
			// Select drawing mode
			selectMode(x, window_h - y, glutGetModifiers());
		}

		float wx, wy;

		wx = (float)(x - 50) / (window_w - 50);
		wy = (float)(window_h - y) / window_h;

		if (state == GLUT_DOWN)
		{
			// Deactivate previus shape
			shapeList[shapeCount - 1].isActivated = false;

			// Handle start draw
			handleStartDraw(wx, wy);
		}
		else
		{
			// Handle finish draw
			handleFinishDraw(wx, wy);
		}
	}
}

// Mouse motion callback
void motion(int x, int y)
{
	if (dragging)
	{
		float wx, wy;

		wx = (float)(x - 50) / (window_w - 50);
		wy = (float)(window_h - y) / window_h;

		// Handle continue draw
		handleContinueDraw(wx, wy);
	}
}

// Main
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600); // Set window size
	glutInitWindowPosition(150, 50); // Set window position
	glutCreateWindow("Assignment 1 - Simple 2D Paint Program");
	init();

	// Create right-click mouse menu
	createMouseMenu();

	// Callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}