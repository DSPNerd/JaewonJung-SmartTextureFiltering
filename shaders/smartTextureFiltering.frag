uniform sampler2D Tex0;
uniform sampler2D Slut;
uniform sampler2D Zlut;
uniform vec2 TexSize;
uniform vec2 TexSizeInverse;
uniform float ColThreshold;

// compute 4-texel color variance.
float var4(vec3 texel[4])
{  
	float var;
	vec3 dif[4];
	vec3 ave = (texel[0]+texel[1]+texel[2]+texel[3])*0.25;
	dif[0] = texel[0] - ave;
	dif[1] = texel[1] - ave;
	dif[2] = texel[2] - ave;
	dif[3] = texel[3] - ave;
	var = dot(dif[0], dif[0]) + dot(dif[1], dif[1]) +
	 	  dot(dif[2], dif[2]) + dot(dif[3], dif[3]);
	var *= 0.25;
	
	return var;
}

// compute 3-texel color variance.
float var3(vec3 texel0, vec3 texel1, vec3 texel2)
{  
	float var;
	vec3 dif[3];
	vec3 ave = (texel0+texel1+texel2)*0.333;
	dif[0] = texel0 - ave;
	dif[1] = texel1 - ave;
	dif[2] = texel2 - ave;
	var = dot(dif[0], dif[0]) + dot(dif[1], dif[1]) +
      	  dot(dif[2], dif[2]);
	var *= 0.333;
        
	return var;
}

// compute 2-texel color variance.
float var2(vec3 texel0, vec3 texel1)
{  
	float var;
	vec3 dif = texel0-texel1;
	var = dot(dif, dif) * 0.25;
        
	return var;
}

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
	texel[0] = texture2D(Tex0, texelIndex).rgb;
	texel[1] = texture2D(Tex0, texelIndex + vec2(0, TexSizeInverse.t)).rgb;
	texel[2] = texture2D(Tex0, texelIndex + TexSizeInverse).rgb;
	texel[3] = texture2D(Tex0, texelIndex + vec2(TexSizeInverse.s, 0)).rgb;
	   
	vec2 slutCoord;
	vec4 weight;
   
	// compute several variances.
	float v0123 = var4(texel);
	float v013 = var3(texel[0], texel[1], texel[3]);
	float v023 = var3(texel[0], texel[2], texel[3]);
	float v123 = var3(texel[1], texel[2], texel[3]);
	float v012 = var3(texel[0], texel[1], texel[2]);
	float v03 = var2(texel[0], texel[3]);
	float v12 = var2(texel[1], texel[2]);
	float v01 = var2(texel[0], texel[1]);
	float v23 = var2(texel[2], texel[3]);
	float v02 = var2(texel[0], texel[2]);
	float v13 = var2(texel[1], texel[3]);
	
	// determine a filtering kernel(0~13).
	// 4
	if(v0123 < ColThreshold)
	{
		slutCoord.t = 0.0;
	}
	// 3-1
	else if(v013 < ColThreshold)
	{
		slutCoord.t = 0.1875;		// (3.0/16.0)
	}
	else if(v023 < ColThreshold)
 	{
  		slutCoord.t = 0.25;			// (4.0/16.0)
	}
   	else if(v123 < ColThreshold)
   	{
      	slutCoord.t = 0.3125;		// (5.0/16.0)
   	}
   	else if(v012 < ColThreshold)
   	{
      	slutCoord.t = 0.375;		// (6.0/16.0)
   	}
   	// 2-2
   	else if(v03 < ColThreshold &&
           	v12 < ColThreshold)
   	{
      	slutCoord.t = 0.0625;		// (1.0/16.0)
   	}
   	else if(v01 < ColThreshold &&
           	v23 < ColThreshold)
   	{
      	slutCoord.t = 0.125;		// (2.0/16.0)
   	}
   	// 2-1-1
   	else if(v12 < ColThreshold)
   	{
      	slutCoord.t = 0.4375;		// (7.0/16.0)
   	}
   	else if(v03 < ColThreshold)
   	{
      	slutCoord.t = 0.5;			// (8.0/16.0)
   	}
   	else if(v23 < ColThreshold)
   	{
      	slutCoord.t = 0.5625;		// (9.0/16.0)
   	}
   	else if(v01 < ColThreshold)
   	{
      	slutCoord.t = 0.625;		// (10.0/16.0)
   	}
   	// 1-2-1
   	else if(v02 < ColThreshold &&
   			v13 < ColThreshold)
   	{
   		slutCoord.t = 0.875;		// (14.0/16.0)
   	}
   	else if(v02 < ColThreshold)
   	{
      	slutCoord.t = 0.6875;		// (11.0/16.0)
   	}
   	else if(v13 < ColThreshold)
   	{
      	slutCoord.t = 0.75;			// (12.0/16.0)
   	}
   	// 1-1-1-1
   	else
   	{
      	slutCoord.t = 0.8125;		// (13.0/16.0)
   	}
   
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
