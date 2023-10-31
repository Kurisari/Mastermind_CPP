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

// Función para verificar la jugada del usuario
bool verificarJugada(const vector<int>& patronSecreto, const vector<int>& jugada) {
    int longitud = patronSecreto.size();
    if (patronSecreto.size() != jugada.size()) {
        return false;
    }
    
    int aciertos = 0;
    int coincidencias = 0;
    for (int i = 0; i < longitud; i++) {
        if (patronSecreto[i] == jugada[i]) {
            aciertos++;
        } else {
            for (int j = 0; j < longitud; j++) {
                if (patronSecreto[i] == jugada[j]) {
                    coincidencias++;
                    break;
                }
            }
        }
    }
    cout << "Aciertos: " << aciertos << " Coincidencias: " << coincidencias << endl;
    return aciertos == longitud;
}

int main() {
    
}
