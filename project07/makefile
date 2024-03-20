build:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp buildings.cpp node.cpp nodes.cpp busstop.cpp busstops.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp -Wno-unused-variable -Wno-unused-function -lcurl

build-offline:
		rm -f ./a.out
		g++ -std=c++17 -g -Wall -DOFFLINE main.cpp building.cpp buildings.cpp node.cpp nodes.cpp busstop.cpp busstops.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp -Wno-unused-variable -Wno-unused-function -lcurl

build-online-save:
		rm -f ./a.out
		g++ -std=c++17 -g -Wall -DSAVE_ONLINE_RESPONSES main.cpp building.cpp buildings.cpp node.cpp nodes.cpp busstop.cpp busstops.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp -Wno-unused-variable -Wno-unused-function -lcurl

run:
	./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp buildings.cpp node.cpp nodes.cpp busstop.cpp busstops.cpp dist.cpp curl_util.cpp osm.cpp tinyxml2.cpp -Wno-unused-variable -Wno-unused-function -lcurl
	valgrind --tool=memcheck --leak-check=full ./a.out

clean:
	rm -f ./a.out

submit:
	/home/cs211/w2024/tools/project07  submit  *.cpp  *.h  *.hpp
