BIN=./bin/
SOURCE=./src/
EXEC=iSpotifai
CC=@gcc
LDFLAGS=$(shell pkg-config --libs gtk+-3.0 gmodule-export-2.0 libglade-2.0)
CFLAGS=-g -Wall -Wextra $(shell pkg-config --cflags gtk+-3.0 gmodule-export-2.0 libglade-2.0)

#gcc pkg-config --cflags gtk+-3.0 pkg-config
# --cflags gtk+-3.0 gmodule-export-2.0 pkg-config
# --cflags libglade-2.0 -o ispotifai ispot.c pkg-config
# --libs gtk+-3.0 pkg-config --libs gtk+-3.0 gmodule-export-2.0
# pkg-config --cflags libglade-2.0

all: config $(BIN)$(EXEC)

config:
	@mkdir -p bin/

$(BIN)$(EXEC): $(BIN)ispot.o $(BIN)TCPclient.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN)TCPclient.o: $(SOURCE)TCPclient.c
	$(CC) -o $@ -c $^ $(CFLAGS) $(LDFLAGS)

$(BIN)ispot.o: $(SOURCE)ispot.c
	$(CC) -o $@ -c $^ $(CFLAGS) $(LDFLAGS)

clean:
	@echo "Cleaning up object files..."
	@rm -rf $(BIN)*.o
	@echo "Cleaned!"
