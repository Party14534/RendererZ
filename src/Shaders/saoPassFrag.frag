#version 330 core

layout (location = 0) out vec2 gSAO; // r = ao, g = view-space depth (for the blur pass's bilateral weight)

in vec2 TexCoord;

#define M_PI 3.141592653589

uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform vec3 view_pos_z;
uniform vec2 resolution_z;
uniform mat4 view_z;
uniform mat4 projection_z;

vec3[64] saoPoints = vec3[](
    vec3( -0.02372893145416545 ,  -0.010685747521495623 ,  0.047912920308334016 ),
    vec3( 0.030706586959765015 ,  -0.002975497907109882 ,  0.012210718316033351 ),
    vec3( -0.010422413955277907 ,  -0.0531685051473867 ,  0.07271626097898318 ),
    vec3( -0.006305793307862337 ,  0.004638846142257576 ,  0.006854950833772124 ),
    vec3( -0.03536986807157775 ,  -0.02063036564211439 ,  0.02716173437348529 ),
    vec3( 0.04487137864178175 ,  0.06118362656202528 ,  0.04673307080708361 ),
    vec3( 0.02688591425364158 ,  -0.057588346246264015 ,  0.007047801984068037 ),
    vec3( 0.00536589972019919 ,  0.037277638677871316 ,  0.003807359991274586 ),
    vec3( -0.03242173451124501 ,  -0.02066231500542963 ,  0.05493690844402751 ),
    vec3( 0.02226927176892047 ,  0.004398090486545989 ,  0.060987087490114854 ),
    vec3( -0.0016617226950557137 ,  -0.00818915968466086 ,  0.022502115112782523 ),
    vec3( -0.007424208391554006 ,  0.007867693645120459 ,  0.003179078728857938 ),
    vec3( 0.006526928375109353 ,  -0.01748943813280748 ,  0.027117955807914287 ),
    vec3( -0.021351843581283118 ,  -0.022929007944336204 ,  0.0724260017606292 ),
    vec3( 0.0011419106643586242 ,  -0.0005236798964969538 ,  0.0008584273470426976 ),
    vec3( 0.06307836208541234 ,  0.04744803288520203 ,  0.01760732153256181 ),
    vec3( -0.005520981981071949 ,  -0.01772368866489865 ,  0.02463519584183152 ),
    vec3( 0.0003149131279633069 ,  0.0015842111123010364 ,  0.014363335493110517 ),
    vec3( -0.0781013704932523 ,  -0.07530149580082948 ,  0.07038735382467186 ),
    vec3( 0.09514509315220786 ,  -0.022654149022654706 ,  0.14501165650374556 ),
    vec3( -0.021448029340761322 ,  0.028156529752415175 ,  0.01657715613379382 ),
    vec3( 0.055121181948159036 ,  0.07291623545531915 ,  0.011964002267163949 ),
    vec3( 0.031063077914859027 ,  -0.04586442672845807 ,  0.1314852205182114 ),
    vec3( 0.10150420823303015 ,  0.13245397030903203 ,  0.09784918425618358 ),
    vec3( -0.1128373123456364 ,  0.03661816540206416 ,  0.045446466715934324 ),
    vec3( -0.078325472707453 ,  0.038171017837932655 ,  0.026320770491688943 ),
    vec3( -0.1174953760136844 ,  -0.1184423553120412 ,  0.04866836668195859 ),
    vec3( 0.07038989537316538 ,  0.13931588016851018 ,  0.0055103707112068735 ),
    vec3( -0.04955057772000665 ,  0.049098172281922814 ,  0.0246262896495482 ),
    vec3( 0.0002656723073542058 ,  -5.887418465162757e-05 ,  0.00032834137297355136 ),
    vec3( -0.053371931501765175 ,  0.07119560692610617 ,  0.06604624861724655 ),
    vec3( 0.1018018813075825 ,  -0.19206658007513602 ,  0.180788768454355 ),
    vec3( -0.14917710185561325 ,  0.15653374225906352 ,  0.09231838987956027 ),
    vec3( -0.0696784857265841 ,  -0.10055209408689252 ,  0.19616964845589532 ),
    vec3( -0.2199045795567021 ,  0.20548153111256845 ,  0.04381292464682438 ),
    vec3( -0.03204708633333503 ,  -0.040338186272932196 ,  0.05485136349381813 ),
    vec3( 0.08660067715809164 ,  -0.00841812574049208 ,  0.05099953295189786 ),
    vec3( -0.18010114665893898 ,  0.11967267111190869 ,  0.04008942006883749 ),
    vec3( -0.01147578729300095 ,  -0.010831697668944882 ,  0.0014891598124579424 ),
    vec3( 0.0847487352786458 ,  0.1270917066168329 ,  0.12992663397799384 ),
    vec3( 0.10196860962014422 ,  0.0705716160320956 ,  0.04583888109929951 ),
    vec3( -0.20350008721993412 ,  -0.23427963342859628 ,  0.3479254429653516 ),
    vec3( -0.007647239209160012 ,  -0.009384386373736573 ,  0.003374756075861608 ),
    vec3( -0.3508066912738732 ,  0.06571156163310977 ,  0.24400840240229996 ),
    vec3( -0.1838029648741809 ,  0.030409044757307605 ,  0.008120978357477097 ),
    vec3( -0.11677665832285315 ,  -0.21459410931488596 ,  0.13109463379515285 ),
    vec3( 0.08430053414126877 ,  0.33444919481915536 ,  0.21941238904967794 ),
    vec3( -0.07278096838473556 ,  -0.11348428268933242 ,  0.10150541355729199 ),
    vec3( -0.004650470821658717 ,  0.5499886845513609 ,  0.08849087953467369 ),
    vec3( 0.18100861406506802 ,  -0.02909557030288695 ,  0.19301591769680673 ),
    vec3( 0.009567235268504833 ,  -0.019735461350700152 ,  0.02283376168071854 ),
    vec3( -0.1491678500312126 ,  0.1328665564320187 ,  0.024059521965476685 ),
    vec3( 0.20563088027291118 ,  -0.07208034188125287 ,  0.31895692758081706 ),
    vec3( 0.2193470688778334 ,  -0.09616863640619636 ,  0.016651667611196026 ),
    vec3( 0.030852320903776157 ,  0.0065874496358923765 ,  0.0029970491689094176 ),
    vec3( -0.25883262490060077 ,  0.2162873471011407 ,  0.2903788174184938 ),
    vec3( 0.05180283821754115 ,  0.08555974448381819 ,  0.06254710655703008 ),
    vec3( 0.26497345954252394 ,  0.19160434964945597 ,  0.4717410930988804 ),
    vec3( -0.3437716612822554 ,  -0.2232825503117675 ,  0.2879353566327664 ),
    vec3( 0.09131738215002103 ,  0.18821687705631243 ,  0.2621605897918644 ),
    vec3( 0.2554583847902917 ,  -0.3102320328872393 ,  0.24877704618897228 ),
    vec3( -0.29362490653073403 ,  -0.3919109733243835 ,  0.26761198119118157 ),
    vec3( 0.12445697336469423 ,  -0.03149507608682172 ,  0.02731465288548595 ),
    vec3( -0.6285075086055523 ,  0.37992578309378 ,  0.40183797471171784 )
);

