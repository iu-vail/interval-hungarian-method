
///////////////////////////////////////////////////////////////////////////////
// File name: main_ihm.cpp
// File Description: as main function of Interval Hungairan Method (IHM).
// This file calls the compoments of IHM including KMH.
// Lantao Liu, Nov 9, 2009
///////////////////////////////////////////////////////////////////////////////

#include "IHMethod.h"
#include "CmdParser.h"

int main(int argc, char** argv){

    //define a matrix;
  Matrix m;

  //parse command line and generate an assignment
  CmdParser parser(argc, argv);
  parser.DisplayInput();

  Assignment as;
  if(parser.GetInputFileName().empty()){
    if(parser.GetSeed())
      as.SetSeed(parser.GetSeed());
    else
      as.SetSeed(time(NULL));
    cout<<endl<<"  *Seed for random generator: "<<as.GetSeed()<<endl;
    m=as.RandomGenerate( parser.GetAssignmentSize(),
                         parser.GetAssignmentSize(),
                         MAX_RANDOM,
                         parser.GetSeed() );
  }
  else{
    ifstream myfile(parser.GetInputFileName().c_str());
    m=as.ImportAssignment(myfile);
  }
  as.DisplayMatrix(m);

  //define a bipartite graph
  BipartiteGraph bg(m);

  //run Hungarian method
  Hungarian h(bg);
  h.HungarianAlgo();
  
  IHMethod ihm;
  //vector<EID> unmatched_set = h.GetUnMatchedSet();
  //ihm.IHMatchedEdge(h, h.M);  
  //ihm.IHUnMatchedEdge(h, unmatched_set);
  ihm.IHAlgorithm(h);

  return 0;

}
