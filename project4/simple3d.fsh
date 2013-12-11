#version 420 core

in vec4 colorToFS;
in vec2 texCoord;
uniform int usingTexture;

uniform sampler2D texture;

out vec4 fragmentColor;

void main()
{

	if( usingTexture == -1 )
	{
		fragmentColor = colorToFS;
	}
	else
	{
		if( texCoord[0] == 0 && texCoord[1] == 0 ) // check if error
		{
			fragmentColor = vec4( 1.0, 0.0, 0.0, 1.0 );
		}		
		else if( texCoord[0] > 1.0 )
		{
			fragmentColor = vec4( 0.0, 1.0, 0.0, 1.0 );
		}
		else if( texCoord[1] > 1.0 )
		{
			fragmentColor = vec4( 0.0, 0.0, 1.0, 1.0 );
		}
		else
		{
			//fragmentColor = 0.6 * colorToFS + 0.4 * texture2D( texture, texCoord );
			fragmentColor = texture2D( texture, texCoord );
		}
	}

	if( fragmentColor.r > 1.0 ) fragmentColor.r = 1.0;
	if( fragmentColor.g > 1.0 ) fragmentColor.g = 1.0;
	if( fragmentColor.b > 1.0 ) fragmentColor.b = 1.0;
	if( fragmentColor.a > 1.0 ) fragmentColor.a = 1.0;
}
