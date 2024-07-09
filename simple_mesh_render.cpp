# include <GL/glut.h>
# include <GL/gl.h>

# include <iostream>
# include <math.h>
# include <ctime>
# include <cstdlib>
# include <fstream>
# include <sstream>
# include <string>
# include <random>
# include <vector>
# include <direct.h>

double const PI = acos( -1 ) ;
int const MAX_HEIGHT = 800 ;
int const MAX_WIDTH = 800 ;
float const INF = std::numeric_limits<double>::max() ;

using namespace std ;

class EdgeBucket {

	public :

		int max_y ;
		float min_x ;
		float slopeInverse ;

		EdgeBucket() ;

		EdgeBucket( int temp_y, float temp_x, float temp_slop ) ;

} ; // class EdgeBucket

class EdgeTuple {

	public :

		int howManyBucket ;
		vector<EdgeBucket> bucket ;

		EdgeTuple() ;

		void Sort() ;

} ; // class EdgeTuple

class Point {

	public :

		float x ;	
		float y ;
		float z ;
		float omega ;

		Point() ;

		Point( float x, float y ) ;

		Point( float x, float y, float z ) ;

		Point( float x, float y, float z, float omega ) ;

		virtual Point* Clone() ;

		Point* next ;

} ; // class Point

typedef Point* PointPtr ;

class Obj {

	public :

		string file_name ;

		vector<PointPtr> v ;
		vector<PointPtr> vt ;
		vector<PointPtr> vn ;

		vector<PointPtr> f ;

		Obj() ;

		void ReadObj( string file_name ) ;

		Obj* next ;

} ; // class Obj

typedef Obj* ObjPtr ;

class Matrix {

	public :

		float transform_matrix[4][4] ;

		Matrix() ;

		void CleanTransMatrix() ;

		void PrintTransMatrix() ;

		void MatrixMultiplx( float newTransMatrix[4][4], float oldTransMatrix[4][4] ) ;

} ; // class Matrix

Matrix global_matrix = Matrix() ;

class Operation {

	public :

		Operation() ;

		virtual PointPtr Operate( PointPtr head ) = 0 ;

		Operation* CleanOperator( Operation* ptr ) ;

		Operation* next ;

} ; // class Operator

typedef Operation* OperationPtr ;

class Translate : public Operation {

	public :

		float trans_x ;
		float trans_y ;
		float trans_z ;

		Translate() ;

		Translate( float input_x, float input_y, float input_z ) ;

		PointPtr Operate( PointPtr head ) ;

} ; // class Translate

class Scale : public Operation {

	public :

		float scale_x ;
		float scale_y ;
		float scale_z ;

		Scale() ;

		Scale( float input_x, float input_y, float input_z ) ;

		PointPtr Operate( PointPtr head ) ;

} ; // class Scale

class TriFunction {

	public :

		float r_sin ;
		float r_cos ;

		TriFunction() ;

		void CountTrigonometric( float input_theta ) ;

} ; // class TriFuction

class Rotate : public Operation {

	public :

		TriFunction rotate_x ;
		TriFunction rotate_y ;
		TriFunction rotate_z ;

		Rotate() ;

		Rotate( float input_x, float input_y, float input_z ) ;

		PointPtr Operate( PointPtr head ) ;

} ; // class Rotate

class Color {

	public :

		float r ;
		float g ;
		float b ;

		Color() ;

		Color( float r, float g, float b ) ;

} ; // class Color

class Graphic {

	public:

		int obj_label ;
		bool visible = true ;

		PointPtr point ;

		Point normal_vec ;

		Color color ;

		float coeef[3] = { 0.0 } ; // Kd, Ks, N

		void AssignColor( float r, float g, float b ) ;

		void AssignCoeef( float Kd, float Ks, float N ) ;

		void AssignLabel( int label ) ;

		PointPtr DeletePoint( PointPtr ptr ) ;

		virtual void DrawGraph() = 0 ;

		virtual Graphic* Clone() = 0 ;

		virtual PointPtr Clip( PointPtr head, PointPtr graph_head, PointPtr graph_walk, int edgeCode ) = 0 ;

		virtual void FindNormalVec() = 0 ;

		virtual Point FindCenter() = 0 ;

		virtual float FindZ( Point target ) = 0 ;

} ; // class Graphic

typedef Graphic* GraphicPtr ;

class My_Polygon : public Graphic {

	private :

		int index ;

	public :

		My_Polygon() ;

		void DrawGraph() override ;

		virtual My_Polygon* Clone() override ;

		PointPtr Clip( PointPtr head, PointPtr graph_head, PointPtr graph_walk, int edgeCode ) override ;

		void FindNormalVec() override ;

		Point FindCenter() override ;

		float FindZ( Point target ) override ;

		void ClockWise() ;

} ; // My_Polygon

class Z_Buffer {

	public :

		float z_value ;

		Color color ;

		Z_Buffer( Color color ) ;

} ; // class Z_Buffer

class DrawGraphAlg {

	private :

		static DrawGraphAlg *this_ ;

		EdgeTuple edgeTable[MAX_HEIGHT] ;
		EdgeTuple activeEdge ;

		int a ;
		int b ;
		int c ;

	public:

		DrawGraphAlg() ;

		static void DrawDot( int x, int y ) ;

		static void DrawDot( int x, int y, Color color ) ;

		void Draw8SectorPoint( Point center, Point p ) ;

		static void MidLineDrawingAlg( PointPtr p1, PointPtr p2, bool store ) ;

		static void MidCircleDrawingAlg( Point center, int radius ) ;

		void ClearPoint( PointPtr head ) ;

		static void ClearEdgeTable() ;

		void DeleteEdge( EdgeTuple* edge, int delete_y ) ;

		void UpdateActiveMin_X( EdgeTuple* edge ) ;

		void StoreEdgeInBucket( EdgeTuple* edge, int max_y, int min_x, float slopeInverse ) ;

		void StoreEdgeInTable( Point p1, Point p2 ) ;

		static void StorePolygonEdge( PointPtr head ) ;

		static void DrawColorAlg( GraphicPtr graph, vector<vector<Z_Buffer>> &z_buffer ) ;

} ; // class DrawGraphAlg

DrawGraphAlg *DrawGraphAlg::this_ = NULL ;
DrawGraphAlg dga = DrawGraphAlg() ;

class Camera {

	public :

		Point position ;
		Point COI ;

		float tilt ;
		float hither_plane ;
		float yon_plane ;
		float field_of_view ;
		float tan_hov ;

		float v1[3] = {0} ;
		float v2[3] = {0} ;
		float v3[3] = {0} ;

		float length_v1 ;
		float length_v2 ;
		float length_v3 ;

		Camera() ;

		Camera( Point position, Point COI, 
				float tilt, float hither_plane, float yon_plane, float field_of_view ) ;

		void ComputeGRMVector() ;

} ; // class Camera

typedef Camera* CameraPtr ;

class Viewport {

	public :

		Point left_bottom ;
		Point right_top ;

		Viewport() ;

		Viewport( Point l_b, Point r_t ) ;

} ; // class ViewPort

typedef Viewport* ViewportPtr ;

class Light {

	public :

		int index ;

		Color color ;

		Point light_position ;

		Light() ;

		Light( int index, float x, float y, float z ) ;

		void AssignColor( float r, float g, float b ) ;

		Light* next ;

} ; // class

typedef Light* LightPtr ;

class LightingModel {

	private :

		Color shading_color ;
		static LightingModel* this_ ;

	public :

		LightingModel() ;

		static Point GenVector( Point p1, Point p2 ) ;

		static Point AddVector( Point p1, Point p2 ) ;

		void Ambient( GraphicPtr graph, float ambient[3] ) ;

		void Diffuse( GraphicPtr graph, LightPtr light ) ;

		void Specular( GraphicPtr graph, LightPtr light, CameraPtr camera ) ;

		static Color LightModelCal( GraphicPtr graph, float ambient[3], LightPtr light, CameraPtr camera ) ;

		static void ClearShadingColor() ;

} ; // class

LightingModel* LightingModel::this_ = NULL ;
LightingModel LM = LightingModel() ;

class GlutWindow {

	private:

		static GlutWindow *this_ ;

		bool finADraw ;
		unsigned char drawMode ;

		vector<GraphicPtr> graph_mem ;

	public:

		float temp_color[3] = { 0.0 } ;
		float ambient_coeff[3] = { 0.0 } ;

		float width ;
		float height ;
		float aspect_ratio ;

		Color background ;

		ObjPtr obj_files ;

		GraphicPtr temp_graphic ;

		OperationPtr operator_head ;
		OperationPtr operator_tail ;

		CameraPtr camera ;
		ViewportPtr viewport ;
		LightPtr light ;

		GlutWindow() ;

		GlutWindow( float width, float height ) ;

		void CleanPointer() ;

		void SetNewColor() ;

		void ClearScreen() ;

		void ClearData() ;

		void CloneGraph( GraphicPtr graph ) ;

		void Triangle() ;

		void Square() ;

		void Polygon( PointPtr pic ) ;

		void DrawWindow( Point left_top, Point right_bottom ) ;

		ObjPtr SearchObj( string file_name ) ;

		LightPtr InsertLight( LightPtr walk, LightPtr target ) ;

		void Illuminate( GraphicPtr graph ) ;

		void TransToEyeSpace( GraphicPtr graph ) ;

		void TransToProjectionSpace( GraphicPtr graph ) ;

		void PerspectiveDivide( GraphicPtr graph, Point LB, Point RT ) ;

		bool IsPointInGraph( GraphicPtr graph, Point test_point ) ;

		float FindZ( GraphicPtr graph, Point point ) ;

		void DoCommand( ifstream &file, string command, int obj_label ) ;

} ; // class GlutWindow

GlutWindow *GlutWindow::this_ = NULL ;

int window ;
void display() ;
float Random021() ;

vector<string> Split( string target, char deli ) ;

