.PHONY : clean func unit release debug
CC := g++
CFLAGS := -std=c++20 -g3 -Wall -Werror -pedantic -Wextra -Wvla -Wfloat-equal -Wfloat-conversion -lcurl -lmyhtml -liconv

INC := include
OUT := out
SRC := source


SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp,$(OUT)/%.o, $(SRCS))

APP_OUT := app.out

release: $(APP_OUT)

$(APP_OUT): $(OUT) | $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lm -lcurl -lmyhtml -liconv

$(OUT)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@ -lm -lcurl -lmyhtml -liconv

$(OUT):
	mkdir -p $@

clean:
	rm -f -r $(OUT) *.out