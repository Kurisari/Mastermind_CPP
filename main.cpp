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
    srand(time(0)); // Inicializar la semilla aleatoria
    int longitud = 4; // Longitud del patrón
    int numColores = 6; // Número de colores disponibles
    int modoJuego;
    cout << "Elige el modo de juego (1: Usuario crea, 2: Computadora crea): ";
    cin >> modoJuego;
    if (modoJuego == 1) {
        vector<int> patronSecreto;
        cout << "Crea un patrón secreto de " << longitud << " elementos usando números del 1 al " << numColores << endl;
        for (int i = 0; i < longitud; i++) {
            int color;
            cin >> color;
            patronSecreto.push_back(color);
        }
        cout << "Patrón secreto establecido. Ahora es turno de la computadora." << endl;
        vector<int> jugadaComputadora;
        while (!verificarJugada(patronSecreto, jugadaComputadora)) {
            jugadaComputadora = generarPatronAleatorio(longitud, numColores);
        }
        cout << "¡La computadora adivinó el patrón!" << endl;
    } else if (modoJuego == 2) {
        vector<int> patronSecreto = generarPatronAleatorio(longitud, numColores);
        cout << "La computadora ha creado un patrón secreto. Intenta adivinarlo." << endl;
        vector<int> jugadaUsuario;
        while (true) {
            cout << "Ingresa tu jugada de " << longitud << " elementos usando números del 1 al " << numColores << endl;
            jugadaUsuario.clear();
            for (int i = 0; i < longitud; i++) {
                int color;
                cin >> color;
                jugadaUsuario.push_back(color);
            }
            if (verificarJugada(patronSecreto, jugadaUsuario)) {
                cout << "¡Adivinaste el patrón secreto!" << endl;
                break;
            } else {
                cout << "Inténtalo de nuevo." << endl;
            }
        }
    } else {
        cout << "Modo de juego no válido. Debe ser 1 o 2." << endl;
    }
    return 0;
}
