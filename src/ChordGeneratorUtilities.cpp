//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"

int* IntVarArray_to_int_pointer(IntVarArray vars){
    try{
        int* res = new int[vars.size()];
        for(int i = 0; i < vars.size(); i++){
            res[i] = vars[i].val();
        }
        return res;
    }
    catch (...) {
        throw runtime_error("Error happened in IntVarArray_to_int_pointer function");
    }
}

vector<int> IntVarArray_to_int_vector(IntVarArray vars){
    try{
        vector<int> res;
        for(const auto & var : vars)
            res.push_back(var.val());
        return res;
    }
    catch (...) {
        throw runtime_error("Error happened in IntVarArray_to_int_vector function");
    }
}