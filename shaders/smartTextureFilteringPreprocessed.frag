uniform sampler2D Tex0;
uniform sampler2D Slut;
uniform sampler2D Zlut;
uniform vec2 TexSize;
uniform vec2 TexSizeInverse;
uniform float ColThreshold;

// get a sub-texel zone index(0~7).
float getZone(vec2 texFract)
{
	// first, determine in which corner zone it is.
	bvec2 bv = lessThan(vec2(0.5, 0.5), texFract);
	ivec4 shuffle = ivec4(0, 1, 3, 2);
	int index = shuffle[int(bv.s)*2 + int(bv.t)];
   
	// now, determine whether it is in an inner zone or an outer zone.
	vec4 coeff[4];
	coeff[0] = vec4(-1.0, -1.0, 1.0, 1.0);
	coeff[1] = vec4(1.0, -1.0, -1.0, 1.0);
	coeff[2] = vec4(0.5, 0.0, -1.0, -0.5);
	coeff[3] = vec4(0.0, 0.5, 0.5, 0.0);
   
	vec2 tmp;
	tmp.s = coeff[0][index] * texFract.x + coeff[2][index];
	tmp.t = coeff[1][index] * texFract.y + coeff[3][index];
   
	return float(index + 4 * int(tmp.s < tmp.t)) * 0.125;
}

// get a sub-texel zone index(0~7) by using a zone lookup texture.
float getZoneT(vec2 texFract)
{
	return texture2D(Zlut, texFract).a;
}

void main(void)
{   
	// get 4 neighboring samples.
	vec2 texelIndex = vec2(gl_TexCoord[0]) * TexSize;
	vec2 texelFract = fract(texelIndex);
	texelIndex -= texelFract;
	texelIndex *= TexSizeInverse;
   
	vec3 texel[4];
	vec4 texel0 = texture2D(Tex0, texelIndex);
	texel[0] = texel0.rgb;
	texel[1] = texture2D(Tex0, texelIndex + vec2(0, TexSizeInverse.t)).rgb;
	texel[2] = texture2D(Tex0, texelIndex + TexSizeInverse).rgb;
	texel[3] = texture2D(Tex0, texelIndex + vec2(TexSizeInverse.s, 0)).rgb;
   
	vec2 slutCoord;
	vec4 weight;
     
	// determine a filtering kernel(0~13).
	slutCoord.t = texel0.a;

	// determine a zone(0~7).
	slutCoord.s = getZoneT(texelFract);
  
	// get a weight from the slut(Smart texture filtering LookUp Table).
	weight = texture2D(Slut, slutCoord);
   
	// smart filtering
	// w0, w1 are weights for vertical(v direction) blending.
	float w0 = min(weight.x+weight.w, 1.0);
	float w1 = min(weight.y+weight.z, 1.0);
	// in case of minification, just do lerping. 
	/*vec2 fw = fwidth(gl_TexCoord[0].st);
	if(fw.s > TexSizeInverse.s)
	{
		weight = vec4(1.0, 1.0, 1.0, 1.0);
	}
	if(fw.t > TexSizeInverse.t)
	{
		w0 = w1 = 1.0;
	}*/
	// 2 horizontal blending
	vec3 c0 = max(1.0-texelFract.s, 1.0-weight.w) * weight.x * texel[0] + 
          	  max(texelFract.s, 1.0-weight.x) * weight.w * texel[3];      	  
    vec3 c1 = max(1.0-texelFract.s, 1.0-weight.z) * weight.y * texel[1] + 
	  		  max(texelFract.s, 1.0-weight.y) * weight.z * texel[2];
	// final vertical blending
	gl_FragColor = vec4(max(1.0-texelFract.t, 1.0-w1) * w0 * c0 + 
            			max(texelFract.t, 1.0-w0) * w1 * c1, 1.0);
}
