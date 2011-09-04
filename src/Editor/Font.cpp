#define _CRT_SECURE_NO_WARNINGS
#include "Font.h"
#include "Video.h"
#include <iostream>
#include <fstream>
#include "OpenGL.h"

using namespace std;

Font::Font(unsigned int tex, int tw, int th, int size, const char* infofile): tex(tex), size(size), tw(tw), th(th)
{
	ifstream in(infofile);
	if (!in.is_open())
	{
		gLog("Error opening font %s\n", infofile);
		exit(1);
	}

	// fontinfo file generated by fontpack [fontpack.sourceforge.net]

	memset(chars,0,256*sizeof(charinfo));

	while (!in.eof())
	{
		int line[7];
		for (int i=0; i<7; i++) in >> line[i];

		if (line[1] != size) continue;

		charinfo ci;
		ci.baseline = line[2];
		ci.x = line[3];
		ci.y = line[4];
		ci.w = line[5];
		ci.h = line[6];

		ci.tx1 = ci.x / (float)tw;
		ci.tx2 = (ci.x + ci.w) / (float)tw;
		ci.ty1 = ci.y / (float)th;
		ci.ty2 = (ci.y + ci.h) / (float)th;

		chars[line[0]] = ci;
	}
	charinfo space;
	space.baseline = 0;
	space.h = size;
	space.w = size / 3;
	space.x = space.y = 0;
	space.tx1 = space.tx2 = space.ty1 = space.ty2 = 0;
	chars[32] = space;
	Scale=1.0;
}

Font::~Font()
{
}

void Font::drawchar(float x, float y, const char ch)
{
	if (ch==32) return;

	charinfo *c = &chars[ch];

    glBegin(GL_QUADS);

	glTexCoord2f(c->tx1,c->ty1);
	glVertex2f((float)x, (float)y);

	glTexCoord2f(c->tx2,c->ty1);
	glVertex2f((float)(x+Scale*c->w), (float)y);

	glTexCoord2f(c->tx2,c->ty2);
	glVertex2f((float)(x+Scale*c->w), (float)(y+Scale*c->h));

	glTexCoord2f(c->tx1,c->ty2);
	glVertex2f((float)x, (float)(y+Scale*c->h));

	glEnd();
}

// ASSUME: ortho mode
void Font::drawtext(int x, int y, const char *text)
{
	glBindTexture(GL_TEXTURE_2D, tex);

    float base = y + size*Scale;
	float xpos = x;
    const char *c = text;

	while (*c!=0)
	{

		if (*c != '\n' && *c != '\r')
		{
			drawchar(xpos, base - chars[*c].baseline*Scale, *c);
			xpos += max(Scale*chars[*c].w,0.5f) + Scale*2;
		}
		else
		{
			base += size;
			xpos = x;
		}
		c++;
	}
}

void Font::shdrawtext(int x, int y, const char *text)
{
	GLfloat col[4];
	glGetFloatv(GL_CURRENT_COLOR, col);
	glColor4f(0,0,0,1);

	drawtext(x+2*Scale,y+2*Scale,text);

	glColor4fv(col);
	drawtext(x,y,text);
}

void Font::print(int x, int y, const char *str, ...)
{
	char buf[1024];

	va_list ap;

	va_start (ap, str);
	vsprintf (buf, str, ap);
	va_end (ap);

	drawtext(x,y,buf);
}

void Font::shprint(int x, int y, const char *str, ...)
{
	char buf[1024];

	va_list ap;

	va_start (ap, str);
	vsprintf (buf, str, ap);
	va_end (ap);

	shdrawtext(x,y,buf);
}

int Font::textwidth(const char *text)
{
	int w = 0,maxw=0;
    const char *c = text;
	while (*c!=0)
	{

		if (*c != '\n' && *c != '\r')
		{
			w += chars[*c].w +2;
		}
		else
		{
			if (w>maxw) maxw = w;
			w = 0;
		}
		c++;
	}
	if (w>maxw) maxw = w;
	return maxw;
}

void Font::setScale(float sc)
{
	Scale=sc;
	if(Scale<=0.1)
		Scale=1.0;
}