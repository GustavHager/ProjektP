#include "terrain.h"

Point3D* getQuad(GLfloat x, GLfloat z){

Point3D *corners;
corners = malloc(sizeof(Point3D)*4);

int bottomX,bottomZ,topX,topZ;

bottomX = floor(x);
bottomZ = floor(z);

topX = ceil(x);
topZ = ceil(z);

if ((x <= worldWidth) && (z <= worldHeight) && (z >= 0) && (x >= 0)){
    //first corner
    corners[0].x = vertexArray[(bottomX + bottomZ * worldWidth)*3+0];
    corners[0].y = vertexArray[(bottomX + bottomZ * worldWidth)*3+1];
    corners[0].z = vertexArray[(bottomX + bottomZ * worldWidth)*3+2];

    //second corner
    corners[1].x = vertexArray[(bottomX + topZ * worldWidth)*3+0];
    corners[1].y = vertexArray[(bottomX + topZ * worldWidth)*3+1];
    corners[1].z = vertexArray[(bottomX + topZ * worldWidth)*3+2];

    //third corner
    corners[2].x = vertexArray[(topX + topZ * worldWidth)*3+0];
    corners[2].y = vertexArray[(topX + topZ * worldWidth)*3+1];
    corners[2].z = vertexArray[(topX + topZ * worldWidth)*3+2];

    //fourth corner
    corners[3].x = vertexArray[(topX + bottomZ * worldWidth)*3+0];
    corners[3].y = vertexArray[(topX + bottomZ * worldWidth)*3+1];
    corners[3].z = vertexArray[(topX + bottomZ * worldWidth)*3+2];

	}else{

	    int i=0;
        for(i=0; i < 4; i++){
            corners[i].x = corners[i].y = corners[i].z = 0;
        }
	}
    return corners;
}

GLfloat getHeightInPoint(GLfloat x,GLfloat z){

    GLfloat height;
    Point3D *quadCorners;
    quadCorners = getQuad(x,z);

    Point3D diag,pVector,crossResult;

    Point3D currPoint = {.x = x, .y=0, .z=z};

    VectorSub(&quadCorners[2],&quadCorners[0],&diag);
    VectorSub(&currPoint,&quadCorners[0],&pVector);


    CrossProduct(&diag, &pVector, &crossResult);
    if(crossResult.y > 0){
        height = (quadCorners[0].y + quadCorners[1].y + quadCorners[2].y) / 3;
    }else{
        height = (quadCorners[0].y + quadCorners[3].y + quadCorners[2].y) / 3;
    }

    return height;
}


void initWorldgen(int width, int height){
	heightmap = malloc(sizeof(GLfloat)*width*height);
	int x, z;
	for(x=0; x < width; x++){
	  for(z=0; z < height; z++){
              heightmap[x*width+z] = 0;
	   }
	}
}

double random_number(double range){
	return (((double)rand() / (double)RAND_MAX) * range);
}


struct square* make_square(int x, int z, int w, int h){

	struct square* s = malloc(sizeof(struct square));
	s->x = x;
	s->z = z;
	s->w = w;
	s->h = h;
	s->next = NULL;

	return s;
}

void s_stack_push(struct square *s){
	s->next = s_stack;
	s_stack = s;
}

struct square* s_stack_pop(){

	struct square *ss = s_stack;

	if(s_stack != NULL){
		s_stack = s_stack->next;
	}

	return ss;
}

double mean_corners(struct square* s){
	double sum	= heightmap[s->x*worldWidth+s->z]+
	heightmap[(s->x+s->w)*worldWidth+s->z] +
	heightmap[(s->x+s->w)*worldWidth+(s->z+s->h)] +
	heightmap[s->x*worldWidth+(s->z+s->h)];

	return sum/4.0;
}

int get_index(int x, int z){
return (x*worldWidth+z);
}

int get_midpoint(struct square* s){
	return get_index(floor((s->x+s->w)/2),floor((s->z + s->h)/2));
}

double square_area(struct square* s){
    return (double)(s->w * s->h);
}

