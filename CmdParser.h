
///////////////////////////////////////////////////////////////////////////////
// File Name: CmdParser.h
// This file defines a class for parsing command line
// Lantao Liu, Nov 1, 2009
///////////////////////////////////////////////////////////////////////////////


#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "Matrix.h"

using namespace std;

extern int _Period;
extern int _Verbose;
extern int _Plot;

class CmdParser{
public:
  CmdParser(int _argc, char **_argv):seed(SEED), 
				     assignment_size(AGENTS_SIZE),
				     period(PERIOD), 
				     verbose_level(VERBOSE_LEVEL),
				     plot(PLOT){ 
    ParseCmd(_argc, _argv); 
    _Plot = plot;
    _Period = period;	
    _Verbose = verbose_level;
  }
  ~CmdParser(){}
  
  string GetInputFileName(void){ return input_file; }
  unsigned int GetSeed(void){ return seed; }
  size_t GetAssignmentSize(void){ return assignment_size; }

  void ParseCmd(int argc, char **argv);
  
  void DisplayInput(void);
 
  void GiveUsage(char *cmd);
  
public:
  unsigned int seed;
  string input_file;
  size_t assignment_size;
  unsigned int period; 
  unsigned int verbose_level;
  int plot;

};


#endif



