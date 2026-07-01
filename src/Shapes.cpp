// ReSharper disable All
#include "Shapes.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../include/Shaders.h"
#include "glad/glad.h"

namespace
{
    const GLuint g_vertexFloats  = 3;
    const GLuint g_normalFloats  = 3;
    const GLuint g_textureFloats = 2;

    const GLvoid* g_pOffset = 0;
    const GLvoid* g_nOffset = (void*)(sizeof(float) * g_vertexFloats);
    const GLvoid* g_tOffset = (void*)(sizeof(float) * (g_vertexFloats + g_normalFloats));

    const GLint stride = sizeof(float) * (g_vertexFloats + g_textureFloats + g_normalFloats);
}

Shapes::Shapes(Shaders* shaders)
{
	m_shaders = shaders;
}

Shapes::~Shapes()
{
}

void Shapes::LoadPlaneMesh()
{
	const GLfloat planeVertices[] =
	{
//		      position                normal             texture
		-1.0f,  0.0f,  1.0f,    0.0f,  1.0f,  0.0f,    0.0f,  0.0f,
		 1.0f,  0.0f,  1.0f,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,
		 1.0f,  0.0f, -1.0f,    0.0f,  1.0f,  0.0f,    1.0f,  1.0f,
		-1.0f,  0.0f, -1.0f,    0.0f,  1.0f,  0.0f,    0.0f,  1.0f
	};

	const GLuint planeIndices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	Plane.nVertices = sizeof(planeVertices) / (sizeof(planeVertices[0]) *
		(g_normalFloats + g_textureFloats + g_vertexFloats));
	Plane.nIndices = sizeof(planeIndices) / sizeof(planeIndices[0]);

	glGenVertexArrays(1, &Plane.VAO);
	glBindVertexArray(Plane.VAO);

	glGenBuffers(2, Plane.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, Plane.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices),
		&planeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Plane.VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices),
		&planeIndices, GL_STATIC_DRAW);

	SetShaderMemoryLoc();
}

void Shapes::DrawPlaneMesh()
{
	glBindVertexArray(Plane.VAO);
	glDrawElements(GL_TRIANGLES, Plane.nIndices, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Shapes::LoadTriangleMesh()
{
    const GLfloat triangleVertices[] =
    {
//            position                normal             texture
        -0.5f, -0.5f,  0.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,
         0.0f,  0.5f,  0.0f,    0.0f,  0.0f,  1.0f,    0.5f,  1.0f,
         0.5f, -0.5f,  0.0f,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f
    };

    // generate VAO
    glGenVertexArrays(1, &Triangle.VAO);
    glBindVertexArray(Triangle.VAO);

    // generate VBO
    glGenBuffers(1, &Triangle.VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Triangle.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices),
        &triangleVertices, GL_STATIC_DRAW);

    SetShaderMemoryLoc();
}

void Shapes::LoadSquareMesh()
{
    const GLfloat squareVertices[] = {
//            position                normal             texture
        -0.5f, -0.5f,  0.0f,    0.0f,  0.0f,  1.0f,    0.25f, 0.25f,
        -0.5f,  0.5f,  0.0f,    0.0f,  0.0f,  1.0f,    0.25f, 0.75f,
         0.5f,  0.5f,  0.0f,    0.0f,  0.0f,  1.0f,    0.75f, 0.75f,
         0.5f, -0.5f,  0.0f,    0.0f,  0.0f,  1.0f,    0.25f, 0.75f
    };

    glGenVertexArrays(1, &Square.VAO);
    glBindVertexArray(Square.VAO);

    glGenBuffers(1, &Square.VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Square.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices),
        &squareVertices, GL_STATIC_DRAW);

    SetShaderMemoryLoc();
}

void Shapes::LoadRectangleMesh()
{
    const GLfloat rectVertices[] = {
//            position                normal              texture
        -0.5f, -0.25f, 0.0f,    0.0f,  0.0f,  1.0f,    0.25f, 0.375f,
        -0.5f,  0.25f, 0.0f,    0.0f,  0.0f,  1.0f,    0.25f, 0.625f,
         0.5f,  0.25f, 0.0f,    0.0f,  0.0f,  1.0f,    0.75f, 0.625f,
         0.5f, -0.25f, 0.0f,    0.0f,  0.0f,  1.0f,    0.75f, 0.375f
    };

    glGenVertexArrays(1, &Rectangle.VAO);
    glBindVertexArray(Rectangle.VAO);

    glGenBuffers(1, &Rectangle.VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Rectangle.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices),
        &rectVertices, GL_STATIC_DRAW);

    SetShaderMemoryLoc();
}

void Shapes::LoadCubeMesh()
{
    const GLfloat cubeVertices[] = {
//			Positions				  Normals           Texture
    // Back Face
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,

    // Front Face
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    0.0f, 0.0f,

    // Left Face
		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f, 0.0f,    1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f, 0.0f,    1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f, 0.0f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f, 0.0f,    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f, 0.0f,    0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f, 0.0f,    1.0f, 0.0f,

    // Right Face
		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f, 0.0f,    1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f, 0.0f,    0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,    1.0f,  0.0f, 0.0f,    1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f, 0.0f,    0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f, 0.0f,    1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    1.0f,  0.0f, 0.0f,    0.0f, 0.0f,

    // Bottom Face
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 1.0f,

    // Top Face
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f,    0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f,    0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f,    1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f,    1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f,    0.0f, 1.0f
};

	glGenVertexArrays(1, &Cube.VAO);
	glBindVertexArray(Cube.VAO);

	glGenBuffers(1, &Cube.VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Cube.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices),
		&cubeVertices, GL_STATIC_DRAW);

	SetShaderMemoryLoc();
}

void Shapes::LoadSphereMesh()
{
}

void Shapes::DrawTriangleMesh()
{
    glBindVertexArray(Triangle.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Shapes::DrawSquareMesh()
{
	glBindVertexArray(Square.VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

void Shapes::DrawRectangleMesh()
{
	glBindVertexArray(Rectangle.VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

void Shapes::DrawCubeMesh()
{
	glBindVertexArray(Cube.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Shapes::DrawSphereMesh()
{
}

void Shapes::SetShaderMemoryLoc()
{
    // position
    glVertexAttribPointer(0, g_vertexFloats, GL_FLOAT,
        GL_FALSE, stride, g_pOffset);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, g_normalFloats, GL_FLOAT,
        GL_FALSE, stride, g_nOffset);
    glEnableVertexAttribArray(1);

    // texture
    glVertexAttribPointer(2, g_textureFloats, GL_FLOAT,
        GL_FALSE, stride, g_tOffset);
    glEnableVertexAttribArray(2);
}
