#version 420 core

in vec4 colorToFS;
//in vec2 texCoord;

//uniform int usingTexture;
//uniform sampler2D texture;

out vec4 fragmentColor;

void main()
{
//	if( usingTexture == -1 )
//	{
		fragmentColor = colorToFS;
//	}
//	else
//	{
//		fragmentColor = colorToFS * texture2D( texture, texCoord );
//	}
}
