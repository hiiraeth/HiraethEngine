#ifndef INC_3D_SHAPES_H
#define INC_3D_SHAPES_H
#include "Shaders.h"

struct GLMesh
{
    unsigned int VAO;
    unsigned int VBO[2];
    unsigned int EBO;

	unsigned int nVertices;
	unsigned int nIndices;
};

class Shapes
{
public:
    // 2D meshes
    GLMesh Triangle;
    GLMesh Square;
    GLMesh Rectangle;
	GLMesh Plane;
    // HiraethEngine Meshes
    GLMesh Cube;
    GLMesh Sphere;

    Shaders* m_shaders;

public:
    Shapes(Shaders* shader);
    ~Shapes();

    // loading mesh data into GPU
    void LoadPlaneMesh();
    void LoadTriangleMesh();
    void LoadSquareMesh();
    void LoadRectangleMesh();
    void LoadCubeMesh();
    void LoadSphereMesh();

    // rendering mesh data to screen
    void DrawPlaneMesh();
    void DrawTriangleMesh();
    void DrawSquareMesh();
    void DrawRectangleMesh();
    void DrawCubeMesh();
    void DrawSphereMesh();

    void SetShaderMemoryLoc();
};


#endif //INC_3D_SHAPES_H