// Autor: Valery Sokolov Morgenshteyn ID: 336424221
#include "SmartReader.h"


SmartReader::SmartReader(std::ifstream* in, std::ofstream* out)
{
	buffer.open("tmp.txt");
	prev = cur = 0;
	this->in = in;
	this->out = out;
	buffer.open("tmp.txt", std::ios::in | std::ios::out | std::ios::trunc);
	if (!buffer)
	{
		*out << "\nCan't open temp.txt\n";
		exit(1);
	}
	ClearBuffer();
	status = Status::Regular;
	length = 0;

}

void SmartReader::BuildFile()
{
	char c;
	while (*in >> std::noskipws >> c)
	{
		prev = cur;
		cur = c;
		try {
			StepForward();
		}
		catch (RemarkException& ex) {
			ex.printErr(out);
		}
	}
	endOfFile();


}


void SmartReader::StepForward() throw (RemarkException)
{
	switch (status)
	{
	case SmartReader::Status::SingleLineComment:
		if (cur == '\n')
		{
			status = Status::Regular;
			*out << cur;
		}
		break;
	case SmartReader::Status::MultiLineComment:
		if (prev == '*' && cur == '/' ) {
			status = Status::Regular;

		}
		break;
	case SmartReader::Status::Regular:
		buffer << cur;
		length++;
		if (prev == '*' && cur == '/')
		{
			ClearBuffer();
			throw CloseRemarkWithoutOpenRemarkException();
		}
		else if (prev == '/' && cur == '*')
		{
			length -= 2;
			copyFromBuffer();
			if (!hasClosed())
			{
				status = Status::Regular;
				throw UnclosedRemarkException();
			}
			status = Status::MultiLineComment;
		}
		else if (cur == '/' && prev == '/')
		{
			length -= 2;
			copyFromBuffer();
			status = Status::SingleLineComment;
		}
		else if (cur == '"')
		{
			status = Status::String;
		}
		break;
	case SmartReader::Status::String:
		buffer << cur;
		length++;
		if (cur == '"')
			status = Status::Regular;
		break;
	default:
		break;
	}
}

void SmartReader::ClearBuffer()
{
	buffer.close();
	buffer.open("tmp.txt", std::ios::in | std::ios::out | std::ios::trunc);
	if (!buffer)
	{
		*out << "\nCan't open temp.txt\n";
		exit(1);
	}
	length = 0;
}

void SmartReader::copyFromBuffer()
{
	buffer.seekp(std::ios::beg);
	char c;
	for (int i = 0; i < length; i++)
	{
		buffer >> std::noskipws >> c;
		* out << c;
	}
	ClearBuffer();
	
}

void SmartReader::endOfFile()
{
	if (status == Status::Regular)
		copyFromBuffer();

	//DebugOnly
	else
		*out << "Alarm!!!";
}

bool SmartReader::hasClosed()
{
	std::streampos curP = in->tellg();
	char c, prev = 0, cur = 0;
	bool inString=false;
	while (*in >> std::noskipws >> c)
	{
		prev = cur;
		cur = c;
		if (c == '"')
			inString = !inString;
		if (prev == '*' && cur == '/'&&!inString)
		{
			in->seekg(curP);
			return true;
		}
	}
	in->clear();
	in->seekg(curP);
	return false;
}

SmartReader::~SmartReader()
{
	buffer.close();
	std::remove("tmp.txt");
}

void SmartReader::CloseRemarkWithoutOpenRemarkException::printErr(std::ofstream* out)
{
	*out << "\nError, encounter a close remark character without a preceding open remark character.\n";
}



void SmartReader::UnclosedRemarkException::printErr(std::ofstream* out)
{
	*out << "\nError, encounter an unclose remark.\n";
}