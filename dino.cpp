#include "dino.h"
#include <string>

bool dino::loadOBJ(const char* path, std::vector<glm::vec3>& out_verts, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_norms)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Cannot open object file!\n");
		return false;
	}

	while (1)
	{
		char lineHeader[128];
		// read first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break;
		}

		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
																	   &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
																	   &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("Obj file can't be read by simple parser! faces is not 9 sized\nmatches = %d", matches);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_verts.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++)
	{
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++)
	{
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_norms.push_back(normal);
	}

    return true;
}

dino::~dino(void)
{
	if (initialized)
	{
		glDeleteLists(display_list, 1);
		glDeleteTextures(1, &texture_obj);
	}
}

bool dino::Initialize(float posx, float posy, float posz, float scalex, float scaley, float scalez)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	if (!loadOBJ("dino3.obj", vertices, uvs, normals))
	{
		printf("Failed to load obj file!\n");
		return false;
	}


    // Now do the geometry. Create the display list.
    display_list = glGenLists(1);
    glNewList(display_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(0.0, 0.0, 1.0);

	glRotatef(90, 1, 0, 0);
	glRotatef(60, 0, 1, 0);
	//glTranslatef(35, 2, -35);
	// Draw the ground as a quadrilateral, specifying texture coordinates.
	glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			glNormal3f(normals[i].x, normals[i].y, normals[i].z);
			glVertex3f((vertices[i].x*scalex)+posx, (vertices[i].y*scaley)+posy, (vertices[i].z*scalez)+posz);
		}
	glEnd();
    glEndList();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;
	return true;
}

void dino::Draw(void)
{
	glPushMatrix();
	glCallList(display_list);
	glPopMatrix();
}
