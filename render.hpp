#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "glad/glad.h"
#include "utils.hpp"
#include "math.hpp"
#include "stb_image.h" 


#define FVF_XYZ              1<<0
#define FVF_NORMAL           1<<1
#define FVF_COLOR            1<<2
#define FVF_FCOLOR           1<<3
#define FVF_BINORMAL         1<<4
#define FVF_TANGENT          1<<5

#define FVF_TEX1             1<<6
#define FVF_TEX2             1<<7
#define FVF_TEX3             1<<8
#define FVF_TEX4             1<<9
#define FVF_TEX5             1<<10


#define FMT_INDEX16              1<<0
#define FMT_INDEX32              1<<1


	enum E_PRIMITIVE_TYPE
	{
		//! All vertices are non-connected points.
		EPT_POINTS=0,

		//! All vertices form a single connected line.
		EPT_LINE_STRIP,

		//! Just as LINE_STRIP, but the last and the first vertex is also connected.
		EPT_LINE_LOOP,

		//! Every two vertices are connected creating n/2 lines.
		EPT_LINES,

		//! After the first two vertices each vertex defines a new triangle.
		//! Always the two last and the new one form a new triangle.
		EPT_TRIANGLE_STRIP,

		//! After the first two vertices each vertex defines a new triangle.
		//! All around the common first vertex.
		EPT_TRIANGLE_FAN,

		//! Explicitly set all vertices for each triangle.
		EPT_TRIANGLES
	};

enum VertexElementType
{
    VET_FLOAT1,
    VET_FLOAT2,
    VET_FLOAT3,
    VET_FLOAT4,
    VET_SHORT1,
    VET_SHORT2,
    VET_SHORT3,
    VET_SHORT4,
    VET_BYTE3,
    VET_BYTE4,
};

enum VertexElement
{
    VF_POSITION      = 1<<0, // vertex position 3 floats x,y,z
    VF_COLOR         = 1<<1, // vertex color 4 bytes r,g,b,a
    VF_FLOATCOLOR    = 1<<2, // vertex color 4 floats r,g,b,a
    VF_NORMAL        = 1<<3, // normal 3 floats x,y,z
    VF_TANGENT       = 1<<4, // normal 3 floats x,y,z
    VF_BINORMAL      = 1<<5, // normal 3 floats x,y,z
    VF_TEXCOORD1     = 1<<6, // texcoord 1 2 floats u,v
    VF_TEXCOORD2   = 1<<7, // texcoord 1 2 floats u,v
    VF_TEXCOORD3   = 1<<8, // texcoord 1 2 floats u,v
    VF_TEXCOORD4   = 1<<9, // texcoord 1 2 floats u,v
    VF_TEXCOORD5   = 1<<10, // texcoord 1 2 floats u,v

};


struct 		VertexDeclaration
{
 VertexElement   element;
 VertexElementType type;
};


inline int FVFDecodeLength(unsigned int fvf)
{
	int nSize = 0;
	if (fvf & FVF_XYZ)		nSize+=3 * sizeof(FLOAT);
	if (fvf & FVF_NORMAL)	nSize+=3 * sizeof(FLOAT);

	if (fvf & FVF_COLOR)	nSize+=4 * sizeof(BYTE);

    if (fvf & FVF_FCOLOR)	nSize+=4 * sizeof(FLOAT);

	if (fvf & FVF_BINORMAL)	nSize+=3 * sizeof(FLOAT);
    if (fvf & FVF_TANGENT)	nSize+=3 * sizeof(FLOAT);


	if (fvf & FVF_TEX1)		nSize+=2 * sizeof(FLOAT);
	if (fvf & FVF_TEX2)		nSize+=2 * sizeof(FLOAT);
	if (fvf & FVF_TEX3)		nSize+=2 * sizeof(FLOAT);
	if (fvf & FVF_TEX4)		nSize+=2 * sizeof(FLOAT);
	if (fvf & FVF_TEX5)		nSize+=2 * sizeof(FLOAT);

	return nSize;
}



