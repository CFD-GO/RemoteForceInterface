all: master

test: all


% : %.o
	mpic++ -o $@ $^

%.o : %.cpp
	mpic++ -c -o $@ $<
