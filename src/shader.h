#ifndef SHADER_INC
#   define SHADER_INC

#define WHITE vec4(1., 1., 1., 1.)
#define BLACK vec4(0.,0.,0.,1.)
#define S(A) #A
#define STRINGIFY(A) S(A)

/*generic fragment shader */
const char* genericFS = STRINGIFY(
void main( void )                                           \n
{                                                           \n
   gl_FragColor = gl_Color;                                 \n
}                                                           \n
);


/* sillhouette vertex shader */
const char* sillhouetteVS = STRINGIFY(
void main()                                                         \n
{                                                                   \n
    gl_Position = ftransform();                                     \n
}                                                                   \n
);

/* sillhoutte fragment shader */
const char* silhouetteFS = STRINGIFY(
void main()                                                              \n
{                                                                        \n
    /* set pixel color to diffuse color */                               \n
    gl_FragColor = gl_FrontMaterial.diffuse;                             \n
}                                                                        \n
);


/* simple texture vertex shader */
const char* textureVS = STRINGIFY(
varying vec2 texture_coordinate;
void main()                                                         \n
{                                                                   \n
    /* pass texture coords to fragment shader */                    \n
    texture_coordinate = gl_MultiTexCoord0.st;                      \n
    gl_Position = ftransform();                                     \n
}                                                                   \n
);

/* simple texture fragment shader */
const char* textureFS = STRINGIFY(
varying vec2 texture_coordinate;                                         \n
uniform sampler2D texture;                                               \n
void main()                                                              \n
{                                                                        \n
    /* sample the texture to get pixel color */                          \n
    gl_FragColor = texture2D(texture, texture_coordinate);               \n
}                                                                        \n
);

/* black and white checkerboard vertex shader */
const char* checkerVS = STRINGIFY(
varying vec2 texture_coordinate;
void main()                                                         \n
{                                                                   \n
    /* pass texture coords to fragment shader */                    \n    
    texture_coordinate = gl_MultiTexCoord0.st;                      \n   
    gl_Position = ftransform();                                     \n
}                                                                   \n
);


/* black and white checkerborad fragment shader */
const char* checkerFS = STRINGIFY(
varying vec2 texture_coordinate;                                                    \n
                                                                                    \n
/* checkerboard function from class */                                              \n
vec4 checkerboard(vec2 texture_coordinate, vec4 color0, vec4 color1)                \n
{                                                                                   \n
    /* float mod operation b/c graphics hardware does not support integer mod       \n
     * so we use < 0.5 to compensate for possible error instead of == 0 */          \n
    if (mod((floor(texture_coordinate.x) + floor(texture_coordinate.y)), 2.) < 0.5) \n
        return color0;                                                              \n
    else                                                                            \n
        return color1;                                                              \n
}                                                                                   \n
                                                                                    \n
void main()                                                              \n
{                                                                        \n
    gl_FragColor = checkerboard(texture_coordinate * 10., WHITE, BLACK); \n
}                                                                        \n
);


/* checkerboard vertex shader that also uses blinn-phong shading */
const char* blinncheckerVS = STRINGIFY(
uniform int numlights;                                                      \n
varying vec2 texture_coordinate;                                            \n
void main()                                                                 \n
{                                                                           \n
	vec3 normal;		                                                    \n
	float NdotH;                                                            \n
	float intensity;                                                        \n
	                                                                        \n
	/* calculate normal */                                                  \n
	normal = normalize(gl_NormalMatrix * gl_Normal);                        \n
                                                                            \n
    /* iterate over light sources */                                        \n
    for(int i = 0; i < numlights; i++) {                                    \n
    	/* calculate N dot H */                                             \n
        NdotH = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));  \n
	                                                                        \n
    	/* calculate the intensity at the vertex, NdotH^alpha*/             \n
        intensity = pow(NdotH, gl_FrontMaterial.shininess);                 \n
	                                                                        \n
    	/* add the ambient component */                                     \n
        gl_FrontColor = gl_LightSource[i].ambient *                         \n
                        gl_FrontMaterial.ambient;                           \n
                                                                            \n
        /* add the diffuse component */                                     \n
        gl_FrontColor += gl_LightSource[i].diffuse *                        \n
                         gl_FrontMaterial.diffuse * NdotH;                  \n
                                                                            \n
        /* add the specular component */                                    \n
        gl_FrontColor += gl_LightSource[i].specular *                       \n
                         gl_FrontMaterial.specular * intensity;             \n
    }                                                                       \n
                                                                            \n
	/* transform the vertex by the current opengl transformation matrix	*/  \n
	gl_Position = ftransform();                                             \n
    /* pass texture coords to fragment shader */                            \n    
    texture_coordinate = gl_MultiTexCoord0.st;                              \n
}                                                                           \n
);

