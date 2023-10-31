#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// Función para generar un patrón aleatorio
vector<int> generarPatronAleatorio(int longitud, int numColores) {
    vector<int> patron;
    for (int i = 0; i < longitud; i++) {
        patron.push_back(rand() % numColores + 1);
    }
    return patron;
}

int main() {
    
}
