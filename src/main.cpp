#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

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

vector<int> obtenerPosicionesCoincidencias(const vector<int>& patronSecreto, const vector<int>& jugada){
    vector<int> posicionesCoincidencias;
    for (int i = 0; i < patronSecreto.size(); i++) {
        for (int j = 0; j < jugada.size(); j++) {
            if (patronSecreto[i] == jugada[j]) {
                posicionesCoincidencias.push_back(i);
            }
        }
    }
    return posicionesCoincidencias;
}

bool verificarJugada(const vector<int>& patronSecreto, const vector<int>& jugada) {
    int longitud = patronSecreto.size();
    if (patronSecreto.size() != jugada.size()) {
        return false;
    }
    vector<int> posicionesAciertos = obtenerPosicionesAciertos(patronSecreto, jugada);
    vector<int> posicionesCoincidencias = obtenerPosicionesCoincidencias(patronSecreto, jugada);
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
            cout << posicionesAciertos[i] + 1 << " ";
        }
        cout << endl;
    }
    if(coincidencias > 0) {
        cout << "Posiciones coincidencias: ";
        for (int i = 0; i < coincidencias; i++) {
            cout << posicionesCoincidencias[i] + 1 << " ";
        }
        cout << endl;
    }
    return aciertos == longitud;
}

// Función para evaluar la coincidencia entre dos vectores
pair<int, int> evaluarCoincidencias(const vector<int>& patron, const vector<int>& intento) {
    int coincidencias = 0;
    int noCoincidencias = 0;
    for (size_t i = 0; i < patron.size(); ++i) {
        if (patron[i] == intento[i]) {
            coincidencias++;
        } else if (count(patron.begin(), patron.end(), intento[i]) > 0) {
            noCoincidencias++;
        }
    }
    return make_pair(coincidencias, noCoincidencias);
}

// Función para adivinar el patrón
vector<int> adivinarPatron(const vector<int>& patronSecreto, int longitud, int numColores) {
    vector<int> intento(longitud, 1);  // Comenzar con un intento inicial
    int intentos = 0;  // Contador de intentos
    // Mientras no se haya adivinado el patrón, seguir haciendo intentos
    while (true) {
        // Mostrar el intento actual
        cout << "Intento de la computadora: ";
        for (int num : intento) {
            cout << num << " ";
        }
        cout << endl;
        // Evaluar la coincidencia con el patrón
        auto coincidencias = evaluarCoincidencias(patronSecreto, intento);
        // Mostrar las coincidencias
        cout << "Coincidencias: " << coincidencias.first << ", No coincidencias: " << coincidencias.second << endl;
        // Si todas las posiciones coinciden, se ha adivinado el patrón
        if (coincidencias.first == longitud) {
            break;
        }
        // Generar un nuevo intento basado en las coincidencias y no coincidencias
        for (size_t i = 0; i < intento.size(); ++i) {
            if (coincidencias.first > 0) {
                if (patronSecreto[i] == intento[i]) {
                    // Coincidencia, mantener el número actual
                } else {
                    // No coincidencia, probar con un número diferente
                    intento[i] = rand() % numColores + 1;
                }
            } else {
                // No hay coincidencias, probar con un número diferente
                intento[i] = rand() % numColores + 1;
            }
        }
        intentos++;
        // Limitar el número de intentos para evitar bucles infinitos
        if (intentos >= 10) {
            cout << "La computadora no pudo adivinar el patron en 10 intentos." << endl;
            break;
        }
    }
    return intento;
}

int main() {
    srand(time(0)); // Inicializar la semilla aleatoria
    int longitud = 4; // Longitud del patrón
    int numColores = 6; // Número de colores disponibles
    int modoJuego;
    cout << "Elige el modo de juego (1: Usuario crea, 2: Computadora crea): ";
    cin >> modoJuego;
    int intentos = 0; // Contador de intentos
    if (modoJuego == 1) {
        // Modo en que el usuario crea el patrón y la computadora adivina
        vector<int> patronSecreto;
        cout << "Crea un patron secreto de " << longitud << " elementos usando números del 1 al " << numColores << endl;
        for (int i = 0; i < longitud; i++) {
            int color;
            cin >> color;
            patronSecreto.push_back(color);
        }
        cout << "Patron secreto establecido. Ahora es el turno de la computadora." << endl;
        vector<int> jugadaComputadora = adivinarPatron(patronSecreto, longitud, numColores);
        
        if (jugadaComputadora == patronSecreto) {
            cout << "La computadora adivino el patrón." << endl;
        } else {
            cout << "La computadora no pudo adivinar el patrón." << endl;
        }
    } else if (modoJuego == 2) {
        bool continuar = true;
        vector<int> patronSecreto = generarPatronAleatorio(longitud, numColores);
        cout << "La computadora ha creado un patron secreto. Intenta adivinarlo." << endl;
        vector<int> jugadaUsuario;
        while (continuar && intentos < 10) {
            cout << "Ingresa tu jugada de " << longitud << " elementos usando numeros del 1 al " << numColores << endl;
            jugadaUsuario.clear();
            for (int i = 0; i < longitud; i++) {
                int color;
                cin >> color;
                jugadaUsuario.push_back(color);
            }
            if (verificarJugada(patronSecreto, jugadaUsuario)) {
                cout << "Adivinaste el patron secreto!" << endl;
                continuar = false;
                break;
            } else {
                cout << "Intentalo de nuevo." << endl;
            }
            intentos++;
        }
        if (intentos >= 10) {
            cout << "No pudiste adivinar el patron en 10 intentos. El patron secreto era: ";
            for (int i = 0; i < longitud; i++) {
                cout << patronSecreto[i] << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Modo de juego no valido. Debe ser 1 o 2." << endl;
    }
    return 0;
}