/* checkered blinn-phong shading */
const char* blinncheckerFS = STRINGIFY(
varying vec2 texture_coordinate;                                                    \n
                                                                                    \n
/* checkerboard function from class */                                              \n
vec4 checkerboard(vec2 texture_coordinate, vec4 color0, vec4 color1)                \n
{                                                                                   \n
    /* float mod operation b/c graphics hardware does not support integer mod       \n
     * so we use < 0.5 to compensate for possible error instead of == 0 */          \n
    if (mod((floor(texture_coordinate.x) + floor(texture_coordinate.y)), 2.) < 0.5) \n
        return color0;                                                              \n
    else                                                                            \n
        return color1;                                                              \n
}                                                                                   \n
                                                                                    \n
void main()                                                              \n
{                                                                        \n
    gl_FragColor = checkerboard(                                         \n
                texture_coordinate * 10.,                                \n
                gl_Color,                                                \n
                gl_Color * 2.                                            \n
            );                                                           \n
}                                                                        \n
);

/* texture-modulated blinn-phong shading */
const char* blinntextureVS = STRINGIFY(
uniform int numlights;                                                      \n
varying vec2 texture_coordinate;                                            \n
void main()                                                                 \n
{                                                                           \n
	vec3 normal;		                                                    \n
	float NdotH;                                                            \n
	float intensity;                                                        \n
	                                                                        \n
	/* calculate normal */                                                  \n
	normal = normalize(gl_NormalMatrix * gl_Normal);                        \n
                                                                            \n
    /* iterate over light sources */                                        \n
    for(int i = 0; i < numlights; i++) {                                    \n
    	/* calculate N dot H */                                             \n
        NdotH = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));  \n
	                                                                        \n
    	/* calculate the intensity at the vertex, NdotH^alpha*/             \n
        intensity = pow(NdotH, gl_FrontMaterial.shininess);                 \n
	                                                                        \n
    	/* add the ambient component */                                     \n
        gl_FrontColor = gl_LightSource[i].ambient *                         \n
                        gl_FrontMaterial.ambient;                           \n
                                                                            \n
        /* add the diffuse component */                                     \n
        gl_FrontColor += gl_LightSource[i].diffuse *                        \n
                         gl_FrontMaterial.diffuse * NdotH;                  \n
                                                                            \n
        /* add the specular component */                                    \n
        gl_FrontColor += gl_LightSource[i].specular *                       \n
                         gl_FrontMaterial.specular * intensity;             \n
    }                                                                       \n
                                                                            \n
	/* transform the vertex by the current opengl transformation matrix	*/  \n
	gl_Position = ftransform();                                             \n
    texture_coordinate = gl_MultiTexCoord0.st;                              \n
}                                                                           \n
);

/* texture-modulated blinn-phong shader */
const char* blinntextureFS = STRINGIFY(
varying vec2 texture_coordinate;                                         \n
uniform sampler2D texture;                                               \n
void main()                                                              \n
{                                                                        \n
    /* multiply shaded color by sample from texture map */               \n
    gl_FragColor = gl_Color * texture2D(texture, texture_coordinate);    \n
}                                                                        \n
);

/* attempt at normal mapping */
const char* normalVS = STRINGIFY(
varying vec2 st;                                                            \n
void main(void)                                                             \n
{	                                                                        \n
	/* transform the vertex by the current opengl transformation matrix	*/  \n
	gl_Position = ftransform();                                             \n
    st = gl_MultiTexCoord0.st;                                              \n
    
}
);

/* attempt at normal mapping */
const char* normalFS = STRINGIFY(                                           \n
uniform vec3 lightDir;                                                   \n
        
varying vec2 st;                                                            \n
uniform sampler2D normal_map;                                               \n
uniform int numlights;                                                      \n
void main()                                                                 \n
{                                                                           \n
	vec3 normal;		                                                    \n
	float NdotH;                                                            \n    
	float intensity;                                                        \n
	                                                                        \n
	/* calculate normal */                                                  \n
	normal = normalize(gl_NormalMatrix * normalize(texture2D(normal_map, st).xyz));    \n
                                                                            \n
    /* iterate over light sources */                                        \n
    for(int i = 0; i < numlights; i++) {                                    \n
    	/* calculate N dot H */                                             \n
        NdotH = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));  \n
	                                                                        \n
    	/* calculate the intensity at the vertex, NdotH^alpha*/             \n
        intensity = dot(normalize(lightDir), normalize(normal));          \n
	                                                                        \n
    	/* add the ambient component */                                     \n
        gl_FragColor = gl_LightSource[i].ambient *                          \n
                        gl_FrontMaterial.ambient;                           \n
                                                                            \n
        /* add the diffuse component */                                     \n
        gl_FragColor += gl_LightSource[i].diffuse *                         \n
                         gl_FrontMaterial.diffuse * NdotH;                  \n
                                                                            \n
        /* add the specular component */                                    \n
        gl_FragColor += gl_LightSource[i].specular *                        \n
                         gl_FrontMaterial.specular * intensity;             \n
    }                                                                       \n
}                                                                           \n
);

/* cartoon example */
const char* toonVS = STRINGIFY(
varying vec3 normal;                                                \n
void main()                                                         \n
{                                                                   \n
    normal = gl_Normal;                                             \n
    gl_Position = ftransform();                                     \n
}                                                                   \n
);

