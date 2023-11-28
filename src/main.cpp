#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

int longitudJuego = 4;
ostringstream oss;

// Sobrecarga de operadores: ** TODO
// E/S de archivos y aleatorios: ** TODO
// E/S con formato: ** TODO
// Funciones y variables estáticas ** TODO

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
    Color* colores;

    MastermindJugada(int len) : longitud(len), colores(new Color[len]) {}
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
    mutable MastermindJugada ultimaJugada;
    mutable bool primerIntento;

public:
    JugadorPC(MastermindJugada codigoSecreto) : Jugador(codigoSecreto), ultimaJugada(longitudJuego) {
        this->codigoSecreto = codigoSecreto;
        primerIntento = true;
    }

    MastermindJugada crearJugada() const override {
        MastermindJugada jugada(longitudJuego);

        cout << "PC prueba con: ";
        oss << "PC prueba con: ";
        if (primerIntento) {
            primerIntento = false;
            srand(static_cast<unsigned>(time(nullptr)));

            for (int i = 0; i < longitudJuego; ++i) {
                jugada.colores[i] = static_cast<Color>(rand() % 6 + 1);
                cout << jugada.colores[i] << " ";
                oss << jugada.colores[i] << " ";
            }
        } else {
            auto posicionesCorrectasAnteriores = posicionesCorrectas(ultimaJugada, codigoSecreto);
            auto posicionesColoresAcertadosAnteriores = posicionesColoresAcertados(ultimaJugada, codigoSecreto);

            for (int i = 0; i < longitudJuego; ++i) {
                if (find(posicionesCorrectasAnteriores.begin(), posicionesCorrectasAnteriores.end(), i) != posicionesCorrectasAnteriores.end()) {
                    jugada.colores[i] = ultimaJugada.colores[i];
                } else if (find(posicionesColoresAcertadosAnteriores.begin(), posicionesColoresAcertadosAnteriores.end(), i) != posicionesColoresAcertadosAnteriores.end()) {
                    jugada.colores[i] = static_cast<Color>(rand() % 6 + 1);
                } else {
                    jugada.colores[i] = static_cast<Color>(rand() % 6 + 1);
                }
                cout << jugada.colores[i] << " ";
                oss << jugada.colores[i] << " ";
            }
        }
        cout << "\n";
        oss << "\n\n";

        ultimaJugada = jugada;
        return jugada;
    }
};

class JugadorUsuario : public Jugador {
public:
    JugadorUsuario(MastermindJugada codigoSecreto) : Jugador(codigoSecreto) {
        this->codigoSecreto = codigoSecreto;
    }

    MastermindJugada crearJugada() const override {
        MastermindJugada jugada(longitudJuego);

        while(true) {
            cout << "Ingrese su jugada (por ejemplo, 1 1 2 3 para seleccionar colores): ";
            oss << "Ingrese su jugada (por ejemplo, 1 1 2 3 para seleccionar colores): ";

            bool correcto = true;

            for (int i = 0; i < longitudJuego; i++) {
                int color;
                cin >> color;
                oss << color << " ";

                if (color < 1 || color > 6)
                    correcto = false;

                if (color == 999)
                    correcto = true;

                jugada.colores[i] = static_cast<Color>(color);
            }

            if (correcto)
                break;

            cout << "Al menos un color no esta en el rango permitido (1-6)\n";
            oss << "Al menos un color no esta en el rango permitido (1-6)\n";
        }
        oss << endl;

        return jugada;
    }
};

class MastermindJuego {
private:
    MastermindJugada ultimaJugada;
    Jugador* jugadorActual;
    int intento = 1;

public:
    MastermindJugada codigoSecreto;

    MastermindJuego(Jugador* jugador) : jugadorActual(jugador), codigoSecreto(longitudJuego), ultimaJugada(longitudJuego) {
        codigoSecreto = jugador->getCodigoSecreto();
    }

    int realizarJugada() {
        if (intento > 10)
            return 1;

        cout << " ----------- " << "Intento " << intento << " ---------- " << endl;
        oss << " ----------- " << "Intento " << intento << " ---------- " << endl;
        ultimaJugada = jugadorActual->crearJugada();

        if (ultimaJugada.colores[0] == static_cast<Color>(999))
            return 2;

        intento++;
        return 0;
    }

    void evaluarJugada() const {
        vector<int> posicionescorrectas = posicionesCorrectas(ultimaJugada, codigoSecreto);
        vector<int> coloresAcertados = posicionesColoresAcertados(ultimaJugada, codigoSecreto);

        if (posicionescorrectas.size() == 4) {
            cout << "Victoria! ";
            oss << "Victoria! ";
            this->mostrarCodigoSecreto();

        } else {
            cout << "Hay " << posicionescorrectas.size() << " color(es) correcto(s) en: ";
            oss << "Hay " << posicionescorrectas.size() << " color(es) correcto(s) en: ";
            for (int pos : posicionescorrectas) {
                cout << pos + 1 << " ";
                oss << pos + 1 << " ";
            }

            cout << "\nHay " << coloresAcertados.size() << " colores acertados en: ";
            oss << "\nHay " << coloresAcertados.size() << " colores acertados en: ";
            for (int col : coloresAcertados) {
                cout << col + 1 << " ";
                oss << col + 1 << " ";
            }

            cout << "\n\n";
            oss << "\n\n";
        }
    }

    bool isGameOver() const {
        return posicionesCorrectas(ultimaJugada, codigoSecreto).size() == 4;
    }

