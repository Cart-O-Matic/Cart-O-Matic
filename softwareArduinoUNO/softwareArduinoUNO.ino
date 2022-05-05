// Librerías
#include <LiquidCrystal.h>
#include <IRremote.hpp>
#include <Servo.h>

// Convenio de pines
// Sensor infrarrojos
int infrarrojo = 7;

// Pantalla
int db7 = 0;
int db6 = 1;
int db5 = 2;
int db4 = 12;
int screenEnable = 4;
int screenRS = 6;
LiquidCrystal lcd(screenRS, screenEnable, db4, db5, db6, db7);

// Motor de abajo
int motorMicroservo = 9;
Servo microservo;

// Motor arriba
int rodilloEnable = 8;
int rodilloDirA = 10;
int rodilloDirB = 11;
// Piezo
int piezo = 5;

// Variables globales
// Nº de jugadores
int numJugadores = 0;
// Nº de cartas disponibles
int numCartas = 0;
// Cuántas a cada uno
int cartasCadaUno = 0;
// Cuando el número de personas no es compatible con 360, es necesario saber a
// cuántas personas hay que repartir con más ángulo
int personasMasAngulo = 0;
// Número de cartas sobrantes tras realizar el reparto (si sobran)
int cartasSobrantes = 0;

// Valores prefijados para giro de microservo
int frecuenciaMicroservoRpm = 20;
int velocidadSentidoCW = 45;
int velocidadSentidoCCW = 135;


// 1 -> Interfaz de gestión de la pantalla LCD
void imprimirYPosicionarLCD(String texto,
                            int posCursorColumnaInicial,
                            int posCursorFilaInicial,
                            int posCursorColumnaFinal,
                            int posCursorFilaFinal) {
    lcd.setCursor(posCursorColumnaInicial, posCursorFilaInicial);
    lcd.print(texto);
    lcd.setCursor(posCursorColumnaFinal, posCursorFilaFinal);
    lcd.cursor();
}

void inicializarLCD() {
    lcd.begin(16, 2);
    lcd.print("Bienvenido...");
    lcd.setCursor(0, 1);
    for (int posicionCursor = 0; posicionCursor < 16; posicionCursor++) {
        lcd.print("*");
        delay(50);
    }
    delay(500);
    lcd.clear();
}

int escribirNumeros(int valorMaximo, String rotulo, bool obligatorio) {
    int valorNumeroFinal = 0;
    int posColumnaCursor = 0;
    bool pulsacionPlay = false;
    imprimirYPosicionarLCD(rotulo, 0, 0, 0, 1);
    IrReceiver.begin(infrarrojo, DISABLE_LED_FEEDBACK);
    while (!pulsacionPlay) {
        if (IrReceiver.decode()) {
            int ultimoValorIntroducido = decodificarNumeros((unsigned long) IrReceiver.decodedIRData.decodedRawData);
            if (ultimoValorIntroducido != -3) {
                if (ultimoValorIntroducido != -1 && ultimoValorIntroducido != -2) {
                    if (!(ultimoValorIntroducido == 0 && posColumnaCursor == 0)) {
                        int nuevoValor = valorNumeroFinal * 10 + ultimoValorIntroducido;
                        if (nuevoValor <= valorMaximo) {
                            valorNumeroFinal = valorNumeroFinal * 10 + ultimoValorIntroducido;
                            posColumnaCursor = posColumnaCursor + 1;
                            imprimirYPosicionarLCD((String) valorNumeroFinal, 0, 1, posColumnaCursor, 1);
                        }
                    }
                    IrReceiver.resume();
                } else if (ultimoValorIntroducido != -2) {
                    if (obligatorio) {
                        if (valorNumeroFinal != 0) {
                            pulsacionPlay = true;
                            lcd.clear();
                            return valorNumeroFinal;
                        } else {
                            IrReceiver.resume();
                        }
                    } else {
                        pulsacionPlay = true;
                        lcd.clear();
                        return valorNumeroFinal;
                    }
                } else {
                    if (posColumnaCursor > 0) {
                        valorNumeroFinal = valorNumeroFinal / 10;
                        posColumnaCursor = posColumnaCursor - 1;
                        imprimirYPosicionarLCD(" ", posColumnaCursor, 1, posColumnaCursor, 1);
                    }
                    IrReceiver.resume();
                }
            } else {
                IrReceiver.resume();
            }
        }
    }
}


