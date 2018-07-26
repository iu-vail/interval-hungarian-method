

#include "IHMethod.h"

//int _Verbose;
//int _Plot;

vector< pair<EID,Interval> > 
IHMethod::IHAlgorithm(Hungarian& _h){

  vector< pair<EID, Interval> > intervals, it_m, it_um;
  it_m = IHMatchedEdge(_h, _h.M);
  it_um = IHUnMatchedEdge(_h, _h.GetUnMatchedSet());
 
  //combine two vectors into one 
  intervals.reserve(it_m.size() + it_um.size());
  intervals.insert(intervals.end(), it_m.begin(), it_m.end());
  intervals.insert(intervals.end(), it_um.begin(), it_um.end());

  if(intervals.size() != (_h.GetBG()->GetNumAgents())*(_h.GetBG()->GetNumTasks())){
    cerr<<"Error: Intervals discrepancy during combining results. Stopped."<<endl;
    exit(-1);
  }

  //fill values into interval_matrix
  //interval_matrix.resize(_h.GetBG()->GetNumAgents(), _h.GetBG()->GetNumTasks());
  interval_matrix.resize(_h.GetBG()->GetNumAgents());
  for(size_t i=0; i<_h.GetBG()->GetNumAgents(); i++) 
    interval_matrix[i].resize(_h.GetBG()->GetNumTasks());

  for(vector< pair<EID,Interval> >::iterator itr=intervals.begin(); itr!=intervals.end(); itr++)
    interval_matrix[(*itr).first.first][(*itr).first.second] = (*itr).second;

  cout<<endl<<"*****************************************************"<<endl;
  cout<<endl<<"Your queried assignment problem is:"<<endl;
  DisplayMatrix(*(_h.GetBG()->GetMatrix()));
  cout<<"The matching configuration is:"<<endl;
  DisplayConfig(*(_h.GetBG()));
  cout<<"The interval matrix for this assignment is: "<<endl;
  DisplayIntervals(interval_matrix);
  cout<<endl<<"*****************************************************"<<endl;
  cout<<endl;
 
  return intervals;

}


pair<EID, EID> 
IHMethod::FindIncidentMatchedEdges(const Hungarian& _h, EID _e){

  pair<EID, EID> edge_pair; 
  Hungarian my_h(_h);

  if(my_h.GetBG()->GetMatrix(_e)->GetMatchedFlag()){
    cerr<<"Error: Objective edge is already matched. I cannot find incident matched edge pairs. Stopped."<<endl;
    exit(-1); 
  }

  VID this_agent = _e.first;
  VID this_task = _e.second;

  //find matched edge e(this_agent, agent_mate)
  unsigned int task_index = 0;
  for(task_index=0; task_index<my_h.GetBG()->GetNumTasks(); task_index++)
    if(my_h.GetBG()->GetMatrix(this_agent, task_index)->GetMatchedFlag()){
	edge_pair.first = EID(this_agent, task_index);
	break;
    }
 
  if(task_index == my_h.GetBG()->GetNumTasks()){
    cerr<<"Error: Could not find a matched mate for appointed agent. Stopped."<<endl;
    exit(-1);
  }
  
  //find matched edge e(task_mate, this_task)
  unsigned int agent_index = 0;
  for(agent_index=0; agent_index<my_h.GetBG()->GetNumAgents(); agent_index++)
    if(my_h.GetBG()->GetMatrix(agent_index, this_task)->GetMatchedFlag()){
    	edge_pair.second = EID(agent_index, this_task);
	break;
    }
 
  if(agent_index == my_h.GetBG()->GetNumAgents()){
    cerr<<"Error: Could not find a matched mate for appointed task. Stopped."<<endl;
    exit(-1);
  }

  return edge_pair;

}


void 
IHMethod::GetMatrixComplements(BipartiteGraph& _bg){

  for(unsigned int i=0; i<_bg.GetNumAgents(); i++)
    for(unsigned int j=0; j<_bg.GetNumTasks(); j++){
      double c = _bg.GetAgent(i)->GetLabel() + _bg.GetTask(j)->GetLabel() - _bg.GetMatrix(i,j)->GetWeight();
      if(c < 0 && fabs(c) > DOUBLE_EPSILON)
	cerr<<"Warning: I detected that bipartite graph is not feasible. Continue anyway."<<endl;
      _bg.GetMatrix(i,j)->SetComplement(c);
    }

}



