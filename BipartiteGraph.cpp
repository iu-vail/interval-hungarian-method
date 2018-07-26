
#include "BipartiteGraph.h"


//arg must be const since it is constructed within const Hungarian obj
BipartiteGraph::BipartiteGraph(const BipartiteGraph& _bg):
	num_matched(_bg.GetNumMatched()),
	num_agents(_bg.GetNumAgents()),
	num_tasks(_bg.GetNumTasks()),
	min_delta(_bg.GetMinDelta()),
	agents(_bg.agents),
	tasks(_bg.tasks){
	//bg_matrix(_bg.bg_matrix){
  
  //below implements a deep copy (mtl required)
/*
  bg_matrix.resize(_bg.GetNumAgents(), _bg.GetNumTasks()); 
  for(size_t i=0; i<_bg.GetNumAgents(); i++)
    for(size_t j=0; j<_bg.GetNumTasks(); j++)
      bg_matrix[i][j] = _bg.bg_matrix[i][j]; 
*/
  bg_matrix = _bg.bg_matrix;

}


Edge
BipartiteGraph::GetMaxWeightEdge(Vertex& agent){
  size_t row_num = agent.GetVID();
  size_t col_num = 0;
  double max_wt = bg_matrix[row_num][0].GetWeight();
  //for(vector<Edge>::iterator itr = bg_matrix[row_num].begin(); itr != bg_matrix[row_num].begin() + bg_matrix.ncols(); itr++)
  for(unsigned int i=0; i<bg_matrix[0].size(); i++)
    if(bg_matrix[row_num][i].GetWeight() > max_wt){ 
      max_wt = bg_matrix[row_num][i].GetWeight();
      col_num = i;
    }
  //cout<<row_num<<" "<<col_num<<" "<<max_wt<<endl;
  return bg_matrix[row_num][col_num];
}



void
BipartiteGraph::ConstructBG(const Matrix& m){

  num_agents = m.size();
  num_tasks = m[0].size();
  //bg_matrix = m;  //shallow

  agents.resize(num_agents);
  tasks.resize(num_tasks);

  //deep copy of matrix (mtl required)
/*
  bg_matrix.resize(num_agents, num_tasks);
  for(size_t i=0; i<num_agents; i++)
    for(size_t j=0; j<num_tasks; j++)
      bg_matrix[i][j] = m[i][j];
*/
  bg_matrix = m;


  //assign task VID and assign label to be 0;
  int _vid = 0;
  for(vector<Vertex>::iterator itr = tasks.begin(); itr != tasks.end(); itr++){
    (*itr).SetVID(_vid++);
    (*itr).SetObj("TASK");
    (*itr).SetLabel(0);
    (*itr).SetActive(true);
    (*itr).path.clear();
  }

  //assign agent VID and assign label to be max;
  _vid = 0;
  for(vector<Vertex>::iterator itr = agents.begin(); itr != agents.end(); itr++){
    (*itr).SetVID(_vid++);
    (*itr).SetObj("AGENT");
    (*itr).SetLabel(this->GetMaxWeightEdge(*itr).GetWeight());
    (*itr).SetActive(true);
    (*itr).path.clear();
  }
  
  //init all edges
  for(unsigned int i=0; i<num_agents; i++)
    for(unsigned int j=0; j<num_tasks; j++)
      bg_matrix[i][j].SetEID(EID(i,j));

}
  
void
BipartiteGraph::DisplayLabels(vector<Vertex>& v){
  for(vector<Vertex>::iterator itr = v.begin(); itr != v.end(); itr++)
    cout<<(*itr).GetLabel()<<" ";
  cout<<endl;
}


void
BipartiteGraph::DisplayLabels(void){
  cout<<"Labels for agents:"<<endl;
  this->DisplayLabels(this->agents);
  cout<<"Labels for tasks:"<<endl;
  this->DisplayLabels(this->tasks);
}
 
bool
BipartiteGraph::CheckFeasibility(const Matrix& _m, 
		const vector<Vertex>& _agents, const vector<Vertex>& _tasks){
  //check the size for args
  if(_m.size() != _agents.size() || _m[0].size() != _tasks.size()){
     cout<<"Error: Size discrepency found during feasibility checking. Stopped."<<endl;
     exit(-1);
  }
  
  bool result = true;

  for(size_t i=0; i<_m.size(); i++)
    for(size_t j=0; j<_m[0].size(); j++)
      if(_agents[i].GetActive() && _tasks[j].GetActive() &&
		(_m[i][j].GetWeight() -( _agents[i].GetLabel() + _tasks[j].GetLabel()) ) > DOUBLE_EPSILON ){
        if(_m[i][j].GetWeight() > _agents[i].GetLabel() + _tasks[j].GetLabel()){
	   cout<<"Labels are not feasible any more!"<<endl;
	   cout<<"\tGetweight: "<<_m[i][j].GetWeight()<<endl;
	   cout<<"\tGetAlabel: "<<_agents[i].GetLabel()<<endl;
	   cout<<"\tGetTlabel: "<<_tasks[j].GetLabel()<<endl;
	}
	result = false;
      }
 
  return result;

}


bool
BipartiteGraph::CheckFeasibility(void){

  bool res = this->CheckFeasibility(bg_matrix, agents, tasks);
  //if(!res) cout<<"Warning: Bipartite graph is NOT feasible!"<<endl;
  return res;
}


void
BipartiteGraph::DisplayMatched(const Matrix& _m){

   for(unsigned int i=0; i<_m.size(); i++)
     for(unsigned int j=0; j<_m[0].size(); j++)
       if(_m[i][j].GetMatchedFlag())
	 cout<<"("<<i<<","<<j<<") ";
    
   cout<<endl; 

}


void
BipartiteGraph::DisplayMatched(void){
 
  this->DisplayMatched(this->bg_matrix);

}


void
BipartiteGraph::DisplayAdmissible(const Matrix& _m){

   for(unsigned int i=0; i<_m.size(); i++)
     for(unsigned int j=0; j<_m[0].size(); j++)
       if(_m[i][j].GetAdmissibleFlag())
	 cout<<"("<<i<<","<<j<<") ";
    
   cout<<endl; 

}


void
BipartiteGraph::DisplayAdmissible(void){
 
  this->DisplayAdmissible(this->bg_matrix);

}



