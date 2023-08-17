#include "cube.hpp"

Cube::Cube(glm::vec3 dimensions, glm::vec3 corner, glm::vec3 color)
	:corner(corner), dim(dimensions)
{
	m_color = glm::vec4(color, 1);

#define X glm::vec3(1,0,0)
#define Y glm::vec3(0,1,0)
#define Z glm::vec3(0,0,1)
#define XY glm::vec3(1,1,0)
#define YZ glm::vec3(0,1,1)
#define ZX glm::vec3(1,0,1)
#define XYZ glm::vec3(1,1,1)
	std::vector<glm::vec3> positions = {
		corner,
		corner + dim * X,
		corner + dim * XY,
		corner + dim * Y,

		corner,
		corner + dim * Y,
		corner + dim * YZ,
		corner + dim * Z,

		corner,
		corner + dim * Z,
		corner + dim * ZX,
		corner + dim * X,

		corner + dim * Z,
		corner + dim * ZX,
		corner + dim * XYZ,
		corner + dim * YZ,

		corner + dim * X,
		corner + dim * XY,
		corner + dim * XYZ,
		corner + dim * ZX,

		corner + dim * Y,
		corner + dim * YZ,
		corner + dim * XYZ,
		corner + dim * XY
	};

	std::vector<glm::vec3> normals = {
		-Z,-Z,-Z,-Z,
		-X,-X,-X,-X,
		-Y,-Y,-Y,-Y,
		Z,Z,Z,Z,
		X,X,X,X,
		Y,Y,Y,Y
	};

	std::vector<glm::vec2> texcoords = {
		glm::vec2(0,0),
		glm::vec2(1,0),
		glm::vec2(0,1),
		glm::vec2(1,1),
		
		glm::vec2(0,0),
		glm::vec2(1,0),
		glm::vec2(0,1),
		glm::vec2(1,1),
		
		glm::vec2(0,0),
		glm::vec2(1,0),
		glm::vec2(0,1),
		glm::vec2(1,1),
		
		glm::vec2(0,0),
		glm::vec2(1,0),
		glm::vec2(0,1),
		glm::vec2(1,1),
		
		glm::vec2(0,0),
		glm::vec2(1,0),
		glm::vec2(0,1),
		glm::vec2(1,1),
		
		glm::vec2(0,0),
		glm::vec2(1,0),
		glm::vec2(0,1),
		glm::vec2(1,1),

	};

	std::vector<unsigned int> indices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4,
	8, 9, 10, 10, 11, 8,
	12, 13, 14, 14, 15, 12,
	16, 17, 18, 18, 19, 16,
	20, 21, 22, 22, 23, 20
	};

	glGenVertexArrays(1, &m_vao);

	glGenBuffers(1, &pos_BO);
	glGenBuffers(1, &normal_BO);
	glGenBuffers(1, &texcoord_BO);
	glGenBuffers(1, &index_BO);

	glBindVertexArray(m_vao);
	//positions
	glBindBuffer(GL_ARRAY_BUFFER, pos_BO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec3) * positions.size(),
		&positions[0],
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//normals
	glBindBuffer(GL_ARRAY_BUFFER, normal_BO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec3) * normals.size(),
		&normals[0],
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//texcoords
	glBindBuffer(GL_ARRAY_BUFFER, texcoord_BO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec2)* texcoords.size(),
		&texcoords[0],
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_BO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * indices.size(),
		&indices[0],
		GL_STATIC_DRAW
	);

}


void Cube::render(glm::mat4& modelviewprojmatrix, glm::mat4 modelMatrix) const
{

	m_shader->use();
	m_shader->setUniformMat4("uProjViewModelMatrix", modelviewprojmatrix);
	m_shader->setUniformMat4("uModelMatrix", modelMatrix);
	m_shader->setUniformVec4("uColor", m_color);
	m_shader->setUniformInt("uColTex", m_texture_slot);
	m_shader->setUniformInt("uIsTextured", m_is_textured);

 glBindVertexArray(m_vao);
	glDrawElements(
		GL_TRIANGLES,
		36,
		GL_UNSIGNED_INT,
		0
	);
}

void Skybox::render()
{
	glBindVertexArray(m_vao);
	glDrawElements(
		GL_TRIANGLES,
		36,
		GL_UNSIGNED_INT,
		0
	);
}