Interval
IHMethod::IHUnMatchedEdge(const Hungarian& _h, EID _e){

  Interval interval = Interval(NEG_INF, POS_INF);

  Hungarian my_h(_h);
  double ori_wt = my_h.GetBG()->GetMatrix(_e)->GetWeight();
  double ori_label = my_h.GetBG()->GetAgent(_e.first)->GetLabel() + my_h.GetBG()->GetTask(_e.second)->GetLabel();

  //Calculate the complements based on original matching config
  this->GetMatrixComplements(my_h.bg);

  //Imagine this edge has been set to be matched 
  //Through assigning it with positive infinite, and reset matched flag true
  //my_h.GetBG()->GetMatrix(_e)->EnforceEdge(ori_label);

  //Find incident matched edges (should be two) for this unmatched edge _e
  pair<EID, EID> edge_pair = this->FindIncidentMatchedEdges(my_h, _e);

  //Remove the pair from matched set, via reseting matched flag to be false
  my_h.GetBG()->GetMatrix(edge_pair.first)->SetMatchedFlag(false);
  my_h.GetBG()->GetMatrix(edge_pair.second)->SetMatchedFlag(false);
  //Disable _e vertex pair
  my_h.GetBG()->GetAgent(_e.first)->SetActive(false);
  my_h.GetBG()->GetTask(_e.second)->SetActive(false);
 
  my_h.RefreshBG();

  PlotGraph plot;
  if(_Plot>=2){
    //remove all previous temp files in /tmp
    //if(system("rm /tmp/gnuplot-i*")) cout<<endl;
    plot.InitPlot();
  }

  if(_Verbose){
    cout<<"The initial labels for vertices:"<<endl;
    my_h.GetBG()->DisplayLabels();
  }
 
  bool PlotMode = _Plot >= 2 ? true : false; 
  double this_delta = HungarianIteration(my_h.bg, my_h.S, my_h.T, my_h.N, my_h.EG, my_h.M, plot, PlotMode);
  this->SetAccumDelta(this_delta);

  //get the interval upper bound via summing up the complements (tolerance) of each new matched edges
  double sum_complements = 0;
  for(vector<EID>::iterator itr=my_h.M.begin(); itr!=my_h.M.end(); itr++)
    sum_complements += my_h.GetBG()->GetMatrix(*itr)->GetComplement();

  interval.second = ori_label + sum_complements; 

  cout<<endl<<"@_@ After Removing AGENT "<<_e.first<<" and TASK "
	    <<_e.second<<", Interval Analysis New Perfect Matching:"<<endl;
  DisplayData(my_h.M);
  cout<<"Optimization result: "<<this->OptimalValue(my_h.bg, my_h.M)<<endl;
  cout<<endl<<"~_~ The interval for edge ("<<_e.first<<","<<_e.second<<") with weight "<<ori_wt<<":"<<endl;
  DisplayInterval(interval);
  //cout<<"  Epsilon: "<<this->GetAccumDelta()<<endl;
  cout<<endl;

  if(_Plot>=2){
    cout << "Please press Enter to terminate: "<<endl<<endl;
    plot.SetPeriod(POS_INF);
    cin.get();
    plot.ClosePlot();
  }
  return interval;

}


vector< pair<EID,Interval> > 
IHMethod::IHUnMatchedEdge(const Hungarian& _h, const vector<EID>& _UM){ 

  vector< pair<EID,Interval> > intervals;
  for(vector<EID>::const_iterator itr = _UM.begin(); itr != _UM.end(); itr++){
    cout<<endl<<"============ IH UnMatched Edge: ("<<(*itr).first<<","
        <<(*itr).second<<") ============"<<endl;
    Interval it = IHUnMatchedEdge(_h, *itr);
    intervals.push_back(pair<EID, Interval>(*itr, it));
  }

  //this->DisplayIntervals(intervals);

  return intervals;

}



