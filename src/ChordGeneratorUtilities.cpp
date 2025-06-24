//
// Created by Damien Sprockeels on 02/07/2024.
//

#include "../headers/ChordGeneratorUtilities.hpp"

/**
 * Converts an IntVarArray to a pointer to an array of integers.
 * @param vars The IntVarArray to convert.
 * @return A pointer to an array of integers representing the values in the IntVarArray.
 */
int* IntVarArray_to_int_pointer(IntVarArray vars){
    try{
        const auto res = new int[vars.size()];
        for(int i = 0; i < vars.size(); i++){
            res[i] = vars[i].val();
        }
        return res;
    }
    catch (...) {
        throw runtime_error("Error happened in IntVarArray_to_int_pointer function");
    }
}

/**
 * Converts an IntVarArray to a vector of integers.
 * @param vars The IntVarArray to convert.
 * @return A vector of integers representing the values in the IntVarArray.
 */
vector<int> IntVarArray_to_int_vector(const IntVarArray& vars){
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