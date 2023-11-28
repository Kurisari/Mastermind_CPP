#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int longitudJuego = 4;

// Arrays y cadenas: listo
// Estructuras de control: listo
// Funciones y sobrecarga: listo
// Punteros y referencias: listo
// Estructuras, uniones y enumeraciones: listo
// POO, clases y objetos: listo
// Sobrecarga de operadores: **** TODO
// E/S de archivos y aleatorios: **** TODO
// E/S con formato: **** TODO
// Herencia y funciones virtuales: listo
// Polimorfismo: listo
// Funciones y variables estáticas **** TODO

enum Color {
    ROJO = 1,
    VERDE,
    AZUL,
    AMARILLO,
    NEGRO,
    BLANCO
};

struct MastermindJugada {
    int longitud;
    vector<int> colores;

    MastermindJugada(int len) : longitud(len), colores(len) {}
    MastermindJugada(const vector<int>& colores) : longitud(colores.size()), colores(colores) {}
};


vector<int> posicionesCorrectas(MastermindJugada ultimaJugada, MastermindJugada codigoSecreto) {
    vector<int> correctPositions;
    for (int i = 0; i < longitudJuego; ++i)
        if (ultimaJugada.colores[i] == codigoSecreto.colores[i])
            correctPositions.push_back(i);

    return correctPositions;
}

vector<int> posicionesColoresAcertados(MastermindJugada ultimaJugada, MastermindJugada codigoSecreto) {
    vector<int> correctColors;

    for (int i = 0; i < longitudJuego; ++i) {
        for (int j = 0; j < longitudJuego; ++j) {
            if (i != j && ultimaJugada.colores[i] == codigoSecreto.colores[j]) {
                correctColors.push_back(i);
                break;
            }
        }
    }
    return correctColors;
}

class Jugador {
public:
    MastermindJugada codigoSecreto;

    Jugador(MastermindJugada codigoSecreto) : codigoSecreto(codigoSecreto) {}

    virtual MastermindJugada crearJugada() const = 0;

    MastermindJugada getCodigoSecreto() {
        return codigoSecreto;
    };
};

class JugadorPC : public Jugador {
public:
    JugadorPC(MastermindJugada codigoSecreto) : Jugador(codigoSecreto) {
        this->codigoSecreto = codigoSecreto;
    }

    MastermindJugada crearJugada() const override {
        MastermindJugada jugada(longitudJuego);

        cout << "Computadora ingresa: ";
        for (int i = 0; i < longitudJuego; ++i) {
            jugada.colores[i] = static_cast<Color>(rand() % 6 + 1);
            cout << jugada.colores[i] << " ";
        }
        cout << endl;

        return jugada;
    }
    MastermindJugada adivinarPatron(const MastermindJugada& patronSecreto) const {
        vector<int> intento(longitudJuego, 1);  // Comenzar con un intento inicial
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
            auto coincidencias = evaluarCoincidencias(patronSecreto.colores, intento);
            // Mostrar las coincidencias
            cout << "Coincidencias: " << coincidencias.first << ", No coincidencias: " << coincidencias.second << endl;
            // Si todas las posiciones coinciden, se ha adivinado el patrón
            if (coincidencias.first == longitudJuego) {
                break;
            }
            // Generar un nuevo intento basado en las coincidencias y no coincidencias
            for (size_t i = 0; i < intento.size(); ++i) {
                if (coincidencias.first > 0) {
                    if (patronSecreto.colores[i] == intento[i]) {
                        // Coincidencia, mantener el número actual
                    } else {
                        // No coincidencia, probar con un número diferente
                        intento[i] = rand() % 6 + 1;
                    }
                } else {
                    // No hay coincidencias, probar con un número diferente
                    intento[i] = rand() % 6 + 1;
                }
            }
            intentos++;
            // Limitar el número de intentos para evitar bucles infinitos
            if (intentos >= 10) {
                cout << "La computadora no pudo adivinar el patron en 10 intentos." << endl;
                break;
            }
        }
        return MastermindJugada(intento);
    }
private:
    // Función para evaluar la coincidencia entre dos vectores
    pair<int, int> evaluarCoincidencias(const vector<int>& patron, const vector<int>& intento) const {
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
};



class JugadorUsuario : public Jugador {
public:
    JugadorUsuario(MastermindJugada codigoSecreto) : Jugador(codigoSecreto) {
        srand(static_cast<unsigned>(time(nullptr)));

        for (int i = 0; i < longitudJuego; ++i)
            codigoSecreto.colores[i] = static_cast<Color>(rand() % 6 + 1);
    }

    MastermindJugada crearJugada() const override {
        MastermindJugada jugada(longitudJuego);

        while(true) {
            cout << "Ingrese su jugada (por ejemplo, 1 1 2 3 para seleccionar colores): ";

            bool correcto = true;

            for (int i = 0; i < longitudJuego; i++) {
                int color;
                cin >> color;

                if (color < 1 || color > 6)
                    correcto = false;

                jugada.colores[i] = static_cast<Color>(color);
            }

            if (correcto)
                break;

            cout << "Al menos un color no esta en el rango permitido (1-6)" << endl;
        }

        return jugada;
    }
};

