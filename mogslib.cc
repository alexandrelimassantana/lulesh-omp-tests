#include <vector>
#include <policies/binlpt.h>
#include <lulesh.h>

#include <omp.h>

std::vector<unsigned> workloads;

void mogslib_set_domain(Domain *domain) {
  #ifndef DOMP_STATIC
    auto n = domain->numNode();
    workloads.resize(n);
    for(auto i = 0; i < n; ++i)
      workloads[i] = domain->nodeElemCount(i);
    omp_set_workload(workloads.data(), workloads.size());
  #endif
}
/**
 *  @brief Call the scheduler within MOGSLib to obtain a task map.
 *  @details A C++ proxy function to call the selected MOGSLib scheduler.
 *  @return The task map represented as an array to where the task should execute.
 */
inline unsigned *mogslib_call_strategy_map(unsigned *tasks, unsigned ntasks, unsigned nthreads, unsigned nchunks) {
  return MOGSLib::Policy::BinLPT::work(tasks, ntasks, nthreads, nchunks);
}

extern "C" {

/**
 *  @brief A function to interface with MOGSLib to call the scheduler task mapping.
 *  @details A C function to interface with OpenMP and direct the execution flow back to C++.
 */
unsigned *mogslib_strategy_map(unsigned *tasks, unsigned ntasks, unsigned nthreads, unsigned nchunks) {
  return mogslib_call_strategy_map(tasks, ntasks, nthreads, nchunks);
}

bool mogslib_has_persistent_schedule() {
  return false;
}

}