int main( int argc, char *argv[] ) {

	if ( argc == 0 )
		exit( 0 ) ;

	string command = "\0" ;
	ifstream file ;

	int obj_label = 0 ;
	int start = 0 ;
	int end = 0 ;

	if ( argc == 2 ) {

		cout << argv[1] << endl ;
		file.open( argv[1] ) ;

	} // if

	glutInit( &argc, argv ) ;
	system( "PAUSE" ) ;

	srand( ( unsigned int )time( NULL ) ) ;

	file >> command ;
	char* width = const_cast<char*>( command.c_str() ) ;

	file >> command ;
	char* height = const_cast<char*>( command.c_str() ) ;

	GlutWindow gWindow = GlutWindow( atof( width ), atof( height ) ) ;

	start = time( 0 ) ;
 
	while ( true ) {

		file >> command ;

		if ( command[0] != '#' )
			cout << command ;

		if ( command == "object" )
			obj_label++ ;

		gWindow.DoCommand( file, command, obj_label ) ;

		//glutMainLoop() ;

		if ( command == "display" ) {

			end = time( 0 ) ;

			cout << "########## Using Time ##########" << endl
 				 << "##########	" << end - start << "	##########" << endl
				 << "################################" << endl ;

			system( "PAUSE" ) ;

			start = time( 0 ) ;

		} // if

	} // while

} // main()

void display() {

	// glClearColor( 0.0, 0.0, 0.0, 0.0 ) ;
	// glClear( GL_COLOR_BUFFER_BIT ) ;  

	glFlush() ;

} // display()

float Random021() {

	return rand() / float( RAND_MAX ) ;

} // Random021()

vector<string> Split( string target, char deli ) {

	vector<string> split ;

	stringstream stream_target( target ) ;
	string temp = "\0" ;

	while ( getline( stream_target, temp, deli ) )
		split.push_back( temp ) ;

	if ( split.size() == 2 )
		split.push_back( "\0" ) ;

	return split ;

} // Split()

Color::Color() {

	r = 0 ;
	g = 0 ;
	b = 0 ;

} // Color::Color()

Color::Color( float r, float g, float b ) {

	this->r = r ;
	this->g = g ; 
	this->b = b	;

} // Color::Color()

void Graphic::AssignColor( float r, float g, float b ) {

	color.r = r ;
	color.g = g ;
	color.b = b ;

} // Graphic::AssignColor()

void Graphic::AssignCoeef( float Kd, float Ks, float N ) {

	coeef[0] = Kd ;
	coeef[1] = Ks ;
	coeef[2] = N ;

} // Graphic::AssignCoeef()

void Graphic::AssignLabel( int label ) {

	this->obj_label = label ;

} // Graphic::AssignLabel

PointPtr Graphic::DeletePoint( PointPtr ptr ) {

	if ( ptr != NULL ) {

		DeletePoint( ptr->next ) ;
		delete( ptr ) ;
		ptr = NULL ;

	} // if

	return NULL ;

} // Graphic::DeletePoint( PointPtr ptr )

Obj::Obj() {

	file_name.clear() ;

	v.clear() ;
	vt.clear() ;
	vn.clear() ;
	f.clear() ;

	next = NULL ;

} // Obj::Obj()

void Obj::ReadObj( string file_name ) {

	this->file_name = file_name ;

	vector<string> f_temp1 ;
	vector<string> f_temp2 ;
	vector<string> f_temp3 ;
	vector<string> f_temp4 ;

	string temp_string = "\0" ;
	string pre_string = "\0" ;
	char skipLine[100] ;

	ifstream obj_file ;

	PointPtr temp_point = NULL ;
	PointPtr temp_walk = NULL ;

	_getcwd( skipLine, 100 ) ;

	//cout << skipLine ;

	string cwd( skipLine ) ;

	obj_file.open( cwd + "\\Mesh\\" + file_name ) ;

	while ( !obj_file.eof() ) {

		if ( pre_string == "\0" )
			obj_file >> temp_string ;

		else {

			temp_string = pre_string ;
			pre_string = "\0" ;

		} // else

		if ( temp_string[0] == '#' )
			obj_file.getline( skipLine, sizeof( skipLine ), '\n' ) ;

		else if ( temp_string == "v" ) {

			string postision[3] ;

			obj_file >> postision[0] >> postision[1] >> postision[2] ;

			char *x_char = const_cast<char*>( postision[0].c_str() ) ;
			char *y_char = const_cast<char*>( postision[1].c_str() ) ;
			char *z_char = const_cast<char*>( postision[2].c_str() ) ;

			temp_point = new Point( atof( x_char ), atof( y_char ), atof( z_char ) ) ;

			v.push_back( temp_point ) ;

			temp_point = NULL ;

		} // else if


		else if ( temp_string == "vt" ) {



		} // else if 

		else if ( temp_string == "vn" ) {



		} // else if

		else if ( temp_string == "f" ) {

			string face_index = "\0" ;

			obj_file >> temp_string ;
			f_temp1 = Split( temp_string, '/' ) ;
			char *v1_index = const_cast<char*>( f_temp1[0].c_str() ) ;

			obj_file >> temp_string ;
			f_temp2 = Split( temp_string, '/' ) ;
			char *v2_index = const_cast<char*>( f_temp2[0].c_str() ) ;

			obj_file >> temp_string ;
			f_temp3 = Split( temp_string, '/' ) ;
			char *v3_index = const_cast<char*>( f_temp3[0].c_str() ) ;

			temp_point = v[atoi( v1_index ) - 1]->Clone() ;
			temp_walk = temp_point ;

			temp_walk->next = v[atoi( v2_index ) - 1]->Clone() ;
			temp_walk = temp_walk->next ;

			temp_walk->next = v[atoi( v3_index ) - 1]->Clone() ;
			temp_walk = temp_walk->next ;

			f.push_back( temp_point ) ;

			obj_file >> temp_string ;
			f_temp4 = Split( temp_string, '/' ) ;
			char *v4_index = const_cast<char*>( f_temp4[0].c_str() ) ;

			if ( obj_file.eof() )
				break ;

			if ( f_temp4[0] != "0.0" && f_temp4[0] != "0" && f_temp4[0] != ".0" && atof( v4_index ) == 0 )
				pre_string = temp_string ;

			//else {

			//	temp_walk->next = v[atoi( v4_index ) - 1]->Clone() ;
			//	temp_walk = temp_walk->next ;

			//} // else

			//f.push_back( temp_point ) ;

			else {

				temp_point = v[atoi( v3_index ) - 1]->Clone() ;
				temp_walk = temp_point ;

				temp_walk->next = v[atoi( v4_index ) - 1]->Clone() ;
				temp_walk = temp_walk->next ;

				temp_walk->next = v[atoi( v1_index ) - 1]->Clone() ;
				temp_walk = temp_walk->next ;

				f.push_back( temp_point ) ;

			} // else

		} // else if

		else
			obj_file.getline( skipLine, sizeof( skipLine ), '\n' ) ;

	} // while

} // Obj::ReadObj()

Matrix::Matrix() {

	CleanTransMatrix() ;

} // Matrix::Matrix()

void Matrix::CleanTransMatrix() {

	for ( int i = 0 ; i < 4 ; i++ )
		for ( int j = 0 ; j < 4 ; j++ )
			transform_matrix[i][j] = 0.0 ;

} // Operation::CleanTransMatrix()

void Matrix::PrintTransMatrix() {

	for ( int i = 0 ; i < 4 ; i++ ) {

		cout << transform_matrix[i][0] ;

		for ( int j = 1 ; j < 4 ; j++ )
			cout << ", " << transform_matrix[i][j] ;

		cout << endl ;

	} // for

	cout << endl ;

} // Operation::PrintTransMatrix()

void Matrix::MatrixMultiplx( float newTransMatrix[4][4], float oldTransMatrix[4][4] ) {

	float temp = 0.0 ;
	bool zero = true ;

	for ( int row = 0 ; row < 4 ; row++ ) {

		for ( int col = 0 ; col < 4 ; col++ ) {

			for ( int k = 0 ; k < 4 ; k++ )
				temp += newTransMatrix[row][k] * oldTransMatrix[k][col] ;

			transform_matrix[row][col] = temp ;
			temp = 0 ;

		} // for

	} // for

	for ( int i = 0 ; i < 4 ; i++ ) {

		for ( int j = 0 ; j < 4 ; j++ ) {

			if ( transform_matrix[i][j] != 0 ) {

				zero = false ;
				break ;

			} // if

		} // for

	} // for

	if ( zero )
		memcpy( transform_matrix, newTransMatrix, sizeof( transform_matrix ) ) ;

} // Matrix::MatrixMultiplx()

Operation::Operation() {

	next = NULL ;

} // Operate::Operate()

Operation* Operation::CleanOperator( Operation* ptr ) {

	Operation* walk = ptr ;

	if ( walk != NULL ) {

		CleanOperator( walk->next ) ;
		delete( walk ) ;
		walk = NULL ;

	} // if

	return walk ;

} // Operation::CleanOperator()

Translate::Translate() {

	trans_x = 0.0 ;
	trans_y = 0.0 ;
	trans_z = 0.0 ;

} // Translate::Translate()

Translate::Translate( float input_x, float input_y, float input_z ) {

	//float temp_m[4][4] = {0.0} ;
	//float old_m[4][4] = {0.0} ;

	//memcpy( old_m, global_matrix.transform_matrix, sizeof( old_m ) ) ;

	trans_x = input_x ;
	trans_y = input_y ;
	trans_z = input_z ;

	//for ( int i = 0 ; i < 4 ; i++ )
	//	temp_m[i][i] = 1.0 ;

	//temp_m[0][3] = trans_x ;
	//temp_m[1][3] = trans_y ;

	//global_matrix.MatrixMultiplx( temp_m, old_m ) ;
	//global_matrix.PrintTransMatrix() ;

} // Translate::Translate()

PointPtr Translate::Operate( PointPtr head ) {

	PointPtr walk = head ;

	if ( walk == NULL )
		return NULL ;

	walk->x += trans_x ;
	walk->y += trans_y ;
	walk->z += trans_z ;

	walk->next = Operate( walk->next ) ;

	return walk ;

} // Translate::Operate()

Scale::Scale() {

	scale_x = 1.0 ;
	scale_y = 1.0 ;
	scale_z = 1.0 ;

} // Scale::Scale()

Scale::Scale( float input_x, float input_y, float input_z ) {

	//float temp_m[4][4] = {0.0} ;
	//float old_m[4][4] = {0.0} ;

	//memcpy( old_m, global_matrix.transform_matrix, sizeof( old_m ) ) ;

	scale_x = input_x ;
	scale_y = input_y ;
	scale_z = input_z ;

	//temp_m[0][0] = scale_x ;
	//temp_m[1][1] = scale_y ;
	//temp_m[3][3] = 1 ;

	//global_matrix.MatrixMultiplx( temp_m, old_m ) ;
	//global_matrix.PrintTransMatrix() ;

} // Scale::Scale()

PointPtr Scale::Operate( PointPtr head ) {

	PointPtr walk = head ;

	if ( walk == NULL )
		return NULL ;

	walk->x *= scale_x ;
	walk->y *= scale_y ;
	walk->z *= scale_z ;

	walk->next = Operate( walk->next ) ;

	return walk ;

} // Scale::Operate()

