#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>

GLuint program;
GLint attribute_coord2d;
GLint uniform_offset_x;
GLint uniform_scale_x;
GLint uniform_sprite;

float offset_x = 0.0;
float scale_x = 1.0;

struct point {
	GLfloat x;
	GLfloat y;
};

GLuint vbo;

int init_resources() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	point graph[2000];
	for (int i = 0; i < 2000; i++) {
		float x = (i - 1000.0) / 100.0;

		graph[i].x = x;
		graph[i].y = sin(x * 10.0) / (1.0 + x * x);
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_STATIC_DRAW);

	return 1;
}

void display() {
	glUseProgram(program);

	glUniform1f(uniform_offset_x, offset_x);
	glUniform1f(uniform_scale_x, scale_x);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(attribute_coord2d);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glUniform1f(uniform_sprite, 0);
	glDrawArrays(GL_LINE_STRIP, 0, 2000);
	glutSwapBuffers();
}

void free_resources() {
	glDeleteProgram(program);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Data");

	GLenum glew_status = glewInit();

	if (init_resources()) {
		glutDisplayFunc(display);
		glutMainLoop();
	}
	free_resources();
	return 0;
}
