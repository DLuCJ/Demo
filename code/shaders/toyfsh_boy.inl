// Modification of shader credited below
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

static const char *fsh_boy="\
uniform sampler2D iChannel0;\
uniform vec3 iResolution;\
uniform float iGlobalTime;\
uniform float BPM;\
float text( vec2 p ) \
{\
	p.x += 0.6*floor(20.0*(0.5+0.5*sin(iGlobalTime)))/10.0;	\
	float x = floor( p.x*100.0 ) - 23.0;\
	float y = floor( p.y*100.0 ) - 82.0;\
	if( y<0.0 || y> 5.0) return 0.0;\
	if( x<0.0 || x> 60.0) return 0.0;\
    float v = 0.0;\
 	if( x>47.5) {         v=30720.0;\
	                    if(y>0.5) v=52224.0;\
	                    if(y>1.5) v=50688.0;\
	                    if(y>2.5) v=50688.0;\
	                    if(y>3.5) v=50688.0;\
	                    if(y>4.5) v=31744.0; }\
	else if( x>31.5 ) {           v=64786.0;\
	                    if(y>0.5) v=49426.0;\
	                    if(y>1.5) v=49426.0;\
	                    if(y>2.5) v=63794.0;\
	                    if(y>3.5) v=49580.0;\
	                    if(y>4.5) v=64584.0; }\
	else if( x>15.5 ) {           v=496.0;\
	                    if(y>0.5) v=408.0;\
	                    if(y>2.5) v=396.0;\
	                    if(y>3.5) v=396.0;\
	                    if(y>4.5) v=504.0; }\
	else	          {         v=39408.0;\
	                    if(y>0.5) v=45464.0;\
	                    if(y>1.5) v=57740.0;\
	                    if(y>2.5) v=57740.0;\
	                    if(y>3.5) v=45452.0;\
	                    if(y>4.5) v=39416.0; }\
	return floor( mod(v/pow(2.0,15.0-mod( x, 16.0 )), 2.0) );\
}\
void main()\
{\
float BOPTIME = 23.0;\
vec2 mouz = vec2(300.0 * cos(iGlobalTime), 300.0 * sin(iGlobalTime));\
vec2 uv;\
 if(iGlobalTime < BOPTIME + 27.0)\
    {\
    	uv = -1.0 + 2.0*gl_FragCoord.xy / iResolution.xy;\
    }\
    else\
    {\
        const float initialZoom = 1.5;\
    	float time =mod( iGlobalTime, BOPTIME+26.0 );\
   		float zoom = pow(initialZoom, (2.5-time - initialZoom) / 5.0);\
    	uv = -1.0 + 2.0*zoom*(gl_FragCoord.xy / iResolution.xy);\
    }\
vec2 uvo = uv;\
	uv.x *= iResolution.x/iResolution.y;\
   vec2 mo = sin(iGlobalTime/4)/2 + mouz.xy / iResolution.xy;	\
	mo.x *= iResolution.x/iResolution.y;\
    vec3 col = vec3( 0.0 );\
if(iGlobalTime > BOPTIME && iGlobalTime < BOPTIME + 21.0) {\
vec2 uvb = gl_FragCoord.xy / iResolution.xy;\
    uvb.x = clamp( (uvb.x-0.03)/0.94, 0.0, 1.0 );\
    vec2 res = floor( 60.0*vec2(1.0,iResolution.y/iResolution.x) );\
    vec2 iuv = floor( uvb * res )/res;\
    float dx = 1.0-abs(-1.0+2.0*fract( uvb.x * res.x ));\
    float dy = 1.0-abs(-1.0+2.0*fract( uvb.y * res.y )); \
    float c=0.;    \
    c = texture2D( iChannel0, vec2(iuv.x,0.25) ).x;\
    c= 0.8*c*c;\
    if( iuv.y<c-0.075 )\
        {\
		    if( dx>0.3 && dy>0.3 ) col = vec3(40.0,44.0,4.0);\
		    if( dx>0.5 && dy>0.5 ) col = vec3(255.0);\
        }\
}\
	float pa = smoothstep( -0.1, 0.1, sin(128.0*uv.x + 2.0*sin(32.0*uv.y)) );\
	float la = 1.0 - smoothstep( 0.0, 0.90, length(mo-uv) );\
    float v = 0.0;\
    float h = fract( 0.25 + 0.5*iGlobalTime*BPM/60.0 );\
    float r = 0.0;\
    if(iGlobalTime < BOPTIME || iGlobalTime > BOPTIME + 21.0) {\
        r = length(uv);\
    }\
    else\
    {\
        r = length(uv) + (cos(iGlobalTime)/10.0)*cos(25.0*h)*exp(-4.0*h);\
    }\
	col = mix( col, vec3(0.3,0.4,0.5), 1.0 - smoothstep( 0.2, 0.21, r ) );    \
    if(iGlobalTime < BOPTIME+0.3) {\
	r = length( vec2(abs(uv.x),uv.y)-vec2(0.2,0.0) );\
    } \
    else \
    {\
    vec2 c1 = 0.9*sin( iGlobalTime + vec2(0.4*iGlobalTime,0.5) + 1.0);\
    vec2 c2 = 0.9*sin( iGlobalTime*0.65 + vec2(0.4*iGlobalTime,2.0) +2.0);\
    float r2 = length( uv-vec2(0.2,0.0)-c1)+ (cos(iGlobalTime)/20.0)*cos(25.0*h)*exp(-4.0*h);\
    float r1 = length( uv-vec2(-0.2,0.0)+c2)+ (cos(iGlobalTime)/20.0)*cos(25.0*h)*exp(-4.0*h);    \
    v+= 1.0-smoothstep(0.0,0.4,r2);\
    v+= 1.0-smoothstep(0.0,0.4,r1);\
    }\
	col = mix( col, vec3(0.3,0.4,0.5), 1.0 - smoothstep( 0.05, 0.06, r ) );\
	float bl = sin(1.0*iGlobalTime) + sin(8.61*iGlobalTime);\
    bl = smoothstep( 1.3, 1.4, bl );\
	float gi = sin(10.0*iGlobalTime);\
	gi = gi*gi*gi; gi = gi*gi*gi; gi *= 0.0015;\
    if(iGlobalTime < BOPTIME) {\
	r = length( (uv-vec2(0.1,0.05))*vec2(1.0,1.0+bl*12.0) );\
    }\
    else \
    {\
    r = length(uv-vec2(0.1,0.05)) + (cos(iGlobalTime)/24.0)*cos(25.0*h)*exp(-4.0*h);\
    }\
	float e = 1.0 - smoothstep( 0.04, 0.05, r );\
	col = mix( col, vec3(1.0), e );\
	r = length( uv-vec2(gi+0.1 + 0.025*mo.x,0.05+0.025*mo.y) );\
	col = mix( col, vec3(0.0), e*(1.0 - smoothstep( 0.01, 0.02, r )) );\
    if(iGlobalTime < BOPTIME)\
    {\
	r = length( (uv-vec2(-0.1,0.05))*vec2(1.0,1.0+bl*12.0) );\
    }\
    else\
    {\
    r = length(uv-vec2(-0.1,0.05)) + (cos(iGlobalTime)/24.0)*cos(25.0*h)*exp(-4.0*h);	\
    }\
	e = 1.0 - smoothstep( 0.04, 0.05, r );\
	col = mix( col, vec3(1.0), e );\
	r = length( uv-vec2(-0.1+gi+ 0.025*mo.x,0.05+0.025*mo.y) );\
	col = mix( col, vec3(0.0), e*(1.0 - smoothstep( 0.01, 0.02, r )) );\
	float smb = 1.0-smoothstep(0.18,0.22,length(mo));\
	r = length( (uv-vec2(0.0,-0.07))-smb*vec2(0.0,0.5)*abs(uv.x) );\
	float sm = 1.0 - smoothstep( 0.04, 0.05, r*(2.0-smb) );\
	col = mix( col, vec3(0.0), sm );\
	r = length( (uv-vec2(0.1,-0.4))-vec2(0.0,0.5)*abs(uv.x-0.1) );\
	float be = 2.0 - 1.0*pow(0.5 + 0.5*sin(iGlobalTime*5.0),2.0);\
	float he = 1.0 - smoothstep( 0.05, 0.06, r*be );\
	float li = 1.0 - smoothstep( 0.05, 0.20, length(mo-vec2(0.1,-0.4)) );\
	col = mix( col, vec3(1.0,0.0,0.0), he*clamp(li+smb,0.0,1.0) );\
	float ls = 0.05 + 0.5*length(mo);\
	r = length( uv - mo );\
	col += 0.5*vec3(1.0,0.8,0.5) * (1.0-smoothstep(0.0,ls,r) );\
    	col = mix( col, vec3(0.3,0.4,0.5), smoothstep(0.9,0.91,v) );\
if(iGlobalTime > BOPTIME+21.0){ \
		float t = text( uvo );\
		col = mix( col, vec3(255.0,255.0,255.0),t);\
}\
	gl_FragColor = vec4(col,1.0);\
}";




