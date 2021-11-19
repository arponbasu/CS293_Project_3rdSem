//The SFML package
#include <SFML/Graphics.hpp>

//Threads were used for better performance. But the program also works well enough with a single chain of execution
//Thus this library is not necessary for the execution of the program, but just an additional edge to it 
#include <thread>

//Only the sqrt and exp functions were used from this library, that too for implementing a single coloring algorithm
//Thus the program can technically work with <thread> and <cmath> !
#include <cmath>

//Width of window
static constexpr int IMAGE_WIDTH = 1000;

//Height of window : Change it if you may, but keep it divisible by 2
static constexpr int IMAGE_HEIGHT = 600;

//Amount by which the fractal is zoomed in or out in a single key press
static constexpr long double ZOOM_FACTOR = 0.9;

//Step size by which fractal moves left or right in a single key press
static constexpr int OFFSET_FACTOR = 40;

//Threshold after which we declare a particular point in the Argand plane has diverged
//You may decrease this threshold for more "fragmented" fractals
static constexpr double MANDELBROT_THRESHOLD = 2.0;

//General templated growable vector class 
template <typename T> class myVector{

    //The underlying array beneath the structure
	T* arr; 

    //Maximum capacity of the array. Is dynamically changed
	int capacity;

    //Size of array
	int current;

public:
	// Default constructor
	// Initial capacity = 4, grows and shrinks as required
	// allocating storage using heap allocation
	myVector(){
		arr = new T[4];
		capacity = 4;
		current = 0;
	}

	// Equivalent of push_back in STL
	void push(T data){

		// if the number of elements is equal to the
		// capacity, that means we don't have space to
		// accommodate more elements. We double the
		// capacity, according to the DSA lectures
		if (current == capacity) {
			T* temp = new T[2 * capacity];

			// copying old array elements to new array
			for (int i = 0; i < capacity; i++) 
				temp[i] = arr[i];
			

			// deallocating old memory
			delete[] arr;
			capacity *= 2;
			arr = temp;
		}

		// Inserting data
		arr[current] = data;
		current++;
	}

	// function to add element at any index (rank based insertion). push function overloaded twice
	void push(T data, int index){

		// if index is equal to capacity then this
		// function is same as push defined above
		if (index == capacity)
			push(data);
		else
			arr[index] = data;
	}

	// function to extract element at any index
    // Also implemented below by overloading the [] operator 
	T get(int index){

		// if index is within the range
		if (index < current)
			return arr[index];
        
	}

	// function to delete last element
	void pop() { 
        current--; 
    }

	// function to get size of the vector
	int size() { 
        return current; 
    }

	// function to get capacity of the vector
	int getcapacity() { 
        return capacity; 
    }

	
    //Operator overload of the [] operator
    //Essentially getter method for arr without allowing for modification 
    // (Modification through the push function only)
    T& operator [](int idx) const{
        return arr[idx];
    }

    //Multiplies vector by given scalar 
    void multScalar (double k){
        for(int i = 0; i < current; ++i) 
            arr[i] *= k;
    }

    //Addition operator for two vectors. It's assumed that they're of equal sizes
    myVector operator + (myVector const &obj) {
         myVector retval;
         for(int i = 0; i < current; ++i) 
            retval.push(arr[i] + obj[i]);
         return retval;
    }

    //Destructor. Default destructors don't work as memory is heap allocated
    //Memory deallocation needs to happen explicitly
    ~myVector(){
		delete[] arr;

	}

};

//Growable stack data structure
template <typename T> class myStack {

    T* arr;
	int capacity;
	int current;

public:
	// Default constructor to initialise
	// an initial capacity of 1 element and
	// allocating storage using dynamic allocation
	myStack(){
		arr = new T[4];
		capacity = 4;
		current = 0;
	}

	// Function to add an element at the last
	void push(T data){

		
		if (current == capacity) {
			T* temp = new T[2 * capacity];

			// copying old array elements to new array
			for (int i = 0; i < capacity; i++) 
				temp[i] = arr[i];
			

			// deleting previous array
			delete[] arr;
			capacity *= 2;
			arr = temp;
		}

		// Inserting data
		arr[current] = data;
		current++;
	}

	

	// function to delete last element
	void pop() { 
        current--; 
    }

	// function to get size of the stack
	int size() { 
        return current; 
    }

	// function to get capacity of the stack
	int getcapacity() { 
        return capacity; 
    }

	//Returns topmost element
    //Note how class abstraction is used to prevent access at any index
    T top (){
        return arr[current - 1];
    }

    //Destructor
    ~myStack(){
		delete[] arr;

	}



};


