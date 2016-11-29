/*
  *@aim:�������,����ĺ�����֮һ
  &2016-3-7 16:39:59
  */
//Version 3.0:����Լ�����ɫ����֧��,ֻ����OpenGL
//Version 4.0:����Լ�����ɫ����֧��,������OpenGL
#ifndef    __PROGRAM_OBJECT_H__
#define   __PROGRAM_OBJECT_H__
#include<engine/GLObject.h>
#include<engine/GLState.h>
class      GLProgram:public    GLObject
{
private:
//�������
	unsigned           _object;
//������ɫ��
	unsigned           _vertex;
//Ƭ����ɫ��
	unsigned           _frame;
//������ɫ��
#ifdef __GEOMETRY_SHADER__
	unsigned          _geometry;
#endif
private:
	GLProgram(GLProgram &);
	GLProgram();
//ʹ���ļ���ʼ��
	bool          initWithFile(const   char    *vertex_file_name, const   char *frame_file_name);
//ʹ���ַ�����ʼ��
	bool          initWithString(const  char  *vertex_string, const  char *frame_string);
//֧�ּ�����ɫ��
#ifdef  __GEOMETRY_SHADER__
	bool         initWithFile(const char *, const char *, const char *);//����Ϊ����,����,ƬԪ
	bool         initWithString(const char *, const char *, const char *);
#endif
public:
       static           GLProgram       *createWithFile(const   char    *vertex_file_name,const   char *frame_file_name);
       static           GLProgram       *createWithString(const  char  *vertex_string,const  char *frame_string);
//������ɫ��
#ifdef  __GEOMETRY_SHADER__
	   static           GLProgram       *createWithFile(const   char    *vertex, const   char *geometry,const char *frame);
	   static           GLProgram       *createWithString(const   char    *vertex, const   char *geometry, const char *frame);
#endif
	   ~GLProgram();
 //��������,���ڱ任����,_attr_type:���������洢�ķ�ʽ
	   void          feedbackVaryingsWith(const  char *_varyings[], int   count, int   _attr_type);
 //ʹ�ó������
	   void          enableObject();
	   void          perform();
	   void          disable();
//��ֹ�������
	   void          disableObject();
 //��ȡ�������,�˺����ӿ�ֻ���𷵻�����,�������𱣻�
	   unsigned      getObject(){ return     _object; };
//��ȡͳһ����λ��
	   unsigned      getUniformLocation(const  char  *);
//��ȡ���Ա�����λ��
	   unsigned      getAttribLocation(const char *);
};
//ֻ��OpenGL�в���ʹ��
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
//������ά���Ϸַ�����Ŀ
	void                     dispatch(int  dispatch_x_size,int  dispatch_y_size,int  dispatch_z_size);
//������ɫ��
	void                     enableObject();
//��ȡ��ɫ������
	unsigned             getObject();
//��ȡͳһ������λ��
	int                       getUniformLocation(const char *name);
};
#endif
//������ɫ��
static    unsigned         __compile_shader(unsigned    _type, const   char    *_shader_source);
//���ļ��ж�ȡ�ַ���
#endif