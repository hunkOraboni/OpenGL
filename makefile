#NVCC := "/mnt/c/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.4/bin/nvcc.exe"
#@$(NVCC) -I $(INCLUDE) HelloWorld.cu -o HelloWorld.exe
#CC := "/mnt/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64/cl.exe" # Visual Studio 2017
#C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx86\x86 # Visual Studio 2019
#CC := gcc
CC := g++
#CC := "/mnt/c/Program\ Files\ \(x86\)/Microsoft\ Visual\ Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx86/x86/cl.exe" # Visual Studio 2019

#INCLUDE := "C:\ProgramData\NVIDIA Corporation\CUDA Samples\v11.4\common\inc"
INCLUDE_GLFW := ./lib/include
LIB_PATH_GLFW := ./lib-vc2019/glfw3.lib

CPPFLAGS := -I $(INCLUDE_GLFW) -L $(LIB_PATH_GLFW)

.PHONY: clean

Application.exe: Application.o
	g++ -o target/Application.exe target/Application.o -L ./lib-vc2019 -lglfw3 -lopengl32 -lgdi32

Application.o: src/Application.cpp
	g++ -c -I ./lib/include src/Application.cpp -o target/Application.o

#build_cpp: src/Application.cpp
#	@echo "Build Application.cpp"
#	$(CC) $(CPPFLAGS) src/Application.cpp -o target/Application.exe

clean:
	@rm target/*.exe target/*.o