// 2 -> Interfaz de gestión de receptor de señales infrarrojas
int decodificarNumeros(unsigned long numeroIR) {
    switch (numeroIR) {
        case 3910598400:
            return 0;
            break;
        case 4077715200:
            return 1;
            break;
        case 3877175040:
            return 2;
            break;
        case 2707357440:
            return 3;
            break;
        case 4144561920:
            return 4;
            break;
        case 3810328320:
            return 5;
            break;
        case 2774204160:
            return 6;
            break;
        case 3175284480:
            return 7;
            break;
        case 2907897600:
            return 8;
            break;
        case 3041591040:
            return 9;
            break;
        case 3208707840: // PLAY
            return -1;
            break;
        case 3141861120: // BACK
            return -2;
            break;
        default:         // OTHER VALUES
            return -3;
            break;
    }
}


// 3 -> Interfaz de control del rodillo
void girarRodillo(int tiempoGiroOriginal) {
    // Se activa giro hacia delante
    digitalWrite(rodilloDirA, HIGH);
    delay(tiempoGiroOriginal);

    // Giro hacia atrás
    digitalWrite(rodilloDirA, LOW);
    digitalWrite(rodilloDirB, HIGH);
    delay(tiempoGiroOriginal + 300);

    // Se desactiva el motor
    digitalWrite(rodilloDirA, LOW);
    digitalWrite(rodilloDirB, LOW);
}


// 4 -> Interfaz de control del Piezo
void sonar(int sonido) {
    // Sonido = 0 => Sonido de éxito
    // Sonido = 1 => Sonido de fallo
    if (sonido == 0) {
        int melodia[] = {659, 8, 659, 8, 0, 8, 659, 8, 0, 8, 523, 8, 659, 8, 784, 4};
        int numNotas = sizeof(melodia) / sizeof(melodia[0]) / 2;
        int tempo = 140;
        for (int notaActual = 0; notaActual < numNotas * 2; notaActual = notaActual + 2) {
            tone(piezo, melodia[notaActual], tempo * 0.9 * melodia[notaActual + 1]);
            delay(tempo);
        }
        noTone(piezo);
    } else if (sonido == 1) {
        int melodia[] = {440, 4, 392, 4, 349, 4};
        int numNotas = sizeof(melodia) / sizeof(melodia[0]) / 2;
        int tempo = 50;
        for (int notaActual = 0; notaActual < numNotas * 2; notaActual = notaActual + 2) {
            tone(piezo, melodia[notaActual], tempo * 0.9 * melodia[notaActual + 1]);
            delay(tempo);
        }
        noTone(piezo);
    }
}


// 5 -> Lógica del reparto
// Obtiene el ángulo para el jugador actual y metiendo el ángulo estándar
void girarBase(int jugadorActual, int valorServo) {
    if (valorServo > 90){
        microservo.write(velocidadSentidoCCW);
    } else {
        microservo.write(velocidadSentidoCW);
    }
    delay(tiempoParaAngulo(anguloProximoGiro(jugadorActual)));
    microservo.write(90);
}

bool combinacionValida() {
    if (numJugadores < 2 || numJugadores > 10) {
        return false;
    }

    if (cartasCadaUno == 0) {
      cartasCadaUno = (int) numCartas / numJugadores;
    }
    int cartasRepartidas = numJugadores * cartasCadaUno;
    cartasSobrantes = numCartas - cartasRepartidas;
    return cartasSobrantes >= 0;
}

// Cálculo del ángulo según el número de jugadores
// Tiene en cuenta el posible desfase producido al tener un número de jugadores
// no divisible por 360 grados (p. ej. 7 jugadores).
int calculoAngulo() {
    float anguloDecimales = 360 / numJugadores;
    float anguloEntero = floor(anguloDecimales);
    personasMasAngulo = (int) (anguloDecimales - anguloEntero) * numJugadores;
    return anguloEntero;
}

