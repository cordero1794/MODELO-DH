// Define los pines y parámetros para el motor Z
#define z_paso 4
#define z_dire 7
#define z_habi 8
#define potPin_z A2
#define potCenter_z 384

// Define los pines y parámetros para el motor A
#define a_paso 12
#define a_dire 13
#define a_habi 8
#define potPin_a A1
#define potCenter_a 484

// Define los pines y parámetros para el motor Y
#define y_paso 3
#define y_dire 6
#define y_habi 8
#define potPin_y A0
#define potCenter_y 500

// Define los pines y parámetros para el motor X
#define x_paso 2
#define x_dire 5
#define x_habi 8
#define potPin_x A2// cambiar por a3 cuando tenga el poten
#define potCenter_x 384

int retardo = 1000;  // Menor número, el giro es más rápido

void calibrarMotor(int pasoPin, int direPin, int habiPin, int potPin, int potCenter) {
  int potValue = analogRead(potPin);  // Leer el valor del potenciómetro
  int potDelta = potValue - potCenter;

  digitalWrite(habiPin, LOW);

  if (potDelta < 0) {
    digitalWrite(direPin, LOW);  // Dirección de giro 1
    for (int i = 0; i > potDelta * 8; i--) {
      digitalWrite(pasoPin, HIGH);
      delayMicroseconds(retardo);
      digitalWrite(pasoPin, LOW);
      delayMicroseconds(retardo);
    }
  } else if (potDelta > 0) {
    digitalWrite(direPin, HIGH);  // Dirección de giro en sentido contrario
    for (int i = 0; i < potDelta * 8; i++) {
      digitalWrite(pasoPin, HIGH);
      delayMicroseconds(retardo);
      digitalWrite(pasoPin, LOW);
      delayMicroseconds(retardo);
    }
  }
}

void calibrarMotores() {
  calibrarMotor(z_paso, z_dire, z_habi, potPin_z, potCenter_z);
  calibrarMotor(a_paso, a_dire, a_habi, potPin_a, potCenter_a);
  calibrarMotor(y_paso, y_dire, y_habi, potPin_y, potCenter_y);
  calibrarMotor(x_paso, x_dire, x_habi, potPin_x, potCenter_x);
}

void setup() {
  pinMode(z_paso, OUTPUT);
  pinMode(z_dire, OUTPUT);
  pinMode(z_habi, OUTPUT);
  pinMode(potPin_z, INPUT);  // Configurar el pin del potenciómetro Z como entrada
  pinMode(a_paso, OUTPUT);
  pinMode(a_dire, OUTPUT);
  pinMode(a_habi, OUTPUT);
  pinMode(potPin_a, INPUT);  // Configurar el pin del potenciómetro A como entrada
  pinMode(y_paso, OUTPUT);
  pinMode(y_dire, OUTPUT);
  pinMode(y_habi, OUTPUT);
  pinMode(potPin_y, INPUT);  // Configurar el pin del potenciómetro Y como entrada
  pinMode(x_paso, OUTPUT);
  pinMode(x_dire, OUTPUT);
  pinMode(x_habi, OUTPUT);
  pinMode(potPin_x, INPUT);  // Configurar el pin del potenciómetro X como entrada

  Serial.begin(9600); // Inicializar la comunicación serie

  Serial.println("Calibrando los motores Z, A, Y y X al valor central de los potenciómetros...");
  calibrarMotores();  // Realizar la calibración al inicio del ciclo
}

// Variables para controlar el estado de los motores
bool motor_z_en_movimiento = false;
bool motor_a_en_movimiento = false;
bool motor_y_en_movimiento = false;
bool motor_x_en_movimiento = false;

