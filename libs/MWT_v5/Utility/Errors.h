#ifndef _ERRORS_H_
#define _ERRORS_H_

#define OUTPUT_ERROR 0

static enum ShapeType {
	ERROR_CURVE,
	ERROR_NORM,
	ERROR_MATCH,
	ERROR_LIMIT,
	ERROR_TETGEN,
	ERROR_OTHERS
};

static void errors(int type, char * filename){
	#if OUTPUT_ERROR
	switch(type){
	case ERROR_CURVE:
		std::cout<<"ERROR: Cannot open curve file!"<<std::endl; 
		break;
	case ERROR_NORM:
		std::cout<<"ERROR: Cannot open normal file!"<<std::endl; 
		break;
	case ERROR_MATCH:
		std::cout<<"ERROR: Number of points and number of normals are not match!"<<std::endl; 
		break;
	case ERROR_LIMIT:
		std::cout<<"ERROR: Number of points exceeds the limit!"<<std::endl; 
		break;
	case ERROR_TETGEN:
		std::cout<<"ERROR: Tetgen errors happen in edge protection!"<<std::endl; 
		break;
	case ERROR_OTHERS:
		std::cout<<"ERROR: Got exceptions in main function!"<<std::endl; 
		break;
	default:
		break;
	}
	std::cout<<"Curve: "<<filename<<std::endl; 
	#endif
	//exit(1);
}
#endif