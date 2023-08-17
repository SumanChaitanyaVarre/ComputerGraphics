#include "hexahedron.hpp"

Hexahedron::Hexahedron(glm::vec3* coordinates, glm::vec3 color)
    :cordinates(coordinates)
{
 m_color = glm::vec4(color, 1);
 cube_vertices();

 glGenVertexArrays(1, &m_vao);

	glGenBuffers(1, &pos_BO);
	glGenBuffers(1, &normal_BO);
	// glGenBuffers(1, &texcoord_BO);
	glGenBuffers(1, &index_BO);

	glBindVertexArray(m_vao);
	//positions
	glBindBuffer(GL_ARRAY_BUFFER, pos_BO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_positions), m_positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//normals
	glBindBuffer(GL_ARRAY_BUFFER, normal_BO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_normals), m_normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);   
}

void Hexahedron::quad(int a, int b, int c, int d)
{
 m_positions[m_idx] =cordinates[a];
 m_idx++;
 m_positions[m_idx] =cordinates[b];
 m_idx++;
 m_positions[m_idx] =cordinates[c];
 m_idx++;

 m_idx -= 3;
 glm::vec3 d1, d2;
 d1 = cordinates[b]-cordinates[a];
 d2 = cordinates[c]-cordinates[a];

 m_normals[m_idx] = -glm::normalize(glm::cross(d1, d2));
 m_idx++;
 m_normals[m_idx] = -glm::normalize(glm::cross(d1, d2));
 m_idx++;
 m_normals[m_idx] = -glm::normalize(glm::cross(d1, d2));
 m_idx++;


 m_positions[m_idx] =cordinates[a];
 m_idx++;
 m_positions[m_idx] =cordinates[c];
 m_idx++;
 m_positions[m_idx] =cordinates[d];
 m_idx++;

 m_idx -= 3;
 //glm::vec3 d1, d2;
 d1 = cordinates[c]-cordinates[a];
 d2 = cordinates[d]-cordinates[a];

 m_normals[m_idx] = -glm::normalize(glm::cross(d1, d2));
 m_idx++;
 m_normals[m_idx] = -glm::normalize(glm::cross(d1, d2));
 m_idx++;
 m_normals[m_idx] = -glm::normalize(glm::cross(d1, d2));
 m_idx++;
}

void Hexahedron::render(glm::mat4& modelviewprojmatrix, glm::mat4 modelMatrix) const
{
 m_shader->use();
 m_shader->setUniformMat4("uProjViewModelMatrix", modelviewprojmatrix);
 m_shader->setUniformMat4("uModelMatrix", modelMatrix);
 m_shader->setUniformVec4("uColor", m_color);
 m_shader->setUniformInt("uColTex", m_texture_slot);
 m_shader->setUniformInt("uIsTextured", m_is_textured);

 glBindVertexArray(m_vao);
 glDrawArrays(GL_TRIANGLES, 0, 36);
}

// generate 12 triangles: 36 vertices and 36 colors
void Hexahedron::cube_vertices(void)
{
    quad(1, 0, 3, 2);
    
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);
}