float hash12(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec3 randomVec(vec2 seed) {
    float x = hash12(seed);
    float y = hash12(seed + vec2(37.0, 17.0)); // offset so x and y don't correlate
    return normalize(vec3(x * 2.0 - 1.0, y * 2.0 - 1.0, 0.0));
}

vec2 calcSAO(vec3 normal, vec3 fragPos, vec2 uv) {
    vec3 cPos = (view_z * vec4(fragPos, 1.)).xyz;
    float zc = cPos.z;
    vec3 nc = normalize(mat3(view_z) * normal);

    float w = resolution_z.x;
    float h = resolution_z.y;
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y;

    // Alchemy AO variables
    float beta = .0001; // Set for look
    float epsilon = .0001;
    float sigma = 1.;

    // Calculate radius
    float FOV = radians(90.);
    float focalLen = resolution_z.y / (2. * tan(FOV * .5));
    float wR = 1.; // World space radius
    float radius2 = wR * wR;
    float radius = (wR * focalLen) / -zc;
    float tau = 7. * 1.5;

    //float phi = float( (30 * int(x)) ^ int(y)) + 10.*x*y;
    float phi = hash12(vec2(x, y)) * 2. * M_PI;
    float radiusJitter = hash12(vec2(x, y) + vec2(91.3, 47.7));

    float summation = 0.;
    float s = 9. * 1.5;
    float validCount = 0.;
    for (float i = 0.; i < s; i++) {
        //float a = (1. / s) * (i + .5);
        float a = (1. / s) * (i + radiusJitter);
        float sampleRadius = radius * a;
        float theta = 2. * M_PI * a * tau + phi;
        vec2 u = vec2(cos(theta), sin(theta));

        vec2 sampleCoords = vec2(x, y) + sampleRadius * u;
        vec2 sampleUv = sampleCoords / vec2(w, h);
        if (sampleUv.x < 0. || sampleUv.x > 1. || sampleUv.y < 0. || sampleUv.y > 1.) continue;

        vec3 qPos = (view_z * vec4(texture(gPosition, sampleUv).xyz, 1.)).xyz;
        vec3 vi = qPos - cPos; // Displacement
        summation += max(0., dot(vi, nc) + zc*beta) / (dot(vi, vi) + epsilon);
        validCount += 1.;
    }

    float val = 1. - ((2.*sigma) / validCount) * summation;

    return vec2(max(0., val), zc); // To the power of k but k is one in Alchemy AO
}

float calcSSAO(vec3 normal, vec3 fragPos, vec2 uv) {
    vec3 viewFragPos = (view_z * vec4(fragPos, 1.)).xyz;
    vec3 viewNormal = normalize(mat3(view_z) * normal);

    vec3 randomVec = randomVec(gl_FragCoord.xy);
    vec3 tangent = normalize(randomVec - viewNormal * dot(randomVec, viewNormal));
    vec3 bitangent = cross(viewNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, viewNormal);

    float occlusion = 0.;
    float radius = .5;
    float bias = .025;
    for (int i = 0; i < 64; i++) {
        vec3 pointPos = viewFragPos + (TBN * saoPoints[i]) * radius;

        vec4 offset = projection_z * vec4(pointPos, 1.);
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * .5 + .5; // transform to range 0.0 - 1.0
        if (offset.x < 0. || offset.x > 1. || offset.y < 0. || offset.y > 1.) continue;

        vec3 sceneWorldPos = texture(gPosition, offset.xy).xyz;
        float sceneWorldDepth = (view_z * vec4(sceneWorldPos, 1.)).z;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(viewFragPos.z - sceneWorldDepth));
        occlusion += (sceneWorldDepth >= pointPos.z + bias ? 1. : 0.) * rangeCheck;
    }

    return 1. - (occlusion / 64.);
}

void main()
{
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal =  texture(gNormal, TexCoord).rgb;

    vec3 viewDir = normalize(view_pos_z - FragPos);

    gSAO = calcSAO(Normal, FragPos, TexCoord);
}
