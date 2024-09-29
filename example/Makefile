CC=gcc
CFLAGS=-Wall -Wextra -I../inc -O3
LDFLAGS=
SOURCES=../src/membership_function.c ../src/class.c ../src/classifier.c ../src/inference.c ../src/defuzzifier.c
OBJECTS=$(notdir $(SOURCES:.c=.o))
EXECUTABLE=TecFanControl.out
OUTPUT_DIR=out

all: $(OUTPUT_DIR)/$(EXECUTABLE)

$(OUTPUT_DIR)/$(EXECUTABLE): $(addprefix $(OUTPUT_DIR)/, $(OBJECTS)) $(OUTPUT_DIR)/TecFanControl.o
	$(CC) $(LDFLAGS) $(addprefix $(OUTPUT_DIR)/, $(OBJECTS)) $(OUTPUT_DIR)/TecFanControl.o -o $(OUTPUT_DIR)/$(EXECUTABLE)

$(OUTPUT_DIR)/TecFanControl.o: TecFanControl.c | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -c TecFanControl.c -o $(OUTPUT_DIR)/TecFanControl.o

$(OUTPUT_DIR)/%.o: ../src/%.c | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf $(OUTPUT_DIR)