    void mostrarCodigoSecreto() const {
        for (int i = 0; i < longitudJuego; i++) {
            cout << jugadorActual->codigoSecreto.colores[i] << " ";
            oss << jugadorActual->codigoSecreto.colores[i] << " ";
        }

        cout << "era el codigo correcto\n";
        oss << "era el codigo correcto\n";
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
                oss.str("");
                oss.clear();

                cout << "Bienvenido a Mastermind. Intenta adivinar el codigo secreto.\n\n";
                oss << "Bienvenido a Mastermind. Intenta adivinar el codigo secreto.\n\n";

                MastermindJugada codigoPC(longitudJuego);
                int opcionCodigo = 0;

                while (opcionCodigo != 1 && opcionCodigo != 2) {
                    cout << "PC crea un codigo aleatorio (1) / Usuario crea un codigo (2): ";
                    oss << "PC crea un codigo aleatorio (1) / Usuario crea un codigo (2): ";
                    cin >> opcionCodigo;
                    oss << opcionCodigo << endl;

                    switch(opcionCodigo) {
                        case 1:
                            srand(static_cast<unsigned>(time(nullptr)));

                            for (int i = 0; i < longitudJuego; ++i)
                                codigoPC.colores[i] = static_cast<Color>(rand() % 6 + 1);
                            break;
                        case 2:
                            while(true) {
                                cout << "Ingresa un codigo secreto (por ejemplo, 1 1 2 3). El usuario tratara de adivinarlo: ";
                                oss << "Ingresa un codigo secreto (por ejemplo, 1 1 2 3). El usuario tratara de adivinarlo: ";
                                bool correcto = true;

                                for (int i = 0; i < longitudJuego; i++) {
                                    int color;
                                    cin >> color;
                                    oss << color << " ";

                                    if (color < 1 || color > 6)
                                        correcto = false;

                                    codigoPC.colores[i] = static_cast<Color>(color);
                                }

                                if (correcto)
                                    break;

                                cout << "Al menos un color no esta en el rango permitido (1-6)\n";
                                oss << "Al menos un color no esta en el rango permitido (1-6)\n";
                            }
                            cout << "\n";
                            oss << "\n";

                            break;
                        default:
                            cout << "Ingresa una opcion valida" << endl;
                            break;
                    }
                }

                cout << " > Ingresa (999) en la primera opcion para salir" << endl;

                JugadorUsuario humanPlayer(codigoPC);
                Jugador* jugador = &humanPlayer;
                MastermindJuego juego(jugador);
                int estado;

                while (!juego.isGameOver()) {
                    estado = juego.realizarJugada();

                    if (estado == 1 || estado == 2)
                        break;

                    juego.evaluarJugada();
                }

                if (estado == 1) {
                    cout << "No pudiste adivinar en menos de 10 intentos :(\n";
                    oss << "No pudiste adivinar en menos de 10 intentos :(\n";
                    juego.mostrarCodigoSecreto();
                    cout << endl;
                    oss << endl;

                } else if (estado == 0) {
                    cout << "Felicidades! Has adivinado el codigo secreto.\n\n";
                    oss << "Felicidades! Has adivinado el codigo secreto.\n\n";
                }

                break;
            }

            case 2: {
                MastermindJugada codigoUsuario(longitudJuego);
                oss.str("");
                oss.clear();

                cout << "Bienvenido a Mastermind. La computadora adivinara el codigo secreto.\n\n";
                oss << "Bienvenido a Mastermind. La computadora adivinara el codigo secreto.\n\n";

                while(true) {
                    cout << "Ingresa un codigo secreto (por ejemplo, 1 1 2 3). La computadora tratara de adivinarlo: ";
                    oss << "Ingresa un codigo secreto (por ejemplo, 1 1 2 3). La computadora tratara de adivinarlo: ";
                    bool correcto = true;

                    for (int i = 0; i < longitudJuego; i++) {
                        int color;
                        cin >> color;
                        oss << color << " ";

                        if (color < 1 || color > 6)
                            correcto = false;

                        codigoUsuario.colores[i] = static_cast<Color>(color);
                    }

                    if (correcto)
                        break;

                    cout << "Al menos un color no esta en el rango permitido (1-6)\n";
                    oss << "Al menos un color no esta en el rango permitido (1-6)\n";
                }
                cout << "\n";
                oss << "\n";

                JugadorPC jugadorPC(codigoUsuario);
                Jugador* jugador = &jugadorPC;
                MastermindJuego juego(jugador);
                int estado;

                while (!juego.isGameOver()) {
                    estado = juego.realizarJugada();

                    if (estado == 1)
                        break;

                    juego.evaluarJugada();
                }

                if (estado == 1) {
                    cout << "La computadora no pudo adivinar tu codigo secreto :(\n";
                    oss << "La computadora no pudo adivinar tu codigo secreto :(\n";
                    juego.mostrarCodigoSecreto();
                    cout << endl;
                    oss << endl;
                } else {
                    cout << "La computadora adivino tu codigo!\n\n";
                    oss << "La computadora adivino tu codigo!\n\n";
                }
                break;
            }

            case 3: {
                cout << "Archivo binario" << endl;
                break;
            }

            case 4: {
                cout << "Guardando en un archivo .txt ..." << endl;
                string str = oss.str();
                ofstream file("output.txt");
                file << str;

                cout << "Archivo guardado!" << endl;

                oss.str("");
                oss.clear();
                break;
            }

            case 5: {
                cout << "Saliendo...";
                break;
            }

            default:
                cout << "Opcion invalida. Ingresa una opcion del menu." << endl;
                break;
        }

    } while (modoJuego != 5);

    return 0;
}
