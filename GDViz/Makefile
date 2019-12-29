PROGRAM = GDViz
CC = g++ -std=c++14
LIBS = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o shared.o stb_image.o ManifoldBase.o EllipticParaboloid.o HyperbolicParaboloid.o MultivariateSine.o CubicProduct.o Texture.o
	$(CC) $(LIBS) -o $(PROGRAM) main.o shared.o stb_image.o ManifoldBase.o EllipticParaboloid.o HyperbolicParaboloid.o MultivariateSine.o CubicProduct.o Texture.o

main.o: main.cpp CubicProduct.h EllipticParaboloid.h HyperbolicParaboloid.h main.h MultivariateSine.h shared.h Texture.h
	$(CC) $(LIBS) -c main.cpp -o main.o

shared.o: shared.cpp shared.h
	$(CC) -c shared.cpp -o shared.o

Texture.o: Texture.cpp Texture.h stb_image.h
	$(CC) $(LIBS) -c Texture.cpp -o Texture.o

stb_image.o: stb_image.cpp stb_image.h
	$(CC) -c stb_image.cpp -o stb_image.o

ManifoldBase.o: ManifoldBase.cpp shared.h ManifoldBase.h
	$(CC) -c ManifoldBase.cpp -o ManifoldBase.o

EllipticParaboloid.o: EllipticParaboloid.cpp shared.h ManifoldBase.h EllipticParaboloid.h
	$(CC) -c EllipticParaboloid.cpp -o EllipticParaboloid.o

HyperbolicParaboloid.o: HyperbolicParaboloid.cpp shared.h ManifoldBase.h HyperbolicParaboloid.h
	$(CC) -c HyperbolicParaboloid.cpp -o HyperbolicParaboloid.o

MultivariateSine.o: MultivariateSine.cpp shared.h ManifoldBase.h MultivariateSine.h
	$(CC) -c MultivariateSine.cpp -o MultivariateSine.o

CubicProduct.o: CubicProduct.cpp shared.h ManifoldBase.h CubicProduct.h
	$(CC) -c CubicProduct.cpp -o CubicProduct.o

.PHONY: clean

clean:
	rm -f $(PROGRAM) *.o