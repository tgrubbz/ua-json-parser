
# include <iostream>
# include <fstream>
# include <time.h>
# include "parser.hpp"
# include "value.hpp"
# include "print.hpp"
# include "reader.hpp"
# include "weight.hpp"

using namespace std;

int main()
{
	clock_t start = clock();

	parser p(new reader(& cin));

	value * v = p.parse();

	pretty_print(v);

	cout << "weight: " << weight(v) << endl;

	cout << "time elapsed: "<< float(clock() - start) / CLOCKS_PER_SEC << endl;

	return 0;
}