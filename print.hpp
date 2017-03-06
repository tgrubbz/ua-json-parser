
# include <iostream>
# include <sstream>
# include <streambuf>
# include <vector>
# include <numeric>
# include "value.hpp"

using namespace std;


void print(value * val, ostream * os, int indents = 0)
{
	class vis : public value::visitor
	{
	public:
		int indents = 0;
		ostream * os;
		vis(ostream * os, int indents = 0) : os(os), indents(indents) { }

		string indent()
		{
			string s;
			for(int i = 0; i < indents; ++i)
			{
				s += "    ";
			}
			return s;
		}

		void visit(string_value * v) 
		{
			*os << "\"" + v->val + "\"";
		};

		void visit(number_value * v) 
		{
			*os << v->val;
		};

		void visit(object_value * v) 
		{
			++indents;
			int length = v->val.size();
			if(length > 0)
			{
				*os << "{\n";
				for(int i = 0; i < length; ++i)
				{
					*os << indent();
					print(v->val[i].first, os, indents);

					*os << " : ";

					print(v->val[i].second, os, indents);

					if(i != length - 1)
					{
						*os << ",\n";
					}
					else
					{
						*os << "\n";					
					}
				}

				*os << indent();
			}
			else
			{
				*os << "{ ";
			}
	

			--indents;
			*os << "}";
		};

		void visit(array_value * v) 
		{
			++indents;
			int length = v->val.size();
			if(length > 0)
			{				
				*os << "[\n";
				for(int i = 0; i < length; ++i)
				{
					*os << indent();
					print(v->val.at(i), os, indents);

					if(i != length - 1)
					{
						*os << ",\n";
					}
					else
					{				
						*os << "\n";	
					}
				}

				*os << indent();			
			}
			else
			{
				*os << "[ ";
			}

			--indents;
			*os << "]";
		};

		void visit(bool_value * v) 
		{
			*os << (v->val ? "true" : "false");
		};

		void visit(null_value * v) 
		{
			*os << "null";
		};
	};

	vis v(os, indents);
	val->accept(v);
}

void pretty_print(value * v)
{
	ofstream * of = new ofstream("output.txt");
	print(v, of);
}