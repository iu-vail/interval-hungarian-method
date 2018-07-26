
#include "PlotGraph.h"

//extern variable, definition can only in source.
int _Period;

PlotGraph::PlotGraph():g(NULL),p(NULL){ 

  period = _Period != 0 ? _Period : PERIOD; 

}

PlotGraph::~PlotGraph(){}

void 
PlotGraph::PlotBipartiteGraph(BipartiteGraph& _bg,
			      vector<VID>& _S, 
			      vector<VID>& _T,
			      vector<VID>& _N,
			      vector<EID>& _EG,
			      vector<EID>& _M, 
			      int target_task){

    //get assignment size, assume the sizes of agents and tasks are identical
    size_t as_size = _bg.GetNumAgents();
    double plx[as_size];
    double ply[as_size];

    //g=gnuplot_init();
    gnuplot_resetplot(g);
    gnuplot_cmd(g, (char*)"unset label");
    gnuplot_cmd(g, (char*)"set xrange [-0.25:1.25]");
    gnuplot_cmd(g, (char*)"set yrange [-0.25:%f]", as_size-1+.25);
    gnuplot_cmd(g, (char*)"set xtics 0");
    gnuplot_cmd(g, (char*)"set ytics 0");
    gnuplot_cmd(g, (char*)"set nokey");

    // Plot matching
    //cout<<endl<<"Set M: "<<endl;
    //DisplayData(_M);    
    for (unsigned int i = 0; i < _M.size(); i++){
        plx[0] = 0.0;
        ply[0] = as_size - _M[i].first - 1;
        plx[1] = 1.0;
        ply[1] = as_size - _M[i].second - 1;

        gnuplot_setstyle(g, (char*)"lines lc rgb \"#55DD99\" lw 3");
        gnuplot_plot_xy(g, plx, ply, 2, NULL);
    }

    // Plot admissible edges (EG)
    //cout<<"EG: "<<endl;
    //DisplayData(_EG);
    for(unsigned int i=0; i<_EG.size(); i++){
        plx[0] = 0.0;
        ply[0] = as_size - _EG[i].first -1;
        plx[1] = 1.0;
        ply[1] = as_size - _EG[i].second -1;

        gnuplot_setstyle(g, (char*)"lines lc rgb \"gray\"");
        gnuplot_plot_xy(g, plx, ply, 2, NULL);
                
        plx[0] = 0.11;
        ply[0] = (0.9*ply[0] + 0.1*ply[1]);
        gnuplot_setstyle(g, (char*)"points pointsize 2 lc rgb \"#FFFFFF\" pt 7");
        gnuplot_plot_xy(g, plx, ply, 1, NULL);

        //cout<<"EG "<<i<<": "<<_bg.GetMatrix(_EG[i])->GetWeight()<<endl;
        gnuplot_cmd(g, (char*)"set label \"%d\" at 0.1,%f front tc rgb \"magenta\"", _bg.GetMatrix(_EG[i])->GetWeight(), ply[0]);
    }


    // Plot membership of set S
    //cout<<"S: "<<endl;
    //DisplayData(_S);
    for(unsigned int i=0; i<_S.size(); i++){
        plx[0] = 0.0;
        ply[0] = as_size - _S[i] - 1;
        gnuplot_setstyle(g, (char*)"points pointsize 3 lc rgb \"#99AAFF\" pt 7");
        gnuplot_plot_xy(g, plx, ply, 1, NULL);
    }

    // Plot membership of the set T
    //cout<<"T: "<<endl;
    //DisplayData(_T);
    for(unsigned int i=0; i<_T.size(); i++){
        plx[0] = 1.0;
        ply[0] = as_size - _T[i] - 1;
        gnuplot_setstyle(g, (char*)"points pointsize 3 lc rgb \"#FF99AA\" pt 7");
        gnuplot_plot_xy(g, plx, ply, 1, NULL);
    }

        if (target_task != -1) // not yet unioned, but the reversal point for the 
        {                         // laternating path
            plx[0] = 1.0;
            ply[0] = as_size - target_task -1;
            gnuplot_setstyle(g, (char*)"points pointsize 3 lc rgb \"#FF99FF\" pt 7");
            gnuplot_plot_xy(g, plx, ply, 1, NULL);
        }


    // Plot nodes
    for(unsigned int i = 0; i < as_size; i++)
    {
        plx[i] = 0.0;
        ply[i] = i;
    }

    gnuplot_setstyle(g, (char*)"points pointsize 2 lc rgb \"#102063\" pt 7");
    gnuplot_plot_xy(g, plx, ply, as_size, NULL);

    for(unsigned int i = 0; i < as_size; i++)
    {
        plx[i] = 1.0;
        ply[i] = i;
    }

    gnuplot_setstyle(g, (char*)"points pointsize 2 lc rgb \"#102063\" pt 7");
    gnuplot_plot_xy(g, plx, ply, as_size, NULL);

    for (unsigned int i = 0; i < as_size; i++)
    {
        gnuplot_cmd(g, (char*)"set label \"x%d\" at -0.075,%d tc rgb \"black\"", i, as_size - i -1);
        gnuplot_cmd(g, (char*)"set label \"y%d\" at  1.02,%d tc rgb \"black\"", i, as_size - i -1);
	//cout<<"Label "<<i<<": "<<_bg.GetAgent(i)->GetLabel()<<endl;
	//cout<<"Label "<<i<<": "<<_bg.GetTask(i)->GetLabel()<<endl;
        gnuplot_cmd(g, (char*)"set label \"%d\" at -0.175,%d tc rgb \"#112244\"", _bg.GetAgent(i)->GetLabel(), as_size -i-1);
        gnuplot_cmd(g, (char*)"set label \"%d\" at 1.12,%d tc rgb \"#112244\"", _bg.GetTask(i)->GetLabel(), as_size - i -1);
    }

    gnuplot_cmd(g, (char*)"replot");

    sleep(period);
    //gnuplot_close(g);
}


