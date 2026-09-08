#include "hls_stream.h"
#include <ap_int.h>

struct DataWord {
  ap_uint<32> data;
};

void producer(hls::stream<DataWord>& in_raw, hls::stream<DataWord>& fifo_stream) {
#ifndef __SYNTHESIS__
  while (!in_raw.empty()) {
#else
  while (1) {
#endif
    if (!in_raw.empty()) {
      DataWord item = in_raw.read();
      fifo_stream.write(item);
    }
  }
}

void consumer(hls::stream<DataWord>& fifo_stream, hls::stream<DataWord>& out_raw) {
#ifndef __SYNTHESIS__
  while (!fifo_stream.empty()) {
#else
  while (1) {  
#endif
    if (!fifo_stream.empty()) {
      DataWord item = fifo_stream.read();
      out_raw.write(item);
    }
  }
}

void hls_fifo_top(hls::stream<DataWord>& in, hls::stream<DataWord>& out) {
  #pragma HLS INTERFACE mode=ap_fifo port=in
  #pragma HLS INTERFACE mode=ap_fifo port=out
  #pragma HLS INTERFACE mode=ap_ctrl_none port=return

  hls::stream<DataWord> internal_fifo;
  
  #pragma HLS STREAM variable=internal_fifo depth=16
  #pragma HLS DATAFLOW
  producer(in, internal_fifo);
  consumer(internal_fifo, out);
}
