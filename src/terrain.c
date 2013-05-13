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
	heightmap = malloc(sizeof(GLfloat)*(width)*(height));
	int x, z;
	for(x=0; x < width; x++){
	  for(z=0; z < height; z++){
              heightmap[x*width+z] = 0;
	   }
	}
}

double random_number(double range){
	return (((double)rand() / (double)RAND_MAX) * range) - range/2;
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
	return get_index(((s->x+s->w)/2),((s->z + s->h)/2));
}

double square_area(struct square* s){
    return (double)(s->w * s->h);
}

bool index_exists(int x, int z){
    if ((x >= 0 && x < worldHeight) && (z >= 0 && z < worldHeight)){
        return true;
}else {
    return false;
    }
}

void set_square(int x, int z, int width, double range){
        double average = 0.0;
        double divFactor = 0.0;

        if (index_exists(x+width/2, z)){
            average = average + heightmap[get_index(x+width/2, z)];
            divFactor = divFactor + 1.0;

        }
        if(index_exists(x,z+width/2)){
            average = average + heightmap[get_index(x,z+width/2)];
            divFactor = divFactor + 1.0;

        }
        if(index_exists(x-width/2, z)){
            average = average + heightmap[get_index(x-width/2, z)];
            divFactor = divFactor + 1.0;

        }
        if(index_exists(x, z-width/2)){
            average = average + heightmap[get_index(x, z-width/2)];
            divFactor = divFactor + 1.0;

        }
            average = average/divFactor;
            heightmap[get_index(x,z)] = average + random_number(range);
}


void square_step(int width, double range){
        int x = 0;
        int z = 0;

        for (x=width/2; x < worldWidth-width/2; x = x+width){
            for(z = 0; z < worldHeight; z = z+width){
                set_square(x,z,width, range);
            }
        }
        for (x=0; x < worldWidth; x = x+width){
            for(z = width/2; z < worldHeight-width/2; z = z+width){
                set_square(x,z,width, range);
            }
        }
}

void diamond_step(int width, double range){
        int x = 0;
        int z = 0;

        double corners_mean = 0;

        for (x=width/2; x <= worldWidth-width/2; x = x+width){
            for(z = width/2; z <= worldHeight-width/2; z = z+width){

                corners_mean = heightmap[get_index(x-width/2,z-width/2)]+heightmap[get_index(x-width/2,z+width/2)] + heightmap[get_index(x+width/2,z-width/2)]+heightmap[get_index(x+width/2,z+width/2)];
                heightmap[get_index(x,z)] = corners_mean/4+random_number(range);
            }
        }
}


