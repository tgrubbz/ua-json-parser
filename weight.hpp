
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
			int length = v->val.size();
			++w;
			for(auto & item : v->val)
			{
				w += weight(item.second);
			}
		};

		void visit(array_value * v) 
		{
			int length = v->val.size();
			++w;
			for(int i = 0; i < length; ++i)
			{
				w += weight(v->val.at(i));
			}
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