inline UINT getTypeFormat(VertexElementType etype)
	{
		switch(etype)
		{
		case VET_FLOAT1:
			return GL_FLOAT;
		case VET_FLOAT2:
			return GL_FLOAT;
		case VET_FLOAT3:
			return GL_FLOAT;
		case VET_FLOAT4:
			return GL_FLOAT;
		case VET_SHORT1:
			return GL_SHORT;
		case VET_SHORT2:
			return GL_SHORT;
		case VET_SHORT3:
			return GL_SHORT;
		case VET_SHORT4:
			return GL_SHORT;
        case VET_BYTE3:
            return GL_UNSIGNED_BYTE;
        case VET_BYTE4:
            return GL_UNSIGNED_BYTE;
		}
		return 0;
	}
	inline UINT getTypeSize(VertexElementType etype)
	{
		switch(etype)
		{
		case VET_FLOAT1:
			return sizeof(float);
		case VET_FLOAT2:
			return sizeof(float)*2;
		case VET_FLOAT3:
			return sizeof(float)*3;
		case VET_FLOAT4:
			return sizeof(float)*4;
		case VET_SHORT1:
			return sizeof(short);
		case VET_SHORT2:
			return sizeof(short)*2;
		case VET_SHORT3:
			return sizeof(short)*3;
		case VET_SHORT4:
			return sizeof(short)*4;
        case VET_BYTE3:
            return sizeof(unsigned char)*3+1;
        case VET_BYTE4:
            return sizeof(unsigned char)*4+1;
		}
		return 0;
	}


	inline unsigned short getTypeCount(VertexElementType etype)
	{
		switch (etype)
		{
		case VET_FLOAT1:
			return 1;
		case VET_FLOAT2:
			return 2;
		case VET_FLOAT3:
			return 3;
		case VET_FLOAT4:
			return 4;
		case VET_SHORT1:
			return 1;
		case VET_SHORT2:
			return 2;
		case VET_SHORT3:
			return 3;
		case VET_SHORT4:
			return 4;
        case VET_BYTE3:
            return 3;
        case VET_BYTE4:
            return 4;
		}
      return 0;
	}



struct Color
{
    float r,g,b,a;

	Color() : r( 1.0f ), g( 1.0f ), b( 1.0f ), a( 1.0f )
	{
	}
	
	Color( const float r, const float g, const float b, const float a) :
		r( r ), g( g ), b( b ), a( a )
	{
	}

	Color(const Color &v ) : r( v.r ), g( v.g ), b( v.b ), a( v.a )
	{
	}
    bool operator==( const Color &v ) const
	{
		return (r == v.r  && 
				g == v.g  && 
                b == v.b  && 
                a == v.a  );
	}
};

class Texture2D
{
    public:
    Texture2D()
    {

    }
    ~Texture2D()
    {
         if (id != 0) 
         {
           glDeleteTextures(1, &id);
           Log(0, "TEXTURE2D: [ID %i] Unload Opengl Texture2D", id);
        }
    }
    bool Load(const std::string &file_name)
    {
        unsigned int bytesRead;
       unsigned char *fileData =  LoadFileData(file_name.c_str(),&bytesRead);
       if(!fileData)
            return false;

      stbi_set_flip_vertically_on_load(1);
      unsigned char *data =  stbi_load_from_memory(fileData,bytesRead,&width, &height, &components, STBI_default);
      
      //unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &components, STBI_default);
      if (data == nullptr) 
      {
         Log(2,"Failed to load image: %s",file_name.c_str());
         return false;
      }

      GLenum format;
      switch (components) 
      {
         case STBI_grey:
            format = GL_RED;
            break;
         case STBI_grey_alpha:
            format = GL_RG;
            break;
         case STBI_rgb:
            format = GL_RGB;
            break;
         case STBI_rgb_alpha:
            format = GL_RGBA;
            break;
         default:
            stbi_image_free(data);
            return false;
      }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        Log(0, "TEXTURE2D: [ID %i] Create Opengl Texture2D", id);
        free(fileData);
        stbi_image_free(data);
    }

    void Bind(UINT unit) 
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    private:
        UINT id;   
        int width;          
        int height;         
        int components;         
};

struct Vertex
{
	Vec3  pos;
    Vec3  normal;
    Color color;
	Vec2  coord;
	Vertex() {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, Color c, float tu, float tv)
		: pos(x,y,z), normal(nx,ny,nz), color(c), coord(tu,tv) {}

	Vertex(const Vec3& pos, const Vec3& normal,  Color color, const Vec2& tcoords)
		: pos(pos), normal(normal), color(color), coord(tcoords) {}
};

class Shader
{
    public:
    Shader()
    {
        m_program = 0;
    }
    ~Shader()
    {
        glDeleteProgram(m_program);
        Log(0, "SHADER: [ID %i] Unloaded shader program", m_program);
    }
    
    bool operator ==(const Shader&      other) const { return m_program == other.m_program; }
    bool operator !=(const Shader&      other) const { return m_program != other.m_program; }
  
    bool create(const char* vShaderCode, const char* fShaderCode)
    {
        // 2. compile shaders
        unsigned int vertex, fragment;
       
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
       
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
       
        // shader Program
        m_program = glCreateProgram();
        glAttachShader(m_program, vertex);
        glAttachShader(m_program, fragment);
        glLinkProgram(m_program);
        checkCompileErrors(m_program, "PROGRAM");

        if (m_program>0)
            Log(0, "SHADER: [ID %i] Create shader program.", m_program);
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        Bind();
        return true;
    }
    
