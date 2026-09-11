# Variables del compilador
CXX = g++
# Usamos -O3 para que el compilador optimice el código al máximo (estándar en mediciones de rendimiento)
CXXFLAGS = -Wall -Wextra -O3

# Archivos fuente (basado en la estructura de tu directorio)
SRCS = matrix_multiplication.cpp \
       algorithms/naive.cpp \
       algorithms/strassen.cpp

# Nombre del ejecutable
EXEC = run_matrix

# Regla por defecto: limpia, compila, genera datos, ejecuta, grafica y limpia el binario
all: clean compile generate run plot clean

# Regla para compilar
compile:
	@echo "Compilando los algoritmos de matrices..."
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXEC)
	@echo "Compilación exitosa."

# Regla para generar los datos de prueba (Python)
generate:
	@echo "Generando matrices de prueba..."
	cd scripts && python3 matrix_generator.py

# Regla para generar los gráficos (Python)
plot:
	@echo "Generando gráficos..."
	cd scripts && python3 plot_generator.py

# Regla para medir (ejecutar el C++)
run:
	@echo "Iniciando mediciones..."
	./$(EXEC)

# Regla para limpiar los binarios
clean:
	@echo "Limpiando archivos compilados..."
	rm -f $(EXEC)