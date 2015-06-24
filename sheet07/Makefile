APP:=$(basename $(wildcard *.cpp))
CXX:=g++ -Wall -O2 -I/usr/include/opencv
CXX_LIBS=-lglut -lGLU -lpthread -lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_imgproc -lopencv_calib3d -lGL

.PHONY: all clean

all: $(APP)

%: %.cpp
	$(CXX) $^ $(CXX_LIBS) -o $@

clean:
	@rm -rf $(APP) *~