    void Bind() 
    { 
        glUseProgram(m_program); 
    }
    void unBind() 
    { 
        glUseProgram(0); 
    }
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(m_program, name.c_str()), value); 
    }
    void setFloat4(const std::string &name, float x,float y, float z,float w) const
    { 
        glUniform4f(glGetUniformLocation(m_program, name.c_str()), x,y,z,w); 
    }
    void setFloat3(const std::string &name, float x,float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(m_program, name.c_str()), x,y,z); 
    }
    void setVector3(const std::string &name, const Vec3 &v) const
    { 
        glUniform3f(glGetUniformLocation(m_program, name.c_str()), v.x,v.y,v.z); 
    }
    void setFloat2(const std::string &name, float x,float y) const
    { 
        glUniform2f(glGetUniformLocation(m_program, name.c_str()), x,y); 
    }
    
    void setMatrix(const std::string &name, const GLfloat *value, GLboolean transpose = GL_FALSE) const
    { 
        glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1 , transpose,value); 
    }
    void setMatrix4(const std::string &name, Mat4 mat, GLboolean transpose = GL_FALSE) const
    { 
       setMatrix(name, mat.x, transpose); 
    }
    
    bool findUniform(const std::string name)const
    {
        std::map<std::string, int>::const_iterator it = m_uniforms.begin();
        while(it != m_uniforms.end())
        {
            if (strcmp(it->first.c_str(),name.c_str())==0)
            {
            return true;
            }
            it++;
        }
        return false;
    }
    int  getUniform(const std::string name)const
    {
        std::map<std::string, int>::const_iterator it = m_uniforms.begin();
        while(it != m_uniforms.end())
        {
            if (strcmp(it->first.c_str(),name.c_str())==0)
            {
            return it->second;
            }

            it++;
        }
         return -1;
    }
    int getUniformLocation(const std::string &uniformName) const
    {
        int location = -1;
        location =getUniform(uniformName);//uniforms[uniformName];
        if (location == -1)
            Log(2, "SHADER: [ID %i] Failed to find shader uniform: %s", m_program, uniformName.c_str());

    //  else SDL_Log( "SHADER: [ID %i] IShader uniform (%s) set at location: %i", id, uniformName, location);
        return location;
    }

    int getAttribLocation( const std::string &attribName) const
    {
        int location = -1;
        location = glGetAttribLocation(m_program, attribName.c_str());
        if (location == -1)
            Log(2, "SHADER: [ID %i] Failed to find shader attribute: %s", m_program, attribName.c_str());
        //App::getInstance()->ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SHADER Error",TextFormat("SHADER Failed to find shader attribute: %s)",attribName));
    //  else SDL_Log( "SHADER: [ID %i] IShader attribute (%s) set at location: %i", id, attribName, location);
        return location;
    }
    bool addUniform(const std::string name)
    {
        int location = -1;
        location = glGetUniformLocation(m_program, name.c_str());
        if (location == -1)
        {
            Log(2, "SHADER: [ID %i] Failed to find shader uniform: %s", m_program, name.c_str());
            //ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SHADER Error",TextFormat("SHADER Failed to find shader uniform: %s)",name));
            return false;
        }

       // Log(0, "SHADER: [ID %i] shader uniform (%s) set at location: %i", m_program, name.c_str(), location);
        m_uniforms.insert(std::make_pair(name, location));

    return true;
    }

    bool addAttribute(const std::string name)
    {
    int location = -1;
        location = glGetAttribLocation(m_program, name.c_str());

        if (location == -1)
        {
            Log(2, "SHADER: [ID %i] Failed to find shader attribute: %s", m_program, name.c_str());
        //ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SHADER Error",TextFormat("SHADER Failed to find shader attribute: %s)",name));
        return false;
        }

      //  Log(2, "SHADER: [ID %i] shader attribute (%s) set at location: %i", m_program, name.c_str(), location);
        m_attributes.insert(std::make_pair(name, location));
      return true;
    }
    void printData()
    {

    Log(0,"[SHADER]  Id(%d) Num Attributes(%d)  Num Uniforms (%d)",m_program, m_numAttributes,m_numUniforms);
    {
            std::map<std::string, int>::iterator it = m_attributes.begin();
            while(it != m_attributes.end())
            {
                Log(0,"[SHADER]  Id(%d)  attribute  index(%d) name(%s)",m_program,it->second, it->first.c_str());
                it++;
            }
    }
    {
        std::map<std::string, int>::iterator it = m_uniforms.begin();
            while(it != m_uniforms.end())
            {
                Log(0,"[SHADER]  Id(%d) uniform index(%d) name(%s)",m_program,it->second, it->first.c_str());

                it++;
            }
        }
    }
    void LoadDefaults()
    {
        GLint numActiveAttribs = 0;
        GLint maxAttribNameLength = 0;
        glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
        glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
        m_numAttributes=numActiveAttribs;
        for(GLint attrib = 0; attrib < numActiveAttribs; ++attrib)
        {
            int namelen = -1;
            int num = -1;
            char name[256]; // Assume no variable names longer than 256
            GLenum type = GL_ZERO;
            glGetActiveAttrib(m_program, attrib,  sizeof(name) - 1, &namelen, &num, &type, name);
            name[namelen] = 0;
            addAttribute(std::string((char*)&name[0]));
            glBindAttribLocation(m_program, attrib, (char*)&name[0]);
            Log(0,"SHADER: [ID %i] Active attribute (%s) set at location: %i", m_program, name,attrib);
        }
        // Get available shader uniforms
        int uniformCount = -1;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);
        m_numUniforms=uniformCount;

        for (int i = 0; i < uniformCount; i++)
        {
            int namelen = -1;
            int num = -1;
            char name[256]; // Assume no variable names longer than 256
            GLenum type = GL_ZERO;
            glGetActiveUniform(m_program, i, sizeof(name) - 1, &namelen, &num, &type, name);
            name[namelen] = 0;
            addUniform(std::string((char*)&name[0]));
            Log(0,"SHADER: [ID %i] Active uniform (%s) set at location: %i", m_program, name, glGetUniformLocation(m_program, name));
        }

    }
    private:
        UINT m_program;
        std::map<std::string, int> m_uniforms;
        std::map<std::string, int> m_attributes;
        int m_numAttributes;
        int m_numUniforms;

    private:
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                Log(2,"SHADER: Error Compile %s Msg: %s", type.c_str(), infoLog );
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                Log(2,"SHADER: Error Link %s Msg: %s", type.c_str(), infoLog );
            }
        }
    }
};


