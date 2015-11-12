CPPFLAGS = -O3 -std=c++11
CXX = clang++
METHODS = m1 m2 m3 m4 m5 m6

all: $(METHODS)

m%: driver.cpp ssq%.hpp
	$(CXX) $(CPPFLAGS) -DMETHOD$(subst m,,$@) driver.cpp -o $@

clean:
	rm -rf $(METHODS)
