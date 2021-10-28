#include <cassert>
#include <iostream>
#include <limits>

#include "kernel.h"

using std::cout;
using std::endl;

int THD_COUNT = 1;

using std::string;


void _gspmm(csr_t* snaph, array2d_t<float> & input, array2d_t<float> & output,
                     op_t op, bool reverse, bool norm /*= true*/)
{
    //cout << "spmm " << op << "reverse = " << reverse << endl;

    //If in backward, normalize it first, else normalize it after computation

    //The core logic goes here.

    int64_t columnCount = output.columnCount;
    int64_t rowCount = output.rowCount;
    vid_t* nbrs = snaph->get_nebrs();
    vid_t* offset = snaph->get_offset();
    vid_t v_count = snaph->get_vcount();

    for(vid_t i = 0; i <v_count; i++){
    vid_t deg = snaph->get_degree(node);
    if(reverse == 0){
    	for (vid_t nbrID = offset[i]; nbrID < offset[i]+ deg; nbrID++){
    		output.row_add(input.operator[](nbrs[nbrID]), i);
    	}
    	output.row_normalize(i, deg);
    }
    else {
    	input.row_normalize(i, deg);
    	for (vid_t nbrID = offset[i]; nbrID < offset[i]+ deg; nbrID++){
    		output.row_add(input.operator[](nbrs[nbrID]), i);
    	}
    }


}

void invoke_gspmm(graph_t& graph, array2d_t<float> & input_array, array2d_t<float> & output_array,
                 bool reverse, bool norm /*= true*/)
{
    if (reverse) {
         return _gspmm(&graph.csr, input_array, output_array, eSUM, reverse, norm);
    } else {
         return _gspmm(&graph.csc, input_array, output_array, eSUM, reverse, norm);
    }
}
