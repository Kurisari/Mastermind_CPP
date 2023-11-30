#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

int longitudJuego = 4; // Longitud de cada jugada
ostringstream oss; // stringstream para guardar en archivos

// Enumeracion con todos los colores y su correspondiente numero
enum Color {  // <---- Tema: Estructuras, uniones y enumeraciones
    ROJO = 1,
    VERDE,
    AZUL,
    AMARILLO,
    NEGRO,
    BLANCO
};

// Estructura para representar cada jugada
struct MastermindJugada { // <---- Tema: Estructuras, uniones y enumeraciones
    int longitud;
    Color* colores; // <---- Tema: Arrays y cadenas

    MastermindJugada(int len) : longitud(len), colores(new Color[len]) {}
};

// Función para evaluar y retornar las posiciones correctas en una jugada   <---- Tema: Funciones y sobrecarga
vector<int> posicionesCorrectas(MastermindJugada ultimaJugada, MastermindJugada codigoSecreto) {
    vector<int> correctPositions;
    for (int i = 0; i < longitudJuego; ++i)
        if (ultimaJugada.colores[i] == codigoSecreto.colores[i]) // Guarda las posiciones que coincidan
            correctPositions.push_back(i);

    return correctPositions;
}

// Función para evaluar y retornar las posiciones con colores correctos, pero en el lugar incorrecto en una jugada
vector<int> posicionesColoresAcertados(MastermindJugada ultimaJugada, MastermindJugada codigoSecreto) {
    vector<int> correctColors;

    // Calcula y guarda las posiciones con colores correctos
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

// Clase para representar cada jugador
class Jugador { // <---- Tema: POO, clases y objetos
    static int intentos; // <---- Tema: Funciones y variables estaticas
public:
    MastermindJugada codigoSecreto;

    Jugador(MastermindJugada codigoSecreto) : codigoSecreto(codigoSecreto) {}

    virtual MastermindJugada crearJugada() const = 0; // <---- Tema: Herencia y funciones virtuales

    MastermindJugada getCodigoSecreto() {
        return codigoSecreto;
    };
};

// Clase para representar a la PC como jugador
class JugadorPC : public Jugador {
    mutable MastermindJugada ultimaJugada;
    mutable bool primerIntento;
    static int intentos; // <---- Tema: Funciones y variables estaticas

public:
    // Cosntructor para JugadorPC
    JugadorPC(MastermindJugada codigoSecreto) : Jugador(codigoSecreto), ultimaJugada(longitudJuego) {
        this->codigoSecreto = codigoSecreto;
        primerIntento = true;
    }

    // Funcion para que la computadora cree una jugada de acuerdo con las posiciones correctas
    MastermindJugada crearJugada() const override { // <---- Tema: Funciones y sobrecarga
        MastermindJugada jugada(longitudJuego);

        cout << "PC prueba con: ";
        oss << "PC prueba con: ";
        if (primerIntento) { // En el primer intento, prueba con una jugada aleatoria
            primerIntento = false;
            srand(static_cast<unsigned>(time(nullptr)));

            for (int i = 0; i < longitudJuego; ++i) {
                jugada.colores[i] = static_cast<Color>(rand() % 6 + 1);
                cout << jugada.colores[i] << " "; // <---- Tema: Arrays y cadenas
                oss << jugada.colores[i] << " ";
            }
        } else {
            auto posicionesCorrectasAnteriores = posicionesCorrectas(ultimaJugada, codigoSecreto);
            auto posicionesColoresAcertadosAnteriores = posicionesColoresAcertados(ultimaJugada, codigoSecreto);

            for (int i = 0; i < longitudJuego; ++i) { // Calcula una nueva juagda usando las posiciones y colores correctos
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

// Clase para representar al usuario como jugador
class JugadorUsuario : public Jugador { // <---- Tema: Herencia y funciones virtuales
public:
    // Constructor para el JugadorUsuario
    JugadorUsuario(MastermindJugada codigoSecreto) : Jugador(codigoSecreto) {
        this->codigoSecreto = codigoSecreto;
    }

    // Funcion para que el usuario ingrese una jugada
    MastermindJugada crearJugada() const override { // <---- Tema: Polimorfismo
        MastermindJugada jugada(longitudJuego);

        while(true) {
            cout << "Ingrese su jugada (por ejemplo, 1 1 2 3 para seleccionar colores): ";
            oss << "Ingrese su jugada (por ejemplo, 1 1 2 3 para seleccionar colores): ";

            bool correcto = true;

            for (int i = 0; i < longitudJuego; i++) {
                int color;
                cin >> color;
                oss << color << " ";

                if (color < 1 || color > 6) // Comprobacion para un color correcto
                    correcto = false;

                if (color == 999) // Comprobacion para codigo de escape
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

// Clase para representar el juego completo
class MastermindJuego { // <---- Tema: POO, clases y objetos
private:
    MastermindJugada ultimaJugada; // Ultima jugada ingresada por Usuario o PC
    Jugador* jugadorActual; // Jugador, que puede ser Usuario o PC
    int intento = 1; // Contador de intentos

public:
    MastermindJugada codigoSecreto; // Codigo de colores secreto

    // Constructor para el juego
    MastermindJuego(Jugador* jugador) : jugadorActual(jugador), codigoSecreto(longitudJuego), ultimaJugada(longitudJuego) {
        codigoSecreto = jugador->getCodigoSecreto();
    }

    // Funcion para ingresar una jugada
    int realizarJugada() {
        if (intento > 10) // Maximo de 10 intentos
            return 1;

        cout << " ----------- " << "Intento " << intento << " ---------- " << endl;
        oss << " ----------- " << "Intento " << intento << " ---------- " << endl;
        ultimaJugada = jugadorActual->crearJugada(); // <---- Tema: Polimorfismo

        if (ultimaJugada.colores[0] == static_cast<Color>(999)) // Codigo de escape
            return 2;

        intento++;
        return 0;
    }

    // Funcion para mostrar los colores acertados y posiciones correctas
    void evaluarJugada() const {
        vector<int> posicionescorrectas = posicionesCorrectas(ultimaJugada, codigoSecreto);
        vector<int> coloresAcertados = posicionesColoresAcertados(ultimaJugada, codigoSecreto);

        if (posicionescorrectas.size() == 4) {
            cout << "\nVictoria! ";
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

    // Funcion para comprobar si todavia no se adivina el color correcto
    bool isGameOver() const {
        return posicionesCorrectas(ultimaJugada, codigoSecreto).size() == 4;
    }

    // Funcion para imprimir el codigo correcto
    void mostrarCodigoSecreto() const {
        for (int i = 0; i < longitudJuego; i++) {
            cout << jugadorActual->codigoSecreto.colores[i] << " ";
            oss << jugadorActual->codigoSecreto.colores[i] << " ";
        }

        cout << "era el codigo correcto\n";
        oss << "era el codigo correcto\n";
    }

    MastermindJuego operator<<(const MastermindJuego& otro) const {  // <---- Tema: Sobrecarga de operadores
        return (reinterpret_cast<Jugador *>(intento + otro.intento));
    }
};

// Funcion principal
int main() {
    int modoJuego;

    do {  // <---- Tema: Estructuras de control
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

        switch(modoJuego) { // <---- Tema: Estructuras de control
            case 1: { // Opcion 1: PC crea, usuario descifra
                oss.str(""); // Nuevo juego = Nuevo archivo (stream)
                oss.clear();

                cout << "Bienvenido a Mastermind. Intenta adivinar el codigo secreto.\n\n";
                oss << "Bienvenido a Mastermind. Intenta adivinar el codigo secreto.\n\n";

                MastermindJugada codigoPC(longitudJuego);
                int opcionCodigo = 0;

                while (opcionCodigo != 1 && opcionCodigo != 2) { // PC crea el codigo o Usuario crea el codigo
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
                Jugador* jugador = &humanPlayer; // <---- Tema: Punteros y referencias
                MastermindJuego juego(jugador);
                int estado;

                // Continuar jugando mientras se cumplan las condiciones
                while (!juego.isGameOver()) {
                    estado = juego.realizarJugada();

                    if (estado == 1 || estado == 2)
                        break;

                    juego.evaluarJugada();
                }

                // Si estado = 1, llegaste al maximo de intentos
                if (estado == 1) {
                    cout << "No pudiste adivinar en menos de 10 intentos :(\n";
                    oss << "No pudiste adivinar en menos de 10 intentos :(\n";
                    juego.mostrarCodigoSecreto();
                    cout << endl;
                    oss << endl;

                } else if (estado == 0) { // Si estado = 0, adivinaste el codigo correcto
                    cout << "Felicidades! Has adivinado el codigo secreto.\n\n";
                    oss << "Felicidades! Has adivinado el codigo secreto.\n\n";
                }

                break;
            }

            case 2: { // Opcion para que la computadora adivine el codigo
                MastermindJugada codigoUsuario(longitudJuego);
                oss.str(""); // Nuevo juego = Nuevo archivo (stream)
                oss.clear();

                cout << "Bienvenido a Mastermind. La computadora adivinara el codigo secreto.\n\n";
                oss << "Bienvenido a Mastermind. La computadora adivinara el codigo secreto.\n\n";

                while(true) { // Ingresar un codigo para que la computadora trate de adivinarlo
                    cout << "Ingresa un codigo secreto (por ejemplo, 1 1 2 3). La computadora tratara de adivinarlo: ";
                    oss << "Ingresa un codigo secreto (por ejemplo, 1 1 2 3). La computadora tratara de adivinarlo: ";
                    bool correcto = true;

                    for (int i = 0; i < longitudJuego; i++) {
                        int color;
                        cin >> color;
                        oss << color << " ";

                        if (color < 1 || color > 6) // Verificar que el color este en el rango correcto
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
                Jugador* jugador = &jugadorPC; // <---- Tema: Punteros y referencias
                MastermindJuego juego(jugador);
                int estado;

                // Continuar jugando mientras se cumplan las condiciones
                while (!juego.isGameOver()) {
                    estado = juego.realizarJugada();

                    if (estado == 1)
                        break;

                    juego.evaluarJugada();
                }

                // Si estado = 1, llegaste al maximo de intentos
                if (estado == 1) {
                    cout << "La computadora no pudo adivinar tu codigo secreto :(\n";
                    oss << "La computadora no pudo adivinar tu codigo secreto :(\n";
                    juego.mostrarCodigoSecreto();
                    cout << endl;
                    oss << endl;
                } else { // Si estado = 0, adivinaste el codigo correcto
                    cout << "La computadora adivino tu codigo!\n\n";
                    oss << "La computadora adivino tu codigo!\n\n";
                }
                break;
            }

            case 3: { // Opcion para crear el archivo binario de la ultima jugada
                cout << "Guardando en un archivo binario ..." << endl;
                ofstream archivo("archivo.bin", ios::binary); // <---- Tema: E/S de archivos y aleatorios
                string str = oss.str(); // Convertir el stream en string

                // Escribe el tamaño del string en el archivo
                size_t size = str.size();
                archivo.write(reinterpret_cast<char*>(&size), sizeof(size));

                // Escribe el string en el archivo
                archivo.write(str.c_str(), str.size());

                cout << setw(20) << setfill('-') << "" << endl; // <---- Tema: E/S con formato
                cout << "Archivo guardado!" << endl;
                cout << setw(20) << setfill('-') << "";
                cout << "\n\n";

                // Cierra el archivo
                archivo.close();
                break;
            }

            case 4: { // Opcion para crear un archivo txt de la ultima jugada
                cout << "Guardando en un archivo .txt ..." << endl;
                string str = oss.str(); // Convertir el stream en string
                ofstream file("output.txt"); // Crear el archivo
                file << str; // <---- Tema: E/S de archivos y aleatorios

                cout << setw(20) << setfill('-') << "" << endl; // <---- Tema: E/S con formato
                cout << "Archivo guardado!" << endl;
                cout << setw(20) << setfill('-') << "";
                cout << "\n\n";
                break;
            }

            case 5: { // Opcion para salir
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
