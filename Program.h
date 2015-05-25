//
// sueda
// October, 2014
//

#ifndef __Program__
#define __Program__
#include "includes.h"
#include "ParticleTexture.h"

//class Texture;

class Program
{
public:
	Program();
	virtual ~Program();
	
	void setShaderNames(const std::string &v, const std::string &f);
	virtual bool init();
	virtual void bind();
	virtual void unbind();

	GLint addAttribute(const std::string &name);
	GLint addUniform(const std::string &name);
	void addTexture(ParticleTexture *texture);
	GLint getAttribute(const std::string &name) const;
	GLint getUniform(const std::string &name) const;
	ParticleTexture *getTexture(const std::string &name) const;
	GLint getPID() { return pid; };
	
protected:
	std::string vShaderName;
	std::string fShaderName;
	
private:
	GLuint pid;
	std::map<std::string,GLint> attributes;
	std::map<std::string,GLint> uniforms;
	std::map<std::string,ParticleTexture*> textures;
	
	bool hasTex;
};

#endif
