#ifndef FREE_NEHE_H
#define FREE_NEHE_H

//FreeType Headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H 

/*#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>*/

//OpenGL Headers 
#include <windows.h>		//(the GL headers need it)
#include <GL/gl.h>
#include <GL/glu.h>

//Some STL headers
#include <vector>
#include <string>
#include "MPQ.h"
//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
namespace freetype
{

//This holds all of the information related to any
//freetype font that we want to create.  
struct font_data
{
	float h;			///< Holds the height of the font.
	GLuint* textures;	///< Holds the texture id's 
	GLuint list_base;	///< Holds the first display list id
	int	charWidths[128];

	//The init function will create a font of
	//of the height h from the file fname.
	void Init(const char* fname, unsigned int h);
	void InitMPQ(const char* fname, unsigned int h);

	//Free all the resources assosiated with the font.
	void clean();
};

//The flagship function of the library - this thing will print
//out text at window coordinates x,y, using the font ft_font.
//The current modelview matrix will also be applied to the text. 
void print(const font_data &ft_font, float x, float y, const char* fmt, ...) ;
void shprint(const font_data &ft_font, float x, float y, const char* fmt, ...);
int width(const font_data &ft_font, const char* fmt, ...);
}

#endif