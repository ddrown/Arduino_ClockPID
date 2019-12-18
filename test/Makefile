ARDUINOFAKEDIR=../../ArduinoFake
INCLUDES=-I$(ARDUINOFAKEDIR)/src/ -I$(ARDUINOFAKEDIR)/external/unity/unity-repo/src/ -I..

test: test.o ClockPID.o
	g++ -o $@ $^ $(ARDUINOFAKEDIR)/build/src/libArduinoFake.so $(ARDUINOFAKEDIR)/build/external/unity/libunity.a
	LD_LIBRARY_PATH=$(ARDUINOFAKEDIR)/build/src/ ./test

clean:
	rm -f *.o test

test.o: test.cpp test-ClockPID.cpp
	g++ -c $(INCLUDES) $<

%.o: ../%.cpp
	g++ -c $(INCLUDES) $<

.PHONY: test clean
