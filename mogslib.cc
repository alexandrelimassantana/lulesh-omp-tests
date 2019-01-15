#include <policies/binlpt.h>

#include <omp.h>

std::vector<unsigned> workloads;

void mogslib_set_domain(Domain *d) {
  #ifndef DOMP_STATIC
    auto n = domain->numNode();
    workloads.resize(n);
    for(auto i = 0; i < n; ++i)
      workloads[i] = domain->nodeElemCount(i);
    omp_set_workload(input.tasks.data(), input.ntasks());
  #endif
}
/**
 *  @brief Call the scheduler within MOGSLib to obtain a task map.
 *  @details A C++ proxy function to call the selected MOGSLib scheduler.
 *  @return The task map represented as an array to where the task should execute.
 */
inline unsigned *mogslib_call_strategy_map() {
  return MOGSLib::Policy::BinLPT::work(tasks, ntasks, nthreads, nchunks);
}

extern "C" {

/**
 *  @brief A function to interface with MOGSLib to call the scheduler task mapping.
 *  @details A C function to interface with OpenMP and direct the execution flow back to C++.
 */
unsigned *mogslib_strategy_map() {
  return mogslib_call_strategy_map();
}

bool mogslib_has_persistent_schedule() {
  return false;
}

}