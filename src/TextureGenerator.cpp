#include "TextureGenerator.hpp"

#include <cmath>

TextureGenerator::TextureGenerator( std::string fontFace, unsigned int width, unsigned int height )
	: _width( width ),
	  _height( height ),
	  _buffer( width * height )
{
	if ( init() ) {
		loadFont( fontFace );
	}
}

TextureGenerator::~TextureGenerator( void )
{
	cleanup();
}

bool TextureGenerator::execute( std::string text )
{
	_pixelSize = _width / std::sqrt( text.length() );
	_cursorX = 0;
	_cursorY = 0;

	memset( &_buffer[ 0 ], 0, sizeof( unsigned char ) * _buffer.size() );

	int error = FT_Set_Pixel_Sizes( _face, _pixelSize, 0 );
	if ( error != 0 ) {
		std::cout << "Error setting char size: " << error << std::endl;
		return false;
	}

	for ( int n = 0; n < text.length(); n++ ) {
		pushChar( text[ n ] );
	}

	return true;
}

void TextureGenerator::save( std::string fileName )
{
	saveTexture( fileName + ".tga" );
	saveGlyphs( fileName + ".txt" );
}

bool TextureGenerator::init( void )
{
	int error = FT_Init_FreeType( &_library );
	if ( error ) {
		std::cerr << "Error opening library " << error << std::endl;
		return false;
	}

	return true;
}

bool TextureGenerator::loadFont( std::string fontFace )
{
	int error = FT_New_Face( _library, fontFace.c_str(), 0, &_face );
	if ( error == FT_Err_Unknown_File_Format ) {
		std::cout << "Unknown file format for face " << fontFace << std::endl;
		return false;
	}
	else if ( error ) {
		std::cout << "Error loading face " << fontFace << std::endl;
		return false;
	}

	return true;
}

void TextureGenerator::cleanup( void )
{
	FT_Done_Face( _face );
	FT_Done_FreeType( _library );
}

void TextureGenerator::pushChar( char c )
{
	int error = FT_Load_Char( _face, c, FT_LOAD_RENDER );
	if ( error ) {
		return;
	}

	FT_Glyph glyph;
	error = FT_Get_Glyph( _face->glyph, &glyph );
	if ( error ) {
		return;
	}

	FT_Glyph_Metrics metrics = _face->glyph->metrics;

	if ( _cursorX + _face->glyph->bitmap.width > _width ) {
		_cursorX = 0;
		_cursorY += _pixelSize;
	}

	std::cout << c 
		<< " " << ( float ) _cursorX / ( float ) _width 
		<< " " << ( float ) _cursorY / ( float ) _height
		<< " " << ( float ) _face->glyph->bitmap.width / ( float ) _width
		<< " " << ( float ) _pixelSize / ( float ) _height
		<< " " << metrics.horiAdvance / 64
		<< std::endl;

	writeBuffer( _face->glyph, _cursorX, _cursorY + _pixelSize - ( metrics.horiBearingY ) / 64 );

	_cursorX += metrics.horiAdvance / 64;
}

void TextureGenerator::writeBuffer( FT_GlyphSlot &slot, int x, int y )
{
	FT_Int x_max = x + slot->bitmap.width;
	FT_Int y_max = y + slot->bitmap.rows;

	for ( FT_Int i = x, p = 0; i < x_max; i++, p++ ) {
		for ( FT_Int j = y, q = 0; j < y_max; j++, q++ ) {
			if ( i < 0 || j < 0 || i >= _width || j >= _height ) {
				continue;
			}

			int bufferIdx =  ( _height - j ) * _width + i;
			if ( bufferIdx >= _width * _height ) {
				continue;
			}

			int slotIdx = q * slot->bitmap.width + p;
			if ( slotIdx >= slot->bitmap.width * slot->bitmap.rows ) {
				continue;
			}

			_buffer[ bufferIdx ] |= slot->bitmap.buffer[ slotIdx ];
		}
	}
}

void TextureGenerator::saveTexture( std::string fileName )
{
	unsigned char identsize = 0;
    unsigned char colorMapType = 0;
    unsigned char imageType = 3;	// grayscale
    unsigned short colorMapStart = 0;
    unsigned short colorMapLength = 0;
    unsigned char colorMapBits = 0;
    unsigned short xstart = 0;
    unsigned short ystart = 0;
    unsigned short width = _width;
    unsigned short height = _height;
    unsigned char bits = 8;
    unsigned char descriptor = 0;

	FILE *out = fopen( fileName.c_str(), "wb" );

	fwrite( &identsize, sizeof( unsigned char ), 1, out );
	fwrite( &colorMapType, sizeof( unsigned char ), 1, out );
	fwrite( &imageType, sizeof( unsigned char ), 1, out );
	fwrite( &colorMapStart, sizeof( unsigned short ), 1, out );
	fwrite( &colorMapLength, sizeof( unsigned short ), 1, out );
	fwrite( &colorMapBits, sizeof( unsigned char ), 1, out );
	fwrite( &xstart, sizeof( unsigned short ), 1, out );
	fwrite( &ystart, sizeof( unsigned short ), 1, out );
	fwrite( &width, sizeof( unsigned short ), 1, out );
	fwrite( &height, sizeof( unsigned short ), 1, out );
	fwrite( &bits, sizeof( unsigned char ), 1, out );
	fwrite( &descriptor, sizeof( unsigned char ), 1, out );

	fwrite( &_buffer[ 0 ], _width * _height, 1, out );

	fclose( out );
}

void TextureGenerator::saveGlyphs( std::string fileName )
{

}
