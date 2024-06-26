#include "../common/testbench.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
void dut(bfloat16 *restrict in0, size_t m1, size_t m2, bfloat16 *restrict in1,
         size_t m3, size_t m4, bfloat16 *restrict out0, size_t m5, size_t m6);
void dut_ref(bfloat16 *in0, bfloat16 *in1, bfloat16 *out0);

constexpr unsigned const IN0_SIZE_DIM0 = 16;
constexpr unsigned const IN0_SIZE_DIM1 = 1024;
constexpr unsigned const IN0_SIZE = IN0_SIZE_DIM0 * IN0_SIZE_DIM1;
constexpr unsigned const IN1_SIZE_DIM0 = 16;
constexpr unsigned const IN1_SIZE_DIM1 = 1024;
constexpr unsigned const IN1_SIZE = IN1_SIZE_DIM0 * IN1_SIZE_DIM1;
constexpr unsigned const OUT0_SIZE_DIM0 = 16;
constexpr unsigned const OUT0_SIZE_DIM1 = 1024;
constexpr unsigned const OUT0_SIZE = OUT0_SIZE_DIM0 * OUT0_SIZE_DIM1;

alignas(32) bfloat16 g_in0[IN0_SIZE];
alignas(32) bfloat16 g_in1[IN1_SIZE];
alignas(32) bfloat16 g_out0[OUT0_SIZE];
alignas(32) bfloat16 g_out0Ref[OUT0_SIZE];

int main(int argc, char *argv[]) {
  std::string dataDir(TO_STR(DATA_DIR));
  srand(10);
  std::generate(g_in0, g_in0 + IN0_SIZE,
                [&]() { return random_bfloat16(-10, 10, 2); });
  std::generate(g_in1, g_in1 + IN1_SIZE,
                [&]() { return random_bfloat16(-10, 10, 2); });

  writeData(g_in0, IN0_SIZE, dataDir + "/in0.txt");
  writeData(g_in1, IN1_SIZE, dataDir + "/in1.txt");

  chess_memory_fence();
  auto cyclesBegin = chess_cycle_count();
  dut(g_in0, IN0_SIZE_DIM0, OUT0_SIZE_DIM1, g_in1, IN1_SIZE_DIM0, IN1_SIZE_DIM1,
      g_out0, OUT0_SIZE_DIM0, OUT0_SIZE_DIM1);
  auto cyclesEnd = chess_cycle_count();
  chess_memory_fence();

  auto cycleCount = (int)(cyclesEnd - cyclesBegin);
  reportCycleCount(cycleCount, dataDir + "/cycle_count.txt");

  writeData(g_out0, OUT0_SIZE, dataDir + "/out0.txt");

  dut_ref(g_in0, g_in1, g_out0Ref);
  writeData(g_out0Ref, OUT0_SIZE, dataDir + "/out0_ref.txt");

  bool ok = true;
  ok &= checkData(g_out0, g_out0Ref, OUT0_SIZE);

  if (ok)
    printf("TEST PASSED\n");
  else
    printf("TEST FAILED\n");

  return ok ? 0 : 1;
}

void dut_ref(bfloat16 *in0, bfloat16 *in1, bfloat16 *out0) {
  for (unsigned k = 0; k < OUT0_SIZE; k += 1) {
    out0[k] = in0[k] - in1[k];
  }
}
