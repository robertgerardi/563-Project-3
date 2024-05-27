#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "sim_proc.h"
#include <vector>
#include <iostream>
#include <iterator>
using namespace std;



Pipeline::Pipeline(int width, int iqSize, int robSize){
    instrWidth = width;
    traceline = 0;
    seq_number = 0;
    PipelineEmpty = false;
    EndFile = false;

    ROBSize = robSize;
    ROBEntries = 0;
    ROB = new ROB_Table[robSize];
    ROBHead = 0;
    ROBTail = 0;

    IQSize = iqSize;
    IQEntries = 0;
    IQ = new vector<instruction>;

    RMT = new RMT_Table[67];

    
    DEStage  = new vector<instruction>;
    RNStage = new vector<instruction> ;
    RRStage = new vector<instruction> ;
    DIStage = new vector<instruction> ;
    EX_FU = new vector<instruction> ;
    WBStage = new vector<instruction> ;

    

}
bool Pipeline::Advance_Cycle(){
    seq_number++;
    if(EndFile && PipelineEmpty){
       // cout << "test" << endl;
        return false;
    }else{
        return true;
    }
}
void Pipeline::retire(){
     
  // cout << "test" << endl;
    if(ROBEntries == 0){
        //cout << "test" << endl;
        return;
    }
    
    for(int i = 0; i < ROBSize; i++){
        
        if(ROB[i].ready){
            ROB[i].ROBInstruct.RT_DUR++;
            //printf("here\n");
        }
    }
    //go through ROB and update retire cycle times
    int limit = instrWidth;
    int tempHead = ROBHead;
    for(int i = 0; i < limit;i++){
      //cout << "test" << endl;
       // cout << "head: " << tempHead << endl;
       // cout << "Rob ready: " << ROB[tempHead].ready << endl;
       
        if(ROB[tempHead].ready){
            
            if((RMT[ROB[tempHead].dest].ROBTag == tempHead) && ROB[tempHead].dest != -1){
                
                RMT[ROB[tempHead].dest].valid = false;
            }
            
        }
        else{
            
            break;} //instr isnt ready so cannot go to next one to 
        /*
        for(int j = 0; j < IQ->size(); j++){
                if(IQ->at(j).RS1ROB && ( tempHead == IQ->at(j).RS1)){
                    IQ->at(j).RS1Ready = true;
                   // cout << "UPDATE 1" << endl;
                }
                if(IQ->at(j).RS2ROB && (tempHead == IQ->at(j).RS2)){
                    IQ->at(j).RS2Ready = true;
                   // cout << "UPDATE 2" << endl;
                }
            }
            
        for(int j = 0; j < DIStage->size(); j++){
                if(DIStage->at(j).RS1ROB && ( tempHead == DIStage->at(j).RS1)){
                    DIStage->at(j).RS1Ready = true;
                    //cout << "UPDATE 3" << endl;
                }
                if(DIStage->at(j).RS2ROB && ( tempHead == DIStage->at(j).RS2)){
                    DIStage->at(j).RS2Ready = true;
                   //cout << "UPDATE 4" << endl;
                }
            }
        for(int j = 0; j < RRStage->size(); j++){
                if(RRStage->at(j).RS1ROB && ( tempHead == RRStage->at(j).RS1)){
                    RRStage->at(j).RS1Ready = true;
                   // cout << "UPDATE 5" << endl;
                }
                if(RRStage->at(j).RS2ROB && ( tempHead == RRStage->at(j).RS2)){
                    RRStage->at(j).RS2Ready = true;
                    //cout << "UPDATE 6" << endl;
                }
            }
            */
        instruction PRINT = ROB[tempHead].ROBInstruct;
        printf("%d fu{%d} src{%d,%d} dst{%d} FE{%d,%d} DE{%d,%d} RN{%d,%d} RR{%d,%d} DI{%d,%d} IS{%d,%d} EX{%d,%d} WB{%d,%d} RT{%d,%d}\n",
					traceline, PRINT.optype, PRINT.ORIGRS1, PRINT.ORIGRS2, PRINT.destReg, PRINT.FE_BC, PRINT.FE_DUR, PRINT.DE_BC, PRINT.DE_DUR, PRINT.RN_BC, PRINT.RN_DUR, PRINT.RR_BC,
					PRINT.RR_DUR, PRINT.DI_BC, PRINT.DI_DUR, PRINT.IS_BC, PRINT.IS_DUR, PRINT.EX_BC, PRINT.EX_DUR, PRINT.WB_BC, PRINT.WB_DUR, PRINT.RT_BC, PRINT.RT_DUR);
        traceline++;
        ROBHead = (ROBHead + 1)% ROBSize;
        ROBEntries--;
        tempHead = (tempHead +1) %ROBSize;
       
        if(ROBEntries == 0 && EndFile){
        //cout << "TESTT" << endl;
        PipelineEmpty = true;
        final_cycles_count = PRINT.RT_BC + PRINT.RT_DUR;
        return;
    }
    }
   //cout << "====" <<endl;
    
}
void Pipeline::writeback(){
    int size = WBStage->size();

    for(int i = 0; i < size; i++){
        ROB[WBStage->at(i).ROBID].ready = true;
       
       
        for(int j = 0; j < IQ->size(); j++){
                if(IQ->at(j).RS1ROB && ( WBStage->at(i).ROBID == IQ->at(j).RS1)){
                    IQ->at(j).RS1Ready = true;
                   // cout << "UPDATE 1" << endl;
                }
                if(IQ->at(j).RS2ROB && (WBStage->at(i).ROBID == IQ->at(j).RS2)){
                    IQ->at(j).RS2Ready = true;
                   // cout << "UPDATE 2" << endl;
                }
            } 
            /*
            for(int j = 0; j < DIStage->size(); j++){
                if(DIStage->at(j).RS1ROB && ( WBStage->at(i).ROBID == DIStage->at(j).RS1)){
                    DIStage->at(j).RS1Ready = true;
                    //cout << "UPDATE 3" << endl;
                }
                if(DIStage->at(j).RS2ROB && ( WBStage->at(i).ROBID == DIStage->at(j).RS2)){
                    DIStage->at(j).RS2Ready = true;
                   //cout << "UPDATE 4" << endl;
                }
            }
            for(int j = 0; j < RRStage->size(); j++){
                if(RRStage->at(j).RS1ROB && ( WBStage->at(i).ROBID == RRStage->at(j).RS1)){
                    RRStage->at(j).RS1Ready = true;
                   // cout << "UPDATE 5" << endl;
                }
                if(RRStage->at(j).RS2ROB && ( WBStage->at(i).ROBID == RRStage->at(j).RS2)){
                    RRStage->at(j).RS2Ready = true;
                    //cout << "UPDATE 6" << endl;
                }
            } */
        //cout << ROB[WBStage->at(i).ROBID].dest << endl;
    }

    for(int i = 0; i < size; i++){
            WBStage->at(0).WB_DUR++;
            WBStage->at(0).RT_BC = WBStage->at(0).WB_BC + WBStage->at(0).WB_DUR;
            
            //cout << "WB "<< WBStage->at(0).WB_BC << " " <<  WBStage->at(0).WB_DUR << endl;


            ROB[WBStage->at(0).ROBID].ROBInstruct = WBStage->at(0);
            WBStage->erase(WBStage->begin());
        }

}