TriFunction::TriFunction() {

	r_sin = 0.0 ;
	r_cos = 0.0 ;

} // TriFunction::Trifunction()

void TriFunction::CountTrigonometric( float input_theta ) {

	float theta = 0.0 ;

	if ( input_theta >= 360 )
		theta = ( int )input_theta % 360 ;

	else
		theta = ( float )input_theta ;

	theta *= PI / 180 ;

	if ( theta == 0 ) {

		r_sin = 0 ;
		r_cos = 1 ;

	} // if

	else if ( theta == 90 ) {

		r_sin = 1 ;
		r_cos = 0 ;

	} // else if

	else if ( theta == 180 ) {

		r_sin = 0 ;
		r_cos = -1 ;

	} // else if

	else if ( theta == 270 ) {

		r_sin = -1 ;
		r_cos = 0 ;

	} // else if

	else {

		r_sin = sin( theta ) ;
		r_cos = cos( theta ) ;

	} // else

} // TriFUnction::CountTrugonometric()

Rotate::Rotate() {

	rotate_x = TriFunction() ;
	rotate_y = TriFunction() ;
	rotate_z = TriFunction() ;

} // Rotate::Rotate()

Rotate::Rotate( float input_x, float input_y, float input_z ) {

	//float temp_m[4][4] = {0.0} ;
	//float old_m[4][4] = {0.0} ;

	//memcpy( old_m, global_matrix.transform_matrix, sizeof( old_m ) ) ;

	rotate_x.CountTrigonometric( input_x ) ;
	rotate_y.CountTrigonometric( input_y ) ;
	rotate_z.CountTrigonometric( input_z ) ;

	//temp_m[0][0] = r_cos ;
	//temp_m[0][1] = -r_sin ;
	//temp_m[1][0] = r_sin ;
	//temp_m[1][1] = r_cos ;
	//temp_m[2][2] = 1 ;
	//temp_m[3][3] = 1 ;

	//global_matrix.MatrixMultiplx( temp_m, old_m ) ;
	//global_matrix.PrintTransMatrix() ;

} // Rotate::Rotate()

PointPtr Rotate::Operate( PointPtr head ) {

	PointPtr walk = head ;
	Point temp = Point() ;

	if ( walk == NULL )
		return NULL ;

	// rotate y

	temp.x = walk->x * rotate_y.r_cos + walk->z * rotate_y.r_sin ;
	temp.z = -walk->x * rotate_y.r_sin + walk->z * rotate_y.r_cos ;

	walk->x = temp.x ;
	walk->z = temp.z ;

	// rotate z

	temp.x = walk->x * rotate_z.r_cos - walk->y * rotate_z.r_sin ;
	temp.y = walk->x * rotate_z.r_sin + walk->y * rotate_z.r_cos ;

	walk->x = temp.x ;
	walk->y = temp.y ;

	// rotate x

	temp.y = walk->y * rotate_x.r_cos - walk->z * rotate_x.r_sin ;
	temp.z = walk->z * rotate_x.r_sin + walk->z * rotate_x.r_cos ;

	walk->x = temp.x ;
	walk->y = temp.y ;

	walk->next = Operate( walk->next ) ;

	return walk ;

} // Rotate::Operate()

EdgeBucket::EdgeBucket() {

	max_y = 0 ;
	min_x = 0.0 ;
	slopeInverse = 0.0 ;

} // EdgeBucket::EdgeBucket()

EdgeBucket::EdgeBucket( int temp_y, float temp_x, float temp_slop ) {

	max_y = temp_y ;
	min_x = temp_x ;
	slopeInverse = temp_slop ;

} // EdgeBucket::EdgeBucket()

EdgeTuple::EdgeTuple() {

	howManyBucket = 0 ;
	bucket.clear() ;

} // EdgeTuple::EdgeTuple()

void EdgeTuple::Sort() {

	EdgeBucket temp = EdgeBucket() ;

	int j = 0 ;

	for ( int i = 1 ; i < howManyBucket ; i++ ) {

		temp.max_y = bucket[i].max_y ;
		temp.min_x = bucket[i].min_x ;
		temp.slopeInverse = bucket[i].slopeInverse ;

		j = i - 1 ;

		while ( j >= 0 && temp.min_x < bucket[j].min_x ) {

			bucket[j + 1].min_x = bucket[j].min_x ;
			bucket[j + 1].max_y = bucket[j].max_y ;
			bucket[j + 1].slopeInverse = bucket[j].slopeInverse ;

			j-- ;

		} // while

		bucket[j + 1].min_x = temp.min_x ;
		bucket[j + 1].max_y = temp.max_y ;
		bucket[j + 1].slopeInverse = temp.slopeInverse ;

	} // for

} // EdgeTuple::Sort()

Point::Point() {

	x = 0 ;
	y = 0 ;
	z = 0 ;
	omega = 1 ;

	next = NULL ;

} // Point::Point()

Point::Point( float x, float y ) {

	this->x = x ;
	this->y = y ;
	this->z = 0 ;
	this->omega = 1 ;

	next = NULL ;

} // Point::Point()

Point::Point( float x, float y, float z ) {

	this->x = x ;
	this->y = y ;
	this->z = z ;
	this->omega = 1 ;

	next = NULL ;

} // Point::Point()

Point::Point( float x, float y, float z, float omega ) {

	this->x = x ;
	this->y = y ;
	this->z = z ;
	this->omega = omega ;

	next = NULL ;

} // Point::Point()

Point* Point::Clone() {

	return new Point( *this ) ;

} // Point::Clone()

My_Polygon::My_Polygon() {

	index = 0 ;

	point = NULL ;
	normal_vec = Point() ;
	color = Color() ;

} // My_Polygon::My_Polygon()

void My_Polygon::DrawGraph() {

	glColor3f( color.r, color.g, color.b ) ;

	PointPtr walk = point ;

	while ( walk != NULL && walk->next != NULL ) {

		DrawGraphAlg::MidLineDrawingAlg( walk, walk->next, true ) ;

		walk = walk->next ;

	} // while

	  //DrawGraphAlg::DrawColorAlg() ;

} // My_Polygon::DrawGraph()

My_Polygon* My_Polygon::Clone() {

	My_Polygon* temp = new My_Polygon( *this ) ;
	PointPtr temp_point_walk = temp->point ;
	PointPtr walk = NULL ;

	while ( temp_point_walk != NULL ) {

		if ( walk == NULL ) {

			walk = temp_point_walk->Clone() ;
			temp->point = walk ;

		} // if

		else {

			walk->next = temp_point_walk->Clone() ;

			walk = walk->next ;

		} // else

		temp_point_walk = temp_point_walk->next ;

	} // while

	return temp ;

} // MyPolyGon::Clone()

PointPtr My_Polygon::Clip( PointPtr head, PointPtr graph_head, PointPtr graph_walk, int edgeCode ) {

	float c1 = 0 ;
	float c2 = 0 ;
	float t = 0 ;

	float intersection[4] = {0} ;

	PointPtr walk = head ;
	PointPtr g_walk = graph_walk ;

	if ( ( walk == NULL || walk->next == NULL ) && graph_head == NULL )
		return NULL ;

	if ( ( walk == NULL || walk->next == NULL ) ) {

		g_walk = new Point( graph_head->x, graph_head->y, graph_head->z, graph_head->omega ) ;
		return g_walk ;

	} // if

	else if ( edgeCode == 1 ) {

		c1 = walk->x + walk->omega ;
		c2 = walk->next->x + walk->next->omega ;

	} // if

	else if ( edgeCode == 2 ) {

		c1 = walk->y + walk->omega ;
		c2 = walk->next->y + walk->next->omega ;

	} // else if

	else if ( edgeCode == 3 ) {

		c1 = walk->omega - walk->x ;
		c2 = walk->next->omega - walk->next->x ;

	} // else if

	else if ( edgeCode == 4 ) {

		c1 = walk->omega - walk->y ;
		c2 = walk->next->omega - walk->next->y ;

	} // else if

	else if ( edgeCode == 5 ) {

		c1 = walk->z ;
		c2 = walk->next->z ;

	} // else if

	else if ( edgeCode == 6 ) {

		c1 = walk->omega - walk->z ;
		c2 = walk->next->omega - walk->next->z ;

	} // else if

	if ( c1 >= 0 && c2 >= 0 ) {

		if ( graph_head == NULL ) {

			graph_head = new Point( walk->x, walk->y, walk->z, walk->omega ) ;
			g_walk = graph_head ;

		} // if

		else
			g_walk = new Point( walk->x, walk->y, walk->z, walk->omega ) ;

		g_walk->next = Clip( walk->next, graph_head, g_walk->next, edgeCode ) ;

		return g_walk ;

	} // if keep the line

	else if ( c1 >= 0 && c2 < 0 ) {

		if ( graph_head == NULL ) {

			graph_head = new Point( walk->x, walk->y, walk->z, walk->omega ) ;
			g_walk = graph_head ;

		} // if

		else
			g_walk = new Point( walk->x, walk->y, walk->z, walk->omega ) ;

		t = c1 / ( c1 - c2 ) ;

		intersection[0] = walk->x + t * ( walk->next->x - walk->x ) ;
		intersection[1] = walk->y + t * ( walk->next->y - walk->y ) ;
		intersection[2] = walk->z + t * ( walk->next->z - walk->z ) ;
		intersection[3] = walk->omega + t * ( walk->next->omega - walk->omega ) ;

		g_walk->next = new Point( intersection[0], intersection[1], intersection[2], intersection[3] ) ;

		g_walk->next->next = Clip( walk->next, graph_head, g_walk->next->next, edgeCode ) ;

		return g_walk ;

	} // else if clip

	else if ( c1 < 0 && c2 >= 0 ) {

		t = c1 / ( c1 - c2 ) ;

		intersection[0] = walk->x + t * ( walk->next->x - walk->x ) ;
		intersection[1] = walk->y + t * ( walk->next->y - walk->y ) ;
		intersection[2] = walk->z + t * ( walk->next->z - walk->z ) ;
		intersection[3] = walk->omega + t * ( walk->next->omega - walk->omega ) ;

		if ( graph_head == NULL ) {

			graph_head = new Point( intersection[0], intersection[1], intersection[2], intersection[3] ) ;
			g_walk = graph_head ;

		} // if

		else
			g_walk = new Point( intersection[0], intersection[1], intersection[2], intersection[3] ) ;

		g_walk->next = Clip( walk->next, graph_head, g_walk->next, edgeCode ) ;

		return g_walk ;

	} // else if clip

	else {

		g_walk = Clip( walk->next, graph_head, g_walk, edgeCode ) ;

		return g_walk ;

	} // else trivially reject

} // My_Polygon::Clip()

