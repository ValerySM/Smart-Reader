// Autor: Valery Sokolov Morgenshteyn ID: 336424221
#ifndef _SMARTREADER_H
#define _SMARTREADER_H
#include <fstream>
#include <stdio.h>
class SmartReader
{
private:
	enum class Status
	{
	SingleLineComment,
	MultiLineComment,
	Regular,
	String
	};
	class RemarkException {
	public:
		virtual void printErr(std::ofstream *out) = 0;
	};

	class CloseRemarkWithoutOpenRemarkException : public RemarkException
	{
	public:
		void printErr(std::ofstream* out) override;
	};

	class UnclosedRemarkException : public RemarkException
	{
	public:
		void printErr(std::ofstream* out) override;

	};


	char prev, cur;
	int length;
	std::ifstream* in;
	std::ofstream* out;
	std::fstream buffer;
	Status status;

	void StepForward();
	void ClearBuffer();
	void copyFromBuffer();
	void endOfFile();
	bool hasClosed();

public:
	SmartReader(std::ifstream* in, std::ofstream* out);
	~SmartReader();
	void BuildFile();

};
#endif