//Small utility function to calculate length of null terminated string
size_t string_length (const char *str){
        const char *s;
        for (s = str; *s; ++s);
        return (s - str);
}

//Utility function to copy string from one location to the other
void string_copy(char *c, const char* s){
    while (*c++ = *s++);
}

//Equivalent of strcat in <cstring.h>
char * my_strcat(char *dest, const char *src){
    char *rdest = dest;

    while (*dest)
      dest++;
    while (*dest++ = *src++);
    return rdest;
}

// Class myString
class myString {

	// Prototype for stream insertion
	friend std::ostream& operator<<(std::ostream& os, const myString& obj);

	// Prototype for stream extraction
	friend std::istream& operator>>(std::istream& is, myString& obj);

	// Prototype for '+'
	// operator overloading
	friend myString operator+ (const myString& lhs, const myString& rhs);
	
    //The actual char* array behind the class
    char* str;

public:
	// No arguments constructor
	myString();

	// pop_back() function
	void pop_bk();

	// push_back() function
	void push_bk(char a);

	// To get the length
	int get_length();

	// Function to copy the string
	// of length len from position pos
	void cpy(char s[], int len, int pos);

	// Swap strings function
	void swp(myString& rhs);

	// Constructor with 1 arguments
	myString(char* val);

	// Copy Constructor
	myString(const myString& source);

	

	// Overloading the assignment
	// operator
	myString& operator=(const myString& rhs);


    //The equality operator defined as a friend function
    friend bool operator== (const myString &c1, const myString &c2);

    //The not equality operator, also defined as a friend function
    //It's the negation of the above function
    friend bool operator!= (const myString &c1, const myString &c2);

    //Getter function for the char[] array
    char* ret_string() const{
        return str;
    }

    //To erase the first characters of the string
    //Note that we have essentially established deque like methods here (insertion and deletion from both ends)
    myString erase_begin(int num){
        auto l = string_length(this->str);
        char ret[l - num + 1];
        for(int i = 0; i < l - num; ++i)
            ret[i] = this->str[i + num];
        ret[l - num] = '\0';
        return myString(ret);
    }

    //To delete characters from the end (we simply replace desired position with '\0')
    void erase_end(int num){
        auto l = string_length(this->str);
        str[l - num] = '\0';
    }

    //std:string casting function
    //required as some SFML methods only accept const std::string& arguments
    std::string cast_to_string () const{
        const char* ret = this->str;
        return std::string(this->str);
    }

    
	// Destructor
	~myString() { 
        delete[] str; 
    }
};

// Overloading the assignment operator
myString& myString::operator=(const myString& rhs){
	
    if (this == &rhs)
		return *this;
	delete[] str;
	str = new char[string_length(rhs.str) + 1];
	string_copy(str, rhs.str);
	return *this;

}

// Overloading the addition operator
myString operator+ (const myString& lhs, const myString& rhs){
	int length = string_length(lhs.str) + string_length(rhs.str);

	char* buff = new char[length + 1];

	// Copy the strings to buff[]
	string_copy(buff, lhs.str);
	my_strcat(buff, rhs.str);

	// String temp
	myString temp{ buff };

	// delete the buff[]
	delete[] buff;

	// Return the concatenated string
	return temp;
}

// Overloading the stream
// extraction operator
// This is for integration with iostream
std::istream& operator>>(std::istream& is, myString& obj){
	char* buff = new char[1000];
	is >> buff;
	obj = myString{ buff };
	delete[] buff;
	return is;
}

// Overloading the stream
// insertion operator
std::ostream& operator<<(std::ostream& os, const myString& obj){
	os << obj.str;
	return os;
}

// Function for swapping strings
void myString::swp(myString& rhs){
	myString temp{ rhs };
	rhs = *this;
	*this = temp;
}

