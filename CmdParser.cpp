

#include "CmdParser.h"


void
CmdParser::ParseCmd(int argc, char **argv){

  int c;
  while ((c = getopt (argc, argv, "i:r:m:p:t:v:h")) != -1)
    switch (c)
    {
           case 'i':
             this->input_file = optarg;
             break;
           case 'r':
             this->seed = atoi(optarg);
             break;
           case 'm':
             this->assignment_size = atoi(optarg);
             break;
           case 'p':
             this->plot = atoi(optarg);
             break;
           case 't':
             this->period = atoi(optarg);
             break;
           case 'v':
             this->verbose_level = atoi(optarg);
             break;
	   case 'h':
           case '?':
           default:
	     cout<<"Error: Could not understand the command line."<<endl;
	     this->GiveUsage(argv[0]);
             abort ();
    }
}

void
CmdParser::DisplayInput(void){

  if(this->input_file != "")
    cout<<"  Accept new input file: "<<input_file<<endl;
  
  if(this->seed != SEED)
    cout<<"  Accept new seed: "<<seed<<endl;

  if(this->assignment_size != AGENTS_SIZE)
    cout<<"  Accept new assignment size: "<<assignment_size<<endl;

  if(this->period != PERIOD)
    cout<<"  Accept new period: "<<period<<endl;
  
  if(this->verbose_level != 1)
    cout<<"  Accept new verbose level: "<<verbose_level<<endl;

  if(this->plot != PLOT)
    cout<<"  Accept new plot mode: "<<plot<<endl;

}


void
CmdParser::GiveUsage(char *cmd)
{
   fprintf(stderr,"\nUsage:    %s \n",cmd);
   fprintf(stderr,"          -i   input an assignment file\n");
   fprintf(stderr,"          -r   random generator with seed: unsigned int\n");
   fprintf(stderr,"          -m   matrix size: size_t\n");
   fprintf(stderr,"          -p   plot level: 0, 1, 2\n");
   fprintf(stderr,"          -t   time period for slide show: size_t\n");
   fprintf(stderr,"          -v   verbose level: 0, 1, 2\n");
   fprintf(stderr,"More details can be found in README file\n\n");
   exit(-1);
}

