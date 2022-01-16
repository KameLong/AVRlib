# 変数宣言
## ソース情報
TARGET = example
HEDS =
SRCS = src/example02.cpp
OBJS = $(SRCS:.c=.o)
## AVR デバイス情報
AVR_CHIP = atmega328p
AVR_WRIT = arduino
AVR_PORT = /dev/cu.usbserial-110
F_CPU=8000000

## コンパイラ情報
CC = avr-gcc
CFLAGS = -Wall -O2 -mmcu=$(AVR_CHIP) -DF_CPU=$(F_CPU)
## 他
RM = -rm

# 実行ファイル及び HEX ファイル作成 (各 Cソースのオブジェクトファイルが必要)
$(TARGET).hex: $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o build/$(TARGET)
	avr-objcopy -F ihex build/$(TARGET) build/$(TARGET).hex

# 各 Cソースのオブジェクトファイル作成
%.o: %.c $(HEDS)
	$(CC) $(CFLAGS) -c $< -o $@

# HEX ファイル書き込み (Blink.hex が必要)
write: $(TARGET).hex
	avrdude -c $(AVR_WRIT) -P $(AVR_PORT) -p $(AVR_CHIP) -b 57600 -U build/$(TARGET).hex

# オブジェクトファイルと実行ファイル及び HEX ファイル削除
.PHONY: clean
clean:
	$(RM) build/*.o


