
///////////////////////////////////////////////////////////////////////////////
// File name: IHMatrix.h
// This file defines the class of IHMatrix and IHEdge.
// Lantao Liu, Nov 9, 2009
///////////////////////////////////////////////////////////////////////////////


#ifndef IH_MATRIX_H
#define IH_MATRIX_H

#include "Matrix.h"

typedef pair<double, double> Interval;

class IHEdge;


//typedef mtl::matrix<IHEdge, SHAPE, STORAGE, ORIEN>::type IHMatrix;
typedef vector<vector<IHEdge> > IHMatrix;


class IHEdge: public Edge{
public:
  IHEdge():interval_min(NEG_INF), interval_max(POS_INF){}  
  //~IHEdge(){}
  virtual ~IHEdge(){}

  inline double GetIntervalMin(void){ return interval_min; }
  inline void SetIntervalMin(double _min){ interval_min = _min; }

  inline double GetIntervalMax(void){ return interval_max; }
  inline void SetIntervalMax(double _max){ interval_max = _max; }

  //for interval analysis of matched edge, hide it by a large negtive value
  virtual inline void HideEdge(void){ weight = NEG_INF; } 
 

private:
  double interval_min;
  double interval_max;

};



#endif



