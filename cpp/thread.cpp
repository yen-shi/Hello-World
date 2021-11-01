#include<thread>
#include<vector>
#include<cstdint>
#include<numeric>
#include<iostream>
#include<chrono>

using namespace std;
using namespace std::chrono;

void sumUp(vector<int32_t> const& arr, size_t st, size_t ed, int64_t* result)
{
  for (size_t i = st; i < ed; ++i)
  {
    *result += arr[i];
  }
}

int main() {
  constexpr int64_t kSIZE = 1000000000;
  vector<int32_t> array(kSIZE);
  iota(begin(array), end(array), 0);

  constexpr int32_t kNB_THREADS = 10;
  constexpr int32_t kOFFSET = kSIZE / kNB_THREADS;
  vector<thread> workers;
  vector<int64_t> results(kNB_THREADS, 0);

  high_resolution_clock::time_point timer_begin = high_resolution_clock::now();
  for (int32_t i = 0; i < kNB_THREADS; ++i)
  {
    workers.emplace_back(sumUp, array, i * kOFFSET, (i+1) * kOFFSET, &results[i]);
  }
  for (int32_t i = 0; i < kNB_THREADS; ++i)
  {
    workers[i].join();
  }
  high_resolution_clock::time_point timer_end = high_resolution_clock::now();
  int64_t mt_result = accumulate(begin(results), end(results), 0LL);

  auto const multi_thread_time = duration_cast<std::chrono::nanoseconds>(timer_end - timer_begin);

  timer_begin = high_resolution_clock::now();
  int64_t st_result = accumulate(begin(array), end(array), 0LL);
  timer_end = high_resolution_clock::now();

  auto const single_thread_time = duration_cast<std::chrono::nanoseconds>(timer_end - timer_begin);

  cout << "ground truth: " << ((int64_t)kSIZE * (int64_t)(kSIZE - 1) / 2) << endl;
  cout << "multi-thread result: " << mt_result << ", time in ns: " << multi_thread_time.count() << endl;
  cout << "single-thread result: " << st_result << ", time in ns: " << single_thread_time.count() << endl;
  return 0;
}