// Function to copy the string
void myString::cpy(char s[], int len, int pos){
	
    for (int i = 0; i < len; i++) {
		s[i] = str[pos + i];
	}
	s[len] = '\0';
}

// Function to implement push_bk
void myString::push_bk(char a){
	// Find length of string
	int length = string_length(this->str);

	char* buff = new char[length + 2];

	// Copy character from str
	// to buff[]
	for (int i = 0; i < length; i++) 

        buff[i] = this->str[i];
	
	buff[length] = a;
	buff[length + 1] = '\0';

	// Assign the new string with
	// char a to string str
	*this = myString{ buff };

	// Delete the temp buff[]
	delete[] buff;
}

// Function to implement pop_bk
void myString::pop_bk(){

	int length = string_length(str);
	char* buff = new char[length];

	// Copy character from str
	// to buff[]
	for (int i = 0; i < length - 1; i++)
		buff[i] = str[i];
	buff[length] = '\0';

	// Assign the new string with
	// char a to string str
	*this = myString{ buff };

	// delete the buff[]
	delete[] buff;
}

// Function to implement get_length
int myString::get_length(){
	return string_length(this->str);
}

// Function to illustrate Constructor
// with no arguments
myString::myString(): str{ nullptr }{
	this->str = new char[1];
	this->str[0] = '\0';
}

// Function to illustrate Constructor
// with one arguments (essentially initializing as myString var("abchfg"))
myString::myString(char* val){
	
    if (val == nullptr) {
		this->str = new char[1];
		this->str[0] = '\0';
	}

	else {

		this->str = new char[string_length(val) + 1];

		// Copy character of val[]
		// using string_copy
		string_copy(this->str, val);
	}
}

// Function to illustrate
// Copy Constructor
myString::myString(const myString& source){
	
    this->str = new char[string_length(source.str) + 1];
	string_copy(this->str, source.str);
}

//Necessary for checking "modes" later
bool operator== (const myString &c1, const myString &c2){
    bool b1 = (string_length(c1.str) == string_length(c2.str));
    if(!b1)
        return false;
    for(int i = 0; i < string_length(c1.str); ++i){
        if(c1.str[i] != c2.str[i])
            return false;
    }
    return true;

}
 
//!= comes for free with ==
bool operator!= (const myString &c1, const myString &c2){
    return !(c1 == c2);
}

//double to myString casting function
myString convert_decimal_to_string (long double d) {
    
    char strg[20];
    
    snprintf(strg, sizeof(strg), "%Lf", d);
    return myString(strg);

}

//our complex class
class Complex{
    double x, y;
    public:

    //No argument constructor
    Complex(){

    }

    //Default destructor
    ~Complex(){
        
    }

    //Setter type constructor
    Complex(double x, double y){
        this->x = x;
        this->y = y;
    }


    //Overloaded the addition operator
    Complex operator+ (const Complex& z) const{
        Complex retval;
        retval.x = this->x + z.x;
        retval.y = this->y + z.y;
        return retval;
    }

    //Overloaded the subtraction operator
    Complex operator- (const Complex& z) const{
        Complex retval;
        retval.x = this->x - z.x;
        retval.y = this->y - z.y;
        return retval;
    }

    //Overloaded the multiplication operator
    Complex operator* (const Complex& z) const{
        Complex retval;
        retval.x = (this->x)*(z.x) - (this->y)*(z.y);
        retval.y = (this->x)*(z.y) + (this->y)*(z.x);
        return retval;
    }

    //Overloaded the assignment operator
    Complex operator=(const Complex& rhs){
        this->x = rhs.x;
        this->y = rhs.y;
        return *this;
    }

    //Getter method
    double getX(){
        return x;
    }

    //Getter method
    double getY(){
        return y;
    }

    //Absolute value function
    double absoluteValue (){
        return sqrt(x*x + y*y); //<cmath> needed for only this line

    }
    
};


//Actual class where the implementation of the fractal drawing happens
class Mandelbrot {
public:
    //Initializes the color array
    Mandelbrot();

    //This method allows for the screen to be changed on suitable calls
    void updateImage(double zoom, double offsetX, double offsetY, sf::Image& image, myString mode) const; 
private:
    static const int MAX = 127; // maximum number of iterations for getNumIterations()
                         // don't increase MAX or the colouring will look strange
    
