srcs = $(shell find . -name '*.dot')

.PHONY: all clean

all: $(srcs:.dot=.png)

%.png: %.dot
	dot -Gimagepath=$(dir $<) -Tpng $< -o $@

clean:
	rm -f $(srcs:.dot=.png)
