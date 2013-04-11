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

    //printf("first corner in:(%f,%f,%f)\n",corners[0].x,corners[0].y,corners[0].z);
	}else{
	    //printf("worldX: %i, worldZ: %i, you outside!\n",worldHeight,worldWidth);

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

    //free(quadCorners);
    return height;
}

Model* GenerateTerrain(TextureData *tex){

	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	worldWidth = tex->width;
	worldHeight = tex->height;

	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	GLfloat Xcoord,Ycoord,Zcoord,X2coord,Y2coord,Z2coord,X3coord,Y3coord,Z3coord;

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.

            if(x > 0 && z > 0) {
                Xcoord = vertexArray[(x + z * tex->width)*3 + 0];
                Ycoord = vertexArray[(x + z * tex->width)*3 + 1];
                Zcoord = vertexArray[(x + z * tex->width)*3 + 2];

                X2coord = vertexArray[((x - 1)+ z * tex->width)*3 + 0];
                Y2coord = vertexArray[((x - 1)+ z * tex->width)*3 + 1];
                Z2coord = vertexArray[((x - 1)+ z * tex->width)*3 + 2];

                X3coord = vertexArray[((x + 1)+ z * tex->width)*3 + 0];
                Y3coord = vertexArray[((x + 1)+ z * tex->width)*3 + 1];
                Z3coord = vertexArray[((x + 1)+ z * tex->width)*3 + 2];

                Point3D v1 = {.x=Xcoord - X2coord, .y=Ycoord - Y2coord, .z=Zcoord - Z2coord};
                Point3D v2 = {.x=X2coord - X3coord, .y=Y2coord - Y3coord, .z=Z2coord - Z3coord};
                Point3D normalVector;

                CrossProduct(&v2,&v1,&normalVector);

                normalArray[(x + z * tex->width)*3 + 0] = normalVector.x;
                normalArray[(x + z * tex->width)*3 + 1] = normalVector.y;
                normalArray[(x + z * tex->width)*3 + 2] = normalVector.z;

            }else{
                normalArray[(x + z * tex->width)*3 + 0] = 0.0;
                normalArray[(x + z * tex->width)*3 + 1] = 1.0;
                normalArray[(x + z * tex->width)*3 + 2] = 0.0;
            }

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] =  (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] =  (float)z / tex->height;
		}

	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
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