bool index_exists(int x, int z){
    if (x >= 0 && x < worldHeight && z >= 0 && z < worldHeight){
        return true;
}else {
    return false;
    }
}

void displace_square_midpoint(struct square* s){
    double range = 0.00001;

    heightmap[get_midpoint(s)] = random_number(range) + mean_corners(s);
    double average;
    int divFactor = 2;

    average = (heightmap[get_index(s->x, s->z)] + heightmap[get_index(s->x,s->z+s->w)]) ;
    divFactor = 3;
    if (index_exists(s->x-(s->h/2), s->z +(s->w)/2)){
        average = average + heightmap[get_index(s->x-(s->h/2), s->z +(s->w)/2)];
        divFactor = 4;
    }
    average = random_number(range) + (average + heightmap[get_midpoint(s)])/divFactor;

    heightmap[get_index(s->x, s->z+(s->w)/2)] = random_number(range) + average;

    average = (heightmap[get_index(s->x, s->z+s->w)] + heightmap[get_index(s->x+s->h,s->z+s->w)]) / 2;
    heightmap[get_index(s->x+(s->h)/2, s->z+(s->w))] = random_number(range) + average;

    average = (heightmap[get_index(s->x+s->h, s->z+s->w)] + heightmap[get_index(s->x+s->h,s->z)]) / 2;
    heightmap[get_index(s->x+s->h, s->z+(s->w)/2)] = random_number(range) + average;

    average = (heightmap[get_index(s->x, s->z)] + heightmap[get_index(s->x+s->h,s->z)]) / 2;
    heightmap[get_index(s->x+(s->h)/2, s->z)] = random_number(range) + average;


}

void midplace_generate(int width, int height){
	initWorldgen(width,height);

	struct square *current = NULL;

	s_stack = make_square(0,0,width,height);
    double range = 50.0;
	heightmap[get_index(s_stack->x, s_stack->z)] = random_number(range);
    heightmap[get_index((s_stack->x+s_stack->w), s_stack->z)] = random_number(range);
    heightmap[get_index(s_stack->x+s_stack->w, s_stack->z+s_stack->h)] = random_number(range);
    heightmap[get_index(s_stack->x, s_stack->z+s_stack->h)] = random_number(range);
    heightmap[get_midpoint(s_stack)] = random_number(range) + mean_corners(s_stack);



	while(current = s_stack_pop()){
		//do stuff with current
		displace_square_midpoint(current);
		int nW = (current->w/2);
		int nH = (current->h/2);
        if (current->w >= 1 && current->h >= 1){
            s_stack_push(make_square(current->x,current->z,nW,nH)); //1
            s_stack_push(make_square(current->x+nW,current->z,nW,nH)); //2
            s_stack_push(make_square(current->x+nW,current->z+nH,nW,nH)); //3
            s_stack_push(make_square(current->x,current->z+nH,nW,nH)); //4
        }
	}
}

void generate_world(int width, int height){
#define NUM_ITERS 750

  initWorldgen(width,height);
  int i;
  double displacement = 5.0;
  for(i=0; i < NUM_ITERS; i++){
    displace_terrain(width,height,displacement);
    displacement = displacement - displacement * 1.0/NUM_ITERS;
  }
}

GLfloat get_height(int x, int z, int width, int height){
   if(x < width && z < height){
      //printf("now at: %f \n",heightmap[x*width + z]);
      return heightmap[x*width + z];
   }else{
      printf("invalid coordinate for worldgen **** you!\n");
      return -100000000;
   }
}