class MastermindJuego {
private:
    MastermindJugada codigoSecreto;
    MastermindJugada ultimaJugada;
    Jugador* jugadorActual;
    int intento = 1;

public:
    MastermindJuego(Jugador* jugador) : jugadorActual(jugador), codigoSecreto(longitudJuego), ultimaJugada(longitudJuego) {
        codigoSecreto = jugador->getCodigoSecreto();
    }

    int realizarJugada() {
        if (intento > 10)
            return 1;

        cout << " ----------- " << "Intento " << intento << " ---------- " << endl;
        ultimaJugada = jugadorActual->crearJugada();
        intento++;
        return 0;
    }

    void evaluarJugada() const {
        vector<int> posicionescorrectas = posicionesCorrectas(ultimaJugada, codigoSecreto);
        vector<int> coloresAcertados = posicionesColoresAcertados(ultimaJugada, codigoSecreto);

        if (posicionescorrectas.size() == 4) {
            cout << "Ganaste! ";
            this->mostrarCodigoSecreto();

        } else {
            cout << "Hay " << posicionescorrectas.size() << " color(es) correcto(s) en: ";
            for (int pos : posicionescorrectas)
                cout << pos << " ";

            cout << "\nHay " << coloresAcertados.size() << " colores acertados en: ";
            for (int col : coloresAcertados)
                cout << col << " ";

            cout << "\n\n";
        }
    }

    bool isGameOver() const {
        return posicionesCorrectas(ultimaJugada, codigoSecreto).size() == 4;
    }

    void mostrarCodigoSecreto() const {
        for (int i = 0; i < codigoSecreto.longitud; i++)
            cout << codigoSecreto.colores[i] << " ";

        cout << "era el codigo correcto" << endl;
    }
    
};


int main() {
    int modoJuego;

    do {
        cout << "----------------- Menu ------------------" << endl;
        cout << "1. PC crea el codigo, usuario descifra" << endl;
        cout << "2. Usuario crea el codigo, PC descifra" << endl;
        cout << "3. Listar el ultimo juego" << endl;
        cout << "4. Mandar el ultimo juego a un .txt" << endl;
        cout << "5. Salir" << endl;
        cout << "-----------------------------------------" << endl;
        cout << "Ingresa una opcion: ";
        cin >> modoJuego;
        cout << endl;

        switch(modoJuego) {
            case 1: {
                JugadorUsuario humanPlayer(MastermindJugada(0));
                Jugador* jugador = &humanPlayer;
                MastermindJuego juego(jugador);
                int estado;

                cout << "Bienvenido a Mastermind. Intenta adivinar el codigo secreto.\n\n";

                while (!juego.isGameOver()) {
                    estado = juego.realizarJugada();

                    if (estado == 1)
                        break;

                    juego.evaluarJugada();
                }

                if (estado == 1) {
                    cout << "No pudiste adivinar en menos de 10 intentos :(" << endl;
                    juego.mostrarCodigoSecreto();
                } else {
                    cout << "Felicidades! Has adivinado el codigo secreto.\n\n";
                }

                break;
            }

            case 2: {
                MastermindJugada codigoUsuario(longitudJuego);

                while(true) {
                    cout << "Ingresa un codigo secreto (por ejemplo, 1 1 2 3). La computadora tratara de adivinarlo: ";
                    bool correcto = true;

                    for (int i = 0; i < longitudJuego; i++) {
                        int color;
                        cin >> color;

                        if (color < 1 || color > 6)
                            correcto = false;

                        codigoUsuario.colores[i] = static_cast<Color>(color);
                    }

                    if (correcto)
                        break;

                    cout << "Al menos un color no esta en el rango permitido (1-6)" << endl;
                }
                cout << endl;

                JugadorPC jugadorPC(codigoUsuario);
                Jugador* jugador = &jugadorPC;
                MastermindJuego juego(jugador);
                // int estado;

                // while (!juego.isGameOver()) {
                //     estado = juego.realizarJugada();

                //     if (estado == 1)
                //         break;

                //     juego.evaluarJugada();
                // }

                // if (estado == 1) {
                //     cout << "La computadora no pudo adivinar tu codigo secreto :(" << endl;
                //     juego.mostrarCodigoSecreto();
                //     cout << endl;
                // } else {
                //     cout << "La computadora adivino tu codigo!.\n\n";
                // }
                // Adivinar el patrón usando la nueva función
                MastermindJugada intentoAdivinado = jugadorPC.adivinarPatron(codigoUsuario);
                cout << "La computadora adivino el patron: ";
                for (int num : intentoAdivinado.colores) {
                    cout << num << " ";
                }
                cout << endl;
                break;
            }

            case 3: {
                cout << "Archivo binario" << endl;
                break;
            }

            case 4: {
                cout << "Archivo TXT" << endl;
                break;
            }

            case 5: {
                cout << "Saliendo...";
                break;
            }

            default:
                cout << "Opcion invalida. Ingresa una opcion del menu.";
                break;
        }

    } while (modoJuego != 5);

    return 0;
}