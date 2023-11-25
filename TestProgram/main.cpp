#include <iostream>

int main() {
	std::cout << "hello 1\n";
	std::cout << "hello 2\n";

	__debugbreak();

	int a = 10;
	int b = 0;

	int c = a / b;

	return 0;
}