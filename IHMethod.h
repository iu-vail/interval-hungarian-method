
///////////////////////////////////////////////////////////////////////////////
// File name: IHMethod.h
// This file defines the main algorithm of Interval Hungarian Method.
// The algorithm is decomposed into functional modules.
// Lantao Liu, Nov 9, 2009
///////////////////////////////////////////////////////////////////////////////


#ifndef IH_METHOD_H
#define IH_METHOD_H


#include "IHMatrix.h"
#include "Hungarian.h"


extern int _Verbose;
extern int _Plot;


class IHMethod: public Hungarian{
public:
  IHMethod():accumulative_delta(0){}
  ~IHMethod(){}

  //=====================================================
  //Methods for matched edges

  inline double GetAccumDelta(void){ return accumulative_delta; }
  inline void SetAccumDelta(double _d){ accumulative_delta = _d; }

  //IH for matched edges: interval (a +oo)
  Interval IHMatchedEdge(const Hungarian&, EID);
  vector< pair<EID, Interval> > IHMatchedEdge(const Hungarian&, const vector<EID>&);


  //=====================================================
  //Methods for unmatched edges

  pair<EID, EID> FindIncidentMatchedEdges(const Hungarian&, EID);
  void GetMatrixComplements(BipartiteGraph&);

  Interval IHUnMatchedEdge(const Hungarian&, EID);
  vector< pair<EID, Interval> > IHUnMatchedEdge(const Hungarian&, const vector<EID>&);

  //=====================================================
  //Common Methods
  
  vector< pair<EID,Interval> > IHAlgorithm(Hungarian&);

  void DisplayInterval(Edge&);
  void DisplayInterval(const Interval&);
  void DisplayIntervals(const vector<Interval>&);

  void DisplayIntervals(const vector<vector<Interval> >&);
  
private:
  //BipartiteGraph ihm_bg;  //a copy for handling IH
  double accumulative_delta;

  vector<vector<Interval> > interval_matrix;

};




#endif
