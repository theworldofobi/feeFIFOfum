#include "hls_stream.h"
#include <ap_int.h>
#include <iostream>

struct DataWord {
  ap_uint<32> data;
};

// DUT defined
void hls_fifo_top(hls::stream<DataWord>& in, hls::stream<DataWord>& out);

int main() {
  hls::stream<DataWord> in("in");
  hls::stream<DataWord> out("out");

  const int NUM_WORDS{20}; // intentionally larger than depth=16

  for (int i{}; i < NUM_WORDS; ++i) {
    DataWord w;
    w.data = i;
    in.write(w);
  }

  hls_fifo_top(in, out);

  int errors{};
  for (int i{}; i < NUM_WORDS; ++i) {
    DataWord w = out.read();
    if (w.data != (ap_uint<32>)i) {
      std::cout << "MISMATCH at index " << i
                << ": expected " << i
                << ", received " << w.data << std::endl;
      ++errors;
    }
  }

  if (errors == 0) {
    std::cout << "TEST PASSED (" << NUM_WORDS << " words, depth=16)" << std::endl;
  } else {
    std::cout << "TEST FAILED: " << errors << " mismatches" << std::endl;
  }

  return errors;
}
