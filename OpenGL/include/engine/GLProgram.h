/*
  *@aim:程序对象,引擎的核心类之一
  &2016-3-7 16:39:59
  */
//Version 3.0:加入对几何着色器的支持,只限于OpenGL
//Version 4.0:加入对计算着色器的支持,仅限于OpenGL
#ifndef    __PROGRAM_OBJECT_H__
#define   __PROGRAM_OBJECT_H__
#include<engine/GLObject.h>
#include<engine/GLState.h>
class      GLProgram:public    GLObject
{
private:
//程序对象
	unsigned           _object;
//顶点着色器
	unsigned           _vertex;
//片段着色器
	unsigned           _frame;
//几何着色器
#ifdef __GEOMETRY_SHADER__
	unsigned          _geometry;
#endif
private:
	GLProgram(GLProgram &);
	GLProgram();
//使用文件初始化
	bool          initWithFile(const   char    *vertex_file_name, const   char *frame_file_name);
//使用字符串初始化
	bool          initWithString(const  char  *vertex_string, const  char *frame_string);
//支持几何着色器
#ifdef  __GEOMETRY_SHADER__
	bool         initWithFile(const char *, const char *, const char *);//依次为顶点,几何,片元
	bool         initWithString(const char *, const char *, const char *);
#endif
public:
       static           GLProgram       *createWithFile(const   char    *vertex_file_name,const   char *frame_file_name);
       static           GLProgram       *createWithString(const  char  *vertex_string,const  char *frame_string);
//几何着色器
#ifdef  __GEOMETRY_SHADER__
	   static           GLProgram       *createWithFile(const   char    *vertex, const   char *geometry,const char *frame);
	   static           GLProgram       *createWithString(const   char    *vertex, const   char *geometry, const char *frame);
#endif
	   ~GLProgram();
 //二次链接,用于变换反馈,_attr_type:反馈变量存储的方式
	   void          feedbackVaryingsWith(const  char *_varyings[], int   count, int   _attr_type);
 //使用程序对象
	   void          enableObject();
	   void          perform();
	   void          disable();
//禁止程序对象
	   void          disableObject();
 //获取程序对象,此函数接口只负责返回数据,但不负责保护
	   unsigned      getObject(){ return     _object; };
//获取统一变量位置
	   unsigned      getUniformLocation(const  char  *);
//获取属性变量的位置
	   unsigned      getAttribLocation(const char *);
};
//只有OpenGL中才能使用
#ifdef  __OPENGL_VERSION__
class    GLCompute :public  GLObject
{
private:
	unsigned    _object;
	unsigned    _computeShader;
private:
	GLCompute();
	GLCompute(GLCompute &);
	void            initWithFile(const  char *file_name);
	void            initWithString(const char *shader_string);
public:
	~GLCompute();
	GLCompute       *createWithString(const  char  *shader_string);
	GLCompute       *createWithFile(const char *file_name);
//在三个维度上分发的数目
	void                     dispatch(int  dispatch_x_size,int  dispatch_y_size,int  dispatch_z_size);
//启用着色器
	void                     enableObject();
//获取着色器对象
	unsigned             getObject();
//获取统一变量的位置
	int                       getUniformLocation(const char *name);
};
#endif
//编译着色器
static    unsigned         __compile_shader(unsigned    _type, const   char    *_shader_source);
//从文件中读取字符串
#endif