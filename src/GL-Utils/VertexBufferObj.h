#include <array>

class VertexBufferObj
{
	unsigned int id;
	int vertSize; //How many floats are in a single vertex
public:
	//Binds the vertex buffer
	void Bind();
	
	//Enable the vertex buffer
	void Enable();
	
	//Returns the id of the vertex buffer
	unsigned int GetId();

	//Sends data to the buffer
	void Data(float *dat, int size, int vSize);

	//Constructor
	VertexBufferObj();
};