void Pipeline::execute(){
    int size = EX_FU->size();
   //cout << "test" << endl;
    for(int i = 0; i < size; i++){
        EX_FU->at(i).FU_Cycles--;
        EX_FU->at(i).EX_DUR++;
    }
    for(vector<instruction>::iterator i = EX_FU->begin(); i != EX_FU->end();){
        //cout << "Cyc: " << i->FU_Cycles << endl;
        //cout << i->destReg << endl;
        if(i->FU_Cycles == 0){
           //cout << i->destReg << endl;
            i->WB_BC = i->EX_BC + i->EX_DUR;
            WBStage->push_back(*i);

            for(int j = 0; j < IQ->size(); j++){
                if(IQ->at(j).RS1ROB && ( i->ROBID == IQ->at(j).RS1)){
                    IQ->at(j).RS1Ready = true;
                    //cout << "UPDATE 1" << endl;
                }
                if(IQ->at(j).RS2ROB && ( i->ROBID == IQ->at(j).RS2)){
                    IQ->at(j).RS2Ready = true;
                   // cout << "UPDATE 2" << endl;
                }
            }
            for(int j = 0; j < DIStage->size(); j++){
                if(DIStage->at(j).RS1ROB && ( i->ROBID == DIStage->at(j).RS1)){
                    DIStage->at(j).RS1Ready = true;
                    //cout << "UPDATE 3" << endl;
                }
                if(DIStage->at(j).RS2ROB && ( i->ROBID == DIStage->at(j).RS2)){
                    DIStage->at(j).RS2Ready = true;
                   //cout << "UPDATE 4" << endl;
                }
            }
            for(int j = 0; j < RRStage->size(); j++){
                if(RRStage->at(j).RS1ROB && ( i->ROBID == RRStage->at(j).RS1)){
                    RRStage->at(j).RS1Ready = true;
                   // cout << "UPDATE 5" << endl;
                }
                if(RRStage->at(j).RS2ROB && ( i->ROBID == RRStage->at(j).RS2)){
                    RRStage->at(j).RS2Ready = true;
                    //cout << "UPDATE 6" << endl;
                }
            }
            i = EX_FU->erase(i);
            //cout << "EX" << endl;
        }
        else{i++;}
        
    }
}