    //Colors are assigned based upon the number of iterations (signified by the indices of the array)
    //See getColor method for exact details
    std::array<sf::Color, MAX+1> colors;
    
    //Calculates number of iterations taken by a complex number for it's absolute value to exceed 2
    int getNumIterations(const Complex& z) const;

    //Assigns color based upon number of iterations. Basis for the "normal" mode
    sf::Color getColor(int iterations) const;

    //Implements an exponential-based algorithm for better picture quality.
    //Basis for the "exp-res" mode
    sf::Color getSmoothColor(const Complex& z) const;

    //Utility method for helping the getSmoothColor method
    myVector<double> getVectorColor(int iterations) const;

    //Implements gradient based coloring scheme. Basis for "gradient", "monochrome" and "smoky" modes
    sf::Color getGradientColor(int iterations, double r, double g, double b) const;

    //Function for updating any given patch of the window
    //updateImage calls this method for different patches (executed by different threads)
    void updateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY, myString mode) const;
};

//Constructor initializes "color chart"
Mandelbrot::Mandelbrot() {
    
    for (int i=0; i <= MAX; ++i) 
        
        colors[i] = getColor(i);
    
}

//Based on the mandelbrot equation w = w^2 + z, where z is our point of interest and w an iterator
int Mandelbrot::getNumIterations(const Complex& z) const {
    
    Complex w = z;
    
    for (int counter = 0; counter < MAX; ++counter) {
        
        if (w.absoluteValue() > MANDELBROT_THRESHOLD) //Threshold for crossing
            return counter;
        
        
        w = w*w + z;
    }
    return MAX;
}

//Coloring algorithm for "gradient", "monochrome" and "smoky" modes
//Intensity of color depends upon the number of iteration taken to diverge
//The step color (difference in intensities b/w iter and iter + 1) is determined by r,g,b
//r, g, b are double numbers b/w 0 and 1
//(0,0,0) is white, (1,1,1) is black
sf::Color Mandelbrot::getGradientColor(int iterations, double r, double g, double b) const {
    
    double c = 255*std::min(MAX - iterations,MAX)/double(MAX);
    return sf::Color(c*r,c*g,c*b);





}

//Color chart for number of iterations
sf::Color Mandelbrot::getColor(int iterations) const {
    int r, g, b;

    // colour gradient:      Red | Blue | Green | Red | Black
    // corresponding values:  0  |  16  |  32   | 64  |  127 (or -1)
    if (iterations < 16) {
        r = 16 * (16 - iterations);
        g = 0;
        b = 16 * iterations - 1;
    } 
    else if (iterations < 32) {
        r = 0;
        g = 16 * (iterations - 16);
        b = 16 * (32 - iterations) - 1;
    } 
    else if (iterations < 64) {
        r = 8 * (iterations - 32);
        g = 8 * (64 - iterations) - 1;
        b = 0;
    } 
    else { // range is 64 - 127
        r = 255 - (iterations - 64) * 4;
        g = 0;
        b = 0;
    }
    return sf::Color(r, g, b);
}


//same method as above, but repackages answer in diffenrent data type for convenient further use in the getSmoothColor methods
myVector<double> Mandelbrot::getVectorColor(int iterations) const {
    double r, g, b;
    
    
    if(iterations <= 0){
        r = 0.0;
        g = 0.0;
        b = 0.0;

    } 
    else if (0 < iterations && iterations < 16) {
        r = 16.0 * (16 - iterations);
        g = 0.0;
        b = 16.0 * iterations - 1;
    } 
    else if (iterations < 32) {
        r = 0.0;
        g = 16.0 * (iterations - 16);
        b = 16.0 * (32 - iterations) - 1;
    } 
    else if (iterations < 64) {
        r = 8.0 * (iterations - 32);
        g = 8.0 * (64 - iterations) - 1;
        b = 0.0;
    } 
    else { // range is 64 - 127
        r = 255.0 - (iterations - 64) * 4;
        g = 0.0;
        b = 0.0;
    }
    myVector<double> v;
    v.push(r);
    v.push(g);
    v.push(b);
    return v;
}