class VertexBuffer

{
    public:
        VertexBuffer()
        {
           
            glGenVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);
            Log(0, "VAO: [ID %i] Create vertex array", m_vao);
        }

        ~VertexBuffer()
        {
         glBindVertexArray(0);
         glDeleteVertexArrays(1, &m_vao);
         Log(0, "VAO: [ID %i] Unloaded vertex array", m_vao);
        for (auto id: m_vbs)
        {
                glDeleteBuffers(1, &id);
            Log(0, "VAO: [ID %i] Unloaded vertex data ", id);
        }
        }

        void Bind()
        {
            glBindVertexArray(m_vao);
        }
        void Bind(int index)
        {
            glBindVertexArray(m_vao);
        }
        void unBind()
        {
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        
        void DrawArrays(int mode,int offset, int count)
        {
            glDrawArrays(mode, offset, count);
        }

        void DrawElements(int mode,int offset, int count, void *buffer)
        {
            glDrawElements(mode, count, GL_UNSIGNED_INT, (unsigned int*)buffer + offset);
        }

        void UpdateBuffer(int bufferId, void *data, int dataSize, int offset)
        {
            glBindBuffer(GL_ARRAY_BUFFER, bufferId);
            glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
        }
        UINT LoadBuffer(void *buffer, int size, bool dynamic = false)
        {
            UINT id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            glBufferData(GL_ARRAY_BUFFER, size, buffer, dynamic? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            Log(0, "VBO: [ID %i] Load vertex data ", id);
            m_vbs.push_back(id);
            return id;
        }
        UINT LoadBufferElement(void *buffer, int size, bool dynamic= false)
        {
            UINT id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, dynamic? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            Log(0, "VBO: [ID %i] Load vertex index data ", id);
            m_vbs.push_back(id);
            return id;
        }
        void SetVertexAttribute(UINT index, int compSize, int type, bool normalized, int stride, void *pointer)
        {
            glVertexAttribPointer(index, compSize, type, normalized, stride, pointer);

        }
        void EnableVertexAttribute(UINT index)
        {
            glEnableVertexAttribArray(index);

        }
        
    private:
            UINT m_vao;
            std::vector<UINT> m_vbs;
             
};


class Surface
{
private:
 std::vector<Vertex> vertices;
 std::vector<int>    indices;
 VertexBuffer        *buffer;
UINT m_iVertexCount;
UINT m_iCountVertexDeclaration;
UINT m_iVertexOffSetSize;// size of Vertex
DWORD  m_FVF;
std::vector<VertexDeclaration> m_VertexDeclaration;

public:
    Surface(long fvf=0)
    {
        Log(0,"Create Surface");
        buffer = new VertexBuffer();
        m_FVF = fvf;
        int sstride =  FVFDecodeLength(m_FVF);



        if (m_FVF & FVF_XYZ)
        {
            VertexDeclaration vd;
            vd.element=VF_POSITION;
            vd.type   =VET_FLOAT3;
            
            m_VertexDeclaration.emplace_back(vd);
        }

        if (m_FVF & FVF_TEX1)
        {
            VertexDeclaration vd;
            vd.element=VF_TEXCOORD1;
            vd.type   =VET_FLOAT2;
            m_VertexDeclaration.emplace_back(vd);
        }

        if (m_FVF & FVF_FCOLOR)
        {
            VertexDeclaration vd;
            vd.element=VF_FLOATCOLOR;
            vd.type   =VET_FLOAT4;
            m_VertexDeclaration.emplace_back(vd);
        }

        if (m_FVF & FVF_COLOR)
        {
            VertexDeclaration vd;
            vd.element=VF_COLOR;
            vd.type=VET_BYTE4;
            m_VertexDeclaration.emplace_back(vd);
        }


        if (m_FVF & FVF_NORMAL)
        {
            VertexDeclaration vd;
            vd.element=VF_NORMAL;
            vd.type   =VET_FLOAT3;
            m_VertexDeclaration.emplace_back(vd);
        }




       
        // if (m_FVF & FVF_TEX2)
        // {
        //     VertexDeclaration vd={VF_TEXCOORD2,VET_FLOAT2};
        //     m_VertexDeclaration.emplace_back(vd);
        // }
        // if (m_FVF & FVF_TEX3)
        // {
        //     VertexDeclaration vd={VF_TEXCOORD3,VET_FLOAT2};
        //     m_VertexDeclaration.emplace_back(vd);
        // }

        // if (m_FVF & FVF_TEX4)
        // {
        //     VertexDeclaration vd={VF_TEXCOORD4,VET_FLOAT2};
        //     m_VertexDeclaration.emplace_back(vd);
        // }

        // if (m_FVF & FVF_TEX5)
        // {
        //     VertexDeclaration vd={VF_TEXCOORD5,VET_FLOAT2};
        //     m_VertexDeclaration.emplace_back(vd);
        // }


        // if (m_FVF & FVF_BINORMAL)
        // {
        //     VertexDeclaration vd={VF_BINORMAL,VET_FLOAT3};
        //     m_VertexDeclaration.emplace_back(vd);
        // }

        // if (m_FVF & FVF_TANGENT)
        // {
        //     VertexDeclaration vd={VF_TANGENT,VET_FLOAT3};
        //     m_VertexDeclaration.emplace_back(vd);
        // }



        m_iCountVertexDeclaration = m_VertexDeclaration.size();
        m_iVertexOffSetSize=0;
        for (int i=0; i<m_iCountVertexDeclaration;i++)
        {
           m_iVertexOffSetSize += getTypeSize(m_VertexDeclaration[i].type) ;
           SDL_Log("layer %d %d  %d %d",i,getTypeSize(m_VertexDeclaration[i].type) ,m_iVertexOffSetSize,sizeof(Vertex));
        }

    }
    ~Surface()
    {
        delete buffer;
        Log(0,"Free Surface");
    }

    void* VertexData()
	{
		return (void*)vertices.data();
	}
	const void* ConstVertexData() const
	{
		return (const void*)vertices.data();
	}
    void* IndicesData()
    {
        return (void*)indices.data();
    }
    const void* ConstIndicesData() const
    {
        return (const void*)indices.data();
    }
    
    
    int AddVertex(float x, float y, float z, float nx, float ny, float nz, Color c, float tu, float tv)
    {
        vertices.push_back(Vertex(x,y,z,nx,ny,nz,c,tu,tv));
        return (int)vertices.size() -1;
    }
    int AddVertex(float x, float y, float z, float nx, float ny, float nz, Color c)
    {
        vertices.push_back(Vertex(x,y,z,nx,ny,nz,c,0.0,0.0));
        return (int)vertices.size() -1;
    }
    int AddVertex(float x, float y, float z, float nx, float ny, float nz)
    {
        Color c;
        vertices.push_back(Vertex(x,y,z,nx,ny,nz,c,0.0,0.0));
        return (int)vertices.size() -1;
    }
     int AddVertex(const Vec3 &v)
    {
        Color c;
        vertices.push_back(Vertex(v.x,v.y,v.z,0,0,0,c,0.0,0.0));
        return (int)vertices.size() -1;
    }
     int AddVertex(const Vec3 &v,const Vec3 &n)
    {
        Color c;
        vertices.push_back(Vertex(v.x,v.y,v.z,n.x,n.y,n.z,c,0.0,0.0));
        return (int)vertices.size() -1;
    }
        int AddVertex(const Vec3 &v,const Vec3 &n,const Vec2 &uv)
    {
        Color c;
        c.r=0.0f;
        c.g=0.0f;
        c.b=1.0f;
        c.a=1.0f;

        
        vertices.push_back(Vertex(v.x,v.y,v.z,n.x,n.y,n.z,c,uv.x,uv.y));
        return (int)vertices.size() -1;
    }
    
    int AddVertex(float x, float y, float z)
    {
        Color c;
        vertices.push_back(Vertex(x,y,z,0,0,0,c,0.0,0.0));
        return (int)vertices.size() -1;
    }
     int AddVertex(float x, float y, float z,Color c)
    {
        vertices.push_back(Vertex(x,y,z,0,0,0,c,0.0,0.0));
        return (int)vertices.size() -1;
    }
    
    int AddVertex(const Vertex &vertex)
    {
        vertices.push_back(vertex);
        return (int)vertices.size() -1;
    }
    
    int AddIndice(int i)
    {
        indices.push_back(i);
        return (int)indices.size()-1;
    }
    int AddTriangle(int a, int b, int c)
    {
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
        return (int)indices.size()-1;
    }
    void VertexNormal(int index, float x, float y ,float z)
    {
        if (index < 0 || index > CountVertices())
            return;
        vertices[index].normal.set(x,y,z);
    }
    void VertexTexCoords(int index , float x, float y)
   {
        if (index < 0 || index > CountVertices())
            return;
        vertices[index].coord.set(x,y);
    }

    int CountVertices() const
    {
        return (int)vertices.size();
    }

    int CountIndices() const
    {
        return (int)indices.size();
    }
    		
    int  GetPrimitiveCount(UINT  mode) const
    {
        const int indexCount = CountIndices();
         switch (mode)
         {
             case GL_POINTS:	       return indexCount;
             case GL_LINE_STRIP:       return indexCount-1;
             case GL_LINE_LOOP:         return indexCount;
             case GL_LINES:            return indexCount/2;
             case GL_TRIANGLE_STRIP  : return (indexCount-2);
             case GL_TRIANGLE_FAN:     return (indexCount-2);
             case GL_TRIANGLES:        return indexCount/3;
         }
        return 0;
    }
    
    void Build()
    {
        buffer->LoadBufferElement(IndicesData(),CountIndices() * sizeof(int), false);
        buffer->LoadBuffer(VertexData(),CountVertices()*sizeof(Vertex));



int offSet = 0;



for (int i=0; i<m_iCountVertexDeclaration;i++)
{

    int attribute = i;
    buffer->EnableVertexAttribute(attribute);

    void* pointer = reinterpret_cast<void*>(offSet);

     if( m_VertexDeclaration[i].element & VF_POSITION )
     {
      Log(0,"set position %d",attribute);
      buffer->SetVertexAttribute(0, getTypeCount(m_VertexDeclaration[i].type), getTypeFormat(m_VertexDeclaration[i].type), false, m_iVertexOffSetSize, pointer);
     }

    if( m_VertexDeclaration[i].element & VF_TEXCOORD1 )
    {
        Log(0,"set uv  %d",attribute);
        //  buffer->SetVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, m_iVertexOffSetSize, pointer);
       // buffer->SetVertexAttribute(1, 2, GL_FLOAT,  false,m_iVertexOffSetSize, pointer);
      buffer->SetVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex,coord)));
      //glVertexAttribPointer(index, compSize, type, normalized, stride, pointer);
    }

    if( m_VertexDeclaration[i].element & VF_FLOATCOLOR )
    {
        Log(0,"set color %d",attribute);
    buffer->SetVertexAttribute(2, getTypeCount(m_VertexDeclaration[i].type), getTypeFormat(m_VertexDeclaration[i].type), true, m_iVertexOffSetSize, pointer);
    }

    if( m_VertexDeclaration[i].element & VF_NORMAL )
    {
        Log(0,"set norma %d",attribute);
    buffer->SetVertexAttribute(3, getTypeCount(m_VertexDeclaration[i].type), getTypeFormat(m_VertexDeclaration[i].type), false,m_iVertexOffSetSize, pointer);
    }



    
    offSet += getTypeSize(m_VertexDeclaration[i].type);
}


    //    buffer->SetVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex,pos)));
    //    buffer->EnableVertexAttribute(0);
        
          //buffer->SetVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex,coord)));
        //  buffer->EnableVertexAttribute(1);

        // buffer->SetVertexAttribute(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex,color)));
        // buffer->EnableVertexAttribute(2);

        // buffer->SetVertexAttribute(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex,normal)));
        // buffer->EnableVertexAttribute(3);

        
        
    }
    void Update()
    {

    }
    void Render(UINT  mode = GL_TRIANGLES)
    {

          const int indexCount = CountIndices();
          int count = 0;

         switch (mode)
         {
             case GL_POINTS:	        
             {
                count = indexCount;
             }break;
             case GL_LINE_STRIP:       
             {
                count = indexCount + 1;
             }break;
             case GL_LINE_LOOP: 
             {
                  count = indexCount;
             }break;
             case GL_LINES: 
             {
                 count = indexCount * 2;
             }break;
             case GL_TRIANGLE_STRIP  :
             {
              count =   indexCount + 2;
             }break;
             case GL_TRIANGLE_FAN: 
             {
                  count =  indexCount+2;
             }break;
             case GL_TRIANGLES:        
             {
               count =   indexCount * 3;
             }break;
         }
 
        buffer->Bind();
        buffer->DrawElements(mode, 0, count, 0);
    }

    static Surface *CreateCube()
    {
     Surface *surface = new Surface(FVF_XYZ | FVF_TEX1 | FVF_FCOLOR | FVF_NORMAL);
    surface->AddVertex(-1.0,-1.0,-1.0);
    surface->AddVertex(-1.0, 1.0,-1.0);
    surface->AddVertex( 1.0, 1.0,-1.0);
    surface->AddVertex( 1.0,-1.0,-1.0);

    surface->AddVertex(-1.0,-1.0, 1.0);
    surface->AddVertex(-1.0, 1.0, 1.0);
    surface->AddVertex( 1.0, 1.0, 1.0);
    surface->AddVertex( 1.0,-1.0, 1.0);

    surface->AddVertex(-1.0,-1.0, 1.0);
    surface->AddVertex(-1.0, 1.0, 1.0);
    surface->AddVertex( 1.0, 1.0, 1.0);
    surface->AddVertex( 1.0,-1.0, 1.0);

    surface->AddVertex(-1.0,-1.0,-1.0);
    surface->AddVertex(-1.0, 1.0,-1.0);
    surface->AddVertex( 1.0, 1.0,-1.0);
    surface->AddVertex( 1.0,-1.0,-1.0);

    surface->AddVertex(-1.0,-1.0, 1.0);
    surface->AddVertex(-1.0, 1.0, 1.0);
    surface->AddVertex( 1.0, 1.0, 1.0);
    surface->AddVertex( 1.0,-1.0, 1.0);

    surface->AddVertex(-1.0,-1.0,-1.0);
    surface->AddVertex(-1.0, 1.0,-1.0);
    surface->AddVertex( 1.0, 1.0,-1.0);
    surface->AddVertex( 1.0,-1.0,-1.0);

    surface->VertexNormal(0,0.0,0.0,-1.0);
    surface->VertexNormal(1,0.0,0.0,-1.0);
    surface->VertexNormal(2,0.0,0.0,-1.0);
    surface->VertexNormal(3,0.0,0.0,-1.0);

    surface->VertexNormal(4,0.0,0.0,1.0);
    surface->VertexNormal(5,0.0,0.0,1.0);
    surface->VertexNormal(6,0.0,0.0,1.0);
    surface->VertexNormal(7,0.0,0.0,1.0);

    surface->VertexNormal(8,0.0,-1.0,0.0);
    surface->VertexNormal(9,0.0,1.0,0.0);
    surface->VertexNormal(10,0.0,1.0,0.0);
    surface->VertexNormal(11,0.0,-1.0,0.0);

    surface->VertexNormal(12,0.0,-1.0,0.0);
    surface->VertexNormal(13,0.0,1.0,0.0);
    surface->VertexNormal(14,0.0,1.0,0.0);
    surface->VertexNormal(15,0.0,-1.0,0.0);

    surface->VertexNormal(16,-1.0,0.0,0.0);
    surface->VertexNormal(17,-1.0,0.0,0.0);
    surface->VertexNormal(18,1.0,0.0,0.0);
    surface->VertexNormal(19,1.0,0.0,0.0);

    surface->VertexNormal(20,-1.0,0.0,0.0);
    surface->VertexNormal(21,-1.0,0.0,0.0);
    surface->VertexNormal(22,1.0,0.0,0.0);
    surface->VertexNormal(23,1.0,0.0,0.0);

    surface->VertexTexCoords(0,0.0,1.0);
    surface->VertexTexCoords(1,0.0,0.0);
    surface->VertexTexCoords(2,1.0,0.0);
    surface->VertexTexCoords(3,1.0,1.0);

    surface->VertexTexCoords(4,1.0,1.0);
    surface->VertexTexCoords(5,1.0,0.0);
    surface->VertexTexCoords(6,0.0,0.0);
    surface->VertexTexCoords(7,0.0,1.0);

    surface->VertexTexCoords(8,0.0,1.0);
    surface->VertexTexCoords(9,0.0,0.0);
    surface->VertexTexCoords(10,1.0,0.0);
    surface->VertexTexCoords(11,1.0,1.0);

    surface->VertexTexCoords(12,0.0,0.0);
    surface->VertexTexCoords(13,0.0,1.0);
    surface->VertexTexCoords(14,1.0,1.0);
    surface->VertexTexCoords(15,1.0,0.0);

    surface->VertexTexCoords(16,0.0,1.0);
    surface->VertexTexCoords(17,0.0,0.0);
    surface->VertexTexCoords(18,1.0,0.0);
    surface->VertexTexCoords(19,1.0,1.0);

    surface->VertexTexCoords(20,1.0,1.0);
    surface->VertexTexCoords(21,1.0,0.0);
    surface->VertexTexCoords(22,0.0,0.0);
    surface->VertexTexCoords(23,0.0,1.0);


    surface->AddTriangle(0,1,2); // front
    surface->AddTriangle(0,2,3);
    surface->AddTriangle(6,5,4); // back
    surface->AddTriangle(7,6,4);
    surface->AddTriangle(6+8,5+8,1+8); // top
    surface->AddTriangle(2+8,6+8,1+8);
    surface->AddTriangle(0+8,4+8,7+8); // bottom
    surface->AddTriangle(0+8,7+8,3+8);
    surface->AddTriangle(6+16,2+16,3+16); // right
    surface->AddTriangle(7+16,6+16,3+16);
    surface->AddTriangle(0+16,1+16,5+16); // left
    surface->AddTriangle(0+16,5+16,4+16);
    surface->Build();
     return surface;
    }

