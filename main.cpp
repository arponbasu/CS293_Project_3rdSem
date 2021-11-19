#include <SFML/Graphics.hpp>
#include <thread>
#include <cmath>
#include <iostream>


static constexpr int IMAGE_WIDTH = 1000;
static constexpr int IMAGE_HEIGHT = 600;
static constexpr long double ZOOM_FACTOR = 0.9;
static constexpr int OFFSET_FACTOR = 40;
//static constexpr int THREAD_COUNT = 2; //Try to keep THREAD_COUNT a divisor of IMAGE_HEIGHT


template <typename T> class myVector{
	T* arr;
	int capacity;
	int current;

public:
	// Default constructor to initialise
	// an initial capacity of 1 element and
	// allocating storage using dynamic allocation
	myVector(){
		arr = new T[4];
		capacity = 4;
		current = 0;
	}

	// Function to add an element at the last
	void push(T data){

		// if the number of elements is equal to the
		// capacity, that means we don't have space to
		// accommodate more elements. We need to double the
		// capacity
		if (current == capacity) {
			T* temp = new T[2 * capacity];

			// copying old array elements to new array
			for (int i = 0; i < capacity; i++) {
				temp[i] = arr[i];
			}

			// deleting previous array
			delete[] arr;
			capacity *= 2;
			arr = temp;
		}

		// Inserting data
		arr[current] = data;
		current++;
	}

	// function to add element at any index
	void push(T data, int index){

		// if index is equal to capacity then this
		// function is same as push defined above
		if (index == capacity)
			push(data);
		else
			arr[index] = data;
	}

	// function to extract element at any index
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

	// function to print array elements
	void print(){
		for (int i = 0; i < current; i++) {
			std::cout << arr[i] << " ";
		}
		std::cout << std::endl;
	}

    T& operator [](int idx) const{
        return arr[idx];
    }


    void multScalar (double k){
        for(int i = 0; i < current; ++i) 
            arr[i] *= k;
    }

    myVector operator + (myVector const &obj) {
         myVector retval;
         for(int i = 0; i < current; ++i) 
            retval.push(arr[i] + obj[i]);
         return retval;
    }

    ~myVector(){
		delete[] arr;

	}

};


size_t string_length (const char *str){
        const char *s;
        for (s = str; *s; ++s);
        return (s - str);
}

void string_copy(char *c, const char* s){
    while (*c++ = *s++);
}

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

    //To erase the first character of the string
    //We simply increment the starting pointer
    myString erase_begin(int num){
        auto l = string_length(this->str);
        char ret[l - num + 1];
        for(int i = 0; i < l - num; ++i)
            ret[i] = this->str[i + num];
        ret[l - num] = '\0';
        return myString(ret);
    }


    void erase_end(int num){
        auto l = string_length(this->str);
        str[l - num] = '\0';
    }

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

// Overloading the plus operator
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

// Function for swapping string
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
// with one arguments
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
 
bool operator!= (const myString &c1, const myString &c2){
    return !(c1== c2);
}

myString convert_decimal_to_string (long double d) {
    
    char strg[20];
    
    snprintf(strg, sizeof(strg), "%Lf", d);
    return myString(strg);

}


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
        return sqrt(x*x + y*y);

    }
    
};



class Mandelbrot {
public:
    Mandelbrot();
    void updateImage(double zoom, double offsetX, double offsetY, sf::Image& image, myString mode) const; 
private:
    static const int MAX = 127; // maximum number of iterations for getNumIterations()
                         // don't increase MAX or the colouring will look strange
    std::array<sf::Color, MAX+1> colors;

    int getNumIterations(const Complex& z) const;
    sf::Color getColor(int iterations) const;
    sf::Color getSmoothColor(const Complex& z) const;
    myVector<double> getVectorColor(int iterations) const;
    void updateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY, myString mode) const;
};

Mandelbrot::Mandelbrot() {
    
    for (int i=0; i <= MAX; ++i) 
        
        colors[i] = getColor(i);
    
}


int Mandelbrot::getNumIterations(const Complex& z) const {
    
    Complex w = z;
    
    for (int counter = 0; counter < MAX; ++counter) {
        
        if (w.absoluteValue() > 2.0) 
            return counter;
        
        
        w = w*w + z;
    }
    return MAX;
}