/* frag shader example */
const char* toonFS = STRINGIFY(
uniform vec3 lightDir;                                                   \n
varying vec3 normal;                                                     \n
void main()                                                              \n
{                                                                        \n
    float intensity = dot(normalize(lightDir), normalize(normal));       \n
    if ( intensity > 0.95 )                                              \n
        gl_FragColor = gl_FrontMaterial.diffuse;                         \n
    else if ( intensity > 0.5 )                                          \n
        gl_FragColor = gl_FrontMaterial.diffuse*0.9;                     \n
    else if ( intensity > 0.25 )                                         \n
        gl_FragColor = gl_FrontMaterial.diffuse*0.7;                     \n
    else                                                                 \n
        gl_FragColor = gl_FrontMaterial.diffuse*0.4;                     \n
}                                                                        \n
);

/* gourand vertex shader using phong reflection model */
const char *gourandVS = STRINGIFY(
uniform int numlights;                                                          \n
void main() {                                                                   \n
    vec3 N;                                                                     \n
    vec3 L;                                                                     \n
    vec3 R;                                                                     \n
    vec3 V;                                                                     \n
    float LdotN;                                                                \n
                                                                                \n
    /* calculate normal and view directions */                                  \n
    N = normalize(gl_NormalMatrix * gl_Normal);                                 \n
    V = normalize(vec3(gl_ModelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0)            \n
                       - gl_Vertex));                                           \n
                                                                                \n
    /* iterate over light sources */                                            \n
    for(int i = 0; i < numlights; i++) {                                        \n
        /* add the ambient term */                                              \n
        gl_FrontColor = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;   \n
                                                                                \n
        /* calculate the light direction and L dot N */                         \n
        L = normalize(vec3(gl_LightSource[i].position));                        \n
        LdotN = dot(L, N);                                                      \n
                                                                                \n
        /* only add specular term if L dot N is positive */                     \n
        if (LdotN > 0.0) {                                                      \n
            /* add the diffuse term */                                          \n
            gl_FrontColor += gl_FrontMaterial.diffuse *                         \n
                             LdotN *                                            \n
                             gl_LightSource[i].diffuse;                         \n
                                                                                \n
            /* calculate direction of reflection */                             \n
            R = normalize(reflect(-L, N));                                      \n
                                                                                \n
            /* add the specular term */                                         \n
            gl_FrontColor += gl_FrontMaterial.specular *                        \n
                        max(0.0, pow(dot(R, V), gl_FrontMaterial.shininess)) *  \n
                        gl_LightSource[i].specular;                             \n
        }                                                                       \n
    }                                                                           \n
    gl_Position = ftransform();                                                 \n
}
);

/* gourand fragment shader does nothing */
const char* gourandFS = genericFS;

/* blinn-phong vertex shader */
const char* blinnphongVS = STRINGIFY(
uniform int numlights;                                                      \n
void main()                                                                 \n
{                                                                           \n
	vec3 normal;		                                                    \n
	float NdotH;                                                            \n
	float intensity;                                                        \n
	                                                                        \n
	/* calculate normal */                                                  \n
	normal = normalize(gl_NormalMatrix * gl_Normal);                        \n
                                                                            \n
    /* iterate over light sources */                                        \n
    for(int i = 0; i < numlights; i++) {                                    \n
    	/* calculate N dot H */                                             \n
        NdotH = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));  \n
	                                                                        \n
    	/* calculate the intensity at the vertex, NdotH^alpha*/             \n
        intensity = pow(NdotH, gl_FrontMaterial.shininess);                 \n
	                                                                        \n
    	/* add the ambient component */                                     \n
        gl_FrontColor = gl_LightSource[i].ambient *                         \n
                        gl_FrontMaterial.ambient;                           \n
                                                                            \n
        /* add the diffuse component */                                     \n
        gl_FrontColor += gl_LightSource[i].diffuse *                        \n
                         gl_FrontMaterial.diffuse * NdotH;                  \n
                                                                            \n
        /* add the specular component */                                    \n
        gl_FrontColor += gl_LightSource[i].specular *                       \n
                         gl_FrontMaterial.specular * intensity;             \n
    }                                                                       \n
                                                                            \n
	/* transform the vertex by the current opengl transformation matrix	*/  \n
	gl_Position = ftransform();                                             \n
}
);

/* blinn-phong fragment shader does nothing */
const char* blinnphongFS = genericFS;

/* swirl vertex shader */
const char* swirlVS = blinncheckerVS;
/* swirl fragment shader */
const char* swirlFS =  STRINGIFY(
varying vec2 texture_coordinate;                                                    \n
                                                                                    \n
vec4 swirl(vec2 texture_coordinate, vec4 color)                \n
{                                                                                   \n
    return mod(sin(texture_coordinate.x + texture_coordinate.y), 1.) * color; \n
                                                                                    \n
}                                                                                   \n
                                                                                    \n
void main()                                                              \n
{                                                                        \n
    gl_FragColor = swirl(texture_coordinate * 20., gl_Color); \n
}                                                                        \n
);


#endif
