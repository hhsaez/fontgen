/**
 * Copyright (c) 2013, Hugo Hernan Saez
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "TextureGenerator.hpp"

std::string extractFontName( std::string input )
{
	std::string filename = input.substr( input.find_last_of( "/" ) + 1 );
	std::string fontName = filename.substr( 0, filename.find_last_of( "." ) );
	return fontName;
}

int main( int argc, char **argv )
{
	if ( argc < 2 ) {
		std::cout << "usage: " << argv[ 0 ] << " ttf_file [texture_size]" << std::endl;
		return -1;
	}

	std::string fontFile( argv[ 1 ] );
	std::string fontName = extractFontName( fontFile );

	int size = argc >= 3 ? atoi( argv[ 2 ] ) : 512;

	// generate the list of characters to be processed
	char buffer[ 97 ];
	for ( char i = 0; i < 95; i++ ) {
		buffer[ i ] = 32 + i;
	}
	buffer[ 96 ] = '\0';
	std::string text = buffer;

	TextureGenerator generator( fontFile, size, size );
	if ( generator.execute( text, fontName ) ) {
#ifdef __APPLE__		
		system( ( std::string( "open " ) + fontName + ".tga" ).c_str() );
		system( ( std::string( "open " ) + fontName + ".txt" ).c_str() );
#endif
	}

	return 0;
}