void midplace_generate(int width, int height){
	initWorldgen(width,height);
    double range = ceil(width/1.5);
    Ymin = -range/2;
    Ymax = range/2;

	heightmap[get_index(0, 0)] =  random_number(range);
    heightmap[get_index(width-1, 0)] = random_number(range);
    heightmap[get_index(width-1, height-1)] = random_number(range);
    heightmap[get_index(0,height-1)] = random_number(range);

    double iterRange = width/2;
    int w;

    int c=0;
    for(w = (width-1); w > 1; w = w/2){
        diamond_step(w,iterRange);
        square_step(w,iterRange);
        iterRange = iterRange/2;
    }
    post_process_terrain();
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

void initColormap(void){
    colormap = malloc(worldWidth*worldHeight*3*sizeof(GLubyte));
    int i;


    for(i=0; i < worldWidth*worldHeight*3; i++){
        colormap[i] = 100;
    }
}

TextureData generateColormap(void){
    initColormap();
    int x,z;
    float height;

    printf("sand level %f \n grasslevel %f \n snowlevel %f \n",sandLevel, grassLevel, snowLevel);

    for(x=0; x < worldWidth-1; x++){
        for(z=0; z < worldHeight-1; z++){
            height = heightmap[get_index(x,z)];

            if(height <= waterLevel){
                colormap[(z*(worldWidth-1)+x)*3 + 0] = 84;
                colormap[(z*(worldWidth-1)+x)*3 + 1] = 125;
                colormap[(z*(worldWidth-1)+x)*3 + 2] = 239+random_number(20);
            }else if(height < sandLevel){
                colormap[(z*(worldWidth-1)+x)*3 + 0] = 249;
                colormap[(z*(worldWidth-1)+x)*3 + 1] = 249;
                colormap[(z*(worldWidth-1)+x)*3 + 2] = 109;
            }else if(height < grassLevel){
                colormap[(z*(worldWidth-1)+x)*3 + 0] = 0;
                colormap[(z*(worldWidth-1)+x)*3 + 1] = 123;
                colormap[(z*(worldWidth-1)+x)*3 + 2] = 12;
            }else if(height < snowLevel){
                float r = random_number(15);
                colormap[(z*(worldWidth-1)+x)*3 + 0] = 128+r;
                colormap[(z*(worldWidth-1)+x)*3 + 1] = 128+r;
                colormap[(z*(worldWidth-1)+x)*3 + 2] = 128+r;

            }else{
                colormap[(z*(worldWidth-1)+x)*3 + 0] = 255;
                colormap[(z*(worldWidth-1)+x)*3 + 1] = 255;
                colormap[(z*(worldWidth-1)+x)*3 + 2] = 255;
            }
        }
    }




TextureData groundTexture;
groundTexture.imageData = colormap;
groundTexture.width = worldWidth-1;
groundTexture.height = worldHeight-1;
groundTexture.bpp = 24;



glGenTextures(1, &groundTexture.texID);			// Generate OpenGL texture IDs

glBindTexture(GL_TEXTURE_2D, groundTexture.texID);		// Bind Our Texture

glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, groundTexture.width-1, groundTexture.height-1, 0, GL_RGB, GL_UNSIGNED_BYTE, groundTexture.imageData);

SaveTGA(&groundTexture,"filnamn.tga");

return groundTexture;
}

Model* GenerateTerrain(){

    int worldSize = 1024*4;

	worldWidth = worldSize+1;
	worldHeight = worldSize+1;


	int vertexCount = worldWidth * worldHeight;
	int triangleCount = (worldWidth-1) * (worldHeight-1) * 2;
	int x, z;

	srand(time(0));

	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	GLfloat Xcoord,Ycoord,Zcoord,X2coord,Y2coord,Z2coord,X3coord,Y3coord,Z3coord;

	//generate_world(worldWidth,worldHeight);
	midplace_generate(worldWidth, worldHeight);
    //    initWorldgen(worldWidth,worldWidth);


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

    //generateColormap(void);

    //generate a terrain texture

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

void post_process_terrain(){
//iterate over heightmap
float h=0;
int x,z;

min_point = 1000000000;
max_point = 0;
    for(x=0; x < worldWidth; x++){
        for(z=0; z < worldWidth; z++){
            h = get_height(x,z,worldWidth,worldHeight);

            if(h < min_point){
                min_point = h;
            }

            if(h > max_point){
                max_point = h;
            }
        }
    }


    //smooth the world
    for(x=1; x < worldHeight-1; x++){
        for(z=1; z < worldHeight-1; z++){
            heightmap[get_index(x,z)] = (heightmap[get_index(x-1,z)] + heightmap[get_index(x+1,z)] + heightmap[get_index(x,z+1)]+heightmap[get_index(x,z-1)]+heightmap[get_index(x,z)])/5;
        }
    }

    printf("min is: %f \n", min_point);
    printf("max is: %f \n", max_point);

    waterLevel = 0;
    sandLevel = waterLevel + 4;
    grassLevel = max_point*0.4;
    snowLevel = max_point*0.8;
    //flatten stuff
    for(x=0; x < worldWidth; x++){
        for(z=0; z < worldWidth; z++){
            h = get_height(x,z,worldWidth,worldHeight);

            if(h <= waterLevel){
                heightmap[get_index(x,z)] = 0;
            }else if(h <= grassLevel){
                heightmap[get_index(x,z)] = heightmap[get_index(x,z)] * sin((heightmap[get_index(x,z)] / (grassLevel)) * 3.1451/2);
            }

        }
    }

}
