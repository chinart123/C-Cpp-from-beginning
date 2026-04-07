#include <iostream>


int main()
{
    std::cout << "Please pick a number: 1 or 2 \n";
    int choice{};
    std::cin >> choice; //we gave std::cin '1\n' or '2\n'

    std::cout << "Enter your name: \n";
    std::string name;
    std::getline(std::cin, name); //capture input '\n', because the lack of std::ws
    std::cout << "You name is:" << name << ", you picked " << choice; //std::cout consider '\n' input from std::cin buffer as an 'empty' string
}