
///////////////////////////////////////////////////////////////////////////////
// File name: Matrix.h
// This file defines the basic classes of vertex and edge, and the matrix data
// type is then defined based on them. 
// In the matrix, the grids store the edges, which denote the
// utility or cost in corresponding assignment matrix. There are two vectors of
// vertices, one for agents, and the other for tasks.
// Lantao Liu, Nov 1, 2009
// Last modified: Sep 27, 2011
///////////////////////////////////////////////////////////////////////////////
  

#ifndef MATRIX_H
#define MATRIX_H


#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>

/*
#include "mtl/mtl.h"
#include "mtl/utils.h"
#include "mtl/matrix.h"
#include "mtl/matrix_implementation.h"

//#define NUMTYPE Edge
#define SHAPE mtl::rectangle<>
#define STORAGE mtl::dense<>
#define ORIEN mtl::row_major
*/

#define TASKS_SIZE 3
#define AGENTS_SIZE 3

#define SEED 0
#define PERIOD 0
#define VERBOSE_LEVEL 1
#define PLOT 0 

#define MAX_RANDOM 100


#define POS_INF 10e3
#define NEG_INF -10e3
#define DOUBLE_EPSILON 1e-7 //for the double type comparison, <= as valid, > invalid

#ifndef min 
  #define min(x, y) (((x) > (y)) ? (y) : (x))
#endif

#ifndef max
  #define max(x, y) (((x) > (y)) ? (x) : (y))
#endif

using namespace std;
//using namespace mtl;

typedef size_t VID;
typedef pair<size_t, size_t> EID;

class Edge;
class Vertex;

/////////////////////////////////////////////////////////////
//   Define the matrix type
//   Edge is the basic data type used in it
//   The weight of edge denotes the utility or cost
/////////////////////////////////////////////////////////////

//typedef mtl::matrix<Edge, SHAPE, STORAGE, ORIEN>::type Matrix;
typedef vector<vector<Edge> > Matrix;



////////////////////////////////////////////////////////////
//
//  Edge class: represent an element in matrix
//              or an edge in bipartite graph
//
////////////////////////////////////////////////////////////

class Edge{
public:
  Edge(){
    weight = 0;
    matched_flag = false;         //unmatched
    admissible_flag = false;      //inadmissible
    complement = 0;
  }
  Edge(EID _eid):eid(_eid){
    weight = 0;
    matched_flag = false;         //unmatched
    admissible_flag = false;      //inadmissible
    complement = 0;
  }
  ~Edge(){}

  inline EID GetEID(void) const { return eid; }
  inline void SetEID(EID _eid){ eid = _eid; }

  inline double GetWeight(void) const { return weight; }
  inline void SetWeight(double _weight){ weight = _weight; }

  inline bool GetMatchedFlag(void) const { return matched_flag; }
  inline void SetMatchedFlag(bool _matched_flag){ matched_flag = _matched_flag;}

  inline bool GetAdmissibleFlag(void) const { return admissible_flag; }
  inline void SetAdmissibleFlag(bool _admissible){ admissible_flag = _admissible; }

  /*********for interval analysis**************/

  // forcibly hide an edge
  inline void HideEdge(void){ weight = NEG_INF; 
				this->SetMatchedFlag(false); }
  // forcibly expose an edge
  inline void EnforceEdge(void){ weight = POS_INF; 
				this->SetMatchedFlag(true); }

  // forcibly expose an edge, by putting on an appropriate wt
  inline void EnforceEdge(double _wt){ weight = _wt; 
				this->SetMatchedFlag(true); }

  // to get/set the complementary value of an edge: c = adms_lable - ori_label
  inline double GetComplement(void) const { return complement; }
  inline void SetComplement(double _c){ complement = _c; }

  // each edge finally will have an interval (Min, Max)
  inline double GetIntervalMin(void) const { return interval_min; }
  inline void SetIntervalMin(double _min){ interval_min = _min; }

  inline double GetIntervalMax(void) const { return interval_max; }
  inline void SetIntervalMax(double _max){ interval_max = _max; }


protected:
  //data members describing properties of an edge
  EID eid;
  double weight;
  bool matched_flag;
  bool admissible_flag;

private:
  // for interval method
  double complement;

  double interval_min;
  double interval_max;

};



///////////////////////////////////////////////////////////
//
//  Vertex class: represent an agent or a task 
//
//////////////////////////////////////////////////////////

class Vertex{
public:
  Vertex():label(0), active(true), matched(false), colored(false){ 
    //edges.resize(max(AGENTS_SIZE, TASKS_SIZE)); 
    //deltas.resize(max(AGENTS_SIZE, TASKS_SIZE), 0);
  }
  Vertex(VID _vid):vid(_vid),label(0), active(true), matched(false), colored(false){ 
    //edges.resize(max(AGENTS_SIZE, TASKS_SIZE)); 
    //deltas.resize(max(AGENTS_SIZE, TASKS_SIZE), 0);
  }
  ~Vertex(){}

  inline VID GetVID(void) const { return vid; }
  inline void SetVID(VID _vid){ vid = _vid; }

  inline string GetObj(void) const { return obj; }
  inline void SetObj(string _obj){ obj = _obj; }

  inline double GetLabel(void) const { return label; }
  inline void SetLabel(double _label){ label = _label; }

  inline bool GetMatched(void) const { return matched; }
  inline void SetMatched(bool _matched){ matched = _matched; }

  inline bool GetActive(void) const { return active; }
  inline void SetActive(bool _active){ active = _active; }

  inline bool GetColored(void) const { return colored; }
  inline void SetColored(bool _colored){ colored = _colored; }

  inline bool GetVisited(void) const { return visited; }
  inline void SetVisited(bool _visited){ visited = _visited; }

  inline vector<EID>* GetPath(void){ return &path; }

private:
  //data members describing properties of a vertex
  VID vid;	    //vertex ID
  string obj;	    //property identification, either "AGENT" or "TASK"
  double label;	    //label value
  bool active;	    //flag true if the vertex is allowed to exist
  bool matched;     //flag true if vertex has been matched 
  bool colored;     //color true if in the set of T or S
  bool visited;     //flag true if visited when go through alternating tree

public:
  vector<EID> path; //record previous path so far

};


#endif
 

