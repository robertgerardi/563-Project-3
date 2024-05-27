#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <vector>
#include <iostream>
#include "sim_proc.h"


/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim 256 32 4 gcc_trace.txt
    argc = 5
    argv[0] = "sim"
    argv[1] = "256"
    argv[2] = "32"
    ... and so on
    
*/
    
    FILE *FP;               // File handler
char *trace_file;       // Variable that holds trace file name;
      // look at sim_bp.h header file for the the definition of struct proc_params
  // Variables are read from trace file
 // Variable holds the pc read from input file  
proc_params params; 

int main (int argc, char* argv[])
{
    
    
    if (argc != 5)
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.rob_size     = strtoul(argv[1], NULL, 10);
    params.iq_size      = strtoul(argv[2], NULL, 10);
    params.width        = strtoul(argv[3], NULL, 10);

    trace_file          = argv[4];
    
  
   // printf("rob_size:%lu "
   //         "iq_size:%lu "
   //         "width:%lu "
    //        "tracefile:%s\n", params.rob_size, params.iq_size, params.width, trace_file);
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // The following loop just tests reading the trace and echoing it back to the screen.
    //
    // Replace this loop with the "do { } while (Advance_Cycle());" loop indicated in the Project 3 spec.
    // Note: fscanf() calls -- to obtain a fetch bundle worth of instructions from the trace -- should be
    // inside the Fetch() function.
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    Pipeline pl(params.width, params.iq_size, params.rob_size);
    
    do{

        pl.retire();
        pl.writeback();
        pl.execute();
        pl.issue();
        pl.dispatch();
        pl.regRead();
        pl.rename();
        pl.decode();
        pl.fetch(FP);

    }while(pl.Advance_Cycle());
     
    cout<< "# === Simulator Command =========" << endl;
    printf("# ./sim %lu %lu %lu %s\n", params.rob_size, params.iq_size, params.width,trace_file);
    cout<< "# === Processor Configuration ===" <<endl;
    printf("# ROB_SIZE = %lu\n",params.rob_size);
    printf("# IQ_SIZE  = %lu\n",params.iq_size);
    printf("# WIDTH    = %lu\n",params.width);
    cout <<"# === Simulation Results ========"<<endl;
    printf("# Dynamic Instruction Count    = %d\n",pl.traceline);
    printf("# Cycles                       = %d\n",pl.final_cycles_count);
    float IPC = (float)(pl.traceline)/(float)pl.final_cycles_count;
    printf("# Instructions Per Cycle (IPC) = %.2f",IPC);
    cout  <<endl;


    
    
   // while(fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) != EOF)
   //     printf("%lx %d %d %d %d\n", pc, op_type, dest, src1, src2); //Print to check if inputs have been read correctly

    return 0;
}