void loop() {
  digitalWrite(z_habi, HIGH);  // Habilitar el motor Z
  digitalWrite(a_habi, HIGH);  // Habilitar el motor A
  digitalWrite(y_habi, HIGH);  // Habilitar el motor Y
  digitalWrite(x_habi, HIGH);  // Habilitar el motor X

  int potValue_z = analogRead(potPin_z);  // Leer el valor del potenciómetro Z
  int potValue_a = analogRead(potPin_a);  // Leer el valor del potenciómetro A
  int potValue_y = analogRead(potPin_y);  // Leer el valor del potenciómetro Y
  int potValue_x = analogRead(potPin_x);  // Leer el valor del potenciómetro X

  Serial.print("Valor del potenciómetro Z: ");
  Serial.println(potValue_z);
  Serial.print("Valor del potenciómetro A: ");
  Serial.println(potValue_a);
  Serial.print("Valor del potenciómetro Y: ");
  Serial.println(potValue_y);
  Serial.print("Valor del potenciómetro X: ");
  Serial.println(potValue_x);

  // Espera una entrada del usuario a través de la comunicación serial
  if (Serial.available() > 0) {
    char keyPressed = Serial.read();
    if (keyPressed == 'P' || keyPressed == 'p') {
      int degrees_z = -45;  // Mover 30 grados (ajusta según tus necesidades)
      int degrees_a = 20;  // Mover 30 grados (ajusta según tus necesidades)
      int degrees_y = 35;  // Mover 30 grados (ajusta según tus necesidades)
      int degrees_x = -10;  // Mover 30 grados (ajusta según tus necesidades)

      // Cálculo de los grados finales para cada motor
      int newPotValue_z = potValue_z - potCenter_z;
      int newPotValue_a = potValue_a - potCenter_a;
      int newPotValue_y = potValue_y - potCenter_y;
      int newPotValue_x = potValue_x - potCenter_x;

      int finalDegrees_z = newPotValue_z + degrees_z * 33.3;
      int finalDegrees_a = newPotValue_a + degrees_a * 33.3;
      int finalDegrees_y = newPotValue_y + degrees_y * 33.3;
      int finalDegrees_x = newPotValue_x + degrees_x * 33.3;

      // Deshabilita todos los motores
      digitalWrite(z_habi, LOW);
      digitalWrite(a_habi, LOW);
      digitalWrite(y_habi, LOW);
      digitalWrite(x_habi, LOW);

      // Mueve el motor Z hacia adelante
      if (finalDegrees_z != 0) {
        motor_z_en_movimiento = true;
        if (finalDegrees_z < 0) {
          digitalWrite(z_dire, HIGH);  // Dirección de giro en sentido horario
          finalDegrees_z = abs(finalDegrees_z);
        } else {
          digitalWrite(z_dire, LOW);  // Dirección de giro en sentido antihorario
        }
      } else {
        motor_z_en_movimiento = false;
      }

      // Mueve el motor A hacia adelante
      if (finalDegrees_a != 0) {
        motor_a_en_movimiento = true;
        if (finalDegrees_a < 0) {
          digitalWrite(a_dire, HIGH);  // Dirección de giro en sentido horario
          finalDegrees_a = abs(finalDegrees_a);
        } else {
          digitalWrite(a_dire, LOW);  // Dirección de giro en sentido antihorario
        }
      } else {
        motor_a_en_movimiento = false;
      }

      // Mueve el motor Y hacia adelante
      if (finalDegrees_y != 0) {
        motor_y_en_movimiento = true;
        if (finalDegrees_y < 0) {
          digitalWrite(y_dire, HIGH);  // Dirección de giro en sentido horario
          finalDegrees_y = abs(finalDegrees_y);
        } else {
          digitalWrite(y_dire, LOW);  // Dirección de giro en sentido antihorario
        }
      } else {
        motor_y_en_movimiento = false;
      }

      // Mueve el motor X hacia adelante
      if (finalDegrees_x != 0) {
        motor_x_en_movimiento = true;
        if (finalDegrees_x < 0) {
          digitalWrite(x_dire, HIGH);  // Dirección de giro en sentido horario
          finalDegrees_x = abs(finalDegrees_x);
        } else {
          digitalWrite(x_dire, LOW);  // Dirección de giro en sentido antihorario
        }
      } else {
        motor_x_en_movimiento = false;
      }

      // Calcula el máximo de pasos necesarios
      int maxSteps = max(finalDegrees_z, max(finalDegrees_a, max(finalDegrees_y, finalDegrees_x)));
      
      // Realiza los movimientos de los motores
      for (int i = 0; i < maxSteps; i++) {
        if (motor_z_en_movimiento && i < finalDegrees_z) {
          digitalWrite(z_paso, HIGH);
        }
        if (motor_a_en_movimiento && i < finalDegrees_a) {
          digitalWrite(a_paso, HIGH);
        }
        if (motor_y_en_movimiento && i < finalDegrees_y) {
          digitalWrite(y_paso, HIGH);
        }
        if (motor_x_en_movimiento && i < finalDegrees_x) {
          digitalWrite(x_paso, HIGH);
        }
        delayMicroseconds(retardo);
        if (motor_z_en_movimiento && i < finalDegrees_z) {
          digitalWrite(z_paso, LOW);
        }
        if (motor_a_en_movimiento && i < finalDegrees_a) {
          digitalWrite(a_paso, LOW);
        }
        if (motor_y_en_movimiento && i < finalDegrees_y) {
          digitalWrite(y_paso, LOW);
        }
        if (motor_x_en_movimiento && i < finalDegrees_x) {
          digitalWrite(x_paso, LOW);
        }
        delayMicroseconds(retardo);
      }

      // Espera un tiempo antes de regresar los motores a la posición inicial
      delay(3000);

      // Regresa los motores a la posición inicial si estaban en movimiento
      if (motor_z_en_movimiento) {
        // Invertir la dirección del motor Z
        digitalWrite(z_dire, !digitalRead(z_dire));
      }
      if (motor_a_en_movimiento) {
        // Invertir la dirección del motor A
        digitalWrite(a_dire, !digitalRead(a_dire));
      }
      if (motor_y_en_movimiento) {
        // Invertir la dirección del motor Y
        digitalWrite(y_dire, !digitalRead(y_dire));
      }
      if (motor_x_en_movimiento) {
        // Invertir la dirección del motor X
        digitalWrite(x_dire, !digitalRead(x_dire));
      }

      // Realiza los pasos de retorno de los motores
      for (int i = 0; i < maxSteps; i++) {
        if (motor_z_en_movimiento && i < finalDegrees_z) {
          digitalWrite(z_paso, HIGH);
        }
        if (motor_a_en_movimiento && i < finalDegrees_a) {
          digitalWrite(a_paso, HIGH);
        }
        if (motor_y_en_movimiento && i < finalDegrees_y) {
          digitalWrite(y_paso, HIGH);
        }
        if (motor_x_en_movimiento && i < finalDegrees_x) {
          digitalWrite(x_paso, HIGH);
        }
        delayMicroseconds(retardo);
        if (motor_z_en_movimiento && i < finalDegrees_z) {
          digitalWrite(z_paso, LOW);
        }
        if (motor_a_en_movimiento && i < finalDegrees_a) {
          digitalWrite(a_paso, LOW);
        }
        if (motor_y_en_movimiento && i < finalDegrees_y) {
          digitalWrite(y_paso, LOW);
        }
        if (motor_x_en_movimiento && i < finalDegrees_x) {
          digitalWrite(x_paso, LOW);
        }
        delayMicroseconds(retardo);
      }
    }
  }
}