//Implements an algorithm for smoothening out edges when a color change occurs
sf::Color Mandelbrot::getSmoothColor(const Complex& z) const {

    Complex w = z;
    double expiter = 0;
    int iter = MAX;
    for (int counter = 0; counter < MAX; ++counter) {
        
        if (w.absoluteValue() > 2.0) {
            
            iter = counter;
            break;
        }
        
        
        auto w_old = w;
        
        w = w*w + z;
        
        Complex diff = w - w_old;
        //An exponential interploation b/w the previous value and the current one is carried out
        //This ensures boundaries don't stand out too sharply
        //Takes a bit more time than other coloring algorithms
        //owing to the expensive nature of the exp function from the <cmath> library
        expiter += exp(-w.absoluteValue()-0.5/(diff.absoluteValue()));
    }
    auto toValue = getVectorColor(iter);
    auto fromValue = getVectorColor(std::min(iter + 1, MAX));
    toValue.multScalar(expiter);
    fromValue.multScalar(1 - expiter);
    auto x = toValue + fromValue;
    return sf::Color(int(x[0]),int(x[1]),int(x[2]));
}


void Mandelbrot::updateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY, myString mode) const{
    
    //Some coordinate geometry locate the patch based on parameters defined at beginning of the file
    double real = 0 * zoom - IMAGE_WIDTH / 2.0 * zoom + offsetX;
    double imagstart = minY * zoom - IMAGE_HEIGHT / 2.0 * zoom + offsetY;
    
    for (int x = 0; x < IMAGE_WIDTH; x++, real += zoom) {
        
        double imag = imagstart;
        
        for (int y = minY; y < maxY; y++, imag += zoom) {
            //Point to be colored
            Complex omega(real, imag);
            
            //Normal color chart based algorithm
	        if(mode == "normal") 
                image.setPixel(x, y, colors[getNumIterations(omega)]);
            
            //Exponential smoothing of above algorithm
	        else if (mode == "exp-res") 
                image.setPixel(x, y, getSmoothColor(omega));

            //Grey gradient based coloring algorithm
            //Boundaries are even smoother here than the exponential interpolation algorithm
            else if (mode == "gradient") 
                image.setPixel(x, y, getGradientColor(getNumIterations(omega),1.0,1.0,1.0));

            // Random scrambling of non black pixels to annihilate boundaries altogether
            else if (mode == "smoky"){ 
                double c1 = (double)rand() / (double)RAND_MAX;
                double c2 = (double)rand() / (double)RAND_MAX;
                double c3 = (double)rand() / (double)RAND_MAX;
                image.setPixel(x, y, getGradientColor(getNumIterations(omega),c1,c2,c3));
            }

            //Gradient algorithm on a pinkish background
            //Feel free to change this
            else if (mode == "monochrome"){
                image.setPixel(x, y, getGradientColor(getNumIterations(omega),0.7,0.3,0.6));
            }
                
        }
    }
}


//Function which constructs together an image out of updateImage Slice outputs from individual patches
//Currently only 2 threads, but any thread_count (dividing IMAGE_HEIGHT to prevent integer division issues) from
//1 - 10 should work
void Mandelbrot::updateImage(double zoom, double offsetX, double offsetY, sf::Image& image, myString mode) const{

    std::thread t1(&Mandelbrot::updateImageSlice, *this, zoom, offsetX, 
            offsetY, std::ref(image), 0, IMAGE_HEIGHT/2, mode);
    std::thread t2(&Mandelbrot::updateImageSlice, *this, zoom, offsetX, 
            offsetY, std::ref(image), IMAGE_HEIGHT/2, IMAGE_HEIGHT, mode);
    t1.join();
    t2.join();
}

//Utility function for creating file names after screenshots are taken
myString generateFileName(){

    auto end = std::chrono::system_clock::now();
    
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    
    myString strg(std::ctime(&end_time));
    
    strg = strg.erase_begin(11);
    
    strg.erase_end(6);
    
    strg = myString("Mandelbrot_") + strg + myString(".png");
    
    return strg;
}