void displace_terrain(int width, int height,double displacement){


	double v = rand();
	double a = sin(v);
	double b = cos(v);
	double d = sqrt(pow(width,2) + pow(height,2));
	// rand() / RAND_MAX gives a random number between 0 and 1.
	// therefore c will be a random number between -d/2 and d/2
	double c_random = ((double)rand() / (double)RAND_MAX);
	double c = c_random * d - d/2;
	int x,z;
	double random_displace = displacement * ((double)rand() / (double)RAND_MAX);
	double distance;

	for(x=0; x < width; x++){
		for(z=0; z < height; z++){
		  random_displace = 0.05 * ((double)rand() / (double)RAND_MAX);
			distance = abs(a*x + b*z + c) / sqrt(pow(a,2) + pow(b,2));
			if(a*x + b*z - c > 0){
				heightmap[x*width+z] = heightmap[x*width+z] + displacement*sin(3.14/2.0 * (distance/d)) + (distance/d)*random_displace;
			}else{
				heightmap[x*width+z] = heightmap[x*width+z] - displacement*sin(3.14/2.0 * (distance/d)) - (distance/d)*random_displace;
			}
		}
	}
}

Model* GenerateTerrain(){

	worldWidth = 512;
	worldHeight = 512;


	int vertexCount = worldWidth * worldHeight;
	int triangleCount = (worldWidth-1) * (worldHeight-1) * 2;
	int x, z;

	srand(1124132);


	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	GLfloat Xcoord,Ycoord,Zcoord,X2coord,Y2coord,Z2coord,X3coord,Y3coord,Z3coord;

	//generate_world(worldWidth,worldHeight);
	midplace_generate(worldWidth, worldHeight);


	for (x = 0; x < worldWidth; x++)
		for (z = 0; z < worldHeight; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * worldWidth)*3 + 0] = x / 1.0;
			vertexArray[(x + z * worldWidth)*3 + 1] = get_height(x,z,worldWidth,worldHeight);
			vertexArray[(x + z * worldWidth)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.

            if(x > 0 && z > 0) {
                Xcoord = vertexArray[(x + z * worldWidth)*3 + 0];
                Ycoord = vertexArray[(x + z * worldWidth)*3 + 1];
                Zcoord = vertexArray[(x + z * worldWidth)*3 + 2];

                X2coord = vertexArray[((x - 1)+ z * worldWidth)*3 + 0];
                Y2coord = vertexArray[((x - 1)+ z * worldWidth)*3 + 1];
                Z2coord = vertexArray[((x - 1)+ z * worldWidth)*3 + 2];

                X3coord = vertexArray[((x + 1)+ z * worldWidth)*3 + 0];
                Y3coord = vertexArray[((x + 1)+ z * worldWidth)*3 + 1];
                Z3coord = vertexArray[((x + 1)+ z * worldWidth)*3 + 2];

                Point3D v1 = {.x=Xcoord - X2coord, .y=Ycoord - Y2coord, .z=Zcoord - Z2coord};
                Point3D v2 = {.x=X2coord - X3coord, .y=Y2coord - Y3coord, .z=Z2coord - Z3coord};
                Point3D normalVector;

                CrossProduct(&v2,&v1,&normalVector);

                normalArray[(x + z * worldWidth)*3 + 0] = normalVector.x;
                normalArray[(x + z * worldWidth)*3 + 1] = normalVector.y;
                normalArray[(x + z * worldWidth)*3 + 2] = normalVector.z;

            }else{
                normalArray[(x + z * worldWidth)*3 + 0] = 0.0;
                normalArray[(x + z * worldWidth)*3 + 1] = 1.0;
                normalArray[(x + z * worldWidth)*3 + 2] = 0.0;
            }

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * worldWidth)*2 + 0] =  (float)x / worldWidth;
			texCoordArray[(x + z * worldWidth)*2 + 1] =  (float)z / worldHeight;
		}

	for (x = 0; x < worldWidth-1; x++)
		for (z = 0; z < worldHeight-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (worldWidth-1))*6 + 0] = x + z * worldWidth;
			indexArray[(x + z * (worldWidth-1))*6 + 1] = x + (z+1) * worldWidth;
			indexArray[(x + z * (worldWidth-1))*6 + 2] = x+1 + z * worldWidth;
		// Triangle 2
			indexArray[(x + z * (worldWidth-1))*6 + 3] = x+1 + z * worldWidth;
			indexArray[(x + z * (worldWidth-1))*6 + 4] = x + (z+1) * worldWidth;
			indexArray[(x + z * (worldWidth-1))*6 + 5] = x+1 + (z+1) * worldWidth;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}