void Pipeline::issue(){
    int size = IQ->size();
    int limit = instrWidth;

    vector<int> removeList;
    //goes through list, finding up to width instructions to execute 
    //cout << "SIZE: "<< IQ->size() << endl;
    /*
   for(int i = 0; i< IQ->size();i++){
    if((IQ->at(i).RS1ROB && ROB[IQ->at(i).RS1].ready) || IQ->at(i).RS1ROB == false || IQ->at(i).RS1 == -1){
        IQ->at(i).RS1Ready = true;
       // cout << "up1" << endl;
    }
    if((IQ->at(i).RS2ROB && ROB[IQ->at(i).RS2].ready) || IQ->at(i).RS2ROB == false || IQ->at(i).RS2 == -1){
        IQ->at(i).RS2Ready = true;
      //  cout << "up2" << endl;
    }
   }
   */
  for(int i = 0; i < size; i++){
        //cout<<IQ->at(i).RS1Ready << " " << IQ->at(i).RS2Ready<< IQ->at(i).RS2ROB<< ROB[IQ->at(i).RS2].ready << endl;
        IQ->at(i).IS_DUR++;
        
    }
    auto it = IQ->begin();
    while (it != IQ->end() && limit > 0) {
       // cout << traceline<< " " << it->ORIGRS1<< " " << it->RS1Ready << " "<< it->ORIGRS2<<" "<<  it->RS2Ready <<" "<<  it->destReg<< " " << it->RS1ROB<<endl;
        if (it->RS1Ready && it->RS2Ready) {
            limit--;
            it->EX_BC = it->IS_BC + it->IS_DUR;
            EX_FU->push_back(*it);
            it = IQ->erase(it);
            IQEntries--;
        } else {
            ++it;
            //cout << "Test" << endl;
        }
    }
    /*
    for(int i = 0; i < size; i++){
        //cout<<IQ->at(i).RS1Ready << " " << IQ->at(i).RS2Ready<< IQ->at(i).RS2ROB<< ROB[IQ->at(i).RS2].ready << endl;
        IQ->at(i).IS_DUR++;
        if(limit > 0){
            if(IQ->at(i).RS1Ready && IQ->at(i).RS2Ready){
                limit--;
                removeList.push_back(i);
               //cout << "test" << endl;
            }
        }
    }

    for (auto it = removeList.rbegin(); it != removeList.rend(); ++it) {
    IQEntries--;
    IQ->at(*it).EX_BC = IQ->at(*it).IS_BC + IQ->at(*it).IS_DUR;
    EX_FU->push_back(IQ->at(*it));
    IQ->erase(IQ->begin() + *it);
}

    for(int i = removeList.size() - 1; i >= 0; i--){
     //cout << "IS" << endl;
       // cout << IQ->at(i).destReg << endl;
        IQEntries--;
        IQ->at(removeList.at(i)).EX_BC = IQ->at(removeList.at(i)).IS_BC + IQ->at(removeList.at(i)).IS_DUR;
       EX_FU->push_back(IQ->at(removeList.at(i)));
       IQ->erase(IQ->begin() + removeList.at(i));
    } */
}