int main(int argc, char* argv[]) {

    srand((unsigned)time(NULL));
    double offsetX = -0.7; // and move around
    double offsetY = 0.0;
    double zoom = 0.004; // allow the user to zoom in and out...
    double factor = 1.0;
    
    Mandelbrot mb;
    myString mode;
    
        
    if(argc == 2){
        
        
        myString temp(argv[1]);
        mode = temp;
        
    }
    


    else
        mode = "exp-res"; //Default mode in case of no argument is set to exp-res mode
    
    
    sf::RenderWindow window(sf::VideoMode(IMAGE_WIDTH, IMAGE_HEIGHT), "Mandelbrot");
    
    window.setFramerateLimit(0);
    
    sf::Image image;
    
    image.create(IMAGE_WIDTH, IMAGE_HEIGHT, sf::Color(0, 0, 0));
    
    sf::Texture texture;
    
    sf::Sprite sprite;

    bool stateChanged = true; // track whether the image needs to be regenerated
    
    myStack<char> cmd_log; //Command log stack 
    
    while (window.isOpen()) {
        sf::Event event;
        
        while (window.pollEvent(event)) {
            
            switch (event.type) {
                
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::KeyPressed:
                    
                    stateChanged = true; // image needs to be recreated when the user changes zoom or offset
                    
                    
                    switch (event.key.code) {
                       
                        case sf::Keyboard::Escape: //esc button closes the window
                            
                            window.close();
                            break;
                        
                        case sf::Keyboard::Equal: //Zooms in
                            
                            zoom *= ZOOM_FACTOR;
                            factor /= ZOOM_FACTOR;
                            cmd_log.push('=');
                            break;
                        
                        case sf::Keyboard::Dash: //Zooms out
                            
                            zoom /= ZOOM_FACTOR;
                            factor *= ZOOM_FACTOR;
                            cmd_log.push('-');
                            break;
                        
                        case sf::Keyboard::D: //Figure moves down 
                            
                            offsetY -= OFFSET_FACTOR * zoom;
                            cmd_log.push('D');
                            break;
                        
                        case sf::Keyboard::U: //Figure moves up 
                            
                            offsetY += OFFSET_FACTOR * zoom;
                            cmd_log.push('U');
                            break;
                        
                        case sf::Keyboard::R: //Figure moves right 
                            
                            offsetX -= OFFSET_FACTOR * zoom;
                            cmd_log.push('R');
                            break;
                        
                        
                        case sf::Keyboard::L: //Figure moves left 
                            
                            offsetX += OFFSET_FACTOR * zoom;
                            cmd_log.push('L');
                            break;
                        
                        case sf::Keyboard::S: //Saves current image
                            
                            window.capture().saveToFile(generateFileName().cast_to_string());
                            break;

                        case sf::Keyboard::Z: //Function for undoing previous actions
                        //This undo needs our stack data structure
                            if(cmd_log.size() != 0){
                                if(cmd_log.top() == '='){
                                    cmd_log.pop();
                                    zoom /= ZOOM_FACTOR;
                                    factor *= ZOOM_FACTOR;
                                }
                                else if(cmd_log.top() == '-'){
                                    cmd_log.pop();
                                    zoom *= ZOOM_FACTOR;
                                    factor /= ZOOM_FACTOR;
                                }
                                else if(cmd_log.top() == 'D'){
                                    cmd_log.pop();
                                    offsetY += OFFSET_FACTOR * zoom;
                                }
                                else if(cmd_log.top() == 'U'){
                                    cmd_log.pop();
                                    offsetY -= OFFSET_FACTOR * zoom;
                                }
                                else if(cmd_log.top() == 'R'){
                                    cmd_log.pop();
                                    offsetX += OFFSET_FACTOR * zoom;
                                }
                                else if(cmd_log.top() == 'L'){
                                    cmd_log.pop();
                                    offsetX -= OFFSET_FACTOR * zoom;
                                }
                                break;
                            }
                            break;
                        
                        default: 
                            
                            stateChanged = false;
                            break;
                    }
                default:
                    break;
            }
        }

        if (stateChanged) { 

            mb.updateImage(zoom, offsetX, offsetY, image, mode);

            texture.loadFromImage(image);

            texture.setSmooth(true);

            //This displays current zoom level on the top of the screen
            const myString strg = myString("Mandelbrot: ") + convert_decimal_to_string(factor) + myString("x"); 

            window.setTitle(strg.cast_to_string());	
            
            sprite.setTexture(texture);

            stateChanged = false;
        }
        window.draw(sprite);
        
        
        window.display();
    }
}