Interval
IHMethod::IHMatchedEdge(const Hungarian& _h, EID _e){
  
  Interval interval = Interval(NEG_INF, POS_INF);

  VID this_agent = _e.first;
  VID this_task = _e.second;
  
  Hungarian my_h(_h);
  double ori_wt = my_h.GetBG()->GetMatrix(_e)->GetWeight();
  
  //Hide the edge by assigning it negative infinite & reset matched flag
  my_h.GetBG()->GetMatrix(_e)->HideEdge();
  this->SetAccumDelta(0);

  PlotGraph plot;
  if(_Plot>=2){
    //remove all previous temp files in /tmp
    if(system("rm /tmp/gnuplot-i*")) cout<<endl;
    plot.InitPlot();
  }

  if(_Verbose){
    cout<<"The initial labels for vertices:"<<endl;
    my_h.GetBG()->DisplayLabels();
  }

  my_h.RefreshBG();
  
  bool PlotMode = _Plot >= 2 ? true : false;
  double this_delta = HungarianIteration(my_h.bg, my_h.S, my_h.T, my_h.N, my_h.EG, my_h.M, plot, PlotMode);
  this->SetAccumDelta(this_delta);

/*
    //VID u = PickFreeAgent(_bg);
    VID u = this_agent;
    if(_Verbose)
      cout<<endl<<"Pick an unmatched agent: "<<u<<endl;
    my_h.InitNewRoot(u);
 
    int cnt2 = 0;
    while(cnt2++ < MAX_LOOP){
      my_h.RefreshBG();
      if(_Plot){
        plot.PlotBipartiteGraph(my_h.bg, my_h.S, my_h.T, my_h.N, my_h.EG, my_h.M);
        if(!plot.GetPeriod()){ cout<<"Please press Enter to continue: "<<endl; cin.get(); }
      }

      //if need relabel, update labels
      if(my_h.NeedReLabel()){
        if(_Verbose)
          cout<<"Need relabel. Searching for min_delta..."<<endl;
	//update labels and accumulate it (for IH, actually only once)
        this->accumulative_delta += my_h.UpdateLabels();
        if(_Verbose > 1){
          cout<<"After updating labels: "<<endl;
          my_h.GetBG()->DisplayLabels();
        }
      }
      my_h.RefreshBG();
      if(_Verbose > 1)
        my_h.DisplayData(1,1,1,1,1);
      if(_Plot){
        plot.PlotBipartiteGraph(my_h.bg, my_h.S, my_h.T, my_h.N, my_h.EG, my_h.M);
        if(!plot.GetPeriod()){ cout<<"Please press Enter to continue: "<<endl; cin.get(); }
      }

      //if not need relabel
      if(!my_h.NeedReLabel()){
        //VID y = this_task;
        VID y = my_h.PickAvailableTask();
        if(_Verbose){
          cout<<"No need relabel, pick available task(s) "<<y<<" and put in queue."<<endl;
          cout<<"Got task VID "<<y<<endl;
          if(!my_h.GetBG()->GetTask(y)->GetMatched())
            cout<<"Task VID "<<y<<" has NOT been matched, then "<<u<<"-->"<<y
                <<" is an augmenting path, searching..."<<endl;
          else
            cout<<"Task VID "<<y<<" has been matched, and I gonna extend alternating tree."<<endl;
        }

        //if NOT need relabel and task NOT matched, then found augmenting path
        if(!my_h.GetBG()->GetTask(y)->GetMatched()){
          vector<EID> _path = my_h.BFSAugmentingPath(u, y);
          my_h.AugmentPath(_path);
          my_h.RefreshBG();
          if(_Verbose > 1){
            cout<<"After augmenting path: "<<endl;
            my_h.DisplayData(1,1,1,1,1);
          }
          if(_Plot){
            plot.PlotAugmentingPath(my_h.bg, _path);
            plot.PlotBipartiteGraph(my_h.bg, my_h.S, my_h.T, my_h.N, my_h.EG, my_h.M);
            if(!plot.GetPeriod()){ cout<<"Please press Enter to continue: "<<endl; cin.get(); }
          }
          //cout<<"Delta: "<<my_h.GetBG()->GetMinDelta()<<endl;
  	  //cout<<"Agent: "<<my_h.GetBG()->GetAgent(this_agent)->GetLabel()<<endl;
  	  //cout<<"Task : "<<my_h.GetBG()->GetTask(this_task)->GetLabel()<<endl;
	  interval.first = my_h.GetBG()->GetAgent(this_agent)->GetLabel() 
				+ my_h.GetBG()->GetTask(this_task)->GetLabel()
				;//- my_h.GetBG()->GetMinDelta();
          break; //break innter while
        }
        //if NOT need relabel and task is *matched*, extend tree
        else{
          my_h.ExtendTree(y);
          if(_Verbose)
            cout<<"Extending alternating tree..."<<endl;
            if(_Verbose>1){
              cout<<"After extending tree: "<<endl;
              my_h.DisplayData(1,1,1,1,1);
            }
        }
      }//if(!NeedReLabel)
    } //while(cnt2)
    if(cnt2 >= MAX_LOOP){
      cerr<<"I suspect the correctness of Hungarian inner loop. Given up."<<endl;
      exit(-1);
    }
*/

  interval.first = my_h.GetBG()->GetAgent(this_agent)->GetLabel() 
				+ my_h.GetBG()->GetTask(this_task)->GetLabel()
				;//- my_h.GetBG()->GetMinDelta();
  
  cout<<endl<<"@_@ After Hiding Edge ("<<_e.first<<","<<_e.second<<"), Interval Analysis New Perfect Matching:"<<endl;
  DisplayData(my_h.M);
  cout<<"Optimization result: "<<this->OptimalValue(my_h.bg, my_h.M)<<endl;
  cout<<endl<<"~_~ The interval for edge ("<<_e.first<<","<<_e.second<<") with weight "<<ori_wt<<":"<<endl;
  //cout<<"  ["<<interval.first<<","<<interval.second<<")"<<endl;
  DisplayInterval(interval);
  cout<<"  Epsilon: "<<this->GetAccumDelta()<<endl;
  cout<<endl;

  if(_Plot>=2){
    cout << "Please press Enter to terminate: "<<endl<<endl;
    plot.SetPeriod(POS_INF);
    cin.get();
    plot.ClosePlot();
  }
  return interval;

}



