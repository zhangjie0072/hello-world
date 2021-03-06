/*
  *外形轮廓类实现
*/
//Version:1.0 实现了最基本的物体数据
//Version 2.0:修改了关于立方体,球体生成数据的bug
//Version 3.0:增加了天空盒
//Version 4.0:添加了对切线的直接支持
//Version 5.0:改进了网格模型的生成
//Version 6.0:考虑到使用的方便性,该版本中重命名了类,啥去了类的开头GL
#include<engine/Shape.h>
#include<engine//Geometry.h>
#include<GL/glew.h>
#include<assert.h>
#include<stdlib.h>
/////////////////////////////////////////Shape Class//////////////////////////////
Shape::Shape()
{
	_numberOfVertex = 0;
	_numberOfIndice = 0;
	_indiceVBO = 0;
	_vertexVBO = 0;
	_texCoordVBO = 0;
	_normalVBO = 0;
	_tangentVBO = 0;
}
Shape::~Shape()
{
	glDeleteBuffers(1, &_indiceVBO);
	glDeleteBuffers(1, &_vertexVBO);
	glDeleteBuffers(1, &_texCoordVBO);
	glDeleteBuffers(1,&_normalVBO); 
	glDeleteBuffers(1,&_tangentVBO);
    _indiceVBO = 0;
	_vertexVBO = 0;
	_texCoordVBO = 0;
	_normalVBO = 0;
	_numberOfVertex = 0;
	_numberOfIndice = 0;
}
int    Shape::numberOfIndice()
{
	return _numberOfIndice;
}
int    Shape::numberOfVertex()
{
	return _numberOfVertex;
}
int      Shape::getVertexBufferId()
{
	return _vertexVBO;
}
void    Shape::bindIndiceObject()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiceVBO);
}
void   Shape::bindVertexObject(int  _loc)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	glEnableVertexAttribArray(_loc);
	glVertexAttribPointer(_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
void   Shape::bindNormalObject(int  _loc)
{
	glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
	glEnableVertexAttribArray(_loc);
	glVertexAttribPointer(_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
void    Shape::bindTexCoordObject(int  _loc)
{
	glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
	glEnableVertexAttribArray(_loc);
	glVertexAttribPointer(_loc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}
void   Shape::bindTangentObject(int loc)
{
	glBindBuffer(GL_ARRAY_BUFFER, _tangentVBO);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
void       Shape::finish()
{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void      Shape::drawShape()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiceVBO);
	glDrawElements(GL_TRIANGLES, _numberOfIndice, GL_UNSIGNED_INT, NULL);
}
//////////////////////////////////////////GLGround  Class////////////////////////////////////////////////////////////////////////////
Ground::Ground()
{
}
//析构
Ground::~Ground()
{

}
//创建GLGround对象
Ground       *Ground::createWithGrid(int  _size,float scale)
{
	Ground    *_ground = new    Ground();
	_ground->initWithGrid(_size,scale);
	return   _ground;
}
//
//初始化
void          Ground::initWithGrid(int  _size,float scale)
{
		float     *vertex;
		int       *indice;
		_numberOfIndice = esGenSquareGrid(_size,scale, &vertex, &indice, &_numberOfVertex);
//生成缓冲区对象,绑定并填充数据
		glGenBuffers(1, &_vertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*_numberOfVertex, vertex,GL_STATIC_DRAW);
//tex coord
		float     *texCoord = vertex;
		const   float      _factor = _size - 1;
		for (int i = 0; i < _numberOfVertex; ++i)
		{
			int     _row = i/_size;
			int     _column = i%_size;
			texCoord[i << 1] = _column / _factor;
			texCoord[(i << 1) + 1] = _row / _factor;
		}
		glGenBuffers(1, &_texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float)*_numberOfVertex, texCoord, GL_STATIC_DRAW);
		texCoord = NULL;
//indice 
		glGenBuffers(1, &_indiceVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiceVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*_numberOfIndice, indice,GL_STATIC_DRAW);
//normal
		const     int    _normal_size = _numberOfVertex * 3;
		float      *normal = vertex;
		for (int i = 0; i < _normal_size; i+=3)
		{
			normal[i] = 0.0f;
			normal[i + 1] = 0.0f;
			normal[i + 2] = 1.0f;
		}
		glGenBuffers(1, &_normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*_numberOfVertex, normal, GL_STATIC_DRAW);
		normal = NULL;
//free memory
		delete indice;
		delete vertex;
		indice = NULL;
		vertex = NULL;
}
void     Ground::bindTangentObject(int _loc)
{
	glDisableVertexAttribArray(_loc);
	glVertexAttrib3fv(_loc,(float *)&GLVector3(1.0f,0.0f,0.0f));
}
///////////////////////////////////立方体////////////////////////////////////
Cube::Cube()
{
}
Cube::~Cube()
{
}
//
void        Cube::initWithScale(float  scale)
{
	float     *vertex, *normal, *texCoord,*tangent;
	_numberOfIndice = esGenCube(scale, &vertex, &normal, &texCoord,&tangent ,&_numberOfVertex);
//
	glGenBuffers(1, &_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*_numberOfVertex,vertex,GL_STATIC_DRAW);
//
	glGenBuffers(1, &_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*_numberOfVertex, normal,GL_STATIC_DRAW);
//
	glGenBuffers(1, &_texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*_numberOfVertex, texCoord, GL_STATIC_DRAW);
//
	glGenBuffers(1, &_tangentVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _tangentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * _numberOfVertex, tangent, GL_STATIC_DRAW);
//
//free memory
	delete  vertex;
	delete  normal;
	delete  texCoord;
	delete  tangent;
	vertex = NULL;
	normal = NULL;
	texCoord = NULL;
}
//
Cube       *Cube::createWithScale(float scale)
{
	Cube     *_cube = new   Cube();
	_cube->initWithScale(scale);
	return  _cube;
}
void    Cube::bindIndiceObject()
{

}
void   Cube::bindTexCoordObject(int _loc)
{
	glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
	glEnableVertexAttribArray(_loc);
	glVertexAttribPointer(_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
void   Cube::drawShape()
{
	glDrawArrays(GL_TRIANGLES,0,_numberOfVertex);
}
//
/////////////////////////////////////////////GLSphere///////////////////////////////////////
Sphere::Sphere()
{
}
//
Sphere::~Sphere()
{
}
//
Sphere     *Sphere::createWithSlice(int  slices, float radius)
{
	Sphere   *_sphere = new   Sphere();
	_sphere->initWithSlice(slices, radius);
	return _sphere;
}
//
void      Sphere::initWithSlice(int   slices,float  radius)
{
	float   *vertex, *texCoord, *normal,*tangent;
	int     *indice;
	_numberOfIndice = esGenSphere(slices, radius, &vertex, &normal, &tangent, &texCoord, &indice, &_numberOfVertex);

	glGenBuffers(1, &_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*_numberOfVertex, vertex, GL_STATIC_DRAW);
//
	glGenBuffers(1, &_texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float)*_numberOfVertex, texCoord, GL_STATIC_DRAW);
//
	glGenBuffers(1, &_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*_numberOfVertex, normal, GL_STATIC_DRAW);
//
	glGenBuffers(1, &_indiceVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiceVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*_numberOfIndice, indice, GL_STATIC_DRAW);
//
	glGenBuffers(1, &_tangentVBO);
	glBindBuffer(GL_ARRAY_BUFFER,_tangentVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*_numberOfVertex, tangent,GL_STATIC_DRAW);
//free memory
	delete    vertex;
	delete    texCoord;
	delete   normal;
	delete   indice;
	delete   tangent;
	vertex = NULL;
	texCoord = NULL;
	normal = NULL;
	indice = NULL;
	tangent = NULL;
}
//天空盒实现
Skybox::Skybox()
{

}
Skybox::~Skybox()
{

}
void     Skybox::initWithScale(float scale)
{
//注意三角形的方向,从立方体的内部观察是正面的,
	float    vVertex[] = {
//前
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
//后
		-1.0f, -1.0f, 1.0f,  -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
//左
		-1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
//右
       1.0f, -1.0f, -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
       1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
//上
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, -1.0f,
//下
		-1.0f, -1.0f, -1.0f,  -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
	};
//纹理坐标,纹理坐标与顶点坐标相同,注意发现的方向是从立方体的内部观察的
	float     vNormal[] = {
//前
		0.0f,0.0f,1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
//后
       0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,   0.0f, 0.0f, -1.0f,
	   0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,   0.0f, 0.0f, -1.0f,
//左
      1.0f,0.0f,0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
	  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//右
      -1.0f,0.0f,0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	  -1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
//上
      0.0f,-1.0f,0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
	  0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
//下
     0.0f,1.0f,0.0f, 0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 };
//纹理坐标,与顶点坐标相同
	float   vTexCoord[] = {
//前
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
//后
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
//左
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
//右
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
//上
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f,1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
//下
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
	};
//切线,一把来说,不会使用这个数据
	float    tvTangent[] = {
//前
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//后
        -1.0f,0.0f,0.0f,  -1.0f,0.0f,0.0f,   -1.0f,0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//左
       0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
       0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
//右
       0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
       0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//上
      1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//下
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	_numberOfVertex = sizeof(vVertex) / sizeof(float);
	for (int i = 0; i < _numberOfVertex; ++i)
		vVertex[i] *= scale;
//生成OpenGL缓存对象
	glGenBuffers(1,&_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vVertex), vVertex, GL_STATIC_DRAW);
//
	glGenBuffers(1,&_texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER,_texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vTexCoord),vTexCoord,GL_STATIC_DRAW);
//
	glGenBuffers(1, &_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vNormal), vNormal, GL_STATIC_DRAW);

	glGenBuffers(1,&_tangentVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _tangentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoord), vTexCoord, GL_STATIC_DRAW);
	_numberOfVertex /= 3;
 }
 Skybox     *Skybox::createWithScale(float scale)
 {
	 Skybox	*_Skybox = new   Skybox();
	 _Skybox->initWithScale(scale);
	 return  _Skybox;
 }
 void    Skybox::drawShape()
 {
	 glDrawArrays(GL_TRIANGLES, 0, _numberOfVertex);
 }

 void    Skybox::bindTexCoordObject(int loc)
 {
	 glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
	 glEnableVertexAttribArray(loc);
	 glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
 }

 void    Skybox::bindIndiceObject()
 {

 }
 ///////////////////////////箱子//////////////////////////
Chest::Chest()
{

}
Chest::~Chest()
{

}
//
void     Chest::initWithScale(float scale)
{
	//从立方体的外面观察,所有的三角形都是正方向的
	float cubeVerts[] =
	{
//前
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
//右
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
//后
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
//左
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
//上
		-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 
		1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
//下
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f,
	};

	float cubeNormals[] =
	{
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,

		-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	};
//切线
	float    tangent[] = {
//前
		1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,    1.0f,0.0f,0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//右
       0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	   0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
//后
-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//左
	   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//上
     1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//下
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	float cubeTex[] =
	{
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	_numberOfVertex = sizeof(cubeVerts)/(sizeof(float)*3);
	for (int i = 0; i < sizeof(cubeVerts) / sizeof(float); ++i)
		cubeVerts[i] *= scale;

	glGenBuffers(1, &_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

	glGenBuffers(1, &_texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTex), cubeTex, GL_STATIC_DRAW);
	
	glGenBuffers(1, &_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);

	glGenBuffers(1, &_tangentVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _tangentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangent), tangent, GL_STATIC_DRAW);
}
//
Chest     *Chest::createWithScale(float scale)
{
	Chest    *_glChest = new  Chest();
	_glChest->initWithScale(scale);
	return _glChest;
}
void    Chest::drawShape()
{
	glDrawArrays(GL_TRIANGLES, 0, _numberOfVertex);
}
void   Chest::bindIndiceObject()
{

}
////////////////////////网格//////////////////////////////////
Mesh::Mesh()
{
}
Mesh::~Mesh()
{
}

void          Mesh::initWithMesh(int    size,float scaleX, float scaleY, float texIntensity)
{
	int i, j;
	int numIndices = (size - 1) * (size - 1) * 2 * 3;
// Allocate memory for buffers
	int numVertices = size * size;
	_numberOfVertex = numVertices;
	_numberOfIndice = numIndices;
	float   stepSize = ((float)(size - 1)) / 2.0f;
	float   *_vertex = new float[3 * numVertices];

	for (i = 0; i < size; ++i) // row
	{
		const    float     locX = scaleY*(i / stepSize - 1.0f);
		for (j = 0; j < size; ++j) // column
		{
			int   _index = (i*size + j) * 3;
			_vertex[_index] = scaleX*(j / stepSize - 1.0f);
			_vertex[_index + 1] = locX;
			_vertex[_index + 2] = 0.0f;
		}
	}
// Generate the indices
	int   *_indice = new int[numIndices];
	for (i = 0; i < size - 1; ++i)
	{
		for (j = 0; j < size - 1; ++j)
		{
// two triangles per quad
			int    _index = 6 * (j + i * (size - 1));
			_indice[_index] = j + i* size;
			_indice[_index + 1] = j + i* size + 1;
			_indice[_index + 2] = j + (i + 1) * size + 1;

			_indice[_index + 3] = j + (i + 1) * size + 1;
			_indice[_index + 4] = j + (i + 1) * size;
			_indice[_index + 5] = j + i* size;
		}
	}
//Gen Buffers
	glGenBuffers(1, &_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float)*_numberOfVertex, _vertex,GL_STATIC_DRAW);
//
	glGenBuffers(1, &_indiceVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiceVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*_numberOfIndice, _indice, GL_STATIC_DRAW);
//tex coord
	float     *texCoord = _vertex;
	const   float      _factor = size - 1;
	for (int i = 0; i < _numberOfVertex; ++i)
	{
		int     _row = i / size;
		int     _column = i%size;
		texCoord[i << 1] = texIntensity*_column / _factor;
		texCoord[(i << 1) + 1] = texIntensity*_row / _factor;
	}
	glGenBuffers(1, &_texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float)*_numberOfVertex, texCoord, GL_STATIC_DRAW);

	delete  _vertex;
	delete  _indice;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//
void       Mesh::bindNormalObject(int _loc)
{
	glDisableVertexAttribArray(_loc);
	glVertexAttrib3f(_loc, 0.0f, 0.0f, 1.0f);
}

Mesh         *Mesh::createWithIntensity(int grid_size, float scaleX, float scaleY, float texIntensity)
{
	Mesh		*_mesh = new   Mesh();
	_mesh->initWithMesh(grid_size, scaleX, scaleY, texIntensity);
	return  _mesh;
}