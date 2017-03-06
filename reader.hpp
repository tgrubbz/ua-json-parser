
#ifndef READER_HPP
#define READER_HPP

# include <istream>
# include <exception>

using namespace std;

class reader
{
private:
	int chunk_size = 1024;
	char * buffer_ = new char[chunk_size];
	istream * is;

public:
	reader(istream * standard_input)
	{
		is = standard_input;
	}

	~reader()
	{
		//delete[] buffer_;
	}

	char * getbuffer()
	{
		return buffer_;
	}

	istream & read()
	{
		return is->read(buffer_, chunk_size);
	}

	bool eof()
	{
		return is->eof();
	}

	bool good()
	{
		return is->good();
	}

	int getchunk_size()
	{
		return chunk_size;
	}

	int getcount()
	{
		return is->gcount();
	}
	
};

#endif
