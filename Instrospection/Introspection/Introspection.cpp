#include <iostream>
#include "ClassReader.h"
class Ejemplo {

publish:
    void holaGente();
    int tontoDelCulo(int tonto);

};

void Ejemplo::holaGente() {

    std::cout << "Eres completamente tonto" << std::endl;
}

int Ejemplo::tontoDelCulo(int tonto) {

    return tonto + 1;
}

int main()
{

    auto hola = ClassReader::getMethods("Introspection.cpp");

    std::cout << hola[0].name << std::endl;


}
