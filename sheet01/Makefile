APP:=$(basename $(wildcard *.cpp))
CXX:=g++ -Wall -O2 -I/usr/include/opencv
CXX_LIBS=-lglut -lGLU -lpthread -lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_imgproc 

.PHONY: all clean

all: $(APP)
	./$(APP) distorted.png straightened.png 0.001 0.000005

series: $(APP)
	./$(APP) distorted.png straightened_2_0.0005.png 0.001 0.0005
	./$(APP) distorted.png straightened_2_0.00005.png 0.001 0.00005
	./$(APP) distorted.png straightened_2_0.000005.png 0.001 0.000005
	./$(APP) distorted.png straightened_2_0.0000005.png 0.001 0.0000005
	./$(APP) distorted.png straightened_2_0.00000005.png 0.001 0.00000005
	./$(APP) distorted.png straightened_2_-0.000005.png 0.001 -0.000005
	./$(APP) distorted.png straightened_1_0.1.png 0.1 0.000005
	./$(APP) distorted.png straightened_1_0.01.png 0.01 0.000005
	./$(APP) distorted.png straightened_1_0.001.png 0.001 0.000005
	./$(APP) distorted.png straightened_1_0.0001.png 0.0001 0.000005
	./$(APP) distorted.png straightened_1_0.00001.png 0.00001 0.000005
	./$(APP) distorted.png straightened_1_-0.001.png -0.001 0.000005

%: %.cpp
	$(CXX) $^ $(CXX_LIBS) -o $@

clean:
	@rm -rf $(APP) *~
