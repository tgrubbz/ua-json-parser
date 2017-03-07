
# include <iostream>
# include "value.hpp"

int weight(value * val)
{
	class vis : public value::visitor
	{
	public:
		int w = 0;
		void visit(string_value * v) 
		{
			++w;
		};

		void visit(number_value * v) 
		{
			++w;
		};

		void visit(object_value * v) 
		{
			w += 1 + v->val.size();
		};

		void visit(array_value * v) 
		{
			w += 1 + v->val.size();
		};

		void visit(bool_value * v) 
		{
			++w;
		};

		void visit(null_value * v) 
		{
			++w;
		};
	};

	vis v;
	val->accept(v);
	return v.w;
}