void My_Polygon::FindNormalVec() {

	PointPtr temp_point = point ;

	float len = 0 ;
	int count = 0 ;

	Point result = Point() ;
	Point p[3] ;

	for ( int i = 0 ; i < 3 ; i++ )
		p[i] = Point() ;

	while ( temp_point != NULL ) {

		if ( count < 3 ) {

			p[count].x = temp_point->x ;
			p[count].y = temp_point->y ;
			p[count].z = temp_point->z ;

		} // if

		temp_point = temp_point->next ;

		count++ ;

	} // while

	Point vec1 = Point( p[2].x - p[0].x, p[2].y - p[0].y, p[2].z - p[0].z ) ;
	Point vec2 = Point( p[1].x - p[0].x, p[1].y - p[0].y, p[1].z - p[0].z ) ;

	result.x = vec1.y * vec2.z - vec2.y * vec1.z ;
	result.y = vec1.z * vec2.x - vec2.z * vec1.x ;
	result.z = vec1.x * vec2.y - vec2.x * vec1.y ;

	len = sqrt( pow( result.x, 2 ) + pow( result.y, 2 ) + pow( result.z, 2 ) ) ;

	if ( len != 0 ) {

		normal_vec.x = result.x / len ;
		normal_vec.y = result.y / len ;
		normal_vec.z = result.z / len ;

	} // if

} // My_Polygon::FindNormalVec()

Point My_Polygon::FindCenter() {

	int count = 0 ;

	PointPtr walk = point ;

	Point center = Point() ;

	while ( walk != NULL && walk->next != NULL ) {

		center.x += walk->x ;
		center.y += walk->y ;
		center.z += walk->z ;

		walk = walk->next ;

		count++ ;

	} // while

	center.x /= count ;
	center.y /= count ;
	center.z /= count ;

	return center ;

} // My_Polygon::FindCenter()

float My_Polygon::FindZ( Point target ) {

	float offset = 0 ;
	float z_value = 0 ;

	Point center = FindCenter() ;

	offset = -( point->x * normal_vec.x + point->y * normal_vec.y + point->z * normal_vec.z ) ;
	float off = -( center.x * normal_vec.x + center.y * normal_vec.y + center.z * normal_vec.z ) ;

	if ( normal_vec.z == 0 )
		return 0 ;

	return -( normal_vec.x * target.x + normal_vec.y * target.y + offset ) / normal_vec.z ;

} // My_Polygon::FindZ()

void My_Polygon::ClockWise() {

	PointPtr temp_point = point ;

	float len = 0 ;
	int count = 0 ;

	Point result = Point() ;
	Point p[3] ;
	Point center = Point() ;

	for ( int i = 0 ; i < 3 ; i++ )
		p[i] = Point() ;

	while ( temp_point != NULL ) {

		if ( count < 3 ) {

			p[count].x = temp_point->x ;
			p[count].y = temp_point->y ;
			p[count].z = temp_point->z ;

			center.x += p[count].x ;
			center.y += p[count].y ;
			center.z += p[count].z ;

		} // if

		temp_point = temp_point->next ;

		count++ ;

	} // while

	center.x /= count ;
	center.y /= count ;
	center.z /= count ;

} // My_Polygon::ClockWise()

DrawGraphAlg::DrawGraphAlg() {

	this_ = this ;

	activeEdge = EdgeTuple() ;

	a = 0 ;
	b = 0 ;
	c = 0 ;

} // DrawGraphAlg()

void DrawGraphAlg::DrawDot( int x, int y ) {

	glBegin( GL_POINTS ) ;
	glVertex2i( x, y ) ;
	glEnd() ;

	//display() ;

} // Graphic::drawSquare()

void DrawGraphAlg::DrawDot( int x, int y, Color color ) {

	glBegin( GL_POINTS ) ;
	glColor3f( color.r, color.g, color.b ) ;
	glVertex2i( x, y ) ;
	glEnd() ;

	//display() ;

} // Graphic::drawSquare()

void DrawGraphAlg::Draw8SectorPoint( Point center, Point p ) {

	DrawDot( center.x + p.x, center.y + p.y ) ;
	DrawDot( center.x + p.y, center.y + p.x ) ;
	DrawDot( center.x - p.y, center.y + p.x ) ;
	DrawDot( center.x - p.x, center.y + p.y ) ;
	DrawDot( center.x - p.x, center.y - p.y ) ;
	DrawDot( center.x - p.y, center.y - p.x ) ;
	DrawDot( center.x + p.y, center.y - p.x ) ;
	DrawDot( center.x + p.x, center.y - p.y ) ;

} // DrawGraphAlg::Draw8SectorPoint()

void DrawGraphAlg::MidLineDrawingAlg( PointPtr p1, PointPtr p2, bool store ) {

	PointPtr temp_change = NULL ;
	PointPtr temp_p1 = p1 ;
	PointPtr temp_p2 = p2 ;

	//if ( store )
	//	this_->StoreEdgeInTable( *p1, *p2 ) ;

	Point temp = Point() ;

	float midpoint_d = 0.0 ;

	if ( p1->x > p2->x ) {

		temp_change = temp_p2 ;
		temp_p2 = temp_p1 ;
		temp_p1 = temp_change ;

	} // if p1 is at right side then swap

	this_->a = temp_p2->y - temp_p1->y ;
	this_->b = temp_p1->x - temp_p2->x ;
	this_->c = -this_->b * temp_p1->y - this_->a * temp_p1->x ;

	temp.x = temp_p1->x ;
	temp.y = temp_p1->y ;

	if ( this_->a == 0 ) {

		//temp.y = temp_p1->y ;

		for ( temp.x = temp_p1->x ; temp.x <= temp_p2->x ; temp.x++ )
			DrawDot( temp.x, temp.y ) ;

	} // if the two point is same at y-axis

	else if ( this_->b == 0 ) {

		//temp.x = temp_p1->x ;

		if ( temp_p1->y > temp_p2->y ) {

			temp_change = temp_p2 ;
			temp_p2 = temp_p1 ;
			temp_p1 = temp_change ;

		} // if p2 is at the down of p1

		for ( temp.y = temp_p1->y ; temp.y <= temp_p2->y ; temp.y++ )
			DrawDot( temp.x, temp.y ) ;

	} // else if the two point is same at x-axis

	else if ( this_->a > 0 && ( ( ( double )-this_->a / ( double )this_->b ) <= 1.0 ) ) {

		midpoint_d = this_->a + this_->b / 2 ;

		for ( int i = temp_p1->x ; i <= temp_p2->x ; i++ ) {

			DrawDot( temp.x, temp.y ) ;

			if ( midpoint_d <= 0 ) {

				temp.x += 1 ; 

				midpoint_d += this_->a ;

			} // if 

			else {

				temp.x += 1 ;
				temp.y += 1 ;

				midpoint_d += ( this_->a + this_->b ) ;

			} // else

		} // for

	} // else if

	else if ( this_->a > 0 && ( ( ( double )-this_->a / ( double )this_->b ) > 1.0 ) ) {

		midpoint_d = this_->a / 2 + this_->b ;

		while ( temp.y <= temp_p2->y ) {

			DrawDot( temp.x, temp.y ) ;

			if ( midpoint_d <= 0 ) {

				temp.x += 1 ; 
				temp.y += 1 ;

				midpoint_d += ( this_->a + this_->b ) ;

			} // if 

			else {

				temp.y += 1 ;

				midpoint_d += this_->b ;

			} // else

		} // while

	} // else if

	else if ( this_->a < 0 && ( ( ( double )-this_->a / ( double )this_->b ) >= -1.0 ) ) {

		midpoint_d = this_->a - this_->b / 2 ;

		while ( temp.x <= temp_p2->x ) {

			DrawDot( temp.x, temp.y ) ;

			if ( midpoint_d <= 0 ) {

				temp.x += 1 ;
				temp.y -= 1 ;

				midpoint_d += ( this_->a - this_->b ) ;

			} // if 

			else {

				temp.x += 1 ;

				midpoint_d += this_->a ;

			} // else

		} // for

	} // else if

	else if ( this_->a < 0 && ( ( ( double )-this_->a / ( double )this_->b ) < -1.0 ) ) {

		midpoint_d = this_->a / 2 - this_->b ;

		while ( temp.y >= temp_p2->y ) {

			DrawDot( temp.x, temp.y ) ;

			if ( midpoint_d <= 0 ) {

				temp.y -= 1 ;

				midpoint_d -= this_->b ;

			} // if 

			else {

				temp.x += 1 ;
				temp.y -= 1 ;

				midpoint_d += ( this_->a - this_->b ) ;

			} // else

		} // for

	} // else if

} // DrawGraphAlg::MidLineDrawingAlg()

void DrawGraphAlg::MidCircleDrawingAlg( Point center, int radius ) {

	int d = 1 - radius ;
	int incE = 3 ;
	int incSE = -2 * radius + 5 ;

	Point temp = Point( 0, radius ) ;

	this_->Draw8SectorPoint( center, temp ) ;

	while ( temp.x < temp.y ) {

		if ( d < 0 ) {

			d += incE ;
			incE += 2 ;
			incSE += 2 ;

			temp.x++ ;

		} // if choose E as the point

		else {

			d += incSE ;
			incE += 2 ;
			incSE += 4 ;

			temp.x++ ;
			temp.y-- ;

		} // else choose SE as the point

		this_->Draw8SectorPoint( center, temp ) ;

	} // while drawing the sector of upper part of the circle

} // DrawGraphAlg::MidCircleDrawingAlg()

void DrawGraphAlg::ClearPoint( PointPtr head ) {

	PointPtr walk = head ;

	if ( head != NULL ) {

		ClearPoint( walk->next ) ;
		delete( walk ) ;
		walk = NULL ;

	} // if

} // DrawGraphAlg::ClearPoint()

void DrawGraphAlg::ClearEdgeTable() {

	for ( int i = 0 ; i < MAX_HEIGHT ; i++ ) {

		vector<EdgeBucket>().swap( this_->edgeTable[i].bucket ) ;

		this_->edgeTable[i] = EdgeTuple() ;

	} //for
		
	this_->activeEdge = EdgeTuple() ;

} // DrawGraphAlg::ClearEdgeTable()

