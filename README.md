
#  Simulador de Fila de Banco (Eventos Discretos)

Este repositorio contiene un simulador estocástico programado en lenguaje C, diseñado para modelar el comportamiento, tiempo de espera y atención de clientes en una sucursal bancaria. 

El núcleo del programa opera sobre una estructura de datos de cola (FIFO), la cual es reordenada dinámicamente mediante un algoritmo de inserción para respetar una jerarquía de atención estricta.
##  Equipo de Desarrollo

Proyecto Integrador desarrollado en la **Universidad del Valle de México (UVM) - Campus Reforma**, para la carrera de Ingeniería en Sistemas Computacionales.

![Carlos](https://img.shields.io/badge/Carlos-Líder_del_Equipo-1f77b4?logo=github&labelColor=555)
![Hugo](https://img.shields.io/badge/Hugo-Especialista_de_Lógica-2ea44f?logo=github&labelColor=555)
![Arturo](https://img.shields.io/badge/Arturo-Especialista_de_Lógica-2ea44f?logo=github&labelColor=555)
![Santiago](https://img.shields.io/badge/Santiago-Analista_de_Datos-e36209?logo=github&labelColor=555)
![Omar](https://img.shields.io/badge/Omar-Desarrollador_de_Interfaz-d73a49?logo=github&labelColor=555)
![Seth](https://img.shields.io/badge/Seth-Desarrollador_de_Interfaz-d73a49?logo=github&labelColor=555)
##  Características Técnicas

* **Generación Estocástica:** La llegada de clientes se calcula mediante probabilidad porcentual iterada minuto a minuto.
* **Sistema de Prioridades:** Clasificación de clientes en 3 niveles con diferentes atributos de servicio:
  1. **VIP:** Prioridad alta, menor tiempo de atención, baja tolerancia de espera (abandona a los 8 min).
  2. **Nuevo:** Prioridad media, tolerancia de espera moderada (abandona a los 6 min).
  3. **Normal:** Prioridad estándar, mayor tiempo de atención, alta tolerancia de espera (abandona a los 10 min).
* **Agentes Autónomos (Abandono):** Algoritmo de deserción que purga de la cola a los clientes cuyo tiempo de espera supera su límite de tolerancia.
* **Visualización en Tiempo Real:** Renderizado de una gráfica ASCII en consola (utilizando caracteres de bloque `█` y `░`) para observar la saturación de la fila de manera dinámica.
* **Persistencia de Datos:** Exportación automática de métricas operativas. Al finalizar la simulación, el sistema genera:
  * `logs_banco.csv`: Desglose tabular listo para análisis e importación en software de hojas de cálculo.
  * `logs_banco.txt`: Reporte estructurado en texto plano.

---

##  Compilación y Ejecución

El código fuente está contenido en un único archivo, lo que facilita su portabilidad. Requiere un compilador de C estándar (como GCC o MinGW en Windows).

### Desde la terminal (Windows):

```bash
# 1. Clonar el repositorio
git clone [https://github.com/Spectrum080/Simulador-Fila-de-Banco.git](https://github.com/Spectrum080/Simulador-Fila-de-Banco.git)

# 2. Entrar al directorio
cd Simulador-Fila-de-Banco

# 3. Compilar el archivo fuente
gcc SISTEMAFILABANCOV1.2.c -o simulador_banco.exe

# 4. Ejecutar el simulador
simulador_banco.exe

```

---

##  Estructura de Exportación de Datos

Al finalizar la ejecución de los 400 minutos, los archivos de registro se guardarán en el mismo directorio (CWD) desde donde se lanzó el ejecutable. Las métricas capturadas incluyen:

* Total de clientes generados y clasificados por tipo.
* Total de deserciones (clientes perdidos por límite de tolerancia).
* Estadísticas por caja (1 a 3): Clientes atendidos, tiempo de espera promedio (en segundos) y tiempo de espera máximo (en minutos).

---

Proyecto Integrador desarrollado en la **Universidad del Valle de México (UVM) - Campus Reforma**, para la carrera de Ingeniería en Sistemas Computacionales, cursando el segundo semestre en Lógica y Programación Estructurada, impartida por el profesor César Antonio Rios Olivares.

```

```
