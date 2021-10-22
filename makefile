cc := gcc
flags := -Wall -Werror -pedantic-errors
libs := -lgd -lm
obj := colour.o boilerplate.o
proj := sapphire

all: $(proj)


$(proj): $(proj).c $(obj) config.h
	$(cc) $< $(obj) $(flags) $(libs) -o $@

%.o : %.c %.h config.h
	$(cc) -c $< -o $@ $(flags) $(libs)

clean:
	rm $(proj) $(obj)
cleanPng:
	rm $(proj) $(obj) *.png
cleanAll:
	rm $(proj) $(obj) *.png *.mp4
encode:
	imageToVideo .
	