void DrawGraphAlg::DeleteEdge( EdgeTuple* edge, int delete_y ) {

	for ( int i = 0 ; i < edge->howManyBucket ; i++ ) {

		if ( edge->bucket[i].max_y == delete_y ) {

			for ( int j = i ; j < edge->howManyBucket - 1 ; j++ ) {

				edge->bucket[j].max_y = edge->bucket[j + 1].max_y ;
				edge->bucket[j].min_x = edge->bucket[j + 1].min_x ;
				edge->bucket[j].slopeInverse = edge->bucket[j + 1].slopeInverse ;

			} // for

			edge->howManyBucket-- ;

			if ( i != edge->howManyBucket )
				i-- ;

		} // if

	} // for

} // DrawGraphAlg::DeleteEdge()

void DrawGraphAlg::UpdateActiveMin_X( EdgeTuple* edge ) {

	for ( int i = 0 ; i < edge->bucket.size() ; i++ )
		edge->bucket[i].min_x += edge->bucket[i].slopeInverse ;

} // DrawGraphAlg::UpdateMin_X()

void DrawGraphAlg::StoreEdgeInBucket( EdgeTuple* edge, int max_y, int min_x, float slopeInverse ) {

	EdgeBucket* temp = new EdgeBucket( max_y, min_x, slopeInverse ) ;

	edge->bucket.insert( edge->bucket.begin() + edge->howManyBucket, *temp ) ;
	edge->Sort() ;

	edge->howManyBucket++ ;

} // DrawGraphAlg::StoreEdgeInBucket()

void DrawGraphAlg::StoreEdgeInTable( Point p1, Point p2 ) {

	EdgeTuple* tempEdge = new EdgeTuple() ;

	float slope = 0.0 ;
	float inverseSlope = 0.0 ;
	int max_y = 0 ;
	int min_x = 0 ;
	int scanline = 0 ;

	if ( p1.x == p2.x )
		inverseSlope = 0.0 ;

	else if ( p1.y != p2.y ) {

		slope = ( float )( p2.y - p1.y ) / ( float )( p2.x - p1.x ) ;
		inverseSlope = ( float )( 1 / slope ) ;

	} // else if

	if ( p1.y > p2.y ) {

		scanline = p2.y ;
		max_y = p1.y ;
		min_x = p2.x ;

	} // if

	else {

		scanline = p1.y ;
		max_y = p2.y ;
		min_x = p1.x ;

	} // else

	StoreEdgeInBucket( &edgeTable[scanline], max_y, min_x, inverseSlope ) ;

} // DrawGraphAlg::StoreEdgeInTable()

void DrawGraphAlg::StorePolygonEdge( PointPtr head ) {

	PointPtr walk = head ;

	while ( walk != NULL && walk->next != NULL ) {

		this_->StoreEdgeInTable( *walk, *walk->next ) ;

		walk = walk->next ;

	} // while

} // DrawGraphAlg::StorePolygonEdge()

void DrawGraphAlg::DrawColorAlg( GraphicPtr graph, vector<vector<Z_Buffer>> &z_buffer ) {

	bool parity_odd = false ;
	int coordCount = 0 ;

	Point p1 = Point() ;
	Point p2 = Point() ;

	Point p1_draw = Point() ;
	Point p2_draw = Point() ;

	for ( int i = 0 ; i < z_buffer.size() ; i++ ) {

		for ( int j = 0 ; j < this_->edgeTable[i].howManyBucket ; j++ ) {
			this_->StoreEdgeInBucket( &this_->activeEdge,
									  this_->edgeTable[i].bucket[j].max_y,
									  this_->edgeTable[i].bucket[j].min_x,
									  this_->edgeTable[i].bucket[j].slopeInverse ) ;

			if ( j > 10000 )
				break ;

		} // for
		this_->DeleteEdge( &this_->activeEdge, i ) ;
		this_->activeEdge.Sort() ;

		parity_odd = false ;
		coordCount = 0 ;
		p1 = Point() ;
		p2 = Point() ;

		for ( int j = 0 ; j < this_->activeEdge.howManyBucket ; j++ ) {

			if ( coordCount % 2 == 0 ) {

				p1.x = ( int )( this_->activeEdge.bucket[j].min_x ) ;
				p1.y = ( int )this_->activeEdge.bucket[j].max_y ;

				if ( p1.x == p2.x && coordCount != 0 ) {

					if ( ( ( p1.x == p1.y ) && ( p2.x != p2.y ) ) || ( ( p1.x != p1.y ) && ( p2.x == p2.y ) ) ) {

						p2.x = p1.x ;
						p2.y = p1.y ;

					} // if

					else
						coordCount++ ;

				} // if

				else
					coordCount++ ;

			} // if

			else {

				p2.x = ( int )this_->activeEdge.bucket[j].min_x ;
				p2.y = ( int )this_->activeEdge.bucket[j].max_y ;

				parity_odd = false ;

				if ( p1.x == p2.x ) {

					if ( ( ( p1.x == p1.y ) && ( p2.x != p2.y ) ) || ( ( p1.x != p1.y ) && ( p2.x == p2.y ) ) ) {

						p2.x = p1.x ;
						p2.y = p1.y ;

					} // if

					else {

						coordCount++ ;
						parity_odd = true ;

					} // else


				} // if

				else{

					coordCount++ ;
					parity_odd = true ;

				} // else

			} // else

			if ( parity_odd ) {

				p1_draw.x = p1.x ;
				p1_draw.y = i ;

				p2_draw.x = p2.x ;
				p2_draw.y = i ;

				for ( int draw_x = p1_draw.x ; draw_x < p2_draw.x ; draw_x++ ) {

					Point target = Point( draw_x, p2_draw.y ) ;

					float z_value = graph->FindZ( target ) ;

					if ( draw_x < z_buffer.size() && z_value < z_buffer[draw_x][p2_draw.y].z_value ) {

						/*z_buffer[p2_draw.y][draw_x].color.r = graph->color.r ;
						z_buffer[p2_draw.y][draw_x].color.g = graph->color.g ;
						z_buffer[p2_draw.y][draw_x].color.b = graph->color.b ;

						z_buffer[p2_draw.y][draw_x].z_value = z_value ;*/

						z_buffer[draw_x][p2_draw.y].color.r = graph->color.r ;
						z_buffer[draw_x][p2_draw.y].color.g = graph->color.g ;
						z_buffer[draw_x][p2_draw.y].color.b = graph->color.b ;

						z_buffer[draw_x][p2_draw.y].z_value = z_value ;

						//cout << endl ;

					} // if

				} // for

				//this_->MidLineDrawingAlg( &p1_draw, &p2_draw, false ) ;
				//display() ;

			} // if

		} // for

		this_->UpdateActiveMin_X( &this_->activeEdge ) ;

	} // for

	this_->ClearEdgeTable() ;

} // DrawGraphAlg::DrawColorAlg()

Camera::Camera() {

	position = Point() ;
	COI = Point() ;

	tilt = 0 ;
	hither_plane = 0 ;
	yon_plane = 0 ;
	field_of_view = 0 ;
	tan_hov = 0 ;

	length_v1 = 0 ;
	length_v2 = 0 ;
	length_v3 = 0 ;

} // Camera::Camera()

Camera::Camera( Point position, Point COI, 
				float tilt, float hither_plane, float yon_plane, float field_of_view ) {

	this->position.x = position.x ;
	this->position.y = position.y ;
	this->position.z = position.z ;

	this->COI.x = COI.x ;
	this->COI.y = COI.y ;
	this->COI.z = COI.z ;

	this->tilt = tilt ;
	this->hither_plane = hither_plane ;
	this->yon_plane = yon_plane ;
	this->field_of_view = field_of_view ;

	this->tan_hov = tan( this->field_of_view ) ;

} // Camera::Camera()

void Camera::ComputeGRMVector() {

	float temp[3] = {0, 1, 0} ;
	float right[3] = {0} ;
	float top[3] = {0} ;

	float length_right = 0 ;
	float length_top = 0 ;

	length_v3 = sqrt( pow( COI.x - position.x, 2 ) + pow( COI.y - position.y, 2 ) + pow( COI.z - position.z, 2 ) ) ;
	v3[0] = ( COI.x - position.x ) / length_v3 ;
	v3[1] = ( COI.y - position.y ) / length_v3 ;
	v3[2] = ( COI.z - position.z ) / length_v3 ;

	length_right = sqrt( v3[2] * v3[2] + v3[0] * v3[0] ) ;
	right[0] = -v3[2] / length_right ;
	right[2] = v3[0] / length_right ;

	/*length_top = sqrt( pow( v3[1] * right[2] - v3[2] * right[1], 2 ) + 
					   pow( v3[2] * right[0] - v3[0] * right[2], 2 ) +
					   pow( v3[0] * right[1] - v3[1] * right[0], 2 ) ) ;

	top[0] = -( v3[1] * right[2] - v3[2] * right[1] ) / length_top ;
	top[1] = -( v3[2] * right[0] - v3[0] * right[2] ) / length_top ;
	top[2] = -( v3[0] * right[1] - v3[1] * right[0] ) / length_top ;

	length_v1 = sqrt( pow( top[1] * v3[2] - top[2] * v3[1], 2 ) + 
					  pow( top[2] * v3[0] - top[0] * v3[2], 2 ) +
					  pow( top[0] * v3[1] - top[1] * v3[0], 2 ) ) ;

	v1[0] = ( top[1] * v3[2] - top[2] * v3[1] ) / length_v1 ;
	v1[1] = ( top[2] * v3[0] - top[0] * v3[2] ) / length_v1 ;
	v1[2] = ( top[0] * v3[1] - top[1] * v3[0] ) / length_v1 ;

	length_v2 = sqrt( pow( v3[1] * v1[2] - v3[2] * v1[1], 2 ) +
					  pow( v3[2] * v1[0] - v3[0] * v1[2], 2 ) +
					  pow( v3[0] * v1[1] - v3[1] * v1[0], 2 ) ) ;

	v2[0] = ( v3[1] * v1[2] - v3[2] * v1[1] ) / length_v2 ;
	v2[1] = ( v3[2] * v1[0] - v3[0] * v1[2] ) / length_v2 ;
	v2[2] = ( v3[0] * v1[1] - v3[1] * v1[0] ) / length_v2 ;*/

	length_v2 = sqrt( pow( -v3[0] * v3[1], 2 ) + pow( v3[0] * v3[0] + v3[2] * v3[2], 2 ) + pow( -v3[2] * v3[1], 2 ) ) ;
	v2[0] = ( -v3[0] * v3[1] ) / length_v2 ;
	v2[1] = ( v3[0] * v3[0] + v3[2] * v3[2] ) / length_v2 ;
	v2[2] = ( -v3[2] * v3[1] ) / length_v2 ;

	length_v1 = sqrt( pow( v3[2], 2 ) + pow( -v3[0], 2 ) ) ;
	v1[0] = v3[2] / length_v1 ;
	v1[1] = 0 ;
	v1[2] = -v3[0] / length_v1 ;

} // Camera::ComputeGRMVector()

