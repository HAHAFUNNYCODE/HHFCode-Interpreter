#include <numeric>
#include <vector>

void MinMaxAvg(std::vector<double>& vals, double* min, double* max, double* avg){
    *min = *std::min_element(vals.begin(), vals.end());
    *max = *std::max_element(vals.begin(), vals.end());
    *avg = std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size();
}