void Pipeline::dispatch(){
    int size = DIStage->size();
 //cout << "test" << endl;
    if(DIStage->empty()){return;}

    if(IQEntries + size > IQSize){
        
        for(int i = 0; i < size; i++){
                DIStage->at(i).DI_DUR++;
            }
            return;
    }
    for(int i = 0; i < size; i++){
        DIStage->at(0).DI_DUR++;
        DIStage->at(0).IS_BC = DIStage->at(0).DI_BC + DIStage->at(0).DI_DUR;
        IQ->push_back(DIStage->at(0));
        IQEntries++;
       // cout << "DI "<< DIStage->at(0).DI_BC << " " <<  DIStage->at(0).DI_DUR << endl;
        DIStage->erase(DIStage->begin());
    }

}

void Pipeline::regRead(){
    
    int size = RRStage->size();

    if(RRStage->empty()){return;}

    if(!DIStage->empty()){
        for(int i = 0; i < size; i++){
                RRStage->at(i).RR_DUR++;
            }
            return;
    }
    //check each src for readiness

    for(int i = 0; i < size; i++){

        if(RRStage->at(i).RS1 == -1 || RRStage->at(i).RS1ROB == false || RRStage->at(i).RS2 == -1 || RRStage->at(i).RS2ROB == false){
            if(RRStage->at(i).RS1 == -1 || RRStage->at(i).RS1ROB == false){
                RRStage->at(i).RS1Ready = true;
                
            }
            if(RRStage->at(i).RS2 == -1 || RRStage->at(i).RS2ROB == false){
                RRStage->at(i).RS2Ready = true;
             
            }
            
        }
        if(RRStage->at(i).RS1 != -1 && ROB[RRStage->at(i).RS1].ready){
            RRStage->at(i).RS1Ready = true;
        }

        if(RRStage->at(i).RS2 != -1 && ROB[RRStage->at(i).RS2].ready){
            RRStage->at(i).RS2Ready = true;
        }

        
    }

    for(int i = 0; i < size; i++){
            RRStage->at(0).RR_DUR++;
            RRStage->at(0).DI_BC = RRStage->at(0).RR_BC + RRStage->at(0).RR_DUR;
            DIStage->push_back(RRStage->at(0));
           // cout << "RR "<< RRStage->at(0).RR_BC << " " <<  RRStage->at(0).RR_DUR << endl;
            RRStage->erase(RRStage->begin());
        }

}
void Pipeline::rename(){
    
    int size = RNStage->size();
   
    if(RNStage->empty()){
      //cout << "test" << endl;
        return;
    }
    if(!RRStage->empty() || (ROBEntries + size > ROBSize)){

        
        for(int i = 0; i < size; i++){
            RNStage->at(i).RN_DUR++;
            //cout << "test" << endl;
         }
        return;
    }
    
        //cout << "test" << endl;
    for(int i = 0; i < size; i++){
     
        RNStage->at(i).ROBID = ROBTail;
        ROB[ROBTail].dest = RNStage->at(i).destReg;
        ROB[ROBTail].PC = RNStage->at(i).PC;
        ROB[ROBTail].ready = false;
        ROB[ROBTail].ROBInstruct = RNStage->at(i);

        ROBTail = (ROBTail + 1) % ROBSize;
        ROBEntries++;

        //rename source regs
        if(RNStage->at(i).RS1 != -1 && RMT[RNStage->at(i).RS1].valid){
            RNStage->at(i).RS1ROB = true;
            RNStage->at(i).RS1 = RMT[RNStage->at(i).RS1].ROBTag; 
        }else{
            RNStage->at(i).RS1ROB = false;
           // RNStage->at(i).RS1Ready = true;
            
        }

        if(RNStage->at(i).RS2 != -1 && RMT[RNStage->at(i).RS2].valid){
            RNStage->at(i).RS2ROB = true;
            RNStage->at(i).RS2 = RMT[RNStage->at(i).RS2].ROBTag; 
        }else{
            RNStage->at(i).RS2ROB = false;
            //RNStage->at(i).RS2Ready = true;
             
        }

        if(RNStage->at(i).destReg != -1){
            RMT[RNStage->at(i).destReg].ROBTag = RNStage->at(i).ROBID;
            RMT[RNStage->at(i).destReg].valid = true;
        }

    
    }
    for(int i = 0; i < size; i++){
            RNStage->at(0).RN_DUR++;
            RNStage->at(0).RR_BC = RNStage->at(0).RN_BC + RNStage->at(0).RN_DUR;
            RRStage->push_back(RNStage->at(0));
           // cout << "RN "<<RNStage->at(0).RN_BC << " " <<  RNStage->at(0).RN_DUR << endl;
            RNStage->erase(RNStage->begin());
    }
    
}
void Pipeline::decode(){
    int size = DEStage->size();
    if(DEStage->empty()){
        return;
    }
    if(!RNStage->empty()){
        for(int i = 0; i < size; i++){
            DEStage->at(i).DE_DUR++;
            //cout << "test 1" << endl;
         }
        return;
    }
    for(int i = 0; i < size; i++){
        // cout << "here" << endl;
        DEStage->at(0).DE_DUR++;
        DEStage->at(0).RN_BC = DEStage->at(0).DE_BC + DEStage->at(0).DE_DUR;
        RNStage->push_back(DEStage->at(0));
        //cout << "DE "<<DEStage->at(0).DE_BC << " " <<  DEStage->at(0).DE_DUR << endl;
        DEStage->erase(DEStage->begin());
    }
}