Viewport::Viewport() {

	left_bottom = Point() ;
	right_top = Point() ;

} // ViewPort::ViewPort()

Viewport::Viewport( Point l_b, Point r_t ) {

	left_bottom.x = l_b.x ;
	left_bottom.y = l_b.y ;
	left_bottom.z = l_b.z ;

	right_top.x = r_t.x ;
	right_top.y = r_t.y ;
	right_top.z = r_t.z ;

} // ViewPort::ViewPort()

Light::Light() {

	index = 0 ;
	light_position = Point() ;

	next = NULL ;

} // Light::Light()

Light::Light( int index, float x, float y, float z ) {

	this->index = index ;
	this->light_position = Point( x, y ,z ) ;

	next = NULL ;

} // Light::Light()

void Light::AssignColor( float r, float g, float b ) {

	color.r = r ;
	color.g = g ;
	color.b = b ;

} // Light::AssignColor()

LightingModel::LightingModel() {

	this_ = this ;
	shading_color = Color() ;

} // LightingModel::LightingModel()

Point LightingModel::GenVector( Point p1, Point p2 ) {

	Point vector = Point( p2.x - p1.x, p2.y - p1.y, p2.z - p1.z ) ;
	float len = sqrt( pow( vector.x, 2 ) + pow( vector.y, 2 ) + pow( vector.z, 2 ) ) ;

	vector.x /= len ;
	vector.y /= len ;
	vector.z /= len ;

	return vector ;

} // LightingModel::GenVecor()

Point LightingModel::AddVector( Point p1, Point p2 ) {

	Point vector = Point( p1.x + p2.x, p1.y + p2.y, p1.z + p2.z ) ;
	float len = sqrt( pow( vector.x, 2 ) + pow( vector.y, 2 ) + pow( vector.z, 2 ) ) ;

	vector.x /= len ;
	vector.y /= len ;
	vector.z /= len ;

	return vector ;

} // LightingModel::AddVector()

void LightingModel::Ambient( GraphicPtr graph, float ambient[3] ) {

	shading_color.r += ambient[0] * graph->color.r ;
	shading_color.g += ambient[1] * graph->color.g ;
	shading_color.b += ambient[2] * graph->color.b ;

} // LightingModel::Ambient()

void LightingModel::Diffuse( GraphicPtr graph, LightPtr light ) {

	LightPtr walk_light = light ;
	Point light_vec = Point() ;
	Point center = Point() ;
	float diffuse_weight = 0 ;

	center = graph->FindCenter() ;

	while ( walk_light != NULL ) {

		light_vec = GenVector( center, light->light_position ) ;
		
		diffuse_weight = -( light_vec.x * graph->normal_vec.x + light_vec.y * graph->normal_vec.y + light_vec.z * graph->normal_vec.z ) ;

		if ( diffuse_weight < 0 )
			diffuse_weight = 0 ;

		shading_color.r += graph->coeef[0] * walk_light->color.r * diffuse_weight * graph->color.r ;
		shading_color.g += graph->coeef[0] * walk_light->color.g * diffuse_weight * graph->color.g ;
		shading_color.b += graph->coeef[0] * walk_light->color.b * diffuse_weight * graph->color.b ;

		walk_light = walk_light->next ;

	} // while

} // LightingModel::Diffuse()

void LightingModel::Specular( GraphicPtr graph, LightPtr light, CameraPtr camera ) {

	LightPtr walk_light = light ;
	Point view_vec = Point() ;
	Point light_vec = Point() ;
	Point half_vec = Point() ;
	Point center = graph->FindCenter() ;
	float specular_weight = 0 ;

	while ( walk_light != NULL ) {

		view_vec = GenVector( center, camera->position ) ;
		light_vec = GenVector( center, light->light_position ) ;

		half_vec = AddVector( view_vec, light_vec ) ;
		
		specular_weight = -( half_vec.x * graph->normal_vec.x + half_vec.y * graph->normal_vec.y + half_vec.z * graph->normal_vec.z ) ;

		if ( specular_weight < 0 )
			specular_weight = 0 ;

		if ( specular_weight != 0 && graph->coeef[2] != 0 )
			specular_weight = pow( specular_weight, graph->coeef[2] + 1 ) ;

		shading_color.r += graph->coeef[1] * walk_light->color.r * specular_weight ;
		shading_color.g += graph->coeef[1] * walk_light->color.g * specular_weight ;
		shading_color.b += graph->coeef[1] * walk_light->color.b * specular_weight ;

		walk_light = walk_light->next ;

	} // while

} // LightingModel::Specular()

Color LightingModel::LightModelCal( GraphicPtr graph, float ambient[3], LightPtr light, CameraPtr camera ) {

	this_->Ambient( graph, ambient ) ;
	this_->Diffuse( graph, light ) ;
	this_->Specular( graph, light, camera ) ;

	if ( this_->shading_color.r > 1 )
		this_->shading_color.r = 1 ;

	else if ( this_->shading_color.r < 0 )
		this_->shading_color.r = 0 ;

	if ( this_->shading_color.g > 1 )
		this_->shading_color.g = 1 ;

	else if ( this_->shading_color.g < 0 )
		this_->shading_color.g = 0 ;

	if ( this_->shading_color.b > 1 )
		this_->shading_color.b = 1 ;

	else if ( this_->shading_color.b < 0 )
		this_->shading_color.b = 0 ;

	return this_->shading_color ;

} // LightingModel::LightModelCal()

void LightingModel::ClearShadingColor() {

	this_->shading_color.r = 0 ;
	this_->shading_color.g = 0 ;
	this_->shading_color.b = 0 ;

} // LightingModel::ClearShadingColor()

Z_Buffer::Z_Buffer( Color color ) {

	z_value = INF ;
	this->color = Color( color.r, color.g, color.b ) ;

} // Z_Buffer()

GlutWindow::GlutWindow() {

	this_ = this ;
	finADraw = true ;
	drawMode = '\0' ;

	graph_mem.clear() ;

	temp_graphic = NULL ;
	obj_files = NULL ;

	operator_head = NULL ;
	operator_tail = NULL ;

	camera = NULL ;
	viewport = NULL ;
	light = NULL ;

	background = Color() ;

	width = MAX_WIDTH ;
	height = MAX_HEIGHT ;
	aspect_ratio = 0 ;

	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE ) ;
	glutInitWindowSize( MAX_WIDTH, MAX_HEIGHT ) ;
	glutInitWindowPosition( 0, 100 ) ;
	window = glutCreateWindow( "2022CG_Lab4_111522062" ) ;
	glutSwapBuffers() ;
	glutDisplayFunc( display ) ;
	gluOrtho2D( 0, MAX_HEIGHT, 0, MAX_WIDTH ) ;

	glPointSize( 1 ) ;
	glClearColor( 0.0, 0.0, 0.0, 0.0 ) ;
	glClear( GL_COLOR_BUFFER_BIT ) ;

} // GlutWindow::GlutWindow()

GlutWindow::GlutWindow( float width, float height ) {

	this_ = this ;
	finADraw = true ;
	drawMode = '\0' ;

	graph_mem.clear() ;

	temp_graphic = NULL ;
	obj_files = NULL ;

	operator_head = NULL ;
	operator_tail = NULL ;

	camera = NULL ;
	viewport = NULL ;
	light = NULL ;

	background = Color() ;
		
	this->width = width ;
	this->height = height ;
	this->aspect_ratio = 0 ;

	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE ) ;
	glutInitWindowSize( ( int )width, ( int )height ) ;
	glutInitWindowPosition( 0, 100 ) ;
	window = glutCreateWindow( "2022CG_Lab4_111522062" ) ;
	glutSwapBuffers() ;
	glutDisplayFunc( display ) ;
	gluOrtho2D( 0, ( int )width, 0, ( int )height ) ;

	glPointSize( 1 ) ;
	glClearColor( 0.0, 0.0, 0.0, 0.0 ) ;
	glClear( GL_COLOR_BUFFER_BIT ) ;

} // GlutWindow::GlutWindow()

void GlutWindow::CleanPointer() {

	if ( temp_graphic != NULL ) {

		temp_graphic->DeletePoint( temp_graphic->point ) ;
		delete( temp_graphic ) ;

	} // if

	temp_graphic = NULL ;

} // GlutWindow::CleanPointer()

void GlutWindow::SetNewColor() {

	this_->temp_color[0] = Random021() ;
	this_->temp_color[1] = Random021() ;
	this_->temp_color[2] = Random021() ;

	if ( this_->temp_color[0] == 0 &&
		 this_->temp_color[1] == 0 &&
		 this_->temp_color[2] == 0 )
		SetNewColor() ;

} // GlutWindow::SetNewColor()

void GlutWindow::ClearScreen() {

	glClear( GL_COLOR_BUFFER_BIT ) ;

} // GlutWindow::Clear()

void GlutWindow::ClearData() {

	for ( int i = 0 ; i < graph_mem.size() ; i++ ) {

		graph_mem[i]->point = graph_mem[i]->DeletePoint( graph_mem[i]->point ) ;

		delete( graph_mem[i] ) ;
		graph_mem[i] = NULL ;

	} // for

	graph_mem.clear() ;

} // GlutWindow::ClearData()

void GlutWindow::CloneGraph( GraphicPtr graph ) {

	PointPtr walk_graph = graph->point ;
	PointPtr walk_temp = NULL ;

	temp_graphic = graph->Clone() ;
	walk_temp = temp_graphic->point ;

	while ( walk_graph != NULL ) {

		if ( walk_temp == NULL ) {

			temp_graphic->point = walk_graph->Clone() ;
			walk_temp = temp_graphic->point ;

		} // if

		else {

			temp_graphic->point->next = walk_graph->Clone() ;

			walk_graph = walk_graph->next ;
			walk_temp = walk_temp->next ;

		} // else

	} // while

} // GlutWindow::CloneGraph()

