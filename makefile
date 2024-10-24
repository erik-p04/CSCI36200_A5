btree: main.cpp
	g++ main.cpp -o btree

clean:
	rm ./btree

run: btree
	./btree