sf::Color Mandelbrot::getColor(int iterations) const {
    int r, g, b;

    // colour gradient:      Red -> Blue -> Green -> Red -> Black
    // corresponding values:  0  ->  16  ->  32   -> 64  ->  127 (or -1)
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

myVector<double> Mandelbrot::getVectorColor(int iterations) const {
    double r, g, b;
    // colour gradient:      Red -> Blue -> Green -> Red -> Black
    // corresponding values:  0  ->  16  ->  32   -> 64  ->  127 (or -1)
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
    double real = 0 * zoom - IMAGE_WIDTH / 2.0 * zoom + offsetX;
    double imagstart = minY * zoom - IMAGE_HEIGHT / 2.0 * zoom + offsetY;
    for (int x = 0; x < IMAGE_WIDTH; x++, real += zoom) {
        double imag = imagstart;
        for (int y = minY; y < maxY; y++, imag += zoom) {
            Complex omega(real, imag);
	        if(mode == "normal") 
                image.setPixel(x, y, colors[getNumIterations(omega)]);
            
                
	        else if (mode == "exp-res") 
                image.setPixel(x, y, getSmoothColor(omega));
        }
    }
}

void Mandelbrot::updateImage(double zoom, double offsetX, double offsetY, sf::Image& image, myString mode) const{

    std::thread t1(&Mandelbrot::updateImageSlice, *this, zoom, offsetX, 
            offsetY, std::ref(image), 0, IMAGE_HEIGHT/2, mode);
    std::thread t2(&Mandelbrot::updateImageSlice, *this, zoom, offsetX, 
            offsetY, std::ref(image), IMAGE_HEIGHT/2, IMAGE_HEIGHT, mode);
    t1.join();
    t2.join();
}

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
    double offsetX = -0.7; // and move around
    double offsetY = 0.0;
    double zoom = 0.004; // allow the user to zoom in and out...
    double factor = 1.0;
    
    Mandelbrot mb;
    myString mode;
    //myString a("apple"), b("ball"), c = a + b;
    //std::cout << c << std::endl;
    
    if(argc >= 2){
        
        
        myString temp(argv[1]);
        mode = temp;
        
    }
    else
        mode = "exp-res";
    //std::cin >> mode;
    sf::RenderWindow window(sf::VideoMode(IMAGE_WIDTH, IMAGE_HEIGHT), "Mandelbrot");
    
    window.setFramerateLimit(0);
    
    sf::Image image;
    
    image.create(IMAGE_WIDTH, IMAGE_HEIGHT, sf::Color(0, 0, 0));
    
    sf::Texture texture;
    
    sf::Sprite sprite;

    bool stateChanged = true; // track whether the image needs to be regenerated

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
                       
                        case sf::Keyboard::Escape:
                            
                            window.close();
                            break;
                        
                        case sf::Keyboard::Equal: //Zooms in
                            
                            zoom *= ZOOM_FACTOR;
                            factor /= ZOOM_FACTOR;
                            break;
                        
                        case sf::Keyboard::Dash: //Zooms out
                            
                            zoom /= ZOOM_FACTOR;
                            factor *= ZOOM_FACTOR;
                            break;
                        
                        case sf::Keyboard::D: //Figure moves down 
                            
                            offsetY -= OFFSET_FACTOR * zoom;
                            break;
                        
                        case sf::Keyboard::U: //Figure moves up 
                            
                            offsetY += OFFSET_FACTOR * zoom;
                            break;
                        
                        case sf::Keyboard::R: //Figure moves right 
                            
                            offsetX -= OFFSET_FACTOR * zoom;
                            break;
                        
                        
                        case sf::Keyboard::L: //Figure moves right 
                            
                            offsetX += OFFSET_FACTOR * zoom;
                            break;
                        
                        case sf::Keyboard::S: //Saves current image
                            
                            window.capture().saveToFile(generateFileName().cast_to_string());
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

            const myString strg = myString("Mandelbrot: ") + convert_decimal_to_string(factor) + myString("x"); 

            window.setTitle(strg.cast_to_string());	
            
            sprite.setTexture(texture);

            stateChanged = false;
        }
        window.draw(sprite);
        
        
        window.display();
    }
}