int anguloProximoGiro(int jugadorActual) {
    int angulo = calculoAngulo();
    if (jugadorActual <= personasMasAngulo) {
        return angulo + 1;
    } else {
        return angulo;
    }
}

int tiempoParaAngulo(int angulo) {
    float tiempoVueltaMilisegundos = 1.0 / frecuenciaMicroservoRpm * 60000;
    float tiempoGiroAngulo = (tiempoVueltaMilisegundos * angulo) / 360;
    return round(tiempoGiroAngulo);
}


// 6 -> Gestión de errores (UI)
void imprimirPopupPlayContinuar(String mensaje, int sonidoAsociado) {
    imprimirYPosicionarLCD(mensaje, 0, 0, 0, 1);
    imprimirYPosicionarLCD("Play continuar", 0, 1, 14, 1);
    if (sonidoAsociado >= 0) {
        sonar(sonidoAsociado);
    }
    IrReceiver.begin(infrarrojo, DISABLE_LED_FEEDBACK);
    bool pulsacionPlay = false;
    while (!pulsacionPlay) {
        if (IrReceiver.decode()) {
            int ValorIntroducido = decodificarNumeros((unsigned long) IrReceiver.decodedIRData.decodedRawData);
            if (ValorIntroducido == -1) {
                pulsacionPlay = true;
            } else {
                IrReceiver.resume();
            }
        }
    }
    lcd.clear();
}


// 7 -> Inicialización
void inicializarComprobarParametros() {
    bool valido = false;
    do {
        numJugadores = escribirNumeros(99, "Num jugadores:", true);
        numCartas = escribirNumeros(999, "Num cartas:", true);
        cartasCadaUno = escribirNumeros(99, "Cartas x jug:", false);

        valido = combinacionValida();
        if (!valido) imprimirPopupPlayContinuar("Comb imposible", 1);
        lcd.clear();
    } while (!valido);

    // cartasSobrantes se inicializa al validar
    if (cartasCadaUno == 0) cartasCadaUno = (numCartas - cartasSobrantes) / numJugadores;
}

void setup() {
    // Configuración de pines que no dependen de librerías
    pinMode(rodilloDirA, OUTPUT);
    pinMode(rodilloDirB, OUTPUT);

    // Configuración microservo
    microservo.attach(motorMicroservo);
    microservo.write(90);

    // Inicialización de pantalla y de interacción con usuario
    inicializarLCD();
}


// 8 -> Algoritmo cíclico (loop)
void loop() {
    // 1 -> Inicializar parámetros
    // Interacción con el usuario para inicializar valores
    inicializarComprobarParametros();

    // 2 -> Solo se continúa cuando todo ha ido bien
    // Para empezar a repartir tengo que saber con qué angulo voy a hacerlo
    // Puedo tener ángulos diferentes según el nº de jugadores, importante usar bien el personasMasAngulo
    // Jugadores numerados de 1 a n, donde n es el nº de jugadores total
    int jugadorActual = 1;

    // Una vuelta en cada sentido, así que controlo el sentido
    // Empiezo en horario (0) y luego antihorario (180)
    // El servo para con 90
    int valorServo = 0;

    // He dado x vueltas
    int vueltasCompletas = 0;

    // While hasta llegar al nº de cartas para cada uno
    while (vueltasCompletas < cartasCadaUno) {
        // Lanzo carta
        girarRodillo(260);
        cartasSobrantes -= 1;
        // Sujeto repartido, sumo/resto y a otra cosa
        if (valorServo == 0) jugadorActual += 1;
        else if (valorServo == 180) jugadorActual -= 1;
        // Controlo límites
        // Si he terminado una vuelta en sentido horario cambio valor de servo y vamos a restar, pero a este le reparto carta de nuevo
        if (jugadorActual == numJugadores + 1) {
            valorServo = 180;
            vueltasCompletas += 1;
            jugadorActual -= 1;
        } else if (jugadorActual == 0) {
            valorServo = 0;
            vueltasCompletas += 1;
            jugadorActual += 1;
        } else {
            girarBase(jugadorActual, valorServo);
        }
    }
    // Reparto terminado
    imprimirPopupPlayContinuar("Fin", 0);
}
