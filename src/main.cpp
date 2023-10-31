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

vector<int> obtenerPosicionesAciertos(const vector<int>& patronSecreto, const vector<int>& jugada) {
    vector<int> posicionesAciertos;
    for (int i = 0; i < patronSecreto.size(); i++) {
        if (patronSecreto[i] == jugada[i]) {
            posicionesAciertos.push_back(i);
        }
    }
    return posicionesAciertos;
}

bool verificarJugada(const vector<int>& patronSecreto, const vector<int>& jugada) {
    int longitud = patronSecreto.size();
    if (patronSecreto.size() != jugada.size()) {
        return false;
    }
    vector<int> posicionesAciertos = obtenerPosicionesAciertos(patronSecreto, jugada);
    int aciertos = posicionesAciertos.size();
    int coincidencias = 0;
    for (int i = 0; i < longitud; i++) {
        bool aciertoEnPosicion = false;
        for (int j = 0; j < posicionesAciertos.size(); j++) {
            if (posicionesAciertos[j] == i) {
                aciertoEnPosicion = true;
                break;
            }
        }
        if (!aciertoEnPosicion) {
            for (int j = 0; j < longitud; j++) {
                if (patronSecreto[i] == jugada[j]) {
                    coincidencias++;
                    break;
                }
            }
        }
    }
    cout << "Aciertos: " << aciertos << " Coincidencias: " << coincidencias << endl;
    if (aciertos > 0) {
        cout << "Posiciones acertadas: ";
        for (int i = 0; i < aciertos; i++) {
            cout << posicionesAciertos[i]+1 << " ";
        }
        cout << endl;
    }
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
        cout << "Crea un patron secreto de " << longitud << " elementos usando numeros del 1 al " << numColores << endl;
        for (int i = 0; i < longitud; i++) {
            int color;
            cin >> color;
            patronSecreto.push_back(color);
        }
        cout << "Patron secreto establecido. Ahora es turno de la computadora." << endl;
        vector<int> jugadaComputadora;
        while (!verificarJugada(patronSecreto, jugadaComputadora)) {
            jugadaComputadora = generarPatronAleatorio(longitud, numColores);
            for (int i = 0; i < 4; i++) {
            cout << jugadaComputadora[i] << " ";
            }
            cout << endl;
        }
        cout << "La computadora adivino el patron!" << endl;
    } else if (modoJuego == 2) {
        vector<int> patronSecreto = generarPatronAleatorio(longitud, numColores);
        cout << "La computadora ha creado un patron secreto. Intenta adivinarlo." << endl;
        vector<int> jugadaUsuario;
        while (true) {
            cout << "Ingresa tu jugada de " << longitud << " elementos usando numeros del 1 al " << numColores << endl;
            jugadaUsuario.clear();
            for (int i = 0; i < longitud; i++) {
                int color;
                cin >> color;
                jugadaUsuario.push_back(color);
            }
            if (verificarJugada(patronSecreto, jugadaUsuario)) {
                cout << "Adivinaste el patron secreto!" << endl;
                break;
            } else {
                cout << "Intentalo de nuevo." << endl;
            }
        }
    } else {
        cout << "Modo de juego no valido. Debe ser 1 o 2." << endl;
    }
    return 0;
}