#include "TextureGenerator.hpp"

int main( int argc, char **argv )
{
	if ( argc < 2 ) {
		std::cout << "usage: " << argv[ 0 ] << " ttf_file [text]" << std::endl;
		return -1;
	}

	std::string fontFace( argv[ 1 ] );

	std::string text;
	if ( argc >= 3 ) {
		text = argv[ 2 ];
	}
	else {
		char buffer[ 97 ];
		for ( char i = 0; i < 95; i++ ) {
			buffer[ i ] = 32 + i;
		}
		buffer[ 96 ] = '\0';
		text = buffer;
	}

	TextureGenerator generator( fontFace, 512, 512 );
	if ( generator.execute( text ) ) {
		generator.save( "result" );
#ifdef __APPLE__
		system( "open result.tga" );
		system( "cat result.txt" );
#endif
	}

	return 0;
}