static Surface *CreatePlane(float w,float d)
{
    Surface *surf = new Surface(FVF_XYZ | FVF_TEX1 | FVF_FCOLOR | FVF_NORMAL);


	surf->AddVertex(-w,0.0,-d);
	surf->AddVertex(-w,0.0, d);
	surf->AddVertex( w,0.0, d);
	surf->AddVertex( w,0.0,-d);


	surf->VertexNormal(0,0.0,1.0,0.0);
	surf->VertexNormal(1,0.0,1.0,0.0);
	surf->VertexNormal(2,0.0,1.0,0.0);
	surf->VertexNormal(3,0.0,1.0,0.0);

	surf->VertexTexCoords(0,0.0,1.0);
	surf->VertexTexCoords(1,0.0,0.0);
	surf->VertexTexCoords(2,1.0,0.0);
	surf->VertexTexCoords(3,1.0,1.0);

	surf->AddTriangle(0,1,2); // front
	surf->AddTriangle(0,2,3);

	 surf->Build();
     return surf;
}

};

class Mesh
{

    private:

     struct Face
    {
        int a;
        int b;
        int c;
        Vec3 na;
        Vec3 nb;
        Vec3 nc;
        Vec2 a_uv;
        Vec2 b_uv;
        Vec2 c_uv;
    } ;