void Pipeline::fetch(FILE * FP){
    int op_type, dest, src1, src2;
    uint64_t pc;
    if(!DEStage->empty()){
        
        return;
    }
    for(int i = 0; i < instrWidth; i++){

    
    if(fscanf(FP, "%llx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) == EOF){
        EndFile = true;
        
        return;
    }else{
        //printf("%lx %d %d %d %d\n", pc, op_type, dest, src1, src2);
        instruction instr;
        instr.PC  = pc;
        instr.optype = op_type;
        instr.destReg = dest;
        instr.seqNum = seq_number;
        instr.RS1 = src1;
        instr.RS2 = src2;
        instr.ORIGRS1 = src1;
        instr.ORIGRS2 = src2;
        instr.RS1Ready = false;
        instr.RS2Ready = false;
        instr.RS1ROB = false;
        instr.RS2ROB = false;
        instr.valid = true;

        instr.FE_BC = seq_number;
        instr.FE_DUR = 1;
        instr.DE_BC = seq_number + 1;
        instr.DE_DUR = 0;
        instr.RN_DUR = 0;
        instr.RR_DUR = 0;
        instr.DI_DUR = 0;
        instr.IS_DUR = 0;
        instr.EX_DUR = 0;
        instr.WB_DUR = 0;
        instr.RT_DUR = 0;
        switch(op_type){
            case 0:
                instr.FU_Cycles = 1;
                break;
            case 1:
                instr.FU_Cycles = 2;
                break;
            case 2:
                instr.FU_Cycles = 5;
                break;
        }
    
        DEStage->push_back(instr);

       //cout << "FE "<<instr.FE_BC << " " <<  instr.FE_DUR << endl;
    }
    }
}