vector< pair<EID, Interval> >
IHMethod::IHMatchedEdge(const Hungarian& _h, const vector<EID>& _M){

  vector< pair<EID,Interval> > intervals;
  for(vector<EID>::const_iterator itr = _M.begin(); itr != _M.end(); itr++){
    cout<<endl<<"============ IH Matched Edge: ("<<(*itr).first<<","
	<<(*itr).second<<") ============"<<endl;
    Interval it = IHMatchedEdge(_h, *itr);
    intervals.push_back(pair<EID, Interval>(*itr,it) );
  }
  
  //this->DisplayIntervals(intervals);

  return intervals;

}


void
IHMethod::DisplayInterval(Edge& _e){

  cout<<endl<<"~_~ The interval for edge ("<<_e.GetEID().first<<","<<_e.GetEID().second<<") is:"<<endl;
  if(_e.GetIntervalMin()==NEG_INF)
    cout<<"  (-oo";
  else
    cout<<"  ["<<_e.GetIntervalMin();

  if(_e.GetIntervalMax()==POS_INF)
    cout<<" , +oo)"<<endl;
  else
    cout<<" , "<<_e.GetIntervalMax()<<"]"<<endl;

}


void 
IHMethod::DisplayInterval(const Interval& itv){

  if(itv.first==NEG_INF)
    cout<<"  (-oo";
  else
    cout<<"  ["<<itv.first;

  if(itv.second==POS_INF)
    cout<<" , +oo)";
  else
    cout<<" , "<<itv.second<<"]";

}


void
IHMethod::DisplayIntervals(const vector<Interval>& itvs){
  
  for(vector<Interval>::const_iterator itr=itvs.begin(); itr!=itvs.end(); itr++)
    this->DisplayInterval(*itr);
}


void
IHMethod::DisplayIntervals(const vector<vector<Interval> >& _m){
  
  for(unsigned int i=0; i<_m.size(); i++){
    for(unsigned int j=0; j<_m[0].size(); j++){
      DisplayInterval(_m[i][j]);
      cout<<"\t";
    }
  cout<<endl;
  }

}



