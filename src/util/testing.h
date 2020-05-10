#include <numeric>
#include <vector>
#include <algorithm>

namespace Util{

/**
 * Calculates the minimum, maximum, and average of a std::vector<double>
 * 
 * @param[in] vals The vector of doubles to use for calculations
 * @param[out] min, max, avg A pointer to the where store the results
 */
void MinMaxAvg(std::vector<double>& vals, double* min, double* max, double* avg){
    *min = *std::min_element(vals.begin(), vals.end());
    *max = *std::max_element(vals.begin(), vals.end());
    *avg = std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size();
}

} //Namespace Util end