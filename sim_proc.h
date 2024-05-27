#ifndef SIM_PROC_H
#define SIM_PROC_H
#include <vector>
using namespace std;



typedef struct proc_params{
    unsigned long int rob_size;
    unsigned long int iq_size;
    unsigned long int width;
}proc_params;



class instruction{

    public:
    
    bool valid = 0;
    int seqNum = 0;
    unsigned int long PC = 0;
    int optype = 0;

    int destReg = 0;
    int ROBID = 0;

    int RS1 = 0;
    int ORIGRS1 = 0;
    bool RS1ROB = 0;
    bool RS1Ready = 0;

    int RS2 = 0;
    int ORIGRS2 = 0;
    bool RS2ROB = 0;
    bool RS2Ready = 0;

    int FU_Cycles = 0;


    int FE_BC = 0;
    int FE_DUR = 0;

    int DE_BC = 0;
    int DE_DUR;

    int RN_BC = 0;
    int RN_DUR = 0;

    int RR_BC = 0;
    int RR_DUR = 0;

    int DI_BC = 0; 
    int DI_DUR = 0;

    int IS_BC = 0;
    int IS_DUR = 0;

    int EX_BC = 0;
    int EX_DUR = 0;

    int WB_BC = 0;
    int WB_DUR = 0;

    int RT_BC = 0;
    int RT_DUR = 0;


};



class RMT_Table{
    public:
     bool valid = 0;
     int ROBTag= 0;

};

class ROB_Table{
    public:
        int dest = 0;
	    bool ready = 0;
	
	    unsigned long int PC = 0;
	    instruction ROBInstruct;
};



class Pipeline{
    public:

    Pipeline(int width, int IQSize, int ROBSize);
    void retire();
	void writeback();
	void execute();
	void issue();
	void dispatch();
	void regRead();
	void rename();
	void decode();
	void fetch(FILE * FP);

    bool Advance_Cycle();

    bool PipelineEmpty;
    bool EndFile;

    int traceline;
    int seq_number;

    int final_cycles_count;
    private:

    int instrWidth;

    ROB_Table * ROB;
    int ROBSize;
    int ROBEntries;
    int ROBHead;
    int ROBTail;

    vector<instruction> * IQ;
    int IQSize;
    int IQEntries;

    RMT_Table * RMT;

   
    vector<instruction> * DEStage;
    vector<instruction> * RNStage;
    vector<instruction> * RRStage;
    vector<instruction> * DIStage;
    vector<instruction> * EX_FU;
    vector<instruction> * WBStage;
};


#endif
