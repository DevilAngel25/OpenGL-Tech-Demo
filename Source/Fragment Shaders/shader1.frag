varying vec3 lightDir,normal;
uniform sampler2D tex;
 
void main()
{
    vec3 ct,cf;
    vec4 texel;
    float intensity,at,af;
	//calculates the intensity 
    intensity = max(dot(lightDir,normalize(normal)),0.0);
	
    cf = intensity * (gl_FrontMaterial.diffuse).rgb + gl_FrontMaterial.ambient.rgb;
    //difuse
	af = gl_FrontMaterial.diffuse.a;
	//texture color for fragment
    texel = texture2D(tex,gl_TexCoord[0].st);
	//sets the frag color to the mix of texture and lighting
    ct = texel.rgb;
    at = texel.a;
    gl_FragColor = vec4(ct * cf, at * af);
 
}