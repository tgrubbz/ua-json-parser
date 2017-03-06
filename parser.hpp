
#ifndef PARSER_HPP
#define PARSER_HPP

# include "value.hpp"
# include "reader.hpp"
# include <exception>

using namespace std;

bool isDigit(char c)
{
	return c == '.' || 
		c == '0' || 
		c == '1' || 
		c == '2' || 
		c == '3' || 
		c == '4' || 
		c == '5' || 
		c == '6' || 
		c == '7' || 
		c == '8' || 
		c == '9';
}

bool isImportant(char c)
{
	return c == 't' ||
		c == 'f' ||
		c == 'n' ||
		c == '"' ||
		isDigit(c) ||
		c == '[' ||
		c == '{' ||
		c == '}' ||
		c == ']';
}

class parser
{
private:
	reader * r;
	char * current;
	char * endbuf;
	bool empty();
	void next();
	char now();
	void ignore();
	value * parse_true();
	value * parse_false();
	value * parse_null();
	value * parse_float();
	value * parse_string();
	value * parse_array();
	value * parse_object();
	string_value * parse_string_value();
	value * parse_recr();

public:
	parser(reader * r) : r(r) { }
	~parser() { }

	value * parse();
	
};

bool parser::empty()
{
	bool is_empty = current >= endbuf;
	if(is_empty)
	{
		// read a chunk into the buffer
		if(r->read())
		{
			// set the current char ptr and the end buffer ptr
			char * buffer = r->getbuffer();
			current = & (&buffer)[0][0];
			endbuf = & (&buffer)[0][r->getcount()];
			is_empty = false;
		}
	}

	return is_empty;
}

void parser::next()
{
	++current;
	empty();
}

char parser::now()
{
	return * current;
}

void parser::ignore()
{
	while(!empty() && !isImportant(now()))
	{
		next();
	}
}

value * parser::parse()
{
	try
	{
		// read a chunk into the buffer
		r->read();

		char * buffer = r->getbuffer();
		// set the current char ptr and the end buffer ptr
		current = & (&buffer)[0][0];
		endbuf = & (&buffer)[0][r->getcount()];

		return parse_recr();
	}
	catch(exception e)
	{
		// cout the exception and rethrow it
		cout << "Exception: " << e.what() << endl;
		throw e;
	}	
}

value * parser::parse_recr()
{			
	while(!empty())
	{
		switch(now())
		{
			case 't':
				return parse_true();
			case 'f':
				return parse_false();
			case 'n':
				return parse_null();
			case '.':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return parse_float();
			case '"':
				return parse_string();
			case '[':
				return parse_array();
			case '{':
				return parse_object();
			default:
				ignore();
				break;
		}
	}

	throw exception("Invalid JSON");
}

value * parser::parse_true()
{
	// skip 't'
	next();

	// ignore trailing garbage
	ignore();

	return new bool_value(true);
}

value * parser::parse_false()
{
	// skip 'f'
	next();
	
	// ignore trailing garbage
	ignore();

	return new bool_value(false);
}

value * parser::parse_null()
{
	// skip 'n'
	next();
	
	// ignore trailing garbage
	ignore();

	return new null_value();
}

value * parser::parse_float()
{
	string s;

	while(isDigit(now()))
	{
		s += now();
		next();
	}

	float num;
	try
	{
		num = stof(s);
	}
	catch(exception & e)
	{
		cout << e.what() << endl;
	}
	
	// ignore trailing garbage
	ignore();
	return new number_value(num);
}

value * parser::parse_string()
{
	return parse_string_value();
}

string_value * parser::parse_string_value()
{
	string s;

	// skip the '"'
	next();

	while(now() != '"')
	{
		if(empty())
		{
			break;
			// throw exception("Invalid string");
		}

		char c = now();
		s += c;

		if(c == '\\')
		{
			// add the escaped character and continue
			next();
			s += now();
		}

		next();
	}

	// skip the '"'
	next();

	// ignore trailing garbage
	ignore();
	return new string_value(s);
}

value * parser::parse_array()
{
	array_value * array = new array_value();

	// skip the '['
	next();

	// ignore junk
	ignore();

	// check to see if the array is empty
	if(!empty())
	{
		while(now() != ']')
		{
			// cout << now() << endl;
			if(empty())
			{
				break;
				// throw exception("Invalid array");
			}
			array->add(parse_recr());
		}
	}

	next();

	// ignore trailing garbage
	ignore();
	return array;
}

value * parser::parse_object()
{
	object_value * object = new object_value();

	// skip the '{'
	next();

	// ignore junk
	ignore();

	if(!empty())
	{
		while(now() != '}')
		{
			if(empty())
			{
				break;
				// throw exception("Invalid object");
			}
			string_value * s = parse_string_value();
			value * v = parse_recr();
			object->add(s, v);
		}	
	}

	next();

	// ignore trailing garbage
	ignore();
	return object;
}

#endif
