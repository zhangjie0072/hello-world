/*
  *����
  *2016-6-14 20:31:10
 */
#ifndef  __GLTEXTURE_H__
#define __GLTEXTURE_H__
#include<engine/GLObject.h>
#include<engine/Geometry.h>
class  GLTexture:public GLObject
{
private:
	unsigned int      _textureId;
	Size       _contentSize;//ͼƬ�Ĵ�С
private:
	GLTexture(GLTexture &);
	GLTexture();
	void      initWithFile(const char *_file_name);
public:
	~GLTexture();
	static   GLTexture    *createWithFile(const char  *_file_name);
	void         setWrapParam(unsigned wrap_param,unsigned _param);
//��ȡ�����ĳߴ�
	Size&      getContentSize();
	float     getWidth();
	float     getHeight();
//����������
	unsigned int  name();
//����Mipmap
	void        genMipmap();
};
//��������ͼ
/*
  *
  */
class   GLCubeMap :public GLObject
{
	unsigned         _cubeMapId;
	Size                  _contentSize;
private:
	GLCubeMap(GLCubeMap &);
	GLCubeMap();
//�ļ����Ų�����Ϊ+X,-X,+Y,-Y,+Z,-Z
	void                initWithFiles(const  char   **file_list);
	void                initWithEmpty(Size    &contentSize);
public:
	~GLCubeMap();
	static     GLCubeMap	      *createWithFiles(const char **file_list);
	static     GLCubeMap      *createEmptyMap(Size    &contentSize);
	unsigned       name();
};
#endif