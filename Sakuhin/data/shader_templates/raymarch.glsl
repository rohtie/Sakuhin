const float PI = acos(-1.);

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float hardness;
};

mat2 rotate(float angle) {
    return mat2(-sin(angle), cos(angle),
                 cos(angle), sin(angle));
}

vec3 triPlanar(sampler2D tex, vec3 normal, vec3 p) {
    vec3 cX = texture(tex, p.yz).rgb;
    vec3 cY = texture(tex, p.xz).rgb;
    vec3 cZ = texture(tex, p.xy).rgb;

    vec3 blend = abs(normal);
    blend /= blend.x + blend.y + blend.z + 0.001;

    return blend.x * cX + blend.y * cY + blend.z * cZ;
}

float map(vec3 p) {
    return length(p) - 0.0;
}

vec3 getNormal(vec3 p) {
    vec2 extraPolate = vec2(0.002, 0.0);

    return normalize(vec3(
        map(p + extraPolate.xyy),
        map(p + extraPolate.yxy),
        map(p + extraPolate.yyx)
    ) - map(p));
}

float intersect (vec3 camera, vec3 ray) {
    const float maxDistance = 10.0;
    const float distanceTreshold = 0.001;
    const int maxIterations = 50;

    float distance = 0.0;

    float currentDistance = 1.0;

    for (int i = 0; i < maxIterations; i++) {
        if (currentDistance < distanceTreshold || distance > maxDistance) {
            break;
        }

        vec3 p = camera + ray * distance;

        currentDistance = map(p);

        distance += currentDistance;
    }

    if (distance > maxDistance) {
        return -1.0;
    }

    return distance;
}

vec3 proceduralTextureRaw(vec3 p) {
    return vec3(smoothstep(0.49, 0.5, mod(p.x * 9.5, 1.)));
}

vec3 proceduralTexture(in vec3 p) {
    const int textureSamples = 10;

    vec3 ddx_p = p + dFdx(p);
    vec3 ddy_p = p + dFdy(p);

    int sx = 1 + int( clamp( 4.0*length(p), 0.0, float(textureSamples-1) ) );
    int sy = 1 + int( clamp( 4.0*length(p), 0.0, float(textureSamples-1) ) );

    vec3 no = vec3(0.0);

    for ( int j=0; j<textureSamples; j++ ) {
        for ( int i=0; i<textureSamples; i++ ) {
            if ( j<sy && i<sx ) {
                vec2 st = vec2( float(i), float(j) ) / vec2( float(sx),float(sy) );
                no += proceduralTextureRaw( p + st.x*(ddx_p-p) + st.y*(ddy_p-p));
            }
        }
    }

    return no / float(sx*sy);
}

Material getMaterial(vec3 p) {
    float distance = map(p);

    return Material(
        vec3(0.15, 0.15, 0.15),
        vec3(0.5, 0.5, 0.5),
        vec3(0.25, 0.25, 0.25),
        0.05
    );
}

vec4 pixel(vec2 p) {
    p /= resolution.xy;
    p = 2.0 * p - 1.0;
    p.x *= resolution.x / resolution.y;

    vec3 camera = vec3(0.0, 0.5, 3.0);
    vec3 light = normalize(vec3(5.0, 5.0, 5.0));

    vec3 ray = normalize(vec3(p, -1.0));

    mat2 rotation = rotate(PI + time * 0.15);

    ray.xz *= rotation;
    camera.xz *= rotation;

    float distance = intersect(camera, ray);

    vec3 col = vec3(0.0);

    if (distance > 0.0) {
        col = vec3(0.0);

        vec3 p = camera + ray * distance;

        vec3 normal = getNormal(p);

        Material material = getMaterial(p);

        col += material.ambient;
        col += material.diffuse * max(dot(normal, light), 0.0);

        vec3 halfVector = normalize(light + normal);
        col += material.specular * pow(max(dot(normal, halfVector), 0.0), material.hardness);

        float att = clamp(1.0 - length(light - p) / 15., 0.0, 1.0); att *= att;
        col *= att;

        col *= vec3(smoothstep(0.25, 0.75, map(p + light))) + 0.5;

    }

    return vec4(col, 1.);
}
