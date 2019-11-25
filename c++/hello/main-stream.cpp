#include <iostream>
#include <iomanip>

int
main(int argc, char **argv)
{
	const char *s = "Hello";
	float v1 = 12.345, v2 = 1234.5;

	std::cout << "Hello, World!" << std::endl;

	std::cout << s << " " << v1 << " " << v2 << " " << std::endl;

	std::cout.setf ( std::ios_base::right, std::ios_base::basefield );
	std::cout.width ( 12 );
	std::cout << s;

	std::cout << " ";

	std::cout.setf ( std::ios_base::right, std::ios_base::floatfield );
	std::cout.width ( 9 );
	std::cout.precision ( 2 );
	std::cout << "(" << v1 << ":" << v2 << ")" << std::endl;
	std::cout << "changing precision (3): ";
	std::cout.precision ( 3 );
	std::cout << "(" << v1 << ":" << v2 << ")" << std::endl;
	std::cout << "changing precision (5): ";
	std::cout.precision ( 5 );
	std::cout << "(" << v1 << ")";
	std::cout << "(" << v2 << ")" << std::endl;

	return 0;
}