void GlutWindow::Triangle() {

	SetNewColor() ;

	temp_graphic = new My_Polygon() ;

	PointPtr walk = new Point( 0, 1 ) ;
	temp_graphic->point = walk ;

	walk->next = new Point( -1, -1 ) ;
	walk = walk->next ;
	walk->next = new Point( 1, -1 ) ;
	walk = walk->next ;
	walk->next = new Point( 0, 1 ) ;

	temp_graphic->AssignColor( temp_color[0],
							   temp_color[1],
							   temp_color[2] ) ;

	cout << "R : " << ( int )( temp_color[0] * 255 ) << endl 
		<< "G : " << ( int )( temp_color[1] * 255 ) << endl
		<< "B : " << ( int )( temp_color[2] * 255 ) << endl ;

} // GlutWindow::Triangle()

void GlutWindow::Square() {

	SetNewColor() ;

	temp_graphic = new My_Polygon() ;

	PointPtr walk = new Point( -1, -1 ) ;
	temp_graphic->point = walk ;

	walk->next = new Point( 1, -1 ) ;
	walk = walk->next ;
	walk->next = new Point( 1, 1 ) ;
	walk = walk->next ;
	walk->next = new Point( -1, 1 ) ;
	walk = walk->next ;
	walk->next = new Point( -1, -1 ) ;

	temp_graphic->AssignColor( temp_color[0],
							   temp_color[1],
							   temp_color[2] ) ;

	cout << "R : " << ( int )( temp_color[0] * 255 ) << endl 
		 << "G : " << ( int )( temp_color[1] * 255 ) << endl
	     << "B : " << ( int )( temp_color[2] * 255 ) << endl ;

} // GlutWindow::Square()

void GlutWindow::Polygon( PointPtr pic ) {

	temp_graphic = new My_Polygon() ;

	PointPtr walk = pic->Clone() ;
	PointPtr head = walk ;
	temp_graphic->point = walk ;

	while ( walk != NULL && walk->next != NULL ) {

		walk->next = walk->next->Clone() ;
		walk = walk->next ;

	} // while

	walk->next = new Point( head->x, head->y, head->z, head->omega ) ;

	temp_graphic->AssignColor( 0, 0.8, 0 ) ;

} // GlutWindow::Polygon()

void GlutWindow::DrawWindow( Point left_top, Point right_bottom ) {

	GraphicPtr window = new My_Polygon() ;
	PointPtr tempPoint = window->point ;

	window->AssignColor( background.r, background.g, background.b ) ;

	tempPoint = new Point( left_top.x, right_bottom.y ) ;
	window->point = tempPoint ;

	tempPoint->next = new Point( 400, right_bottom.y ) ;
	tempPoint = tempPoint->next ;

	tempPoint->next = new Point( 400, 400 ) ;
	tempPoint = tempPoint->next ;

	tempPoint->next = new Point( left_top.x, 400 ) ;
	tempPoint = tempPoint->next ;

	tempPoint->next = new Point( left_top.x, right_bottom.y ) ;

	window->DrawGraph() ;
	display() ;
	DrawGraphAlg::ClearEdgeTable() ;

	window->point = window->DeletePoint( window->point ) ;
	delete( window ) ;

} // GlutWindow::DrawWindow()

ObjPtr GlutWindow::SearchObj( string file_name ) {

	ObjPtr walk_obj = obj_files ;

	while ( walk_obj != NULL && walk_obj->next != NULL && walk_obj->file_name != file_name )
		walk_obj = walk_obj->next ;

	if ( walk_obj == NULL ) {

		obj_files = new Obj() ;
		obj_files->ReadObj( file_name ) ;
		walk_obj = obj_files ;

	} // if

	else if ( walk_obj->file_name == file_name )
		;

	else if ( walk_obj->next == NULL ) {

		walk_obj->next = new Obj() ;
		walk_obj = walk_obj->next ;

		walk_obj->ReadObj( file_name ) ;

	} // else if

	return walk_obj ;

} // GlutWindow::SearchObj()

LightPtr GlutWindow::InsertLight( LightPtr walk, LightPtr target ) {

	LightPtr temp = walk ;

	if ( temp == NULL )
		return target ;

	else if ( temp->index == target->index ) {

		temp->light_position = target->light_position ;
		
		temp->color.r = target->color.r ;
		temp->color.g = target->color.g ;
		temp->color.b = target->color.b ;

	} // else if

	else if ( temp->index < target->index )
		temp->next = InsertLight( walk->next, target ) ;

	else if ( temp->index > target->index ) {

		target->next = temp ;
		temp = target ;

	} // else if

	return temp ;

} // GlutWindow::InsertLight()

void GlutWindow::Illuminate( GraphicPtr graph ) {

	Color shading_color = LightingModel::LightModelCal( graph, ambient_coeff, light, camera ) ;
	LightingModel::ClearShadingColor() ;

	graph->AssignColor( shading_color.r, shading_color.g, shading_color.b ) ;

} // GlutWindow::Illuminate()

void GlutWindow::TransToEyeSpace( GraphicPtr graph ) {

	PointPtr point = graph->point ;
	Point temp = Point() ;

	OperationPtr trans = new Translate( -camera->position.x, -camera->position.y, -camera->position.z ) ;
	OperationPtr rotate = new Rotate( 0, 0, camera->tilt ) ;

	point = trans->Operate( point ) ;

	while ( point != NULL ) {

		temp.x = camera->v1[0] * point->x + camera->v1[1] * point->y + camera->v1[2] * point->z ;
		temp.y = camera->v2[0] * point->x + camera->v2[1] * point->y + camera->v2[2] * point->z ;
		temp.z = camera->v3[0] * point->x + camera->v3[1] * point->y + camera->v3[2] * point->z ;

		point->x = temp.x ; 
		point->y = temp.y ;
		point->z = temp.z ;

		point->x = -point->x ;
		//point->y = -point->y ;

		point = point->next ;

	} // while

	point = rotate->Operate( graph->point ) ;

	delete trans  ;
	delete rotate  ;

} // GlutWindow::TransToEyeSpace()

void GlutWindow::TransToProjectionSpace( GraphicPtr graph ) {

	float yon = camera->yon_plane ;
	float hither = camera->hither_plane ;
	float hov = camera->field_of_view ;
	float tan_hov = camera->tan_hov ;

	PointPtr point = graph->point ;
	Point temp = Point() ;

	while ( point != NULL ) {

		temp.y = aspect_ratio * point->y ;
		temp.z = ( yon * tan_hov * point->z / ( yon - hither ) ) + 
			( yon * hither * tan_hov / ( hither - yon ) ) ;

		temp.omega = tan_hov * point->z ;

		point->y = temp.y ;
		point->z = temp.z ;
		point->omega = temp.omega ;

		point = point->next ;

	} // while

} // GlutWindow::TransToEyeSpace()

void GlutWindow::PerspectiveDivide( GraphicPtr graph, Point LB, Point RT ) {

	PointPtr point = graph->point ;

	while ( point != NULL ) {

		point->x /= point->omega ;
		point->y /= point->omega ;
		point->z /= point->omega ;
		point->omega /= point->omega ;

		// view port

		//point->x *= width ;
		//point->y *= height ;

		point->x = ( point->x + 1 ) * ( viewport->right_top.x - viewport->left_bottom.x ) / 2 + viewport->left_bottom.x ;
		point->y = ( point->y + 1 ) * ( viewport->right_top.y - viewport->left_bottom.y ) / 2 + viewport->left_bottom.y ;

		/*point->x = ( point->x + 1 ) * ( RT.x - LB.x ) / 2 + LB.x ;
		point->y = ( point->y + 1 ) * ( RT.y - LB.y ) / 2 + LB.y ;*/

		point->x = ( point->x - viewport->left_bottom.x ) * ( RT.x - LB.x ) / ( viewport->right_top.x - viewport->left_bottom.x ) ;
		point->y = ( point->y - viewport->left_bottom.y ) * ( RT.y - LB.y ) / ( viewport->right_top.x - viewport->left_bottom.x ) ;

		point = point->next ;

	} // while

	//graph->FindNormalVec() ;

	//if ( graph->normal_vec.z >= 0 )
	//	graph->visible = false ;

} // GlutWindow::PerspectiveDivide()

bool GlutWindow::IsPointInGraph( GraphicPtr graph, Point test_point ) {

	bool isInGraph = true ;

	PointPtr walk = graph->point ;

	Point p1 = Point() ;
	Point p2 = Point() ;

	float test = 0 ;

	while ( walk != NULL && walk->next != NULL ) {

		p1 = *walk ;
		p2 = *walk->next ;

		test = ( test_point.x - p1.x ) * ( p2.y - p1.y ) - ( p2.x - p1.x ) * ( test_point.y - p1.y ) ;

		if ( test < 0 ) {

			isInGraph = false ;
			break ;

		} // if

		walk = walk->next ;

	} // while

	return isInGraph ;

} // GlutWindow::IsPointInGraph()

float GlutWindow::FindZ( GraphicPtr graph, Point point ) {

	float offset = 0 ;
	float z_value = 0 ;

	PointPtr walk = graph->point ;
	Point center = graph->FindCenter() ;

	offset = -( walk->x * graph->normal_vec.x + walk->y * graph->normal_vec.y + walk->z * graph->normal_vec.z ) ;
	float off = -( center.x * graph->normal_vec.x + center.y * graph->normal_vec.y + center.z * graph->normal_vec.z ) ;

	if ( graph->normal_vec.z == 0 )
		return 0 ;

	return -( graph->normal_vec.x * point.x + graph->normal_vec.y * point.y + offset ) / graph->normal_vec.z ;

} // GlutWindow::FindZ()

