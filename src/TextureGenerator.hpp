#ifndef FONTGEN_TEXTURE_GENERATOR_
#define FONTGEN_TEXTURE_GENERATOR_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <string>
#include <vector>
#include <iostream>

class TextureGenerator {
public:
	TextureGenerator( std::string fontFace, unsigned int width, unsigned int height );
	virtual ~TextureGenerator( void );

	bool execute( std::string text );
	void save( std::string fileName );

private:
	bool init( void );
	bool loadFont( std::string fontFace );
	void cleanup( void );
	void pushChar( char c );
	void writeBuffer( FT_GlyphSlot &slot, int x, int y );
	void saveTexture( std::string fileName );
	void saveGlyphs( std::string filename );

	std::vector< unsigned char > _buffer;
	unsigned int _width;
	unsigned int _height;
	unsigned int _pixelSize;
	unsigned int _cursorX;
	unsigned int _cursorY;

	FT_Library _library;
	FT_Face _face;
};

#endif

