
#ifndef VALUE_HPP
#define VALUE_HPP

# include <string>
# include <map>
# include <vector>
# include <memory>

class string_value;
class number_value;
class object_value;
class array_value;
class bool_value;
class null_value;

class value
{
public:
	value() { }
	~value() { }

	class visitor;
	virtual void accept(visitor &) = 0;
	
};

class value::visitor
{
public:
	virtual void visit(string_value *) = 0;
	virtual void visit(number_value *) = 0;
	virtual void visit(object_value *) = 0;
	virtual void visit(array_value *) = 0;
	virtual void visit(bool_value *) = 0;
	virtual void visit(null_value *) = 0;
};

class string_value : public value
{
public:
	std::string val;

	string_value(std::string s) : val(s) { }
	~string_value() { }

	void accept(visitor & v) { return v.visit(this); }	
	
};

class number_value : public value
{
public:
	float val;

	number_value(float f) : val(f) { }
	~number_value() { }

	void accept(visitor & v) { return v.visit(this); }	
	
};

class object_value : public value
{
public:
	std::vector<std::pair<string_value *, value *>> val;

	object_value() : val() { }
	~object_value() { }

	void accept(visitor & v) { return v.visit(this); }	
	void add(string_value * s, value * v)
	{
		val.push_back(std::pair<string_value *, value *>(s, v));
	}
	
};

class array_value : public value
{
public:
	std::vector<value *> val;

	array_value() : val() { }
	~array_value() { }

	void accept(visitor & v) { return v.visit(this); }
	void add(value * v)
	{
		val.push_back(v);
	}
	
};

class bool_value : public value
{
public:
	bool val;

	bool_value(bool b) : val(b) { }
	~bool_value() { }

	void accept(visitor & v) { return v.visit(this); }	
	
};

class null_value : public value
{
public:
	null_value() { }
	~null_value() { }

	void accept(visitor & v) { return v.visit(this); }	
	
};

#endif