void GlutWindow::DoCommand( ifstream& file, string command, int obj_label ) {

	OperationPtr operator_walk = operator_head ;
	GraphicPtr clonePtr = NULL ;
	ObjPtr obj = NULL ;

	string param[10] ;
	char skipLine[100] ;

	if ( command == "end" ) {

		cout << endl ;

		system( "PAUSE" ) ;

		file.close() ;
		exit( 0 ) ;

	} // if

	else if ( command[0] == '#' )
		file.getline( skipLine, sizeof( skipLine ), '\n' ) ;

	else if ( command == "reset" ) {

		cout << endl ;

		operator_head = operator_head->CleanOperator( operator_walk ) ;
		operator_tail = operator_head ;

		global_matrix.CleanTransMatrix() ;

	} // else if command is reset should clear transform_matrix

	else if ( command == "cleardata" || command == "clearData" ) {

		cout << endl ;

		ClearData() ;

	} // else if command is clearData should clear the all object

	else if ( command == "clearScreen" ) {

		cout << endl ;

		ClearScreen() ;

	} // else if command is clearScreen should clear the screen ( not object )

	else if ( command == "display" ) {

		cout << endl ;

		ClearScreen() ;

		float z_value = 0 ;

		GraphicPtr graph = NULL ;
		PointPtr point = NULL ;
		PointPtr walk = NULL ;

		float half_width = width / 2 ;
		float half_height = height / 2 ;

		Point left_bottom = Point( ( viewport->left_bottom.x + 1 ) * width / 2, 
								   ( viewport->left_bottom.y + 1 ) * height / 2 ) ;
		Point right_top = Point( ( viewport->right_top.x + 1 ) * width / 2, 
								 ( viewport->right_top.y + 1 ) * height / 2 ) ;

		//DrawWindow( left_bottom, right_top ) ;

  		int z_buffer_width = right_top.x - left_bottom.x + 1 ;
		int z_buffer_height = right_top.y - left_bottom.y + 1 ;

		vector<vector<Z_Buffer>> z_buffer ;

		for ( int row = 0 ; row < z_buffer_height ; row++ ) {

			vector<Z_Buffer> temp_z ;

			z_buffer.push_back( temp_z ) ;

			for ( int col = 0 ; col < z_buffer_width ; col++ ) {

				Color bg = Color( background ) ;

				Z_Buffer temp_buffer = Z_Buffer( bg ) ;

				z_buffer.back().push_back( temp_buffer ) ;

			} // for

		} // for

		for ( int i = 0 ; i < graph_mem.size() ; i++ ) {

			graph = graph_mem[i]->Clone() ;

			graph->FindNormalVec() ;

			if ( i == 1648 )
				cout << "stop" << endl ;
				
			Illuminate( graph ) ;
			TransToEyeSpace( graph ) ;
			TransToProjectionSpace( graph ) ;

			// 3D clipping

			for ( int j = 1 ; j <= 6 ; j++ ) {

				point = graph->Clip( graph->point, NULL, NULL, j ) ;

				graph->point = graph->DeletePoint( graph->point ) ;
				graph->point = point ;

			} // for

			PerspectiveDivide( graph, left_bottom, right_top ) ;

			graph->FindNormalVec() ;

			DrawGraphAlg::StorePolygonEdge( graph->point ) ;
			DrawGraphAlg::DrawColorAlg( graph, z_buffer ) ;

		} // for

		if ( graph != NULL ) {

			graph->point = graph->DeletePoint( graph->point ) ;
			delete graph ;
			graph = NULL ;

		} // if

		for ( int row = 0 ; row < z_buffer_height ; row++ ) {

			for ( int col = 0 ; col < z_buffer_width ; col++ )
				DrawGraphAlg::DrawDot( ( int )left_bottom.x + row, ( int )left_bottom.y + col, z_buffer[row][col].color ) ;

		} // for

		display() ;

	} // else if command is display do the space transform to all the point and show

	else if ( command == "object" ) {

		char* obj_param[7] ; // obj, Or, Og, Ob, Kd, Ks, N
							 //  0 , 1 , 2 , 3 , 4 , 5 , 6

		for ( int i = 0 ; i < 7 ; i++ ) {

			file >> param[i] ;

			cout << " " << param[i] ;

			obj_param[i] = const_cast<char*>( param[i].c_str() ) ;

		} // for

		cout << endl ;

		obj = SearchObj( param[0] ) ;

		for ( int i = 0 ; i < obj->f.size() ; i++ ) {

			Polygon( obj->f[i] ) ;

			while ( operator_walk != NULL ) {

				temp_graphic->point = operator_walk->Operate( temp_graphic->point ) ;

				operator_walk =	operator_walk->next ;

			} // while

			temp_graphic->AssignColor( atof( obj_param[1] ), atof( obj_param[2] ), atof( obj_param[3] ) ) ;
			temp_graphic->AssignCoeef( atof( obj_param[4] ), atof( obj_param[5] ), atof( obj_param[6] ) ) ;
			temp_graphic->AssignLabel( obj_label ) ;

			graph_mem.push_back( temp_graphic ) ;

			temp_graphic = NULL ;
			operator_walk = operator_head ;

		} // for

	} // else if the command is object, read the polygon in the file

	else if ( command == "translate" ) {

		file >> param[0] >> param[1] >> param[2] ;

		cout << " " << param[0] << " " << param[1] << " " << param[2] << endl ;

		char* x_char = const_cast< char* >( param[0].c_str() ) ;
		char* y_char = const_cast< char* >( param[1].c_str() ) ;
		char* z_char = const_cast< char* >( param[2].c_str() ) ;

		if ( operator_tail == NULL ) {

			operator_head = new Translate( atof( x_char ), atof( y_char ), atof( z_char ) ) ;
			operator_tail = operator_head ;

		} // if

		else {

			operator_tail->next = new Translate( atof( x_char ), atof( y_char ), atof( z_char ) ) ;
			operator_tail = operator_tail->next ;

		} // else

	} // else if command is translate should move x and y

	else if ( command == "scale" ) {

		file >> param[0] >> param[1] >> param[2] ;

		cout << " " << param[0] << " " << param[1] << param[2] << endl ;

		char* scale_x = const_cast< char* >( param[0].c_str() ) ;
		char* scale_y = const_cast< char* >( param[1].c_str() ) ;
		char* scale_z = const_cast< char* >( param[2].c_str() ) ;

		if ( operator_tail == NULL ) {

			operator_head = new Scale( atof( scale_x ), atof( scale_y ), atof( scale_z ) ) ;
			operator_tail = operator_head ;

		} // if

		else {

			operator_tail->next = new Scale( atof( scale_x ), atof( scale_y ), atof( scale_z ) ) ;
			operator_tail = operator_tail->next ;

		} // else

	} // else if command is scale should scaling x and y

	else if ( command == "rotate" ) {

		file >> param[0] >> param[1] >> param[2] ;

		cout << " " << param[0] << " " << param[1] << " " << param[2] << endl ;

		char* rotate_x_char = const_cast< char* >( param[0].c_str() ) ;
		char* rotate_y_char = const_cast< char* >( param[1].c_str() ) ;
		char* rotate_z_char = const_cast< char* >( param[2].c_str() ) ;

		if ( operator_tail == NULL ) {

			operator_head = new Rotate( atof( rotate_x_char ), atof( rotate_y_char ), atof( rotate_z_char ) ) ;
			operator_tail = operator_head ;

		} // if

		else {

			operator_tail->next = new Rotate( atof( rotate_x_char ), atof( rotate_y_char ), atof( rotate_z_char ) ) ;
			operator_tail = operator_tail->next ;

		} // else

	} // else if command is rotation should rotate theta

	else if ( command == "observer" ) {

		// cam_x, cam_y, cam_z				-> 0 1 2
		// COI_x, COI_y, COI_z				-> 3 4 5
		// Tilt -> the angle of camera look -> 6
		// Hither -> the very close plane	-> 7
		// Yon -> the very far plane		-> 8
		// Hav -> field of view				-> 9

		char* observer_param[10] ;

		float fov = 0 ;

		for ( int i = 0 ; i < 10 ; i++ ) {

			file >> param[i] ;
			observer_param[i] = const_cast<char*>( param[i].c_str() ) ;

			cout << " " << param[i] ;

		} // for

		cout << endl ;

		Point position = Point( atof( observer_param[0] ), atof( observer_param[1] ), atof( observer_param[2] ) ) ;
		Point COI = Point( atof( observer_param[3] ), atof( observer_param[4] ), atof( observer_param[5] ) ) ;

		fov = atof( observer_param[9] ) ;
		//fov /= 2 ;
		fov = fov * PI / 180 ;

		if ( camera != NULL )
			delete camera ;

		camera = new Camera( position, COI, 
							 atof( observer_param[6] ),
							 atof( observer_param[7] ),
							 atof( observer_param[8] ),
							 fov ) ;

		camera->ComputeGRMVector() ;

	} // else if the command is observer, set parameter of camera

	else if ( command == "viewport" ) {

		// view vxl vxr vyb vyt

		int index = 0 ;

		char *x[2] ;
		char *y[2] ;

		for ( int i = 0 ; i < 4 ; i++ ) {

			file >> param[i] ;

			cout << " " << param[i] ;

		} // for

		cout << endl ;

		for ( int i = 0 ; i < 2 ; i++ ) {

			x[i] = const_cast<char*>( param[i].c_str() ) ;
			y[i] = const_cast<char*>( param[i + 2].c_str() ) ;

		} // for

		Point left_bottom = Point( atof( x[0] ), atof( y[0] ) ) ;
		Point right_top = Point( atof( x[1] ), atof( y[1] ) ) ;

		aspect_ratio = ( right_top.x - left_bottom.x ) / ( right_top.y - left_bottom.y ) ;

		if ( viewport != NULL )
			delete viewport ;

		viewport = new Viewport( left_bottom, right_top ) ;

	} // else if command is viewport, set the parameter of view port

	else if ( command == "ambient" ) {

		cout << endl ;

		char* ambient_char[3] ;

		for ( int i = 0 ; i < 3 ; i++ ) {

			file >> param[i] ;

			ambient_char[i] = const_cast<char*>( param[i].c_str() ) ;
			ambient_coeff[i] = atof( ambient_char[i] ) ;

		} // for

	} // else if

	else if ( command == "background" ) {

		cout << endl ;

		char* background_color[3] ;

		for ( int i = 0 ; i < 3 ; i++ ) {

			file >> param[i] ;

			background_color[i] = const_cast<char*>( param[i].c_str() ) ;

		} // for

		background.r = atof( background_color[0] ) ;
		background.g = atof( background_color[1] ) ;
		background.b = atof( background_color[2] ) ;

	} // else if

	else if ( command == "light" ) {

		LightPtr temp_light = NULL ;

		char* light_param[7] ; // index, Ipr, Ipg, Ipb, Ix, Iy, Iz
							   //   0  ,  1 ,  2 ,  3 , 4 , 5 , 6

		for ( int i = 0 ; i < 7 ; i++ ) {

			file >> param[i] ;

			cout << " " << param[i] ;

			light_param[i] = const_cast<char*>( param[i].c_str() ) ;

		} // for

		cout << endl ;

		temp_light = new Light( atof( light_param[0] ), atof( light_param[4] ), atof( light_param[5] ), atof( light_param[6] ) ) ;
		temp_light->AssignColor( atof( light_param[1] ), atof( light_param[2] ) , atof( light_param[3] ) ) ;

		light = InsertLight( light, temp_light ) ;

	} // else if

} // GlutWindow::DoCommand()