    std::vector<Surface*> surfaces;
    public:
        Mesh()
        {

        }
        ~Mesh()
        {
            for (int i = 0; i <(int) surfaces.size(); i++) 
            {
                delete surfaces[i];
            }
            surfaces.clear();
         }
        bool LoadObj(const std::string &file_name)
        {
            if (!FileExists(file_name.c_str()))
            {
                Log(2," File %s don't exists",file_name.c_str());
                return false;
            }
                std::vector<Vec3> vertices;   
                std::vector<Vec3> normals;   
                std::vector<Vec2> texcoords;      
                std::vector<Face>    faces;    

            Surface *surf = new Surface(FVF_XYZ | FVF_TEX1 | FVF_FCOLOR | FVF_NORMAL);
            FILE* file;
            file = fopen(file_name.c_str(), "r");
            if (!file)
                return false;
            char line[1024];
             while (fgets(line, 1024, file)) 
            {
                // Vertex information
                if (strncmp(line, "v ", 2) == 0) 
                {
                    Vec3 vertex ;
                    sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                    vertices.push_back(vertex);
                    
                }
                if (strncmp(line, "vn ", 3) == 0) 
                {
                    Vec3 vertex ;
                    sscanf(line, "vn %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                    normals.push_back(vertex);
                    
                }
                // Texture coordinate information
                if (strncmp(line, "vt ", 3) == 0) 
                {
                    Vec2 texcoord ;
                    sscanf(line, "vt %f %f", &texcoord.x, &texcoord.y);
                    texcoords.push_back(texcoord);
                 }
                // Face information
                if (strncmp(line, "f ", 2) == 0)
                {
                    int vertex_indices[3];
                    int texture_indices[3];
                    int normal_indices[3];
                    sscanf(
                        line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                        &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                        &vertex_indices[2], &texture_indices[2], &normal_indices[2]
                    );
             
                    Face face;
                    face.a = vertex_indices[0];
                    face.b = vertex_indices[1];
                    face.c = vertex_indices[2];
                    face.a_uv = texcoords[texture_indices[0] - 1];
                    face.b_uv = texcoords[texture_indices[1] - 1];
                    face.c_uv = texcoords[texture_indices[2] - 1];
                    face.na = normals[normal_indices[0] - 1];
                    face.nb = normals[normal_indices[1] - 1];
                    face.nc = normals[normal_indices[2] - 1];
                    faces.push_back(face);
                }
            }

            for (int i=0; i < (int)faces.size();i++)
            {
                
                Vec3 v0 = vertices[faces[i].a - 1];
                Vec3 v1 = vertices[faces[i].b - 1];
                Vec3 v2 = vertices[faces[i].c - 1];


                Vec2 uv0 = faces[i].a_uv;
                Vec2 uv1 = faces[i].b_uv;
                Vec2 uv2 = faces[i].c_uv;

                Vec3 n0 = faces[i].na;
                Vec3 n1 = faces[i].nb;
                Vec3 n2 = faces[i].nc;

                int f0 = surf->AddVertex(v0,n0,uv0);
                int f1 = surf->AddVertex(v1,n1,uv1);
                int f2 = surf->AddVertex(v2,n2,uv2);

                surf->AddTriangle(f0,f1,f2);
            }

            fclose(file);

            surf->Build();
            surfaces.push_back(surf);
            return true;
        }

        void Render()
        {
             for (int i = 0; i <(int) surfaces.size(); i++) 
            {
                Surface *surf = surfaces[i];
                surf->Render();
            }
        }
  
};