void
PlotGraph::PlotAugmentingPath(BipartiteGraph& _bg, vector<EID>& _path){

    //get assignment size, assume the sizes of agents and tasks are identical
    size_t as_size = _bg.GetNumAgents();
    double plx[as_size];
    double ply[as_size];
/*
    //gnuplot_resetplot(g);
    gnuplot_cmd(g, (char*)"unset label");
    gnuplot_cmd(g, (char*)"set xrange [-0.25:1.25]");
    gnuplot_cmd(g, (char*)"set yrange [-0.25:%f]", as_size-1+.25);
    gnuplot_cmd(g, (char*)"set xtics 0");
    gnuplot_cmd(g, (char*)"set ytics 0");
    gnuplot_cmd(g, (char*)"set nokey");
*/
    //gnuplot_cmd(g, (char*)"set linestyle 3");
    gnuplot_cmd(g, (char*)"set style line 2 lt 2 lc rgb \"red\" lw 3");
    // Plot matching
    //cout<<endl<<"Set M: "<<endl;
    //DisplayData(_M);    
    for (unsigned int i = 0; i < _path.size(); i++){
        plx[0] = 0.0;
        ply[0] = as_size - _path[i].first - 1;
        plx[1] = 1.0;
        ply[1] = as_size - _path[i].second - 1;

        //gnuplot_setstyle(g, (char*)"linespoints lc rgb \"#55DD99\" lw 3");
        gnuplot_setstyle(g, (char*)"lines lc rgb \"red\" lw 3");
        gnuplot_plot_xy(g, plx, ply, 2, NULL);
    }

   sleep(period);
}

void
PlotGraph::DisplayData(const vector<VID>& vs){

  for(vector<VID>::const_iterator itr = vs.begin(); itr != vs.end(); itr++)
    cout<<*itr<<" ";
  cout<<endl;

}

void
PlotGraph::DisplayData(const vector<EID>& es){

  for(vector<EID>::const_iterator itr = es.begin(); itr != es.end(); itr++)
    cout<<"("<<(*itr).first<<","<<(*itr).second<<") ";
  cout<<endl;

}







