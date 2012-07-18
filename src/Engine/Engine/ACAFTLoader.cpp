#include "ACAFTLoader.h"

FILE* ACAFTLoader::mpFile;

bool ACAFTLoader::CloseFile( bool rBool )
{
	// close file
	if( mpFile )
	{
		fclose( mpFile );
		mpFile = nullptr;
	}

	// bye
	return rBool;
}

bool ACAFTLoader::LoadFromFile( const char* path, ACFont* outFont)
{
	// open the file
	mpFile = fopen( path, "rb" );
	if( !mpFile ) return false;

	outFont->ID = 0;
	outFont->Instance = 0;

	INT version = 0;
	fread( &version, sizeof ( INT ), 1, mpFile );

	char name[64];
	fread ( &name, sizeof ( char ), 64, mpFile);
	outFont->Name = name;

	fread ( &outFont->Size, sizeof ( INT ), 1, mpFile);
	fread ( &outFont->Width, sizeof ( INT ), 1, mpFile);
	fread ( &outFont->Height, sizeof ( INT ), 1, mpFile);

	INT charCount = 0;
	fread ( &charCount, sizeof ( INT ), 1, mpFile);

	//zera toda a estrutura de caracteres
	ZeroMemory(&outFont->Characteres[0], sizeof ( ACCharSettings ) * 256);

	//pega as configuracoes de todos os 256 characteres
	for (int i=0; i < 256; i++)
	{
		ACCharSettings ch;
		ZeroMemory( &ch, sizeof ( ACCharSettings ));

		//resgata do arquivo a estrutura
		fread (&ch, sizeof ( ACCharSettings ), 1, mpFile);
		
		//copia as informacoes da posicao correta ja para o array, assim eu uso o codigo asc como indice direto
		memcpy( &outFont->Characteres[ch.Id], &ch, sizeof ( ACCharSettings ));
	}
	
	return CloseFile( true );
};